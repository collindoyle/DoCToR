//
//  DrLine.h
//  DrEntity
//
//  Created by Chu Yimin on 2013/05/13.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#ifndef __DrEntity__DrLine__
#define __DrEntity__DrLine__

#include <iostream>
#include "DrPhrase.h"
#include "DrAttributeList.h"
#include <list>

class DrLine {
public:
    DrBox m_bbox;
    std::list<DrPhrase *> m_phraselist;
    enum eLineTag
    {
        STARTLINE = 1,
        INSIDELINE = 2,
        ENDLINE = 3
    } m_linetag;
    eZoneLabel label;
    eDirection m_direction;
public:
    DrLine();
    ~DrLine();
    
    void AddPhrase(DrPhrase * phrase);
    
    std::list<DrPhrase *> & GetPhraseList();
    bool IsEmptyLine();
    
    void SetLineTag(eLineTag tag);
    eLineTag GetLineTag();
    
    bool Equals(DrLine &line);
};

#endif /* defined(__DrEntity__DrLine__) */
