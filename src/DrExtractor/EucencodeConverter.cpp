//
//  EucencodeConverter.cpp
//  DrExtractor
//
//  Created by Chu Yimin on 2013/07/19.
//  Copyright (c) 2013年 東京大学. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "EucencodeConverter.h"
#include "eucjptoutf8.h"
#include <map>

EucencodeConverter::EucencodeConverter():convertmap()
{
    Initialize();
}

void EucencodeConverter::Initialize()
{
    unsigned int charcount = sizeof(LUmapEUC_JP)/sizeof(interpreterpair);
    for (int i = 0; i < charcount; i++) {
        convertmap.insert(std::pair<unsigned int, unsigned int>(LUmapEUC_JP[i].euc,LUmapEUC_JP[i].utf8));
    }
}

int EucencodeConverter::Convert(const unsigned int src)
{
    std::map<unsigned int, unsigned int>::iterator it = convertmap.find(src);
    if (it == convertmap.end()) {
        return  src;
    }
    else
        return it->second;
}