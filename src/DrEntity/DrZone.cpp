//
//  DrZone.cpp
//  DrEntity
//
//  Created by Chu Yimin on 2013/05/16.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#include "DrZone.h"
#include "DrAttributeList.h"
DrZone::DrZone(): m_bbox(),m_linelist()
{
    m_label = NONE;
    m_direction = UNSET;
    m_attr = NULL;
}

DrZone::~DrZone()
{
    if(!IsEmptyZone())
    {
        for (std::list<DrLine *>::iterator itline = m_linelist.begin(); itline != m_linelist.end(); itline++) {
            delete *itline;
        }
        m_linelist.clear();
    }
    if (m_attr) {
        delete m_attr;
    }
}


void DrZone::AddLine(DrLine *line)
{
    if (IsEmptyZone()) {
        m_bbox = line->m_bbox;
        m_direction = line->m_direction;
    }
    else
    {
        m_bbox.GrowBox(line->m_bbox);
    }
    m_linelist.push_back(line);
    if (m_direction == UNSET) {
        if (m_bbox.Ratio() > HORIZONAL_THRES) {
            m_direction = HORIZONAL;
        }
        else if (m_bbox.Ratio() < VERTICAL_THRES && m_bbox.Ratio() > 0)
        {
            m_direction = VERTICAL;
        }
    }
    
}

std::list<DrLine *>& DrZone::GetLineList()
{
    return m_linelist;
}

bool DrZone::IsEmptyZone()
{
    return m_linelist.empty();
}

eDirection DrZone::GetDirection()
{
    return m_direction;
}

eZoneLabel DrZone::GetLabel()
{
    return m_label;
}

void DrZone::SetLabel(eZoneLabel label)
{
    m_label = label;
}

bool DrZone::Equals(DrZone &zone)
{
    bool result = true;
    if (m_label != zone.m_label) {
        result = false;
        std::cout<<"label dismatch"<<std::endl;
    }
    if (!m_bbox.Equals(zone.m_bbox)) {
        result = false;
        std::cout<<"bounding box dismatch"<<std::endl;
    }
    if (m_direction != zone.m_direction) {
        result = false;
        std::cout<<"direction dismatch"<<std::endl;
    }
    std::list<DrLine *>::iterator itlinea = m_linelist.begin();
    std::list<DrLine *>::iterator itlineb = zone.m_linelist.begin();
    for (; itlinea != m_linelist.end() && itlineb != zone.m_linelist.end(); itlinea++,itlineb++) {
        if (!(*itlinea)->Equals(*(*itlineb))) {
            result = false;
            std::cout<<"line dismatch"<<std::endl;
            break;
        }
    }
    return result;
}