#include "pch.h"
#include "Allocator.h"

void* BaseAllocator::Alloc(int32 size)
{
	return ::malloc(size);
}
void BaseAllocator::Release(void* ptr)
{
	::free(ptr);
}


void* PoolAllocator::Alloc(int32 size)
{
	return nullptr;
}
void PoolAllocator::Release(void* ptr)
{
}
