//
//  DrThumbnail.cpp
//  DrEntity
//
//  Created by Chu Yimin on 2013/05/17.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#include "DrThumbnail.h"

DrThumbnail::DrThumbnail(fz_context * ctx,fz_pixmap * pix, unsigned int pageno)
{
    m_ctx = ctx;
    m_pixmap = pix;
    m_pageno = pageno;
}

DrThumbnail::~DrThumbnail()
{
    if (m_pixmap) {
        fz_drop_pixmap(m_ctx, m_pixmap);
    }
    m_pixmap = NULL;
}

void DrThumbnail::WriteToFile(const char *filename)
{
    fz_write_png(m_ctx, m_pixmap, (char *)filename, 0);
}

unsigned char * DrThumbnail::GetSample()
{
    if (m_pixmap) {
        return fz_pixmap_samples(m_ctx, m_pixmap);
    }
    else return NULL;
}

int DrThumbnail::GetWidth()
{
    if (m_pixmap) {
        return fz_pixmap_width(m_ctx,m_pixmap);
    }
    else return 0;
}

int DrThumbnail::GetHeight()
{
    if (m_pixmap) {
        return fz_pixmap_height(m_ctx, m_pixmap);
    }
    else return 0;
}

void DrThumbnail::DrawLine(int x0, int y0, int x1, int y1, unsigned int color)
{
    int width = fz_pixmap_width(m_ctx, m_pixmap);
    int height = fz_pixmap_height(m_ctx, m_pixmap);
    int component = fz_pixmap_components(m_ctx, m_pixmap);
    unsigned char *data = fz_pixmap_samples(m_ctx, m_pixmap);
    unsigned char * p = (unsigned char *)&color;
    unsigned char r = *p;
    unsigned char g = *(p+1);
    unsigned char b = *(p+2);
    
    if (x1 == x0) {
        for (int i = y0; i < y1; i++) {
            int d = (i*width + x0) * component;
            data[d] = r;
            data[d+1] = g;
            data[d+2] = b;
        }
    }
    else if (y1 == y0) {
        for (int i = x0; i < x1; i++) {
            int d = (y0*width+i)*component;
            data[d] = r;
            data[d+1] = g;
            data[d+2] = b;
        }
    }
    else {
        float k = ((float)(y1-y0))/(x1-x0);
        for (int i = 0, j = 0, t = 0; i < (x1 - x0); i++,t++) {
            int d = ((y0+j)*width + i) * component;
            data[d] = r;
            data[d+1] = g;
            data[d+2] = b;
            if (t*k > 1) {
                t = 0;
                j++;
            }
        }
        
    }
}

void DrThumbnail::DrawRect(fz_irect *bbox, unsigned int color)
{
    DrawLine(bbox->x0, bbox->y0, bbox->x1, bbox->y0, color);
    DrawLine(bbox->x1, bbox->y0, bbox->x1, bbox->y1, color);
    DrawLine(bbox->x0, bbox->y1, bbox->x1, bbox->y1, color);
    DrawLine(bbox->x0, bbox->y0, bbox->x0, bbox->y1, color);
    
    return;
}