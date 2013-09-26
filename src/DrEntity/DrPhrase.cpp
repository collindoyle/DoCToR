//
//  DrPhrase.cpp
//  DrEntity
//
//  Created by Chu Yimin on 2013/05/10.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#include "DrPhrase.h"


DrPhrase::DrPhrase(): m_bbox(),m_charlist()
{
    m_font = NULL;
    m_direction = UNSET;
}

DrPhrase::~DrPhrase()
{
    for (std::list<DrChar *>::iterator itchar = m_charlist.begin(); itchar != m_charlist.end(); itchar++) {
        delete *itchar;
    }
    m_charlist.clear();
}

void DrPhrase::AddChar(DrChar *achar)
{
    if (this->IsEmptyPhrase()) {
        m_bbox = achar->m_bbox;
        m_font = achar->GetFont();
        m_charlist.push_back(achar);
    }
    else
    {
        m_bbox.GrowBox(achar->m_bbox);
        m_charlist.push_back(achar);
    }
    if (m_bbox.Ratio() > HORIZONAL_THRES) {
        SetDirection(HORIZONAL);
    }
    else if (m_bbox.Ratio() < VERTICAL_THRES && m_bbox.Ratio() > 0)
    {
        SetDirection(VERTICAL);
    }
    else
        SetDirection(UNSET);
}

DrFontDescriptor * DrPhrase::GetFont()
{
    return m_font;
}

std::list<DrChar *> & DrPhrase::GetCharList()
{
    return m_charlist;
}

eDirection DrPhrase::GetDirection()
{
    return m_direction;
}

void DrPhrase::SetDirection(eDirection dir)
{
    m_direction = dir;
}

bool DrPhrase::IsEmptyPhrase()
{
    if (m_charlist.size() == 0) {
        return true;
    }
    else
        return false;
}

bool DrPhrase::IsSpacePhrase()
{
    bool tag = true;
    for (std::list<DrChar *>::iterator itchar = m_charlist.begin(); itchar != m_charlist.end(); itchar++) {
        if ((*itchar)->m_ucs != ' ') {
            tag = false;
        }
    }
    return tag;
}

bool DrPhrase::Equals(DrPhrase &phrase)
{
    bool result = true;
/*    if (m_direction != phrase.m_direction) {
        result = false;
    }
*/
    if (!m_bbox.Equals(phrase.m_bbox)) {
        result = false;
        std::cout<<"bounding box dismatch"<<std::endl;
    }
    if (!m_font->Equals(*phrase.m_font)) {
        result = false;
        std::cout<<"font dismatch"<<std::endl;
    }
    if (m_charlist.size() != phrase.m_charlist.size()) {
        result = false;
        std::cout<<"char length dismatch"<<std::endl;
    }
    std::list<DrChar *>::iterator itchara = m_charlist.begin();
    std::list<DrChar *>::iterator itcharb = phrase.m_charlist.begin();
    for (; itchara != m_charlist.end()&&itcharb != phrase.m_charlist.end(); itchara++,itcharb++) {
        if (!(*itchara)->Equals(*(*itcharb))) {
            result = false;
            std::cout<<"char dismatch"<<std::endl;
            break;
        }
    }
    return result;
}