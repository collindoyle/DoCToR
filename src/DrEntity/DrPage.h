//
//  DrPage.h
//  DrEntity
//
//  Created by Chu Yimin on 2013/05/17.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#ifndef __DrEntity__DrPage__
#define __DrEntity__DrPage__

#include <iostream>
#include <list>
#include "DrZone.h"
#include "DrThumbnail.h"

class DrPage {
public:
    DrBox m_pagebox;
    unsigned int m_pagenum;
    std::list<DrZone *> m_zonelist;
    DrThumbnail * m_thumbnail;
    
public:
    DrPage();
    ~DrPage();
    void AddZone(DrZone * zone);
    bool IsEmpty();
    unsigned int GetPageNo();
    void SetPageNo(unsigned int pageno);
    DrThumbnail * GetThumbnail();
    void OutputPNG(char * filename);
    void CalculatePageBox();
    bool Equals(DrPage & page);
};
#endif /* defined(__DrEntity__DrPage__) */
