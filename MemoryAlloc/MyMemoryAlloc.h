#ifndef _MY_MEMORY_ALLOC_H_
#define _MY_MEMORY_ALLOC_H_

#include <memory>

const int ALIGN_SIZE = 8;
const int MAX_BYTES = 128;
const int FREE_LIST_COUNT = MAX_BYTES / ALIGN_SIZE;
//每个指针头部留了一个字节保存坐标index信息,最大block不要超出126*8个字节即可

class MyMemoryAlloc
{
public:
	MyMemoryAlloc(void);
	~MyMemoryAlloc(void);

	void* AllocMemory(size_t n);
	void DeallocMemory(void* p, size_t n);
	void DeallocMemory(void* p);
private:
	size_t RoundUp(size_t bytes)
	{
		return ( ( ( bytes) + ALIGN_SIZE -1 ) & ~ ( ALIGN_SIZE - 1 ) );
	}
	size_t GetFreeListIndex(size_t bytes)
	{
		return ( ( (bytes) + ALIGN_SIZE -1 ) / ALIGN_SIZE -1 );
	}

	void* RefillMemory(size_t n);
	char* ChunkMemory(size_t nBlockSize, int& nBlockCount);
private:
	union obj
	{
		union obj* pNextBlock;
		char szData[1];
	};
private:
	obj* volatile m_pFreeList[FREE_LIST_COUNT];
	char* m_pMemHead;
	char* m_pMemTail;
	size_t m_nMyHeap_Size;
};


class MyMemoryPool
{
public:
	static MyMemoryAlloc& GetMemoryAlloc() 
	{ 
		static MyMemoryAlloc mma;
		return mma;
	}
};

#endif
