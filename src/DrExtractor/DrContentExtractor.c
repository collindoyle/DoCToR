//
//  DrContentExtractor.c
//  DrExtractor
//
//  Created by Chu Yimin on 2013/05/23.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#include <stdio.h>


#include "DrContentExtractor.h"

extractlist * new_extract_list(fz_context * ctx)
{
    extractlist * newlist;
    newlist = (extractlist *)fz_malloc(ctx, sizeof(extractlist));
    newlist->phead = NULL;
    newlist->ptail = NULL;
    return newlist;
}

extractnode * new_extract_node(fz_context *ctx, fz_matrix ctm, fz_colorspace *colorspace, float *color, float alpha)
{
    extractnode * node;
    int i;
    node = (extractnode *)fz_malloc(ctx, sizeof(extractnode));
    node->ctm = ctm;
    node->bbox = fz_empty_rect;
    node->pNext = NULL;
    node->stroke = NULL;
    node->text = NULL;
    node->alpha = alpha;
    if (colorspace) {
        node->colorspace = fz_keep_colorspace(ctx, colorspace);
        if (color) {
            for (i = 0; i < node->colorspace->n; i++) {
                node->color[i] = color[i];
            }
        }
    }
    else{
        node->colorspace = NULL;
    }
    
    return node;
}

void append_extracted_node(extractlist * list, extractnode * node)
{
    if (list->phead == NULL) {
        list->phead = node;
        list->ptail = node;
    }
    else {
        
        (list->ptail)->pNext = node;
        list->ptail = node;
    }
}


void extract_fill_text(fz_device *dev, fz_text * text, fz_matrix *ctm, fz_colorspace *colorspace, float *color, float alpha)
{
    extractnode *node = new_extract_node(dev->ctx, *ctm,colorspace, color, alpha);
    fz_bound_text(dev->ctx, text, ctm, &(node->bbox));
    node->text = fz_clone_text(dev->ctx, text);
    append_extracted_node(dev->user, node);
}

fz_stroke_state *clone_stroke_state(fz_context *ctx, fz_stroke_state *stroke)
{
    fz_stroke_state * newstroke = (fz_stroke_state *)fz_malloc(ctx, sizeof(fz_stroke_state));
    *newstroke = *stroke;
    return newstroke;
}

void extract_stroke_text(fz_device *dev, fz_text *text, fz_stroke_state *stroke, fz_matrix *ctm, fz_colorspace *colorspace, float * color, float alpha)
{
    extractnode * node = new_extract_node(dev->ctx, *ctm, colorspace, color, alpha);
    fz_bound_text(dev->ctx, text, ctm, &(node->bbox));
    node->text = fz_clone_text(dev->ctx, text);
    node->stroke = clone_stroke_state(dev->ctx,stroke);
    append_extracted_node(dev->user, node);
}


void extract_clip_text(fz_device *dev, fz_text *text, fz_matrix *ctm, int accumulate)
{
    extractnode *node = new_extract_node(dev->ctx, *ctm, NULL, NULL, 0);
    fz_bound_text(dev->ctx, text, ctm, &(node->bbox));
    node->text = fz_clone_text(dev->ctx, text);
    append_extracted_node(dev->user, node);
}

void extract_clip_stroke_text(fz_device *dev, fz_text *text, fz_stroke_state *stroke, fz_matrix *ctm)
{
    extractnode *node = new_extract_node(dev->ctx, *ctm, NULL, NULL, 0);
    fz_bound_text(dev->ctx, text, ctm, &(node->bbox));
    node->text = fz_clone_text(dev->ctx, text);
    node->stroke = clone_stroke_state(dev->ctx, stroke);
    append_extracted_node(dev->user, node);
}

void extract_ignore_text(fz_device *dev, fz_text *text, fz_matrix *ctm)
{
    extractnode * node = new_extract_node(dev->ctx, *ctm, NULL, NULL, 0);
    fz_bound_text(dev->ctx, text, ctm, &(node->bbox));
    node->text = fz_clone_text(dev->ctx, text);
    append_extracted_node(dev->user, node);
}

fz_device * new_extract_device(fz_context *ctx, extractlist *list)
{
    fz_device *dev = fz_new_device(ctx, list);
    dev->fill_text = extract_fill_text;
    dev->stroke_text = extract_stroke_text;
    dev->clip_text = extract_clip_text;
    dev->clip_stroke_text = extract_clip_stroke_text;
    dev->ignore_text = extract_ignore_text;
    return dev;
}

void free_extract_content_node(fz_context *ctx, extractnode *node)
{
    fz_free_text(ctx, node->text);
}

void free_extract_content_list(fz_context *ctx , extractlist *list)
{
    extractnode * pnode = list->phead;
    while (pnode) {
        extractnode *pnext = pnode->pNext;
        free_extract_content_node(ctx, pnode);
        pnode = pnext;
    }
    fz_free(ctx,list);
}