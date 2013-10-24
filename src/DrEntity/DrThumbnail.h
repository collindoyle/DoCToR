//
//  DrThumbnail.h
//  DrEntity
//
//  Created by Chu Yimin on 2013/05/17.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#ifndef __DrEntity__DrThumbnail__
#define __DrEntity__DrThumbnail__

#include <iostream>
extern "C"
{
#include "mupdf/fitz.h"
}

class DrThumbnail {
public:
    fz_pixmap * m_pixmap;
    unsigned int m_pageno;
    fz_context * m_ctx;
    
public:
    DrThumbnail(fz_context * ctx, fz_pixmap * pix, unsigned int pageno);
    ~DrThumbnail();
    unsigned char * GetSample();
    int GetWidth();
    int GetHeight();
    void WriteToFile(const char * filename);
    void DrawLine(int x0, int y0, int x1, int y1, unsigned int color);
    void DrawRect(fz_irect * bbox, unsigned int color);
};

#endif /* defined(__DrEntity__DrThumbnail__) */
