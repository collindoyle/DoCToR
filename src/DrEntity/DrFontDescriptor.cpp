//
//  DrFontDescriptor.cpp
//  DrEntity
//
//  Created by Chu Yimin on 2013/05/10.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#include "DrFontDescriptor.h"
#include <cmath>
#include <string.h>

DrFontDescriptor::DrFontDescriptor(const char * name, float size, int fstyle)
{
    m_fontname = name;
    m_fontsize = size;
    if (fstyle > FS_BOLD_ITALIC || fstyle < FS_NONE) {
        m_fontstyle = FS_NONE;
    }
    else
        m_fontstyle = (style)fstyle;
}

DrFontDescriptor::DrFontDescriptor(const DrFontDescriptor & fd)
{
    m_fontname = fd.m_fontname;
    m_fontsize = fd.m_fontsize;
    m_fontstyle = fd.m_fontstyle;
}

DrFontDescriptor::~DrFontDescriptor()
{
}

bool DrFontDescriptor::IsBold()
{
    if (m_fontstyle == FS_BOLD || m_fontstyle == FS_BOLD_ITALIC) {
        return true;
    }
    
    if (strcasestr(m_fontname,"Bold")) {
        return true;
    }
    return false;
}

bool DrFontDescriptor::IsItalic()
{
    if (m_fontstyle == FS_ITALIC || m_fontstyle == FS_BOLD_ITALIC) {
        return true;
    }
    
    if (strcasestr(m_fontname, "Italic")) {
        return true;
    }
    return false;
}

bool DrFontDescriptor::Equals(DrFontDescriptor &fd)
{
    bool result = true;
    if (strcmp(m_fontname, fd.m_fontname)!= 0) {
        result = false;
    }
    if (fabs(m_fontsize - fd.m_fontsize) > 0.1 ||
        m_fontstyle != fd.m_fontstyle) {
        result = false;
    }
    return result;
}

