//
//  DrPDFExtractor.cpp
//  DrExtractor
//
//  Created by Chu Yimin on 2013/05/21.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#include "DrPDFExtractor.h"
#include "DrTexGrouper.h"
#include "DrContentExtractor.h"

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
    fz_pixmap *pix = fz_new_pixmap_with_bbox(m_ctx, fz_device_rgb, &bbox);
    fz_clear_pixmap_with_value(m_ctx, pix, 0xff);
    
    fz_device * dev = fz_new_draw_device(m_ctx,pix);
    fz_run_page(m_doc, page, dev, &transform, NULL);
    fz_free_device(dev);

    extractlist * list = new_extract_list(m_ctx);
    fz_device * cdev = new_extract_device(m_ctx, list);
    fz_run_page(m_doc, page, cdev, &ttransform, NULL);
    fz_free_device(cdev);
    
    ExtractChars(charlist, list);
    free_extract_content_list(m_ctx, list);
    
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

void DrPDFExtractor::ExtractChars(std::list<DrChar *> &charlist, extractlist *list)
{
    extractnode *pnode;
    float x = -1;
    float y = -1;
    if (list == NULL) {
        return;
    }
    for (pnode = list->phead; pnode != NULL; pnode = pnode->pNext) {
        ExtractCharImp(charlist, pnode, x, y);
    }
}

void DrPDFExtractor::ExtractCharImp(std::list<DrChar *> &charlist, extractnode *textnode, float &x, float &y)
{
    fz_matrix ctm = textnode->ctm;
    fz_text *text = textnode->text;
    fz_font *font = text->font;
	FT_Face face = (FT_Face)((text->font)->ft_face);
	fz_matrix tm = text->trm;
	fz_matrix trm;
	float size;
	float adv;
	fz_rect rect;
	fz_point dir, ndir;
	fz_point delta, ndelta;
	float dist, dot;
	float ascender = 1;
	float descender = 0;
	int multi;
	int i, j, err;
    DrChar * ptchar = NULL;
    char lchar[4]="";
	if (text->len == 0)
		return;
    
	if (font->ft_face)
	{
            //		fz_lock(ctx, FZ_LOCK_FREETYPE);
		err = FT_Set_Char_Size((FT_Face)font->ft_face, 64, 64, 72, 72);
            //		if (err)
            //			fz_warn(ctx, "freetype set character size: %s", ft_error_string(err));
		ascender = (float)face->ascender / face->units_per_EM;
		descender = (float)face->descender / face->units_per_EM;
            //		fz_unlock(ctx, FZ_LOCK_FREETYPE);
        
	}
	else if (font->t3procs && !fz_is_empty_rect(&(font->bbox)))
	{
		ascender = font->bbox.y1;
		descender = font->bbox.y0;
	}
    
 	rect = fz_empty_rect;
    
	if (text->wmode == 0)
	{
		dir.x = 1;
		dir.y = 0;
	}
	else
	{
		dir.x = 0;
		dir.y = 1;
	}
    
	tm.e = 0;
	tm.f = 0;
	fz_concat(&trm,&tm, &ctm);
    
	fz_transform_vector(&dir, &trm);
	dist = sqrtf(dir.x * dir.x + dir.y * dir.y);
	ndir.x = dir.x / dist;
	ndir.y = dir.y / dist;
    
	size = fz_matrix_expansion(&trm);
    char *tfname = new char[32];
    tfname[0]='\0';
    strcpy(tfname, font->name);
    DrFontDescriptor::style tstyle;
    if (font->ft_bold || strstr(font->name, "Bold")!= NULL) {
        tstyle = DrFontDescriptor::FS_BOLD;
    }
    if (font->ft_italic || strstr(font->name, "Italic")!= NULL) {
        if (tstyle == DrFontDescriptor::FS_BOLD) {
            tstyle = DrFontDescriptor::FS_BOLD_ITALIC;
        }
        else {
            tstyle = DrFontDescriptor::FS_ITALIC;
        }
    }
    DrFontDescriptor *fontinfo = m_fontcache->FindDescriptor(tfname, size, tstyle);
    if (fontinfo == NULL) {
        m_fontcache->AddDescriptor(tfname, size, tstyle);
        fontinfo = m_fontcache->FindDescriptor(tfname, size, tstyle);
    }
    
	for (i = 0; i < text->len; i++)
	{
		/* Calculate new pen location and delta */
		tm.e = text->items[i].x;
		tm.f = text->items[i].y;
		fz_concat(&trm, &tm, &ctm);
        
		delta.x = x - trm.e;
		delta.y = y - trm.f;
		if (x == -1 && y == -1)
			delta.x = delta.y = 0;
        
		dist = sqrtf(delta.x * delta.x + delta.y * delta.y);
        
		/* Add space and newlines based on pen movement */
		if (dist > 0)
		{
			ndelta.x = delta.x / dist;
			ndelta.y = delta.y / dist;
			dot = ndelta.x * ndir.x + ndelta.y * ndir.y;
			if (fabsf(dot) > 0.95f && dist > size * SPACE_DIST && lchar[0] != ' ' && dist < size * LINE_DIST)
			{
				fz_rect spacerect;
				spacerect.x0 = -0.2f;
				spacerect.y0 = descender;
				spacerect.x1 = 0;
				spacerect.y1 = ascender;
				fz_transform_rect(&spacerect,&trm);
                
				DrChar *spacechar = new DrChar;
                spacechar->m_ucs = ' ';
                spacechar->m_font = fontinfo;
                spacechar->m_bbox.m_x0 = spacerect.x0;
                spacechar->m_bbox.m_x1 = spacerect.x1;
                spacechar->m_bbox.m_y0 = spacerect.y0;
                spacechar->m_bbox.m_y1 = spacerect.y1;
                charlist.push_back(spacechar);
				lchar[0] = ' ';
			}
        }
        
		/* Calculate bounding box and new pen position based on font metrics */
		if (font->ft_face)
		{
			FT_Fixed ftadv = 0;
			int mask = FT_LOAD_NO_BITMAP | FT_LOAD_NO_HINTING | FT_LOAD_IGNORE_TRANSFORM;
            
			/* TODO: freetype returns broken vertical metrics */
			/* if (text->wmode) mask |= FT_LOAD_VERTICAL_LAYOUT; */
            
                //			fz_lock(ctx, FZ_LOCK_FREETYPE);
			err = FT_Set_Char_Size((FT_Face)font->ft_face, 64, 64, 72, 72);
                //			if (err)
                //				fz_warn(ctx, "freetype set character size: %s", ft_error_string(err));
			FT_Get_Advance((FT_Face)font->ft_face, text->items[i].gid, mask, &ftadv);
			adv = ftadv / 65536.0f;
                //			fz_unlock(ctx, FZ_LOCK_FREETYPE);
            /*          rect.x0 = 0;
             rect.y0 = descender;
             rect.x1 = adv;
             rect.y1 = ascender;
             */
            fz_bound_glyph(m_ctx, font, text->items[i].gid, &trm,&rect);
		}
		else
		{
			adv = font->t3widths[text->items[i].gid];
            /*          rect.x0 = 0;
             rect.y0 = descender;
             rect.x1 = adv;
             rect.y1 = ascender;
             */
			fz_bound_glyph(m_ctx, font, text->items[i].gid, &trm,&rect);
		}
        
            //	rect = fz_transform_rect(trm, rect);
		x = trm.e + dir.x * adv;
		y = trm.f + dir.y * adv;
        
		/* Check for one glyph to many char mapping */
		for (j = i + 1; j < text->len; j++)
			if (text->items[j].gid >= 0)
				break;
		multi = j - i;
        
		if (multi == 1)
		{
                //			fz_add_text_char(ctx, dev, style, text->items[i].ucs, rect);
            if (text->items[i].ucs == ' ' && lchar[0] == ' ') {
                continue;
            }
            if (text->items[i].ucs == ' ') {
                rect.x0 = 0.0f;
				rect.y0 = descender;
				rect.x1 = 0.2f;
				rect.y1 = ascender;
				fz_transform_rect(&rect,&trm);
                    //            std::cout<<rect.x0<<" "<<rect.x1<<" "<<rect.y0<<" "<<rect.y1<<std::endl;
            }
             
            
            ptchar = new DrChar;
            ptchar->m_ucs = text->items[i].ucs;
            ptchar->m_font = fontinfo;
            ptchar->m_bbox.m_x0 = rect.x0;
            ptchar->m_bbox.m_x1 = rect.x1;
            ptchar->m_bbox.m_y0 = rect.y0;
            ptchar->m_bbox.m_y1 = rect.y1;
            charlist.push_back(ptchar);
            ptchar->GetUTF8String(lchar);
		}
		else
		{
			for (j = 0; j < multi; j++)
			{
                DrBox tmpbox(rect.x0,rect.y0,rect.x1,rect.y1);
				DrBox part = Split_bbox(tmpbox, j, multi);
                    //				fz_add_text_char(ctx, dev, style, text->items[i + j].ucs, part);
                ptchar = new DrChar;
                ptchar->m_font = (fontinfo);
                ptchar->m_bbox = part;

                charlist.push_back(ptchar);
			}
			i += j - 1;
		}
        
            //		dev->lastchar = text->items[i].ucs;
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