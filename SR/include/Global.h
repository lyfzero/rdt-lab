//
// Created by lyf on 2020/11/2.
//

#ifndef SR_GLOBAL_H
#define SR_GLOBAL_H

#include "Tool.h"
#include "NetworkService.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;
#define SR_N 10
#define ACK_RCV -2
#define ACK_UNRCV -1
extern  Tool *pUtils;						//指向唯一的工具类实例，只在main函数结束前delete
extern  NetworkService *pns;				//指向唯一的模拟网络环境类实例，只在main函数结束前delete

#endif //SR_GLOBAL_H
