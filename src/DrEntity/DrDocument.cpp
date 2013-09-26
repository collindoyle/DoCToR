//
//  DrDocument.cpp
//  DrEntity
//
//  Created by Chu Yimin on 2013/05/17.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#include "DrDocument.h"

DrDocument::DrDocument():m_pagelist()
{
    m_filename = NULL;
    m_pagecount = 0;
}

DrDocument::~DrDocument()
{
    if (m_filename) {
        delete[] m_filename;
    }
    
    if (!m_pagelist.empty()) {
        for (std::list<DrPage *>::iterator itpage = m_pagelist.begin(); itpage != m_pagelist.end(); itpage++) {
            delete *itpage;
        }
        m_pagelist.clear();
    }
}

void DrDocument::SetFileName(const char *filename)
{
    m_filename = new char[strlen(filename)+1];
    strcpy(m_filename, filename);
}

void DrDocument::AddPage(DrPage *page)
{
    if (page) {
        m_pagelist.push_back(page);
        m_pagecount++;
    }
}

std::list<DrPage *> & DrDocument::GetPageList()
{
    return m_pagelist;
}

unsigned int DrDocument::GetPageCount()
{
    return m_pagecount;
}

void DrDocument::OutputPageImage(unsigned int pageno)
{
    for (std::list<DrPage *>::iterator itpage = m_pagelist.begin(); itpage != m_pagelist.end(); itpage++) {
        if ((*itpage)->m_pagenum == pageno) {
            char * outputname = new char[strlen(m_filename)+10];
            sprintf(outputname, "%s.%d.png",m_filename,pageno);
            (*itpage)->OutputPNG(outputname);
            delete [] outputname;
            return;
        }
    }
}

char * DrDocument::GetFileName()
{
    return m_filename;
}

bool DrDocument::Equals(DrDocument &doc)
{
    bool result = true;
    if (strcmp(m_filename, doc.m_filename)!= 0) {
        result = false;
    }
    if (m_pagecount != doc.m_pagecount) {
        result = false;
    }
    
    std::list<DrPage *>::iterator itpagea = m_pagelist.begin();
    std::list<DrPage *>::iterator itpageb = doc.m_pagelist.begin();
    for (; itpagea != m_pagelist.end() && itpageb != doc.m_pagelist.end(); itpagea++,itpageb++) {
        if (!(*itpagea)->Equals(**itpageb)) {
            result = false;
            std::cout<<(*itpagea)->m_pagenum<<std::endl;
            break;
        }
    }
    return result;
}