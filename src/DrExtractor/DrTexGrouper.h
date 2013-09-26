//
//  DrTexGrouper.h
//  DrExtractor
//
//  Created by Chu Yimin on 2013/05/23.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#ifndef __DrExtractor__DrTexGrouper__
#define __DrExtractor__DrTexGrouper__

#include <iostream>
#include "DrDocument.h"
#include <list>
class DrTextGrouper {
    
    
public:
    static void TextGroup(std::list<DrZone *> &zonelist, std::list<DrLine *> &linelist);
    static void TextGroup(std::list<DrLine *> &linelist, std::list<DrPhrase *> &phraselist);
    static void TextGroup(std::list<DrPhrase *> &phraselist, std::list<DrChar *> &charlist);
    
    static void LineClassification(std::list<DrLine *> &linelist);
    
protected:
    static bool Groupable(DrZone * zone, DrLine *line);
    static bool Groupable(DrLine * line, DrPhrase * phrase);
    static bool Groupable(DrPhrase * phrase, DrChar * achar);
};
#endif /* defined(__DrExtractor__DrTexGrouper__) */
