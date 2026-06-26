#ifndef BASE_INCLUDE_H
#define BASE_INCLUDE_H

#include "base_arena.h"
#include "base_typedefs.h"
#include "base_core.h"
#include "base_memory.h"
#include "base_system.h"
#include "base_thread_context.h"
#include "base_string.h"
#include "base_hash.h"
#include "base_hashmap.h"

#if OS_LINUX
#include "platform/linux/linux_base_impl.h"
#elif OS_WINDOWS
#include "platform/win32/win32_base_impl.h"
#endif

#endif // BASE_INCLUDE_H
