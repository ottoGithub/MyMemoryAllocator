#include "MyMemoryAlloc.h"


MyMemoryAlloc::MyMemoryAlloc(void)
{
	memset((void*)m_pFreeList,0,sizeof(obj*) * FREE_LIST_COUNT);
	m_pMemHead = NULL;
	m_pMemTail = NULL;
	m_nMyHeap_Size = 0;
}


MyMemoryAlloc::~MyMemoryAlloc(void)
{
}

void* MyMemoryAlloc::AllocMemory(size_t n)
{
	if( n > size_t(MAX_BYTES))
	{
		char* p = (char*)::malloc(n + 1);
		*p = char(0xFF);
		return p + 1;
	}
	size_t BlockIndex = GetFreeListIndex(n);
	obj* pTemp = m_pFreeList[BlockIndex];
	if(!pTemp)
	{
		return RefillMemory(RoundUp(n));
	}
	m_pFreeList[BlockIndex] = m_pFreeList[BlockIndex]->pNextBlock;
	return pTemp;
}

void MyMemoryAlloc::DeallocMemory(void* p)
{
	char* pHead = (char*)p-1;
	if(~(*pHead) )
	{
		size_t BlockIndex = size_t(*pHead);
		obj* pSaveData = (obj*)p;
		pSaveData->pNextBlock = m_pFreeList[BlockIndex];
		m_pFreeList[BlockIndex] = pSaveData;
	}
	else
	{
		::free(pHead);
	}
}

void MyMemoryAlloc::DeallocMemory(void* p, size_t n)
{
	DeallocMemory(p);
}

void* MyMemoryAlloc::RefillMemory(size_t n)
{
	int nBlockCount = 20;
	int nRealSize = n + 1;

	char* pChunkHead = ChunkMemory(nRealSize,nBlockCount);
	char* pHeadData = pChunkHead + 1;

	size_t BlockIndex = GetFreeListIndex(n);

	char* pTemp = pChunkHead;
	for(int i=0;i<nBlockCount;++i)
	{
		*pTemp = BlockIndex;
		pTemp += nRealSize;
	}

	if(nBlockCount == 1)
	{
		return pHeadData;
	}
	obj* pNextBlock = m_pFreeList[BlockIndex] = (obj*)(pHeadData + nRealSize);

	obj* pCurrentBlock = NULL;
	for(int i=1;;++i)
	{
		pCurrentBlock = pNextBlock;
		pNextBlock = (obj*)((char*)pNextBlock + nRealSize);
		if(nBlockCount - 1 == i)
		{
			pCurrentBlock->pNextBlock = NULL;
			break;
		}
		else
		{
			pCurrentBlock->pNextBlock = pNextBlock;
		}
	}
	return pHeadData;
}

char* MyMemoryAlloc::ChunkMemory(size_t nBlockSize, int& nBlockCount)
{
	char* pResult = NULL;
	size_t nTotalBytes = nBlockSize * nBlockCount;
	size_t nLeftBytes = m_pMemTail - m_pMemHead;

	if(nLeftBytes >= nTotalBytes)
	{
		pResult = m_pMemHead;
		m_pMemHead += nTotalBytes;
		return pResult;
	}
	else if(nLeftBytes >= nBlockSize)
	{
		//返回所有的block
		nBlockCount = nLeftBytes / nBlockSize;
		nTotalBytes = nBlockSize * nBlockCount;
		pResult = m_pMemHead;
		m_pMemHead += nTotalBytes;
		return pResult;
	}
	else
	{
		if(nLeftBytes > 0)
		{
			//把残余的那点内存分配给合适的FreeList
			size_t BlockIndex = GetFreeListIndex(nLeftBytes - 1);
			*m_pMemHead = BlockIndex;
			((obj*)(m_pMemHead + 1))->pNextBlock = m_pFreeList[BlockIndex];
			m_pFreeList[BlockIndex] = (obj*)(m_pMemHead + 1);
		}
		//新水量的大小为需求量的4倍,再加上 一个随着配置次数增加而愈来愈大的附加量
		size_t nAllocTotalBytes = 4 * nTotalBytes + RoundUp(m_nMyHeap_Size >> 4);

		m_pMemHead = (char*)::malloc(nAllocTotalBytes);
		if(!m_pMemHead)
		{
			//TODO:malloc failure
			return NULL;
		}
		m_nMyHeap_Size += nAllocTotalBytes;
		m_pMemTail = m_pMemHead + nAllocTotalBytes;
		return ChunkMemory( nBlockSize, nBlockCount );
	}
}
