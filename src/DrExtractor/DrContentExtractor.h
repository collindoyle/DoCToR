//
//  DrContentExtractor.h
//  DrExtractor
//
//  Created by Chu Yimin on 2013/05/23.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#ifndef DrExtractor_DrContentExtractor_h
#define DrExtractor_DrContentExtractor_h

#include "fitz-internal.h"

typedef struct _extractnode extractnode;
typedef struct _extractlist extractlist;

struct _extractnode {
    extractnode *pNext;
    fz_rect bbox;
    fz_text * text;
    fz_stroke_state * stroke;
    fz_matrix ctm;
    fz_colorspace *colorspace;
    float alpha;
    float color[FZ_MAX_COLORS];
};

struct _extractlist {
    extractnode * phead;
    extractnode * ptail;
};


extractlist * new_extract_list(fz_context *ctx);
fz_device * new_extract_device(fz_context *ctx, extractlist * anextractlist);
void free_extract_content_list(fz_context *ctx, extractlist * alist);

#endif
