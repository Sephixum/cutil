#include "linux_base_impl.h"

///////////////////////////
/// Memory
#include "../../base_memory.h"
#include <unistd.h>

internal void* MemoryReserve(U64 size)
{
	if (size == 0)
	{
		return NULL;
	}

	void* result = mmap(0, size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (result == MAP_FAILED)
	{
		result = NULL;
	}

	return result;
}

internal B32 MemoryCommit(void* ptr, U64 size)
{
	S32 res = 0;

	res = mprotect(ptr, size, PROT_READ | PROT_WRITE);
	if (res < 0)
	{
		return 0;
	}

	res = madvise(ptr, size, MADV_POPULATE_READ | MADV_POPULATE_WRITE);
	if (res < 0)
	{
		return 0;
	}

	return 1;
}

internal void MemoryDecommit(void* ptr, U64 size)
{
	madvise(ptr, size, MADV_DONTNEED);
	mprotect(ptr, size, PROT_NONE);
}

internal void MemoryRelease(void* ptr, U64 size)
{
	munmap(ptr, size);
}

internal void* MemoryReserveLarge(U64 size)
{
	void* result = mmap(0, size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
	if (result == MAP_FAILED)
	{
		result = 0;
	}
	return result;
}

internal B32 MemoryCommitLarge(void* ptr, U64 size)
{
	S32 res = 0;

	res = mprotect(ptr, size, PROT_READ | PROT_WRITE);
	if (res < 0)
	{
		return 0;
	}

	res = madvise(ptr, size, MADV_POPULATE_READ | MADV_POPULATE_WRITE);
	if (res < 0)
	{
		return 0;
	}

	return 1;
}

///////////////////////////
/// System
#include "../../base_system.h"

internal SystemInfo* GetSystemInfo(void)
{
	local_persist SystemInfo info	= {0};
	local_persist B32		 cached = 0;

	if (!cached)
	{
		info.logical_processor_count = (U32)get_nprocs();
		info.page_size				 = (U64)getpagesize();
		info.allocation_granularity	 = info.page_size;
		info.large_page_size		 = MB(2);
		gethostname(info.machine_name, ArrayCount(info.machine_name));
		cached = 1;
	}

	return &info;
}
