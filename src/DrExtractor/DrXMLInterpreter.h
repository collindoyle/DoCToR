//
//  DrXMLInterpreter.h
//  DrExtractor
//
//  Created by Chu Yimin on 2013/06/06.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#ifndef __DrExtractor__DrXMLInterpreter__
#define __DrExtractor__DrXMLInterpreter__

#include <iostream>
#include <string>
#include "DrDocument.h"
#include "tinyxml2.h"
#include "DrFontCache.h"

class DrXMLInterpreter {
public:
    static DrFontCache * m_fontcache;
    
public:
    static void WriteTo(const char * filename, DrDocument &doc);
    static void ReadFrom(DrDocument &doc, const char * filename);
    static void SetFontCache(DrFontCache * cache);
    // convert the schema
    static void ConvertSource_Takasu(tinyxml2::XMLDocument &dstdoc, const char * srcfilename);
protected:
    static void Write(tinyxml2::XMLElement * parent, tinyxml2::XMLDocument * doc, DrPage * page);
    static void Write(tinyxml2::XMLElement * parent, tinyxml2::XMLDocument * doc, DrZone * zone);
    static void Write(tinyxml2::XMLElement * parent, tinyxml2::XMLDocument * doc, DrLine * line);
    static void Write(tinyxml2::XMLElement * parent, tinyxml2::XMLDocument * doc, DrPhrase * phrase);
    static void Write(tinyxml2::XMLElement * parent, tinyxml2::XMLDocument * doc, DrChar * drchar);
    static void WriteBox(tinyxml2::XMLElement *element, DrBox & box);
    static void WriteFont(tinyxml2::XMLElement *element, DrFontDescriptor * font);
    static void WriteDirection(tinyxml2::XMLElement *element, eDirection dir);
    
    static void Read(DrChar * drchar, tinyxml2::XMLElement * node);
    static void Read(DrPhrase * drphrase, tinyxml2::XMLElement * node);
    static void Read(DrLine * drline, tinyxml2::XMLElement * node);
    static void Read(DrZone * drzone, tinyxml2::XMLElement * node);
    static void Read(DrPage * drpage, tinyxml2::XMLElement * node);
    static void ReadBox(DrBox * drbox, tinyxml2::XMLElement * node);
    static DrFontDescriptor * ReadFont(tinyxml2::XMLElement * node);
    static eDirection ReadDirection(tinyxml2::XMLElement * node);
    // convert all the child or deeper nodes, and add the converted nodes as children of pdstparent
    static void ConvertChild(tinyxml2::XMLElement * pparent, tinyxml2::XMLDocument & dstdoc, tinyxml2::XMLElement * pchild, std::string &encode, DrBox &bbox, tinyxml2::XMLElement ** pcurzone, eDirection &dir, eZoneLabel curlabel);
    
    // parse EUC-JP byte to an integer.  With an interpreter the EUC-JP is converted to UTF-8
    static unsigned int parseEUCstr(const char * str);
    static void parseboundingbox(DrBox &box, tinyxml2::XMLElement *pElem);
    static enum eDirection direction(tinyxml2::XMLElement * pelement);
};



#endif /* defined(__DrExtractor__DrXMLInterpreter__) */

