//
//  DrAnalyzer.cpp
//  DrAnalyzer
//
//  Created by Chu Yimin on 2013/06/28.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#include "DrAnalyzer.h"
#include <fstream>
#include <iostream>
#include <map>




DrAnalyzer::DrAnalyzer()
{
    m_doc = NULL;
}

DrAnalyzer::~DrAnalyzer()
{
    
}

void DrAnalyzer::SetDocument(DrDocument *pdoc)
{
    m_doc = pdoc;
}

void DrAnalyzer::CalculateAttributes(DrPage &page, const char * outputfilename)
{
    std::map<float, int> pagefontmap;
    
    
    for (std::list<DrZone *>::iterator itzone = page.m_zonelist.begin(); itzone != page.m_zonelist.end(); itzone++) {
        std::map<float, int> zonefontmap;
        DrZone * pzone = *itzone;
        pzone->m_attr = new DrAttributeList;
        float leftalign, rightalign, centeralign;
        leftalign = rightalign = centeralign = 0.0;
        float centerzone;
        float inzonedistance = 0.0;
        int linecount = 0;
        int charcount = 0;
        if (pzone->m_label != NONE) {
            pzone->m_attr->m_label = pzone->m_label;
        }
        if ((*itzone)->m_direction == HORIZONAL) {
            centerzone = ((*itzone)->m_bbox.m_x1 + (*itzone)->m_bbox.m_x0)/2;
        }
        else
        {
            centerzone = ((*itzone)->m_bbox.m_y1 + (*itzone)->m_bbox.m_y0)/2;
        }
        
        
        // Compare with every zone on this page and get the closest zone, calculate the offset
        DrPoint prev, next;
        for (std::list<DrZone *>::iterator itcomp = page.m_zonelist.begin(); itcomp != page.m_zonelist.end(); itcomp++) {
            if (itcomp == itzone) {
                continue;
            }
            DrPoint offset = (*itzone)->m_bbox.Offset((*itcomp)->m_bbox);
            if ((*itzone)->m_direction == VERTICAL) {
                if (offset.x < 0) {
                    if (prev.Size() == 0) {
                        prev = offset;
                    }
                    else
                    {
                        if (prev.Size() > offset.Size()) {
                            prev = offset;
                        }
                    }
                }
                else if (offset.x > 0)
                {
                    if (next.Size() == 0) {
                        next = offset;
                    }
                    else
                    {
                        if (next.Size() > offset.Size()) {
                            next = offset;
                        }
                    }
                }
            }

            else  {
                if (offset.y > 0) {
                    if (prev.Size() == 0) {
                        prev = offset;
                    }
                    else
                    {
                        if (prev.Size() > offset.Size()) {
                            prev = offset;
                        }
                    }
                }
                else if (offset.y < 0)
                {
                    if (next.Size() == 0) {
                        next = offset;
                    }
                    else
                    {
                        if (next.Size() > offset.Size()) {
                            next = offset;
                        }
                    }
                }
            }
        }
        
        if ((*itzone)->m_direction == VERTICAL)
        {
            (*itzone)->m_attr->m_spacebefore = prev.x;
            (*itzone)->m_attr->m_spaceafter = next.x;
        }
        else {
            (*itzone)->m_attr->m_spacebefore = prev.y;
            (*itzone)->m_attr->m_spaceafter = next.y;
        }
        
        
        for (std::list<DrLine *>::iterator itline = pzone->m_linelist.begin(); itline != pzone->m_linelist.end(); itline++) {
            DrLine *pline = *itline;

            for (std::list<DrPhrase *>::iterator itphrase = pline->m_phraselist.begin(); itphrase != pline->m_phraselist.end(); itphrase++) {
                
                // count Most used font size
                DrFontDescriptor * pfont = (*itphrase)->m_font;
                if (pfont->m_fontstyle != DrFontDescriptor::FS_NONE) {
                    pzone->m_attr->m_style = pfont->m_fontstyle;
                }
                charcount += (*itphrase)->m_charlist.size();
                std::map<float,int>::iterator itlocalmap = zonefontmap.find(pfont->m_fontsize);
                if (itlocalmap == zonefontmap.end()) {
                    zonefontmap.insert(std::pair<float,int>(pfont->m_fontsize, (*itphrase)->m_charlist.size()));
                }
                else
                {
                    itlocalmap->second += (*itphrase)->m_charlist.size();
                }
                
                std::map<float, int>::iterator itpagemap = pagefontmap.find(pfont->m_fontsize);
                if (itpagemap == pagefontmap.end()) {
                    pagefontmap.insert(std::pair<float,int>(pfont->m_fontsize, (*itphrase)->m_charlist.size()));
                }
                else
                {
                    itpagemap->second += (*itphrase)->m_charlist.size();
                }
            }
            
            // calculate spacing between lines
            std::list<DrLine *>::iterator itnext = (++itline)--;
            if (itnext != pzone->m_linelist.end()) {
                float x, y;
                (*itline)->m_bbox.Distance((*itnext)->m_bbox,x,y);
                if (pzone->m_direction == HORIZONAL) {
                    inzonedistance += y;
                }
                else
                {
                    inzonedistance += x;
                }
                linecount ++;
            }
            // calculate the alignment
            if ((*itzone)->m_direction == HORIZONAL) {
                leftalign+= fabs(((*itline)->m_bbox).m_x0-(*itzone)->m_bbox.m_x0);
                rightalign += fabs(((*itline)->m_bbox.m_x1)-(*itzone)->m_bbox.m_x1);
                float centerline = ((*itline)->m_bbox.m_x0 + (*itline)->m_bbox.m_x1)/2;
                centeralign += fabs(centerline - centerzone);
            }
            else
            {
                leftalign+= fabs(((*itline)->m_bbox).m_y0 - (*itzone)->m_bbox.m_y0);
                rightalign += fabs(((*itline)->m_bbox).m_y1 - (*itzone)->m_bbox.m_y1);
                float centerline = ((*itline)->m_bbox.m_y0 + (*itline)->m_bbox.m_y1)/2;
                centeralign += fabs(centerline - centerzone);
            }

        }
        if (linecount != 0) {
            pzone->m_attr->m_spacein = inzonedistance/linecount;
        }

        float minalignaccum = centeralign < rightalign ? centeralign : rightalign;
        minalignaccum = minalignaccum < leftalign ? minalignaccum : leftalign;
        
        if (minalignaccum == leftalign) {
            (*itzone)->m_attr->m_align = ealign::LEFT;
        }
        else if (minalignaccum == centeralign)
        {
            (*itzone)->m_attr->m_align = ealign::CENTER;
        }
        else
        {
            (*itzone)->m_attr->m_align = ealign::RIGHT;
        }
        int maxcount = 0; float fontsize = 0.0;
        for(std::map<float, int>::iterator itmap = zonefontmap.begin(); itmap != zonefontmap.end(); itmap++)
        {
            if (itmap->second > maxcount) {
                maxcount = itmap->second;
                fontsize = itmap->first;
            }
        }
        pzone->m_attr->m_aversize = fontsize;
        pzone->m_attr->m_charcount = charcount;
    }
    
    int pagefontcount = 0;
    float mostfontsize = 0.0;
    for (std::map<float, int>::iterator itpagefontmap = pagefontmap.begin(); itpagefontmap != pagefontmap.end(); itpagefontmap++) {
        if (itpagefontmap->second > pagefontcount) {
            pagefontcount = itpagefontmap->second;
            mostfontsize = itpagefontmap->first;
        }
    }
    
    // Normalization
    for (std::list<DrZone *>::iterator itzone = page.m_zonelist.begin(); itzone != page.m_zonelist.end(); itzone++) {
        (*itzone)->m_attr->m_aversize = (*itzone)->m_attr->m_aversize/mostfontsize;
    }
    std::ofstream outf;
    outf.open(outputfilename,std::ios_base::app);
    for (std::list<DrZone *>::iterator itzone = page.m_zonelist.begin(); itzone != page.m_zonelist.end(); itzone++) {
        DrAttributeList * attr = (*itzone)->m_attr;
        outf<<attr->m_label<<" 1:"<<attr->m_aversize<<" 2:"<<attr->m_charcount<<" 3:"<<attr->m_align<<" 4:"<<attr->m_spacein<<" 5:"<<attr->m_spacebefore<<" 6:"<<attr->m_spaceafter<<" 7:"<<attr->m_style<<std::endl;
    }
    outf.close();
    
}