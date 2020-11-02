//
// Created by lyf on 2020/11/2.
//

#ifndef GBN_GLOBAL_H
#define GBN_GLOBAL_H

#include "Tool.h"
#include "NetworkService.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
#define GBN_N 10
extern  Tool *pUtils;						//指向唯一的工具类实例，只在main函数结束前delete
extern  NetworkService *pns;				//指向唯一的模拟网络环境类实例，只在main函数结束前delete

#endif //GBN_GLOBAL_H
