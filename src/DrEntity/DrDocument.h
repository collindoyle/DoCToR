//
//  DrDocument.h
//  DrEntity
//
//  Created by Chu Yimin on 2013/05/17.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#ifndef __DrEntity__DrDocument__
#define __DrEntity__DrDocument__

#include <iostream>
#include <list>
#include "DrPage.h"

class DrDocument {
    std::list<DrPage *> m_pagelist;
    char * m_filename;
    unsigned int m_pagecount;
    
    
public:
    DrDocument();
    ~DrDocument();
    void SetFileName(const char * filename);
    char * GetFileName();
    void AddPage(DrPage * page);
    std::list<DrPage *> & GetPageList();
    unsigned int GetPageCount();
    void OutputPageImage(unsigned int pageno);
    bool Equals(DrDocument & doc);
};
#endif /* defined(__DrEntity__DrDocument__) */
