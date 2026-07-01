///////////////////////////
// System
#include "../../base_system.h"
#include "base_include.h"

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
