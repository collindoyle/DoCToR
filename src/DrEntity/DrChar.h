//
//  DrChar.h
//  DrEntity
//
//  Created by Chu Yimin on 2013/05/10.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#ifndef __DrEntity__DrChar__
#define __DrEntity__DrChar__

#include <iostream>
#include "DrFontDescriptor.h"
#include "DrBox.h"

/*
 Class DrChar presents the single character objects in the Document page.  
        
        Member variable m_ucs keeps the Unicode value of this character.
        Member variable m_font keeps pointer of the font descriptor.
        Member variable m_bbox is the bounding box of the character.
 */

class DrChar {
public:
    DrBox m_bbox;
    DrFontDescriptor * m_font;
    unsigned int m_ucs;
    
public:
    DrChar();
    ~DrChar();
    int GetUTF8String(char *str);
    int ConvertUTF8toUCS(char *str);
    
    void Initialize(DrBox &box, unsigned int content, DrFontDescriptor * font);
    DrFontDescriptor * GetFont();
    bool Equals(DrChar &achar);
};
#endif /* defined(__DrEntity__DrChar__) */
