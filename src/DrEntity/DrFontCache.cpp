//
//  DrFontCache.cpp
//  DrEntity
//
//  Created by Chu Yimin on 2013/05/10.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#include "DrFontCache.h"
#include <string.h>

DrFontCache::DrFontCache():m_fontnamelist(), m_DrFontList()
{
    
}

DrFontCache::~DrFontCache()
{
    for (std::list<DrFontDescriptor *>::iterator itfd = m_DrFontList.begin(); itfd != m_DrFontList.end(); itfd++) {
        delete *itfd;
    }
    m_DrFontList.clear();
    for (std::list<char *>::iterator itfn = m_fontnamelist.begin(); itfn != m_fontnamelist.end(); itfn++) {
        delete[] *itfn;
    }
    m_fontnamelist.clear();
}

DrFontDescriptor * DrFontCache::FindDescriptor(const char *fontname, float fontsize, int fontstyle)
{
    const char * FontNameInList = GetNamefromList(fontname);
    if (FontNameInList == NULL) {
        return NULL;
    }
    
    for (std::list<DrFontDescriptor *>::iterator itfd = m_DrFontList.begin(); itfd != m_DrFontList.end(); itfd++) {
        if ((*itfd)->m_fontname == FontNameInList &&
            (*itfd)->m_fontsize == fontsize &&
            (*itfd)->m_fontstyle == fontstyle) {
            return *itfd;
        }
    }
    return NULL;
}

void DrFontCache::AddDescriptor(const char *fontname, float fontsize, int fontstyle)
{
    char * fn;
    if (GetNamefromList(fontname) == NULL) {
        fn = new char[strlen(fontname)+1];
        strcpy(fn, fontname);
        m_fontnamelist.push_back(fn);
    }
    else
        fn = (char *)GetNamefromList(fontname);
    DrFontDescriptor * fd = new DrFontDescriptor(fn, fontsize, fontstyle);
    m_DrFontList.push_back(fd);
}

const char * DrFontCache::GetNamefromList(const char *fontname)
{
    for (std::list<char *>::iterator itfn = m_fontnamelist.begin(); itfn != m_fontnamelist.end(); itfn++) {
        if (strcmp(*itfn, fontname) == 0) {
            return (*itfn);
        }
    }    
    return NULL;
}