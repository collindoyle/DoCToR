//
//  DrXMLInterpreter.cpp
//  DrExtractor
//
//  Created by Chu Yimin on 2013/06/06.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#include "DrXMLInterpreter.h"
#include <string>
#include "EucencodeConverter.h"
#include "DrTexGrouper.h"

DrFontCache * DrXMLInterpreter::m_fontcache = NULL;
static EucencodeConverter converter;



void DrXMLInterpreter::SetFontCache(DrFontCache *cache)
{
    m_fontcache = cache;
}

void DrXMLInterpreter::WriteTo(const char *filename, DrDocument &doc)
{
    tinyxml2::XMLDocument xmldoc;
    tinyxml2::XMLElement * root = xmldoc.NewElement("document");

    for (std::list<DrPage *>::iterator itpage = doc.GetPageList().begin(); itpage != doc.GetPageList().end(); itpage++) {
        Write(root, &xmldoc, *itpage);
    }
    root->SetAttribute("filename", doc.GetFileName());
    xmldoc.InsertEndChild(root);
    xmldoc.SaveFile(filename);
}

void DrXMLInterpreter::WriteBox(tinyxml2::XMLElement *element, DrBox &box)
{
    element->SetAttribute("x0", box.m_x0);
    element->SetAttribute("y0", box.m_y0);
    element->SetAttribute("x1", box.m_x1);
    element->SetAttribute("y1", box.m_y1);
}

void DrXMLInterpreter::WriteDirection(tinyxml2::XMLElement *element, eDirection dir)
{
    switch (dir) {
        case VERTICAL:
            element->SetAttribute("direction", "vertical");
            break;
        case HORIZONAL:
            element->SetAttribute("direction", "horizonal");
            break;
        default:
            element->SetAttribute("direction", "unset");
            break;
    }
}
void DrXMLInterpreter::Write(tinyxml2::XMLElement *parent, tinyxml2::XMLDocument * doc, DrPage *page)
{
    tinyxml2::XMLElement * ppage = doc->NewElement("page");
    ppage->SetAttribute("pageno", page->m_pagenum);
    WriteBox(ppage, page->m_pagebox);
    for (std::list<DrZone *>::iterator itzone = page->m_zonelist.begin(); itzone != page->m_zonelist.end(); itzone++) {
        Write(ppage, doc, (*itzone));
    }
    parent->InsertEndChild(ppage);
}

const char *labels[] = {
    "NONE",
    "TITLE",
    "TEXTBODY",
    "ABSTRACT",
    "AUTHOR",
    "KEYWORD",
    "ALLIFICATION",
    "CAPTION",
    "PAGENUM",
    "SUBTITLE",
    "REFERENCE",
    "START",
    "END"
};

static enum eZoneLabel parsetypestr(const char * typestr)
{
    int i;
    for (i = NONE; i < END; i++) {
        if (strcmp(labels[i], typestr) == 0) {
            return (enum eZoneLabel)i;
        }
    }
    return NONE;
}

void DrXMLInterpreter::Write(tinyxml2::XMLElement *parent, tinyxml2::XMLDocument *doc, DrZone *zone)
{
    tinyxml2::XMLElement * pzone = doc->NewElement("zone");
    WriteBox(pzone, zone->m_bbox);
    WriteDirection(pzone, zone->m_direction);
    pzone->SetAttribute("type", labels[zone->m_label]);
    for (std::list<DrLine *>::iterator itline = zone->m_linelist.begin(); itline != zone->m_linelist.end(); itline++) {
        Write(pzone, doc, *itline);
    }
    parent->InsertEndChild(pzone);
}

void DrXMLInterpreter::Write(tinyxml2::XMLElement *parent, tinyxml2::XMLDocument *doc, DrLine *line)
{
    tinyxml2::XMLElement * pline = doc->NewElement("line");
    WriteBox(pline, line->m_bbox);
    WriteDirection(pline, line->m_direction);
    
    if (line->m_linetag == DrLine::STARTLINE) {
        pline->SetAttribute("tag", "startline");
    }
    else if (line->m_linetag == DrLine::ENDLINE)
    {
        pline->SetAttribute("tag", "endline");
    }
    else
    {
        pline->SetAttribute("tag", "insideline");
    }
    for (std::list<DrPhrase *>::iterator itphrase = line->m_phraselist.begin(); itphrase != line->m_phraselist.end(); itphrase++) {
        Write(pline, doc, *itphrase);
    }
    parent->InsertEndChild(pline);
}

void DrXMLInterpreter::Write(tinyxml2::XMLElement *parent, tinyxml2::XMLDocument *doc, DrPhrase *phrase)
{
    tinyxml2::XMLElement * pphrase = doc->NewElement("phrase");
    WriteBox(pphrase, phrase->m_bbox);
    WriteDirection(pphrase, phrase->m_direction);
    
    DrFontDescriptor * font = phrase->GetFont();
    WriteFont(pphrase, font);
    for (std::list<DrChar *>::iterator itchar = phrase->m_charlist.begin(); itchar != phrase->m_charlist.end(); itchar++) {
        Write(pphrase, doc, *itchar);
    }
    parent->InsertEndChild(pphrase);
}

void DrXMLInterpreter::WriteFont(tinyxml2::XMLElement *element, DrFontDescriptor *font)
{
    element->SetAttribute("font", font->m_fontname);
    element->SetAttribute("size", font->m_fontsize);
    if (font->IsBold()&&font->IsItalic()) {
        element->SetAttribute("style", "boldanditalic");
    }
    else if (font->IsBold())
    {
        element->SetAttribute("style", "bold");
    }
    else if (font->IsItalic())
    {
        element->SetAttribute("style", "italic");
    }
    else
    {
        element->SetAttribute("style", "normal");
    }
}

void DrXMLInterpreter::Write(tinyxml2::XMLElement *parent, tinyxml2::XMLDocument *doc, DrChar *drchar)
{
    char utf8char[4]="";
    tinyxml2::XMLElement * pchar = doc->NewElement("char");
    WriteBox(pchar, drchar->m_bbox);
    WriteFont(pchar, drchar->GetFont());
    drchar->GetUTF8String(utf8char);
    tinyxml2::XMLText * text = doc->NewText(utf8char);
    pchar->InsertEndChild(text);
    parent->InsertEndChild(pchar);
}

void DrXMLInterpreter::ReadFrom(DrDocument &doc, const char *filename)
{
    tinyxml2::XMLDocument xmldoc;
    xmldoc.LoadFile(filename);
    tinyxml2::XMLElement *pdoc = xmldoc.RootElement();
    const char * sourcefile = pdoc->Attribute("filename");
    doc.SetFileName(sourcefile);
    for (tinyxml2::XMLElement * ppage = pdoc->FirstChildElement("page"); ppage != NULL; ppage = ppage->NextSiblingElement("page")) {
        DrPage * page = new DrPage;
        Read(page, ppage);
        doc.AddPage(page);
    }
}

void DrXMLInterpreter::Read(DrPage *drpage, tinyxml2::XMLElement *node)
{
    //drpage->m_thumbnail = NULL;
    ReadBox(&(drpage->m_pagebox), node);
    drpage->m_pagenum = node->IntAttribute("pageno");
    std::list<DrLine *> linelist;
    for (tinyxml2::XMLElement * pline = node->FirstChildElement("line"); pline != NULL; pline = pline->NextSiblingElement("line")) {
        DrLine *line = new DrLine;
        Read(line, pline);
        linelist.push_back(line);
    }
    DrTextGrouper::TextGroup(drpage->m_zonelist, linelist);
    //    std::cout<<"Parsed the line nodes"<<std::endl;
}

void DrXMLInterpreter::Read(DrZone *drzone, tinyxml2::XMLElement *node)
{
    const char * labelname = node->Attribute("label");
    drzone->m_label = NONE;
    for (int i = 0; i < sizeof(labels); i++) {
        if (strcmp(labelname, labels[i]) == 0) {
            drzone->m_label = (eZoneLabel)i;
            break;
        }
    }
    drzone->m_direction = ReadDirection(node);
    
    for (tinyxml2::XMLElement * pline = node->FirstChildElement("line"); pline != NULL; pline = pline->NextSiblingElement("line")) {
        DrLine * line = new DrLine;
        Read(line, pline);
        drzone->AddLine(line);
    }
}

void DrXMLInterpreter::Read(DrLine *drline, tinyxml2::XMLElement *node)
{
    drline->m_direction = ReadDirection(node);
    const char * linetagstr = node->Attribute("tag");
    if (linetagstr) {
        if (strcmp(linetagstr, "startline") == 0) {
            drline->m_linetag = DrLine::STARTLINE;
        }
        else if (strcmp(linetagstr, "endline") == 0) {
            drline->m_linetag = DrLine::ENDLINE;
        }
        else {
            drline->m_linetag = DrLine::INSIDELINE;
        }
    }
    const char * label = node->Attribute("label");
    if (label) {
        for (int i = 0; i < sizeof(labels); i++) {
            if (strcmp(label, labels[i]) == 0) {
                drline->label = (eZoneLabel)i;
                break;
            }
        }
    }
    for (tinyxml2::XMLElement * pphrase = node->FirstChildElement("phrase"); pphrase != NULL; pphrase = pphrase->NextSiblingElement("phrase")) {
        DrPhrase * phrase = new DrPhrase;
        Read(phrase, pphrase);
        drline->AddPhrase(phrase);
    }
}

void DrXMLInterpreter::Read(DrPhrase *drphrase, tinyxml2::XMLElement *node)
{
    drphrase->m_font = ReadFont(node);
    drphrase->m_direction = ReadDirection(node);
    for (tinyxml2::XMLElement * pchar = node->FirstChildElement("char"); pchar != NULL; pchar = pchar->NextSiblingElement("char")) {
        DrChar * achar = new DrChar;
        Read(achar, pchar);
        drphrase->AddChar(achar);
    }
}

void DrXMLInterpreter::Read(DrChar *drchar, tinyxml2::XMLElement *node)
{
    const char * text = node->GetText();
    if (text == NULL)
        drchar->ConvertUTF8toUCS(" ");
    else
        drchar->ConvertUTF8toUCS((char *)text);
    ReadBox(&(drchar->m_bbox), node);
    drchar->m_font = ReadFont(node);
    char c[4]="";
    drchar->GetUTF8String(c);
    std::cout<<c<<std::endl;
}

void DrXMLInterpreter::ReadBox(DrBox *drbox, tinyxml2::XMLElement *node)
{
    drbox->m_x0 = node->FloatAttribute("x0");
    drbox->m_x1 = node->FloatAttribute("x1");
    drbox->m_y0 = node->FloatAttribute("y0");
    drbox->m_y1 = node->FloatAttribute("y1");
}

DrFontDescriptor * DrXMLInterpreter::ReadFont(tinyxml2::XMLElement *node)
{

    const char * fontname = node->Attribute("font");
    float size = node->FloatAttribute("size");
    const char * style = node->Attribute("style");
    DrFontDescriptor::style estyle;
    if (style) {
        if (strcmp(style, "boldanditalic") == 0)
        {
            estyle = DrFontDescriptor::FS_BOLD_ITALIC;
        }
        else if (strcmp(style, "bold") == 0)
        {
            estyle = DrFontDescriptor::FS_BOLD;
        }
        else if (strcmp(style, "italic") == 0)
        {
            estyle = DrFontDescriptor::FS_ITALIC;
        }
        else
        {
            estyle = DrFontDescriptor::FS_NONE;
        }
    }

    DrFontDescriptor * fdp;
    fdp = m_fontcache->FindDescriptor(fontname, size, estyle);
    if (fdp == NULL) {
        m_fontcache->AddDescriptor(fontname, size, estyle);
        fdp = m_fontcache->FindDescriptor(fontname, size, estyle);
    }
    return fdp;
    
}


eDirection DrXMLInterpreter::ReadDirection(tinyxml2::XMLElement *node)
{
    const char * dirstr = node->Attribute("direction");
    eDirection edir;
    
    if (strcmp(dirstr, "vertical") == 0) {
        edir = VERTICAL;
    }
    else if (strcmp(dirstr, "horizonal") == 0) {
        edir = HORIZONAL;
    }
    else
        edir = UNSET;
    return edir;
}

void DrXMLInterpreter::ConvertSource_Takasu(tinyxml2::XMLDocument &dstdoc, const char *srcfilename)
/* Convert the schema of XML files, from src to dst,
 to deal with different encodings and schemas
 */
{
    
    tinyxml2::XMLDocument srcdoc;
    int error = srcdoc.LoadFile(srcfilename);
    if (error == 0) {
        
        // Get the value of source encode
        std::string srcencode;
        tinyxml2::XMLDeclaration * pdeclare = dstdoc.NewDeclaration();
        dstdoc.InsertFirstChild(pdeclare);
        const char * srcdeclare = srcdoc.FirstChild()->Value();
        const char * ptr = strstr(srcdeclare, "encoding=\"");
        if (ptr) {
            const char * encodeptr = ptr+10;
            const char * pend = strchr(encodeptr, '\"');
            unsigned int len = pend - encodeptr;
            char * encoding = new char[len+1];
            strncpy(encoding, encodeptr, len);
            srcencode = encoding;
            delete []encoding;
        }
        
        // Find the root element of the document
        tinyxml2::XMLElement * pElem = srcdoc.RootElement();
        const char * rootname = pElem->Name();
        if (strcmp(rootname, "paper") != 0) {
            error = tinyxml2::XML_ERROR_PARSING_ELEMENT;
        }
        else
        {
            // Create the root element for the new doc.

            const char * pfilename = strstr(srcfilename, ".conv");
            int len = pfilename - srcfilename;
            char * poutname = new char[len+9];
            strncpy(poutname, srcfilename, len);
            strcat(poutname, ".conv.xml");
            tinyxml2::XMLElement * pnewroot = dstdoc.NewElement("document");
            pnewroot->SetAttribute("filename", poutname);
            delete [] poutname;
            dstdoc.InsertEndChild(pnewroot);
            // Find all the childs of the root element in the src document, convert them and add to the new root.
            
            for (tinyxml2::XMLElement * p = pElem->FirstChildElement(); p != NULL; p = p->NextSiblingElement()) {
                // convert child
                DrBox bbox;
                eDirection dir;
				eZoneLabel templ = NONE;
                ConvertChild(pnewroot,dstdoc,p,srcencode,bbox,NULL,dir, templ);
            }
        }
    }
}

void DrXMLInterpreter::ConvertChild(tinyxml2::XMLElement *pparent, tinyxml2::XMLDocument &dstdoc, tinyxml2::XMLElement *pchild, std::string &encode, DrBox &bbox, tinyxml2::XMLElement **pcurzone,eDirection &dir,eZoneLabel curlabel)
/* Given an XML Element, convert it to the desired XML schema, then insert it into the corresponding position of the new XML tree. Convert the child nodes recursively.
 
 for different nodes, the conversion behaves differently.
 
 char --> char: create a char node, convert the character from EUC-JP to UTF-8, calculate the bounding box and size of the character, fill the details of font description with default values
 
 word --> phrase: create a phrase node, calculate the bounding box and size, add all child nodes of word to the segment
 
 line --> line: create a line node,  add all the segment nodes under the line node, calculate the bounding box, determine whether a new block is needed according to the current tag and the tag of current block pointer.
 
 block --> zone: create a zone node, add the line nodes under the block node only when the tags are the same, and they used to belong to the same block.
 
 page --> page: create a page node, add the zone nodes under the page node
 
 j-abstract, e-abstract, j-authors, e-authors, j-title, e-title, j-keywords, e-keywords: no nodes in the converted schema.  set the current tag, and all the child nodes are under this tag. recover it when the tag ends.
 */
{
    const char * srcchildname = pchild->Name();
    if (strcmp(srcchildname, "page") == 0) {
        tinyxml2::XMLElement * pnewelement = dstdoc.NewElement("page");
        pparent->InsertEndChild(pnewelement);
        tinyxml2::XMLElement * psubchild = pchild->FirstChildElement();
        DrBox pagebox;
        while (psubchild) {
            DrBox subbox;
            eDirection dir;
            ConvertChild(pnewelement, dstdoc, psubchild, encode, subbox, NULL, dir,NONE);
            pagebox.GrowBox(subbox);
            psubchild = psubchild->NextSiblingElement();
        }
        bbox = pagebox;
        WriteBox(pnewelement, pagebox);
        int pageno = pchild->IntAttribute("number")-1;
        pnewelement->SetAttribute("pageno", pageno);

    }
    /*
    else if (strcmp(srcchildname,"block") == 0) {
        tinyxml2::XMLElement * pnewelement = dstdoc.NewElement("zone");
        pparent->InsertEndChild(pnewelement);
        tinyxml2::XMLElement * psubchild = pchild->FirstChildElement();
        DrBox zonebox;
        pnewelement->SetAttribute("label", labels[curlabel]);
        eDirection ndir;
        while (psubchild) {
            DrBox linebox;

            ConvertChild(pnewelement, dstdoc, psubchild, encode, linebox, &pnewelement, ndir,curlabel);
            zonebox.GrowBox(linebox);
            psubchild = psubchild->NextSiblingElement();
        }
        bbox = zonebox;
        WriteBox(pnewelement, zonebox);
        WriteDirection(pnewelement, ndir);
        dir = ndir;
        if (parsetypestr(pnewelement->Attribute("label")) == NONE) {
            pnewelement->SetAttribute("label", labels[TEXTBODY]);
        }
    }
     */
    else if (strcmp(srcchildname, "char") == 0) {
        char * utfstr = NULL;
        unsigned int i = 0;
        tinyxml2::XMLElement * pnewelement = dstdoc.NewElement("char");
        pparent->InsertEndChild(pnewelement);
        const char *ptext = pchild->GetText();
        DrBox box;
        float size;
        parseboundingbox(box, pchild);
        size = (box.m_y1 - box.m_y0);
        WriteBox(pnewelement, box);
        pnewelement->SetAttribute("font", "DefaultFont");
        pnewelement->SetAttribute("size", size);
        pnewelement->SetAttribute("style", "normal");
        if(ptext)
        {
 
            if (encode == "euc-jp") {
                unsigned int encodedchar = parseEUCstr(ptext);
                i = converter.Convert(encodedchar);
                unsigned int len = sizeof(unsigned int);
                utfstr = new char[len];
                memset(utfstr,0,sizeof(utfstr)*sizeof(char));
                
                for (int j = 0, k = len-1; k>=0; k--) {
                    if (((unsigned char *)(&i))[k] == '\0') {
                        continue;
                    }
                    utfstr[j] = ((unsigned char *)(&i))[k];
                    j++;
                }
            }
        }
        
        else
        {
            utfstr = new char[2];
            strcpy(utfstr, " ");
        }
        
        tinyxml2::XMLText * ptextnode = dstdoc.NewText((const char *)(utfstr));
        pnewelement->InsertEndChild(ptextnode);
        bbox = box;
        if (utfstr) {
            delete []utfstr;
        }
    }
    else if (strcmp(srcchildname, "word") == 0)
    {
        tinyxml2::XMLElement * presult = dstdoc.NewElement("phrase");
        pparent->InsertEndChild(presult);
        tinyxml2::XMLElement * psubchild = pchild->FirstChildElement();
        DrBox subbox;
        while (psubchild) {
            ConvertChild(presult, dstdoc, psubchild, encode, subbox, NULL,dir,NONE);
            bbox.GrowBox(subbox);
            psubchild = psubchild->NextSiblingElement();
        }
        WriteBox(presult, bbox);
        float size = bbox.m_y1 - bbox.m_y0;
        presult->SetAttribute("font", "DefaultFont");
        presult->SetAttribute("size", size);
        presult->SetAttribute("style", "normal");
        enum eDirection ndir = direction(pchild);
        WriteDirection(presult, ndir);

        dir = ndir;
    }
    else if (strcmp(srcchildname, "line") == 0) {
        /* convert the word node */
        tinyxml2::XMLElement * pnewelement = dstdoc.NewElement("line");
        tinyxml2::XMLElement * psubchild = pchild->FirstChildElement();
        pnewelement->SetAttribute("label", labels[curlabel]);
        DrBox subbox;
        while (psubchild) {
            ConvertChild(pnewelement, dstdoc, psubchild, encode, subbox, NULL,dir, NONE);
            bbox.GrowBox(subbox);
            psubchild = psubchild->NextSiblingElement();
        }
        WriteBox(pnewelement, bbox);
        enum eDirection dir = direction(pchild);
        WriteDirection(pnewelement, dir);
        if (parsetypestr(pnewelement->Attribute("label")) == NONE) {
            pnewelement->SetAttribute("label", labels[TEXTBODY]);
        }
        pparent->InsertEndChild(pnewelement);
        /* check the types, determine whether to start a new block */
      /*  if ((*pcurzone)->Attribute("label") == NULL) {
            (*pcurzone)->SetAttribute("label", labels[curlabel]);
        }
        else
        {
            if (parsetypestr((*pcurzone)->Attribute("label")) != curlabel) {
                enum eDirection dir = direction(*pcurzone);
                WriteDirection(*pcurzone, dir);
                *pcurzone = dstdoc.NewElement("zone");
                dstdoc.RootElement()->InsertEndChild(*pcurzone);
                (*pcurzone)->SetAttribute("label", labels[curlabel]);
            }
        }
        (*pcurzone)->InsertEndChild(presult);*/
    }
    // Set the current page number, start from 0 (the Inoue schema start from 1)
    
    // Only set the tag and parse all the child nodes under the tag
    else if (strcmp(pchild->Name(),"j-title") == 0 || strcmp(pchild->Name(), "e-title") == 0)
    {
        curlabel = TITLE;
        tinyxml2::XMLElement * psubchild = pchild->FirstChildElement();
        while (psubchild) {
            ConvertChild(pparent, dstdoc, psubchild, encode, bbox, NULL, dir, curlabel);
            psubchild = psubchild->NextSiblingElement();
        }
    }
    else if (strcmp(pchild->Name(),"j-authors") == 0 || strcmp(pchild->Name(), "e-authors") == 0)
    {
        curlabel = AUTHOR;
        tinyxml2::XMLElement * psubchild = pchild->FirstChildElement();
        while (psubchild) {
            ConvertChild(pparent, dstdoc, psubchild, encode, bbox, NULL, dir, curlabel);
            psubchild = psubchild->NextSiblingElement();
        }
    }
    else if (strcmp(pchild->Name(),"j-abstract") == 0 || strcmp(pchild->Name(), "e-abstract") == 0)
    {
        curlabel = ABSTRACT;
        tinyxml2::XMLElement * psubchild = pchild->FirstChildElement();
        while (psubchild) {
            ConvertChild(pparent, dstdoc, psubchild, encode, bbox, NULL, dir, curlabel);
            psubchild = psubchild->NextSiblingElement();
        }
    }
    else if (strcmp(pchild->Name(),"j-keywords") == 0 || strcmp(pchild->Name(), "e-keywords") == 0)
    {

        curlabel = KEYWORD;
        tinyxml2::XMLElement * psubchild = pchild->FirstChildElement();
        while (psubchild) {
            ConvertChild(pparent, dstdoc, psubchild, encode, bbox, NULL, dir, curlabel);
            psubchild = psubchild->NextSiblingElement();
        }
    }
    else if (strcmp(pchild->Name(), "ref") == 0)
    {
        curlabel = REFERENCE;
        tinyxml2::XMLElement * psubchild = pchild->FirstChildElement();
        while (psubchild) {
            ConvertChild(pparent, dstdoc, psubchild, encode, bbox, NULL, dir, curlabel);
            psubchild = psubchild->NextSiblingElement();
        }
    }
    else
    {
        tinyxml2::XMLElement * psubchild = pchild->FirstChildElement();
        
        while (psubchild) {
            ConvertChild(pparent, dstdoc, psubchild, encode, bbox, pcurzone, dir, curlabel);
            psubchild = psubchild->NextSiblingElement();
        }
    }
    
}

unsigned int DrXMLInterpreter::parseEUCstr(const char *str)
{
    unsigned int result = 0;
    const char * ptr = str;
    while (*ptr != '\0') {
        switch (*ptr) {
            case ' ':
                ptr++;
                break;
            default:
                result = (result<<8) + (unsigned int)(*(unsigned char *)ptr);
                ptr++;
                break;
        }
    }
    //std::cout<<result<<std::endl;
    return result;
}


void DrXMLInterpreter::parseboundingbox(DrBox &box, tinyxml2::XMLElement *pElem)
{
    box.m_x0 = pElem->IntAttribute("x");
    box.m_x1 = pElem->IntAttribute("width");
    box.m_y0 = pElem->IntAttribute("y");
    box.m_y1 = pElem->IntAttribute("height");
    box.m_x1 = box.m_x1+box.m_x0;
    box.m_y1 = box.m_y1+box.m_y0;
}


enum eDirection DrXMLInterpreter::direction(tinyxml2::XMLElement *pelement)
{
    double width = pelement->DoubleAttribute("width");
    double height = pelement->DoubleAttribute("height");
    if (width > height) {
        return HORIZONAL;
    }
    else
        return VERTICAL;
}


