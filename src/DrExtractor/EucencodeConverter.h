//
//  EucencodeConverter.h
//  DrExtractor
//
//  Created by Chu Yimin on 2013/07/19.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#ifndef __DrExtractor__EucencodeConverter__
#define __DrExtractor__EucencodeConverter__

#include <iostream>
#include "EncodeConverter.h"
#include <map>
class EucencodeConverter : public EncodeConverter {
public:
    std::map<unsigned int, unsigned int> convertmap;
public:
    EucencodeConverter();
    virtual void Initialize();
    virtual int Convert(const unsigned int src);
};


#endif /* defined(__DrExtractor__EucencodeConverter__) */
