//
//  DrPDFExtractor.h
//  DrExtractor
//
//  Created by Chu Yimin on 2013/05/21.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#ifndef __DrExtractor__DrPDFExtractor__
#define __DrExtractor__DrPDFExtractor__

#include <iostream>
extern "C"
{
#include "mupdf/fitz.h"
}

#include "DrDocument.h"
#include "DrFontCache.h"

class DrPDFExtractor {
    char * m_filename;
    bool m_isopen;
    fz_context * m_ctx;
    fz_document * m_doc;
    DrFontCache * m_fontcache;
    
public:
    DrPDFExtractor();
    ~DrPDFExtractor();
    void Initialize();
    int OpenPDFFile(const char * filename);
    bool IsOpen();
    DrPage * ExtractPage(unsigned int pageno);
    void ExtractChars(std::list<DrChar*> &charlist, fz_text_page * fpage);
	DrBox & Split_bbox(DrBox &bbox, int i, int n);
    int GetPageCount();
};
#endif /* defined(__DrExtractor__DrPDFExtractor__) */
