#include "base/base_include.h"

#define LOG(str, ...) fprintf(stderr, "[%*s] " str, Str8Varg(ThreadContextSelectedName()), __VA_ARGS__)

typedef struct String8String8Map String8String8Map;
struct String8String8Map
{
	IMap base;
};
internal U64 String8Hash(void* value)
{
	return HashStr8(*(String8*)value);
}
internal B32 String8Equal(void* lhs, void* rhs)
{
	String8* a = (String8*)lhs;
	String8* b = (String8*)rhs;
	return Str8Match(*a, *b, (StringMatchFlags)0);
}
internal String8String8Map String8String8MapAlloc(Arena* arena, U64 cap)
{
	IMap new_base = MapAlloc(arena, cap, sizeof(String8), sizeof(String8), String8Hash, String8Equal);
	return (String8String8Map){.base = new_base};
}

typedef struct U64String8Map U64String8Map;
struct U64String8Map
{
	IMap base;
};
internal U64 U64Hash(void* value)
{
	return HashU64(*(U64*)value);
}
internal B32 U64Equal(void* lhs, void* rhs)
{
	U64* a = (U64*)lhs;
	U64* b = (U64*)rhs;
	return (*a == *b);
}
internal U64String8Map U64String8MapAlloc(Arena* arena, U64 cap)
{
	IMap new_base = MapAlloc(arena, cap, sizeof(U64), sizeof(String8), U64Hash, U64Equal);
	return (U64String8Map){.base = new_base};
}

int main(void)
{
	{
		ThreadContext* ctx = ThreadContextAlloc();
		ThreadContextSetName(ctx, s("MainThread"));
		ThreadContextSelect(ctx);
	}
	{
		Arena* arena = ArenaAlloc();

		U64String8Map	  u64_map	  = U64String8MapAlloc(arena, 0);
		String8String8Map string8_map = String8String8MapAlloc(arena, 0);

		MapInsert(arena, &u64_map.base, &(U64){1}, &s("1"));
		MapInsert(arena, &u64_map.base, &(U64){1}, &s("1"));
		MapInsert(arena, &u64_map.base, &(U64){1}, &s("1"));
		MapInsert(arena, &u64_map.base, &(U64){2}, &s("2"));
		MapInsert(arena, &u64_map.base, &(U64){3}, &s("3"));

		String8* val = MapLookup(&u64_map.base, &(U64){3});
		if (val)
		{
			LOG("value: %*s\n", Str8Varg(*val));
		}

		LOG("%lu", u64_map.base.cap);

		ArenaRelease(arena);
	}
}

#include "base/base_include.c"
