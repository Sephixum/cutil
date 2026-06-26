#include "base/base_hash.h"
#include "base/base_hashmap.h"
#include "base/base_include.h"
#include "base/base_include.c"
#include "base/base_string.h"
#include "base/base_thread_context.h"

typedef struct Str8IntMap Str8IntMap;
struct Str8IntMap
{
	IMap base;
};

internal U64 String8Hash(void* key)
{
	String8 str_key = *(String8*)key;
	return HashStr8(str_key);
}

internal B32 String8Equal(void* lhs, void* rhs)
{
	String8 a = *(String8*)lhs;
	String8 b = *(String8*)rhs;
	return Str8Match(a, b, (StringMatchFlags)0);
}

internal Str8IntMap Str8IntMapAlloc(Arena* arena, U64 cap)
{
	IMap base = IMapAlloc(arena, cap, sizeof(String8), sizeof(S32), String8Hash, String8Equal);
	return (Str8IntMap){.base = base};
}

int main(void)
{
	{
		ThreadContext* main_thread = ThreadContextAlloc();
		ThreadContextSetName(main_thread, Str8Lit("MainThread"));
		ThreadContextSelect(main_thread);
	}

	Temp	   s   = ScratchBegin(0, 0);
	Str8IntMap map = Str8IntMapAlloc(s.arena, 128);
	{
		String8 key	  = Str8Lit("1");
		int		value = 1;
		IMapInsert(s.arena, &map.base, &key, &value);
	}

	{
		String8 key	  = Str8Lit("99");
		int		value = 99;
		IMapInsert(s.arena, &map.base, &key, &value);
	}

	{
		String8 key	  = Str8Lit("49");
		int		value = 49;
		IMapInsert(s.arena, &map.base, &key, &value);
	}

	{
		String8 key	  = Str8Lit("25");
		int		value = 25;
		IMapInsert(s.arena, &map.base, &key, &value);
	}

	String8 key_25 = Str8Lit("25");
	String8 key_49 = Str8Lit("49");
	String8 key_99 = Str8Lit("99");
	String8 key_1  = Str8Lit("1");
	String8 key_29 = Str8Lit("29");

	int* value_25 = IMapLookup(&map.base, &key_25);
	int* value_29 = IMapLookup(&map.base, &key_29);

	if (value_29)
	{
		fprintf(stdout, "%*s: %d\n", Str8Varg(key_29), *value_29);
	}

	if (value_25)
	{
		fprintf(stdout, "%*s: %d\n", Str8Varg(key_25), *value_25);
	}
}
