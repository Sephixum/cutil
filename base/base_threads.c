#include "base_threads.h"
#include "base_string.h"
#include "base_system.h"
#include "base_thread_context.h"

internal StripeArray StripeArrayAlloc(Arena *arena)
{
	StripeArray array = {0};
	array.count		  = GetSystemInfo()->logical_processor_count;
	array.v			  = ArenaPushArray(arena, Stripe, array.count);
  for
	  EachIndex(idx, array.count)
	  {
		  array.v[idx].arena	= ArenaAlloc();
		  array.v[idx].rw_mutex = RWMutexAlloc();
		  array.v[idx].cv		= CondVarAlloc();
	  }
  return array;
}

internal void StripeArrayRelease(StripeArray *stripes)
{
  for
	  EachIndex(idx, stripes->count)
	  {
		  ArenaRelease(stripes->v[idx].arena);
		  RWMutexRelease(stripes->v[idx].rw_mutex);
		  CondVarRelease(stripes->v[idx].cv);
	  }
}

internal Stripe *StripeFromSlotIndex(StripeArray *stripes, U64 slot_idx)
{
	Stripe *stripe = &stripes->v[slot_idx % stripes->count];
	return stripe;
}

internal void SetThreadName(String8 string)
{
	SetPlatformthreadName(string);
}

internal void SetThreadNameF(char *fmt, ...)
{
	Temp	scratch = ScratchBegin(0, 0);
	va_list args;
	va_start(args, fmt);
	String8 string = PushStr8fv(scratch.arena, fmt, args);
	SetThreadName(string);
	va_end(args);
	ScratchEnd(scratch);
}
