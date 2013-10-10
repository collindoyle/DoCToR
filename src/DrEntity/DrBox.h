//
//  DrBox.h
//  DrEntity
//
//  Created by Chu Yimin on 2013/05/10.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#ifndef __DrEntity__DrBox__
#define __DrEntity__DrBox__

#include <iostream>
extern "C"
{
#include "mupdf/fitz.h"
}
/*
 The class of bounding box for every entity in the file.
 When the entities are edited/modified, the bounding boxes would change, too.
 One const object is empty_box(0.0,0.0,0.0,0.0)
 */
#include "DrPoint.h"
const float HORIZONAL_THRES = 3;
const float VERTICAL_THRES = (1.0/3);

class DrBox {
public:
    float m_x0;
    float m_y0;
    float m_x1;
    float m_y1;
    
public:
    DrBox();
    DrBox(float x0, float y0, float x1, float y1);
    DrBox(const DrBox & box);
    
    
    bool operator ==(const DrBox &box);     // Overload this operator to compare the const object empty_box
    DrBox& GrowBox(const DrBox & abox);     // Grow a bigger box which will contain itself and the parameter abox.
    bool IsInside(float x, float y) const;        // Check whether a point is in the box
    bool Intersect(const DrBox & box);      // Check whether two boxes have intersection
    void Distance(const DrBox &box, float &distancex, float &distancey);     // Calculate the distance of two boxes on x and y axis
    float Size();   // Calculate the size of the box
    float Ratio();  // The width/height ratio of the box. (0,1) is vertical, (1,+inf) is horizonal
    void BasePoint(float &x, float &y); // Get the base point (x,y) of the box. It is equal to the point (m_x0,m_y0)
    
    fz_irect * BoundBox(fz_irect * abox);
    
    bool Equals(DrBox &box);
    DrPoint Offset(DrBox &box);
};

static const DrBox empty_box(0,0,0,0);

#endif /* defined(__DrEntity__DrBox__) */
