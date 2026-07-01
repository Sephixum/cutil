#ifndef BASE_SYSTEM_h
#define BASE_SYSTEM_h

#include "base_core.h"
#include "base_typedefs.h"

typedef struct SystemInfo SystemInfo;
struct SystemInfo
{
	U32	 logical_processor_count;
	U64	 page_size;
	U64	 large_page_size;
	U64	 allocation_granularity;
	char machine_name[32];
};

internal SystemInfo* GetSystemInfo(void);

#endif // BASE_SYSTEM_h
