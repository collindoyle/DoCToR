//
//  DrFontDescriptor.h
//  DrEntity
//
//  Created by Chu Yimin on 2013/05/10.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#ifndef __DrEntity__DrFontDescriptor__
#define __DrEntity__DrFontDescriptor__

#include <iostream>

class DrFontDescriptor {
public:
    const char * m_fontname;
    float m_fontsize;
    enum style
    {
        FS_NONE = 0,
        FS_BOLD = 1,
        FS_ITALIC = 2,
        FS_BOLD_ITALIC = 3
    } m_fontstyle;
public:
    DrFontDescriptor(const char * name, float size, int style);
    DrFontDescriptor(const DrFontDescriptor & fd);
    ~DrFontDescriptor();
    
    bool IsBold();
    bool IsItalic();
    
    bool Equals(DrFontDescriptor & fd);
};
#endif /* defined(__DrEntity__DrFontDescriptor__) */
