//
//  DrPage.cpp
//  DrEntity
//
//  Created by Chu Yimin on 2013/05/17.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#include "DrPage.h"


DrPage::DrPage(): m_pagebox(),m_zonelist()
{
    m_pagenum = -1;
    m_thumbnail = NULL;
}

DrPage::~DrPage()
{
    if (!IsEmpty()) {
        for (std::list<DrZone *>::iterator itzone = m_zonelist.begin(); itzone != m_zonelist.end(); itzone++) {
            delete *itzone;
        }
        m_zonelist.clear();
    }
    if (m_thumbnail) {
        delete m_thumbnail;
        m_thumbnail = NULL;
    }
}


void DrPage::AddZone(DrZone *zone)
{
    if (IsEmpty()) {
        m_pagebox = zone->m_bbox;
    }
    else
    {
        m_pagebox.GrowBox(zone->m_bbox);
        
    }
    m_zonelist.push_back(zone);
}

bool DrPage::IsEmpty()
{
    return m_zonelist.empty();
}

unsigned int DrPage::GetPageNo()
{
    return m_pagenum;
}

void DrPage::SetPageNo(unsigned int pageno)
{
    m_pagenum = pageno;
}

DrThumbnail * DrPage::GetThumbnail()
{
    return m_thumbnail;
}

void DrPage::OutputPNG(char * filename)
{
    for (std::list<DrZone *>::iterator itzone = m_zonelist.begin(); itzone != m_zonelist.end(); itzone++) {
        fz_irect box;
        (*itzone)->m_bbox.BoundBox(&box);
        m_thumbnail->DrawRect(&box, 0x0000ff);
    }
    m_thumbnail->WriteToFile(filename);
    
}

bool DrPage::Equals(DrPage &page)
{
    bool result = true;
    if (!m_pagebox.Equals(page.m_pagebox)) {
        result = false;
        std::cout<<"pagebox dismatch"<<std::endl;
    }
    if (m_pagenum != page.m_pagenum) {
        result = false;
        std::cout<<"pagenum dismatch"<<std::endl;
    }
    
    std::list<DrZone *>::iterator itzonea = m_zonelist.begin();
    std::list<DrZone *>::iterator itzoneb = page.m_zonelist.begin();
    for (; itzonea != m_zonelist.end() && itzoneb != page.m_zonelist.end(); itzonea++,itzoneb++) {
        if (!(*itzonea)->Equals(*(*itzoneb))) {
            result = false;
            std::cout<<"zone dismatch"<<std::endl;
            break;
        }
    }
    
    return result;
}

void DrPage::CalculatePageBox()
{
    DrBox newbox;
    for (std::list<DrZone *>::iterator itzone = m_zonelist.begin(); itzone != m_zonelist.end(); itzone++) {
        newbox.GrowBox((*itzone)->m_bbox);
    }
    m_pagebox = newbox;
}