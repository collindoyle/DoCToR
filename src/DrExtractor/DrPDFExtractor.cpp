//
//  DrPDFExtractor.cpp
//  DrExtractor
//
//  Created by Chu Yimin on 2013/05/21.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#include "DrPDFExtractor.h"
#include "DrTexGrouper.h"

#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_ADVANCES_H

#define LINE_DIST 0.9f
#define SPACE_DIST 0.2f
#define PARAGRAPH_DIST 0.5f

DrPDFExtractor::DrPDFExtractor()
{
    m_filename = NULL;
    m_ctx = NULL;
    m_isopen = false;
    m_doc = NULL;
    m_fontcache = new DrFontCache;
}

DrPDFExtractor::~DrPDFExtractor()
{
    if (m_doc) {
        fz_close_document(m_doc);
        m_doc = NULL;
    }
    if (m_ctx != NULL) {
        fz_free_context(m_ctx);
        m_ctx = NULL;
    }
    if (m_filename) {
        delete [] m_filename;
    }
    if (m_fontcache) {
        delete m_fontcache;
        m_fontcache = NULL;
    }
    m_isopen = false;
}

void DrPDFExtractor::Initialize()
{
    if (m_ctx) {
        fz_free_context(m_ctx);
    }
    m_ctx = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
    if (m_doc) {
        fz_close_document(m_doc);
        m_doc = NULL;
    }
}

int DrPDFExtractor::OpenPDFFile(const char *filename)
{
    fz_document *doc = fz_open_document(m_ctx, filename);
    
    if (doc != NULL) {
        m_doc = doc;
        m_isopen = true;
        m_filename = new char[strlen(filename)+1];
        strcpy(m_filename, filename);
        return 0;
    }
    else return 1;
}

DrPage * DrPDFExtractor::ExtractPage(unsigned int pageno)
{
    fz_page * page = fz_load_page(m_doc, pageno);
    if (page == NULL) {
        return NULL;
    }

    DrPage * dpage = new DrPage;
    dpage->SetPageNo(pageno);
    
    std::list<DrChar *> charlist;
    std::list<DrPhrase *> phraselist;
    std::list<DrLine *> linelist;
    std::list<DrZone *> &zonelist = dpage->m_zonelist;
    
    
    fz_matrix transform;
    fz_rotate(&transform,0);
    fz_pre_scale(&transform, 1.0f, 1.0f);
    
    fz_rect bounds;
    fz_bound_page(m_doc, page, &bounds);
    fz_transform_rect(&bounds, &transform);
    fz_irect bbox;
    fz_round_rect(&bbox, &bounds);
    fz_matrix ttransform = transform;
    fz_pixmap *pix = fz_new_pixmap_with_bbox(m_ctx, fz_device_rgb(m_ctx), &bbox);
    fz_clear_pixmap_with_value(m_ctx, pix, 0xff);
    
    fz_device * dev = fz_new_draw_device(m_ctx,pix);
    fz_run_page(m_doc, page, dev, &transform, NULL);
    fz_free_device(dev);

	fz_text_sheet * sheet = fz_new_text_sheet(m_ctx);
	fz_text_page * tpage = fz_new_text_page(m_ctx);
    fz_device * cdev = fz_new_text_device(m_ctx, sheet, tpage);
    fz_run_page(m_doc, page, cdev, &ttransform, NULL);
    
    
    ExtractChars(charlist,tpage);
    fz_free_device(cdev);
    
    DrThumbnail * thumb = new DrThumbnail(m_ctx,pix,pageno);
    dpage->m_thumbnail = thumb;

    
    DrTextGrouper::TextGroup(phraselist, charlist);
    
    
    std::list<DrPhrase *>::iterator itphrase = phraselist.begin();
    while (itphrase != phraselist.end()) {
        if ((*itphrase)->IsSpacePhrase()) {
            delete *itphrase;
            itphrase = phraselist.erase(itphrase);
        }
        else itphrase++;
    }
    
    DrTextGrouper::TextGroup(linelist, phraselist);
    DrTextGrouper::TextGroup(zonelist, linelist);
    dpage->CalculatePageBox();
        //    fz_free_text_sheet(m_ctx, tsheet);
        //    fz_free_text_page(m_ctx, tpage);
    fz_free_page(m_doc, page);
    return dpage;
}

void DrPDFExtractor::ExtractChars(std::list<DrChar *> &charlist, fz_text_page * tpage)
{
    for (int i = 0; i < tpage->len; i++) {
		fz_page_block * pb = tpage->blocks+i;
		if (pb->type == FZ_PAGE_BLOCK_TEXT) {
			fz_text_block * tb = pb->u.text;
			for (int j = 0; j < tb->len; j++) {
				fz_text_line * line = tb->lines+j;
				for (fz_text_span * span = line->first_span; span->next != NULL; span = span->next) {
					for (int k = 0; k < span->len; k++) {
						fz_rect bbox;
						fz_text_char_bbox(&bbox, span, k);
						fz_text_char * t_char = span->text+k;
						fz_text_style * style = t_char->style;
						DrChar * achar = new DrChar();
						DrBox charbox(bbox.x0, bbox.y0, bbox.x1, bbox.y1);
						int fontstyle;
						if (style->font->ft_bold == 1 && style->font->ft_italic == 0) {
							fontstyle = DrFontDescriptor::FS_BOLD;
						}
						else if (style->font->ft_bold == 0 && style->font->ft_italic == 1)
						{
							fontstyle = DrFontDescriptor::FS_ITALIC;
						}
						else if (style->font->ft_bold == 1 && style->font->ft_italic == 1)
						{
							fontstyle = DrFontDescriptor::FS_BOLD_ITALIC;
						}
						else
							fontstyle = DrFontDescriptor::FS_NONE;
						DrFontDescriptor * fd = m_fontcache->FindDescriptor(style->font->name, style->size, fontstyle);
						if (fd == NULL) {
							m_fontcache->AddDescriptor(style->font->name, style->size, fontstyle);
							fd = m_fontcache->FindDescriptor(style->font->name, style->size, fontstyle);
						}
						achar->Initialize(charbox, t_char->c,fd);
						charlist.push_back(achar);
					}
				}
			}
		}
	}
}


DrBox & DrPDFExtractor::Split_bbox(DrBox &bbox, int i, int n)
{
    DrBox newbox(bbox.m_x0,bbox.m_y0,bbox.m_x1,bbox.m_y1);
	float w = (bbox.m_x1 - bbox.m_x0) / n;
	float x0 = bbox.m_x0;
	newbox.m_x0 = x0 + i * w;
	newbox.m_x1 = x0 + (i + 1) * w;
	return newbox;
}

int DrPDFExtractor::GetPageCount()
{
    return fz_count_pages(m_doc);
}