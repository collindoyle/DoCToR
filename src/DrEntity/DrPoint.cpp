//
//  DrPoint.cpp
//  DrEntity
//
//  Created by Chu Yimin on 2013/07/18.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#include "DrPoint.h"
#include <math.h>

DrPoint::DrPoint(float tx, float ty)
{
    x = tx;
    y = ty;
}

float DrPoint::Dot(DrPoint &point)
{
    return x*point.x+y*point.y;
}

float DrPoint::Size()
{
    return sqrt(x*x+y*y);
}

DrPoint& DrPoint::operator+(DrPoint & point)
{
    DrPoint newpoint;
    newpoint.x = x+point.x;
    newpoint.y = x+point.y;
    return newpoint;
}

DrPoint & DrPoint::operator-(DrPoint &point)
{
    DrPoint newpoint;
    newpoint.x = x-point.x;
    newpoint.y = y-point.y;
    return newpoint;
}