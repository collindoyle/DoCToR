//
//  DrFontCache.h
//  DrEntity
//
//  Created by Chu Yimin on 2013/05/10.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#ifndef __DrEntity__DrFontCache__
#define __DrEntity__DrFontCache__

#include <iostream>
#include "DrFontDescriptor.h"
#include <list>

class DrFontCache {
private:
    std::list<char *> m_fontnamelist;
    std::list<DrFontDescriptor *> m_DrFontList;
    
public:
    DrFontCache();
    ~DrFontCache();
    DrFontDescriptor * FindDescriptor(const char * fontname, float fontsize, int fontstyle);
    void AddDescriptor(const char * fontname, float fontsize, int fontstyle);

protected:
    const char * GetNamefromList(const char * fontname);
};

#endif /* defined(__DrEntity__DrFontCache__) */
