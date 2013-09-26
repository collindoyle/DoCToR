//
//  DrLine.cpp
//  DrEntity
//
//  Created by Chu Yimin on 2013/05/13.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#include "DrLine.h"


DrLine::DrLine():m_bbox(),m_phraselist()
{
    m_linetag = INSIDELINE;
    m_direction = UNSET;
}

DrLine::~DrLine()
{
    if (!IsEmptyLine()) {
        for (std::list<DrPhrase *>::iterator itphrase = m_phraselist.begin(); itphrase != m_phraselist.end(); itphrase++) {
            delete *itphrase;
        }
        m_phraselist.clear();
    }
}

bool DrLine::IsEmptyLine()
{
    if (m_phraselist.empty()) {
        return true;
    }
    else return false;
}

void DrLine::AddPhrase(DrPhrase *phrase)
{
    if (IsEmptyLine()) {
        m_bbox = phrase->m_bbox;
        m_direction = phrase->GetDirection();
        m_phraselist.push_back(phrase);
    }
    else
    {
        m_bbox.GrowBox(phrase->m_bbox);
        m_phraselist.push_back(phrase);
    }
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

void DrLine::SetLineTag(DrLine::eLineTag tag)
{
    m_linetag = tag;
}

DrLine::eLineTag DrLine::GetLineTag()
{
    return m_linetag;
}

std::list<DrPhrase *> & DrLine::GetPhraseList()
{
    return m_phraselist;
}

bool DrLine::Equals(DrLine &line)
{
    bool result = true;
    if (m_linetag != line.m_linetag) {
        result = false;
        std::cout<<"linetag dismatch"<<std::endl;
    }
    if (!m_bbox.Equals(line.m_bbox)) {
        result = false;
        std::cout<<"bounding box dismatch"<<std::endl;
    }
    if (m_direction != line.m_direction) {
        result = false;
        std::cout<<"direction dismatch"<<std::endl;
    }
    if (m_phraselist.size() != line.m_phraselist.size()) {
        result = false;
    }
    std::list<DrPhrase *>::iterator itphrasea = m_phraselist.begin();
    std::list<DrPhrase *>::iterator itphraseb = line.m_phraselist.begin();
    for (; itphrasea != m_phraselist.end() && itphraseb != line.m_phraselist.end(); itphrasea++,itphraseb++) {
        if (!(*itphrasea)->Equals(*(*itphraseb))) {
            result = false;
            std::cout<<"phrase dismatch"<<std::endl;
            break;
        }
    }
    
    return result;
}