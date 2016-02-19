#ifndef _OPERATE_NEW_H_
#define _OPERATE_NEW_H_
#include <exception>
#include "MemoryPool.h"
#include "MyMemoryAlloc.h"

void* operator new(size_t);
void* operator new[](size_t);

void operator delete(void*) throw();//声明无异常抛出函数
void operator delete[](void*) throw();


#endif