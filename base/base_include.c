#include "base_arena.c"
#include "base_thread_context.c"
#include "base_string.c"
#include "base_memory.c"
#include "base_hash.c"
#include "base_hashmap.c"
#include "base_math.c"

#if OS_LINUX
#include "platform/linux/base_impl.c"
#elif OS_WINDOWS
#include "platform/win32/base_impl.c"
#endif
