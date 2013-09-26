//
//  DrBox.cpp
//  DrEntity
//
//  Created by Chu Yimin on 2013/05/10.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#include "DrBox.h"

DrBox::DrBox()
{
    (*this) = empty_box;
}

DrBox::DrBox(float x0, float y0, float x1, float y1)
{
    m_x0 = x0;
    m_y0 = y0;
    m_x1 = x1;
    m_y1 = y1;
}

DrBox::DrBox(const DrBox & box)
{
    m_x0 = box.m_x0;
    m_y0 = box.m_y0;
    m_x1 = box.m_x1;
    m_y1 = box.m_y1;
}

DrBox&  DrBox::GrowBox(const DrBox &abox)
{
    if ((*this) == empty_box) {
        (*this) = abox;
    }
    else
    {
        m_x0 = m_x0 < abox.m_x0 ? m_x0 : abox.m_x0;
        m_y0 = m_y0 < abox.m_y0 ? m_y0 : abox.m_y0;
        m_x1 = m_x1 > abox.m_x1 ? m_x1 : abox.m_x1;
        m_y1 = m_y1 > abox.m_y1 ? m_y1 : abox.m_y1;
    }
    return *this;
}

bool DrBox::operator==(const DrBox &box)
{
    if (m_x0 == box.m_x0 &&
        m_y0 == box.m_y0 &&
        m_x1 == box.m_x1 &&
        m_y1 == box.m_y1) {
        return true;
    }
    else
        return false;
}

bool DrBox::IsInside(float x, float y) const
{
    if ((x-m_x0)*(x-m_x1) < 0 &&
        (y-m_y0)*(y-m_y1) < 0) {
        return true;
    }
    else return false;
}

bool DrBox::Intersect(const DrBox &box)
{
    /*
     If two boxes intersect, there must be one or more than one vertexes inside one of the boxes.
     The relation is symmetric so need to check both ways
     */
    

    if (IsInside(box.m_x0, box.m_y0) ||
        IsInside(box.m_x0, box.m_y1) ||
        IsInside(box.m_x1, box.m_y0) ||
        IsInside(box.m_x1, box.m_y1)) {     // Check whether the vertexes of box in this
        return true;
    }
    
    if (box.IsInside(m_x0, m_y0) ||
        box.IsInside(m_x0, m_y1) ||
        box.IsInside(m_x1, m_y0) ||
        box.IsInside(m_x1, m_y1)) {         // Check whether the vertexes of this in box
        return true;
    }
    return false;
}

float DrBox::Size()
{
    return (m_x1 - m_x0) * (m_y1 - m_y0);
}

float DrBox::Ratio()
{
    if (*this == empty_box) {
        return 0;
    }
    return (m_x1 - m_x0) / (m_y1 - m_y0);  // (0,1) vertical, (1,+) horizonal
}

void DrBox::BasePoint(float &x, float &y)
{
    x = m_x0;
    y = m_y0;
}

void DrBox::Distance(const DrBox &box, float &distancex, float &distancey)
{
    if (Intersect(box)) {
        distancex = 0.0f, distancey = 0.0f;
        return;
    }
    
    if (m_x1 < box.m_x0) {
        distancex = box.m_x0 - m_x1;
    }
    else if (m_x0 > box.m_x1) {
        distancex = m_x0 - box.m_x1;
    }
    else
        distancex = 0.0f;
    
    if (m_y1 < box.m_y0) {
        distancey = box.m_y0 - m_y1;
    }
    else if (m_y0 > box.m_y1) {
        distancey = m_y0 - box.m_y1;
    }
    else
        distancey = 0.0f;
}

fz_irect * DrBox::BoundBox(fz_irect *abox)
{
    abox->x0 = floorf(m_x0);
    abox->x1 = ceilf(m_x1);
    abox->y0 = floorf(m_y0);
    abox->y1 = ceilf(m_y1);
    return abox;
}

bool DrBox::Equals(DrBox &box)
{
    bool result = true;
    if (fabs(m_x0 - box.m_x0) > 0.1) {
        result = false;
    }
    if (fabs(m_x1 - box.m_x1) > 0.1) {
        result = false;
    }
    if (fabs(m_y0 - box.m_y0) > 0.1) {
        result = false;
    }
    if (fabs(m_y1 - box.m_y1) > 0.1) {
        result = false;
    }
    return result;
}

DrPoint DrBox::Offset(DrBox &box)
{
    DrPoint point(0,0);
    
    if (Intersect(box)) {
        return point;
    }
    
    if (m_x0 > box.m_x1) {
        point.x = m_x0 - box.m_x1;
    }
    else if (m_x1 < box.m_x0)
    {
        point.x = m_x1 - box.m_x0;
    }
    
    if (m_y0 > box.m_y1) {
        point.y = m_y0 - box.m_y1;
    }
    else if (m_y1 < box.m_y0)
    {
        point.y = m_y1 - box.m_y0;
    }

    
    return point;
}