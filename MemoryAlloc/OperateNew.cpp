#include "OperateNew.h"



void* operator new(size_t size)
{
	return MyMemoryPool::GetMemoryAlloc().AllocMemory(size);
}
void* operator new[](size_t size)
{
	return operator new(size);
}

void operator delete(void* ptr) throw()
{
	MyMemoryPool::GetMemoryAlloc().DeallocMemory(ptr);
}
void operator delete[](void* ptr) throw()
{
	operator delete(ptr);
}