//
//  DrZone.h
//  DrEntity
//
//  Created by Chu Yimin on 2013/05/16.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#ifndef __DrEntity__DrZone__
#define __DrEntity__DrZone__

#include <iostream>
#include "DrLine.h"
#include <list>
#include "DrAttributeList.h"


class DrZone {
public:
    DrBox m_bbox;
    std::list<DrLine *> m_linelist;
    eZoneLabel m_label;
    eDirection m_direction;
    DrAttributeList *m_attr;
    
public:
    DrZone();
    ~DrZone();
    
    void AddLine(DrLine * line);
    std::list<DrLine *> & GetLineList();
    bool IsEmptyZone();
    eDirection GetDirection();
    eZoneLabel GetLabel();
    void SetLabel(eZoneLabel label);
    bool Equals(DrZone &zone);
};
#endif /* defined(__DrEntity__DrZone__) */
