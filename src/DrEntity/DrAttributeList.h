//
//  DrAttributeList.h
//  DrEntity
//
//  Created by Chu Yimin on 2013/06/28.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#ifndef __DrEntity__DrAttributeList__
#define __DrEntity__DrAttributeList__

#include <iostream>
#include "DrFontDescriptor.h"

enum eZoneLabel {
    NONE = 0,
    TITLE,
    TEXTBODY,
    ABSTRACT,
    AUTHOR,
    KEYWORD,
    ALLIFICATION,
    CAPTION,
    PAGENUM,
    SUBTITLE,
    REFERENCE,
    START,
    END
};

enum ealign {
    LEFT = 0,
    CENTER = 1,
    RIGHT = 2
    };
class DrAttributeList {
public:
    eZoneLabel  m_label;
    float               m_aversize;
    int                 m_charcount;
    float               m_spacebefore;
    float               m_spacein;
    float               m_spaceafter;
    int                 m_align;
    DrFontDescriptor::style m_style;
public:
    DrAttributeList();
};

#endif /* defined(__DrEntity__DrAttributeList__) */
