//
//  DrTexGrouper.cpp
//  DrExtractor
//
//  Created by Chu Yimin on 2013/05/23.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#include "DrTexGrouper.h"


void DrTextGrouper::TextGroup(std::list<DrPhrase *> &phraselist, std::list<DrChar *> &charlist)
{
    std::cout<<charlist.size()<<std::endl;
    DrPhrase * phrase = new DrPhrase;
    for (std::list<DrChar *>::iterator itchar = charlist.begin(); itchar != charlist.end(); itchar++) {
        if (Groupable(phrase, *itchar)) {
            phrase->AddChar(*itchar);
        }
        else
        {
            if (!phrase->IsEmptyPhrase()) {
                phraselist.push_back(phrase);
            }
            phrase = new DrPhrase;
            phrase->AddChar(*itchar);
        }
    }
    if (!phrase->IsEmptyPhrase()) {
        phraselist.push_back(phrase);
    }
    charlist.clear();
    std::cout<<phraselist.size()<<std::endl;
}

void DrTextGrouper::TextGroup(std::list<DrLine *> &linelist, std::list<DrPhrase *> &phraselist)
{
    DrLine * line = new DrLine;
    for (std::list<DrPhrase *>::iterator itphrase = phraselist.begin(); itphrase != phraselist.end(); itphrase++) {
        if (Groupable(line, *itphrase)) {
            line->AddPhrase(*itphrase);
        }
        else
        {
            if (!line->IsEmptyLine()) {
                linelist.push_back(line);
            }

            line = new DrLine;
            line->AddPhrase(*itphrase);
        }
        
    }
    if (!line->IsEmptyLine()) {
        linelist.push_back(line);
    }
    phraselist.clear();
    std::cout<<linelist.size()<<std::endl;
}

void DrTextGrouper::TextGroup(std::list<DrZone *> &zonelist, std::list<DrLine *> &linelist)
{
    DrZone * zone = new DrZone;
    DrTextGrouper::LineClassification(linelist);
    for (std::list<DrLine *>::iterator itline = linelist.begin(); itline != linelist.end(); itline++) {
        if (Groupable(zone, *itline)) {
            zone->AddLine(*itline);
            zone->m_label = (*itline)->label;
        }
        else
        {
            if (!zone->IsEmptyZone()) {
                zonelist.push_back(zone);
            }

            zone = new DrZone;
            zone->m_label = (*itline)->label;
            zone->AddLine(*itline);
        }
    }
    if (!zone->IsEmptyZone()) {
        zonelist.push_back(zone);
    }
    linelist.clear();
    std::cout<<zonelist.size()<<std::endl;
}

bool DrTextGrouper::Groupable(DrZone *zone, DrLine *line)
{
    if (zone->IsEmptyZone()) {
        return true;
    }
    float distancex, distancey;
    zone->m_bbox.Distance(line->m_bbox,distancex, distancey);
    if (distancex > 0 && distancey > 0) {
        return false;
    }
    if (zone->m_label != line->label && zone->m_label != NONE) {
        return false;
    }
    DrLine::eLineTag curtag = line->m_linetag;
    DrLine::eLineTag lasttag = (*(zone->m_linelist.rbegin()))->m_linetag;
    if (lasttag == DrLine::ENDLINE) {
        return false;
    }
    if (curtag == DrLine::STARTLINE) {
        return false;
    }
    return true;
}

bool DrTextGrouper::Groupable(DrLine *line, DrPhrase *phrase)
{
    if (line->IsEmptyLine()) {
        return true;
    }
    eDirection linedir = line->m_direction;
    eDirection phrasedir = phrase->m_direction;
    if (linedir == HORIZONAL) {
        if (phrasedir == VERTICAL) {
            return false;
        }
        float distancex, distancey;
        line->m_bbox.Distance(phrase->m_bbox, distancex, distancey);
        if (distancex > 0 && distancey > 0) {
            return false;
        }
        if (distancey == 0) {
            if (distancex < phrase->GetFont()->m_fontsize * 5) {
                return true;
            }
        }
        return false;
    }
    if (linedir == VERTICAL) {
        if (phrasedir == HORIZONAL) {
            return false;
        }
        float distancex, distancey;
        line->m_bbox.Distance(phrase->m_bbox, distancex, distancey);
        if (distancex > 0 && distancey > 0) {
            return false;
        }
        if (distancex == 0) {
            if (distancey < phrase->GetFont()->m_fontsize * 5) {
                return true;
            }
        }
        return false;
        
    }
    if (linedir == UNSET) {
        float distancex, distancey;
        line->m_bbox.Distance(phrase->m_bbox, distancex, distancey);
        if (phrasedir == HORIZONAL) {
            if (distancey > 0 ) {
                return false;
            }
            if (distancex < phrase->GetFont()->m_fontsize * 5) {
                return true;
            }
            return false;
        }
        if (phrasedir == VERTICAL) {
            if (distancex > 0) {
                return false;
            }
            if (distancey < phrase->GetFont()->m_fontsize * 5) {
                return true;
            }
            return false;
        }
        if (phrasedir == UNSET) {
            if (distancex > 0 && distancey > 0 ) {
                return false;
            }
            return true;
        }
        
    }
    return false;
}

bool DrTextGrouper::Groupable(DrPhrase *phrase, DrChar *achar)
{
    if (phrase->IsEmptyPhrase()) {
        return true;
    }
    DrFontDescriptor * fdchar = achar->GetFont();
    DrFontDescriptor * fdphrase = phrase->GetFont();
    if (fdchar != fdphrase) {
        return false;
    }
    if (phrase->IsSpacePhrase()) {
        return false;
    }
    if (phrase->GetDirection() == UNSET) {
        DrBox tmpbox = phrase->m_bbox;
        tmpbox.GrowBox(achar->m_bbox);
        float sizethre = phrase->m_bbox.Size() > achar->m_bbox.Size() ? phrase->m_bbox.Size() : achar->m_bbox.Size();
        if (tmpbox.Size() - phrase->m_bbox.Size() > 2 * sizethre) {
            return false;
        }
        float distancex, distancey;
        phrase->m_bbox.Distance(achar->m_bbox, distancex, distancey);
        if (distancex > 2 * achar->GetFont()->m_fontsize &&
            distancey > 2 * achar->GetFont()->m_fontsize) {
            return false;
        }
        return true;
    }
    if (phrase->GetDirection() == HORIZONAL) {
        float distancex, distancey;
        phrase->m_bbox.Distance(achar->m_bbox, distancex, distancey);
        if (distancey > 0) {
            return false;
        }
        if (distancex > 4 * achar->GetFont()->m_fontsize) {
            return false;
        }
        return true;
    }
    if (phrase->GetDirection() == VERTICAL) {
        float distancex, distancey;
        phrase->m_bbox.Distance(achar->m_bbox, distancex, distancey);
        if (distancex > 0) {
            return false;
        }
        if (distancey > 4 * achar->GetFont()->m_fontsize) {
            return false;
        }
        return true;
    }
    return false;
}

void DrTextGrouper::LineClassification(std::list<DrLine *> &linelist)
{
    for (std::list<DrLine *>::iterator itline = linelist.begin(); itline != linelist.end(); itline++) {
        if (itline == linelist.begin()) {
            (*itline)->m_linetag = DrLine::STARTLINE;
            continue;
        }
        else
        {
            std::list<DrLine *>::iterator itprev = (--itline)++;
            std::list<DrLine *>::iterator itnext = (++itline)--;
            if (itnext == linelist.end()) {
                (*itline)->m_linetag = DrLine::ENDLINE;
                continue;
            }
            else
            {
                if ((*itprev)->m_direction != (*itline)->m_direction) {
                    (*itline)->m_linetag = DrLine::STARTLINE;
                    continue;
                }
                if ((*itnext)->m_direction != (*itline)->m_direction) {
                    (*itline)->m_linetag = DrLine::ENDLINE;
                    continue;
                }
                float distanceprevx, distanceprevy;
                float distancenextx, distancenexty;
                (*itline)->m_bbox.Distance((*itprev)->m_bbox, distanceprevx, distanceprevy);
                (*itline)->m_bbox.Distance((*itnext)->m_bbox, distancenextx, distancenexty);
                if ((*itline)->m_direction == HORIZONAL) {
                    float size = (*itline)->m_bbox.m_y1 - (*itline)->m_bbox.m_y0;
                    if ( distanceprevy - distancenexty > 0.2 * size) {
                        (*itline)->m_linetag = DrLine::STARTLINE;
                        continue;
                    }
                    else if (distancenexty - distanceprevy > 0.2 * size)
                    {
                        (*itline)->m_linetag = DrLine::ENDLINE;
                        continue;
                    }
                    else
                    {
                        float prevx, nextx, currx;
                        prevx = (*itprev)->m_bbox.m_x0;
                        nextx = (*itnext)->m_bbox.m_x0;
                        currx = (*itline)->m_bbox.m_x0;

                        if (currx > prevx && fabs(currx - prevx) > 1.0 * size)
                        {
                            if (nextx < currx && fabs(prevx - nextx) < 0.5 * size) {
                                (*itline)->m_linetag = DrLine::STARTLINE;
                                continue;
                            }
                            else
                            {
                                (*itline)->m_linetag = DrLine::INSIDELINE;
                                continue;
                            }
                        }
                        
                    }
                }
                if ((*itline)->m_direction == VERTICAL) {
                    float size = (*itline)->m_bbox.m_x1 - (*itline)->m_bbox.m_x0;
                    if (distanceprevx - distancenextx > 0.2 * size) {
                        (*itline)->m_linetag = DrLine::STARTLINE;
                        continue;
                    }
                    else if (distancenextx - distanceprevx > 0.2 * size)  {
                        (*itline)->m_linetag = DrLine::ENDLINE;
                        continue;
                    }
                    else
                    {
                        float prevy, nexty, curry;
                        prevy = (*itprev)->m_bbox.m_y0;
                        nexty = (*itnext)->m_bbox.m_y0;
                        curry = (*itline)->m_bbox.m_y0;

                        if (curry > prevy && fabs(curry - prevy) > 1.0 * size) {
                            if (nexty < curry && fabs(prevy - nexty) < 0.5 * size) {
                                (*itline)->m_linetag = DrLine::STARTLINE;
                                continue;
                            }
                            else
                            {
                                (*itline)->m_linetag = DrLine::INSIDELINE;
                                continue;
                            }
                        }
                    }
                    
                }
            }
        }
    }
}