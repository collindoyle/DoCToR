//
//  DrPoint.h
//  DrEntity
//
//  Created by Chu Yimin on 2013/07/18.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#ifndef __DrEntity__DrPoint__
#define __DrEntity__DrPoint__

#include <iostream>

class DrPoint {
public:
    float x;
    float y;
    
public:
    DrPoint(float tx=0.0,float ty=0.0);
    float Dot(DrPoint &point);
    float Size();
    DrPoint & operator +(DrPoint & point);
    DrPoint & operator -(DrPoint & point);
};

#endif /* defined(__DrEntity__DrPoint__) */
