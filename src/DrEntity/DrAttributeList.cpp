//
//  DrAttributeList.cpp
//  DrEntity
//
//  Created by Chu Yimin on 2013/06/28.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#include "DrAttributeList.h"

DrAttributeList::DrAttributeList()
{
    m_label = NONE;
    m_aversize = 0.0;
    m_charcount = 0;
    m_spacebefore = 0.0;
    m_spacein = 0.0;
    m_align = LEFT;
    m_style = DrFontDescriptor::FS_NONE;
}