#include "base_arena.c"
#include "base_thread_context.c"
#include "base_string.c"
#include "base_memory.c"

#if OS_LINUX
#include "../linux/base/linux_base_impl.c"
#elif OS_WINDOWS
#include "../win32/base/win32_base_impl.c"
#endif
