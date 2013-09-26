//
//  DrAnalyzer.h
//  DrAnalyzer
//
//  Created by Chu Yimin on 2013/06/28.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#ifndef __DrAnalyzer__DrAnalyzer__
#define __DrAnalyzer__DrAnalyzer__

#include <iostream>
#include "DrDocument.h"
/*
extern "C"
{
#include "svm_struct/svm_struct_common.h"
#include "svm_struct/svm_struct_learn.h"
}
*/
class DrAnalyzer {
public:
    DrDocument* m_doc;
    
    
public:
    DrAnalyzer();
    ~DrAnalyzer();
    void SetDocument(DrDocument* pdoc);
    void CalculateAttributes(DrPage & page, const char * outputfilename);


};

#endif /* defined(__DrAnalyzer__DrAnalyzer__) */
