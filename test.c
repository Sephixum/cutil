#include "base/base_core.h"
#include "base/base_include.h"
#include "base/base_typedefs.h"

#include "cglm/cglm.h"

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
	IMap base = MapAlloc(arena, cap, sizeof(String8), sizeof(S32), String8Hash, String8Equal);
	return (Str8IntMap){.base = base};
}

typedef U32 EntityKind;
enum
{
	EntityKind_Transformable   = Bit(1),
	EntityKind_GizmoSelectable = Bit(2),
	EntityKind_Light		   = Bit(3),
	EntityKind_Static		   = Bit(4),
	EntityKind_Drawable		   = Bit(5),
	EntityKind_Named		   = Bit(6),
};

typedef struct Entity Entity;
struct Entity
{
	EntityKind kind;

	struct MeshComponent*	mesh;
	struct Transform*		transform;
	struct GlobalTransform* global_transform;
	struct Name*			name;
	struct LightData*		light;
};

typedef struct EntityList EntityList;
struct EntityList
{
	Entity* v;
	U64		count;
};

void MoveMentSystem(EntityList list)
{
	for
		EachIndex(i, list.count)
		{
			Entity e = list.v[i];
			if (e.kind & EntityKind_Transformable)
			{
				e.transform = 0; // move based on velocity
			}
		}
}

void DrawHierArchy(EntityList list)
{
	for
		EachIndex(i, list.count)
		{
			Entity e = list.v[i];
			if (e.kind & EntityKind_Named)
			{
				e.name = 0; // print
			}
		}
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
		MapInsert(s.arena, &map.base, &key, &value);
	}

	{
		String8 key	  = Str8Lit("99");
		int		value = 99;
		MapInsert(s.arena, &map.base, &key, &value);
	}

	{
		String8 key	  = Str8Lit("49");
		int		value = 49;
		MapInsert(s.arena, &map.base, &key, &value);
	}

	{
		String8 key	  = Str8Lit("25");
		int		value = 25;
		MapInsert(s.arena, &map.base, &key, &value);
	}

	String8 key_25 = Str8Lit("25");
	String8 key_49 = Str8Lit("49");
	String8 key_99 = Str8Lit("99");
	String8 key_1  = Str8Lit("1");
	String8 key_29 = Str8Lit("29");

	int* value_25 = MapLookup(&map.base, &key_25);
	int* value_29 = MapLookup(&map.base, &key_29);

	if (value_29)
	{
		fprintf(stdout, "%*s: %d\n", Str8Varg(key_29), *value_29);
	}

	if (value_25)
	{
		fprintf(stdout, "%*s: %d\n", Str8Varg(key_25), *value_25);
	}
}

#include "base/base_include.c"
