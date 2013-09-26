//
//  EncodeConverter.h
//  DrExtractor
//
//  Created by Chu Yimin on 2013/07/19.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#ifndef __DrExtractor__EncodeConverter__
#define __DrExtractor__EncodeConverter__

#include <iostream>
#include <map>
class EncodeConverter {
    
public:
    virtual void Initialize() = 0;
    virtual int Convert(const unsigned int src) = 0;
};
#endif /* defined(__DrExtractor__EncodeConverter__) */
