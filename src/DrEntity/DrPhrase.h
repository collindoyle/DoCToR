//
//  DrPhrase.h
//  DrEntity
//
//  Created by Chu Yimin on 2013/05/10.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#ifndef __DrEntity__DrPhrase__
#define __DrEntity__DrPhrase__

#include <iostream>
#include "DrChar.h"
#include <list>

enum eDirection {
    UNSET = 0,
    HORIZONAL = 1,
    VERTICAL = 2
};

/*
    Class DrPhrase represents a group of characters which are near with each other both on location and the reading order (according to the source file).
 */

class DrPhrase {
public:
    DrBox m_bbox;
    DrFontDescriptor * m_font;
    std::list<DrChar *> m_charlist;
    eDirection m_direction;
public:
    DrPhrase();
    ~DrPhrase();
    
    void AddChar(DrChar *achar);
    
    DrFontDescriptor * GetFont();
    std::list<DrChar *>& GetCharList();
    
    eDirection GetDirection();
    void SetDirection(eDirection dir);
    
    bool IsEmptyPhrase();
    bool IsSpacePhrase();
    
    bool Equals(DrPhrase &phrase);
};


#endif /* defined(__DrEntity__DrPhrase__) */
