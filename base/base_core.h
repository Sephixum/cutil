#ifndef CORE_H
#define CORE_H

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <string.h>

#if defined(__clang__)

#define COMPILER_CLANG 1

#if defined(_WIN32)
#define OS_WINDOWS 1
#elif defined(__gnu_linux__) || defined(__linux__)
#define OS_LINUX 1
#elif defined(__APPLE__) && defined(__MACH__)
#define OS_MAC 1
#else
#error This compiler/OS combo is not supported.
#endif

#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64)
#define ARCH_X64 1
#elif defined(i386) || defined(__i386) || defined(__i386__)
#define ARCH_X86 1
#elif defined(__aarch64__)
#define ARCH_ARM64 1
#elif defined(__arm__)
#define ARCH_ARM32 1
#else
#error Architecture not supported.
#endif

////////////////////////////////
// MSVC OS/Arch Cracking

#elif defined(_MSC_VER)

#define COMPILER_MSVC 1

#if _MSC_VER >= 1920
#define COMPILER_MSVC_YEAR 2019
#elif _MSC_VER >= 1910
#define COMPILER_MSVC_YEAR 2017
#elif _MSC_VER >= 1900
#define COMPILER_MSVC_YEAR 2015
#elif _MSC_VER >= 1800
#define COMPILER_MSVC_YEAR 2013
#elif _MSC_VER >= 1700
#define COMPILER_MSVC_YEAR 2012
#elif _MSC_VER >= 1600
#define COMPILER_MSVC_YEAR 2010
#elif _MSC_VER >= 1500
#define COMPILER_MSVC_YEAR 2008
#elif _MSC_VER >= 1400
#define COMPILER_MSVC_YEAR 2005
#else
#define COMPILER_MSVC_YEAR 0
#endif

#if defined(_WIN32)
#define OS_WINDOWS 1
#else
#error This compiler/OS combo is not supported.
#endif

#if defined(_M_AMD64)
#define ARCH_X64 1
#elif defined(_M_IX86)
#define ARCH_X86 1
#elif defined(_M_ARM64)
#define ARCH_ARM64 1
#elif defined(_M_ARM)
#define ARCH_ARM32 1
#else
#error Architecture not supported.
#endif

////////////////////////////////
// GCC OS/Arch Cracking

#elif defined(__GNUC__) || defined(__GNUG__)

#define COMPILER_GCC 1

#if defined(__gnu_linux__) || defined(__linux__)
#define OS_LINUX 1
#else
#error This compiler/OS combo is not supported.
#endif

#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64)
#define ARCH_X64 1
#elif defined(i386) || defined(__i386) || defined(__i386__)
#define ARCH_X86 1
#elif defined(__aarch64__)
#define ARCH_ARM64 1
#elif defined(__arm__)
#define ARCH_ARM32 1
#else
#error Architecture not supported.
#endif

#else
#error Compiler not supported.
#endif

////////////////////////////////
// Arch Cracking

#if defined(ARCH_X64)
#define ARCH_64BIT 1
#elif defined(ARCH_X86)
#define ARCH_32BIT 1
#endif

#if ARCH_ARM32 || ARCH_ARM64 || ARCH_X64 || ARCH_X86
#define ARCH_LITTLE_ENDIAN 1
#else
#error Endianness of this architecture not understood by context cracker.
#endif

////////////////////////////////
// Language Cracking

#if defined(__cplusplus)
#define LANG_CPP 1
#else
#define LANG_C 1
#endif

////////////////////////////////
// Build Option Cracking

#if !defined(BUILD_DEBUG)
#define BUILD_DEBUG 1
#endif

#if !defined(BUILD_SUPPLEMENTARY_UNIT)
#define BUILD_SUPPLEMENTARY_UNIT 0
#endif

#if !defined(BUILD_ENTRY_DEFINING_UNIT)
#define BUILD_ENTRY_DEFINING_UNIT 1
#endif

#if !defined(BUILD_CONSOLE_INTERFACE)
#define BUILD_CONSOLE_INTERFACE 0
#endif

#if !defined(BUILD_VERSION_MAJOR)
#define BUILD_VERSION_MAJOR 0
#endif

#if !defined(BUILD_VERSION_MINOR)
#define BUILD_VERSION_MINOR 0
#endif

#if !defined(BUILD_VERSION_PATCH)
#define BUILD_VERSION_PATCH 1
#endif

#define BUILD_VERSION_STRING_LITERAL                                                                                   \
	Stringify(BUILD_VERSION_MAJOR) "." Stringify(BUILD_VERSION_MINOR) "." Stringify(BUILD_VERSION_PATCH)
#if BUILD_DEBUG
#define BUILD_MODE_STRING_LITERAL_APPEND " [Debug]"
#else
#define BUILD_MODE_STRING_LITERAL_APPEND ""
#endif

////////////////////////////////
// Zero All Undefined Options

#if !defined(ARCH_32BIT)
#define ARCH_32BIT 0
#endif
#if !defined(ARCH_64BIT)
#define ARCH_64BIT 0
#endif
#if !defined(ARCH_X64)
#define ARCH_X64 0
#endif
#if !defined(ARCH_X86)
#define ARCH_X86 0
#endif
#if !defined(ARCH_ARM64)
#define ARCH_ARM64 0
#endif
#if !defined(ARCH_ARM32)
#define ARCH_ARM32 0
#endif
#if !defined(COMPILER_MSVC)
#define COMPILER_MSVC 0
#endif
#if !defined(COMPILER_GCC)
#define COMPILER_GCC 0
#endif
#if !defined(COMPILER_CLANG)
#define COMPILER_CLANG 0
#endif
#if !defined(OS_WINDOWS)
#define OS_WINDOWS 0
#endif
#if !defined(OS_LINUX)
#define OS_LINUX 0
#endif
#if !defined(OS_MAC)
#define OS_MAC 0
#endif
#if !defined(LANG_CPP)
#define LANG_CPP 0
#endif
#if !defined(LANG_C)
#define LANG_C 0
#endif

////////////////////////////////
// Unsupported Errors

#if ARCH_X86
#error You tried to build in x86 (32 bit) mode, but currently, only building in x64 (64 bit) mode is supported.
#endif

#if !ARCH_X64
#error You tried to build with an unsupported architecture. Currently, only building in x64 mode is supported.
#endif

#define MemoryZero(s, z) memset((s), 0, (z))
#define MemoryZeroStruct(s) MemoryZero((s), sizeof(*(s)))
#define MemoryZeroArray(a) MemoryZero((a), sizeof(a))
#define MemoryZeroTyped(m, c) MemoryZero((m), sizeof(*(m)) * (c))

////////////////////////////////
// Clamps, Mins, Maxes

#define Min(A, B) (((A) < (B)) ? (A) : (B))
#define Max(A, B) (((A) > (B)) ? (A) : (B))
#define ClampTop(A, X) Min(A, X)
#define ClampBot(X, B) Max(X, B)
#define Clamp(A, X, B) (((X) < (A)) ? (A) : ((X) > (B)) ? (B) : (X))

////////////////////////////////
// Member Offsets

#define Member(T, m) (((T*)0)->m)
#define OffsetOf(T, m) offsetof(T, m)
#define MemberFromOffset(T, ptr, off) (T)((((U8*)ptr) + (off)))
#define MemberFromPtr(T, ptr, m) (void*)((((U8*)ptr) + OffsetOf(T, m)))
#define CastFromMember(T, m, ptr) (T*)(((U8*)ptr) - OffsetOf(T, m))

////////////////////////////////
// Alignment Helpers

#define Compose64Bit(a, b) ((((U64)a) << 32) | ((U64)b))
#define Compose32Bit(a, b) ((((U32)a) << 16) | ((U32)b))
#define AlignPow2(x, b) (((x) + (b) - 1) & (~((b) - 1)))
#define AlignDownPow2(x, b) ((x) & (~((b) - 1)))
#define AlignPadPow2(x, b) ((0 - (x)) & ((b) - 1))
#define IsPow2(x) ((x) != 0 && ((x) & ((x) - 1)) == 0)
#define IsPow2OrZero(x) ((((x) - 1) & (x)) == 0)

////////////////////////////////
// Linked List Building Macros

// linked list macro helpers
#define CheckNil(nil, p) ((p) == 0 || (p) == nil)
#define SetNil(nil, p) ((p) = nil)

// doubly-linked-lists
#define DLLInsert_NPZ(nil, f, l, p, n, next, prev)                                                                     \
	(CheckNil(nil, f)	? ((f) = (l) = (n), SetNil(nil, (n)->next), SetNil(nil, (n)->prev))                            \
	 : CheckNil(nil, p) ? ((n)->next = (f), (f)->prev = (n), (f) = (n), SetNil(nil, (n)->prev))                        \
	 : ((p) == (l))		? ((l)->next = (n), (n)->prev = (l), (l) = (n), SetNil(nil, (n)->next))                        \
						: (((!CheckNil(nil, p) && CheckNil(nil, (p)->next)) ? (0) : ((p)->next->prev = (n))),          \
						   ((n)->next = (p)->next),                                                                    \
						   ((p)->next = (n)),                                                                          \
						   ((n)->prev = (p))))

#define DLLPushBack_NPZ(nil, f, l, n, next, prev) DLLInsert_NPZ(nil, f, l, l, n, next, prev)
#define DLLPushFront_NPZ(nil, f, l, n, next, prev) DLLInsert_NPZ(nil, l, f, f, n, prev, next)
#define DLLRemove_NPZ(nil, f, l, n, next, prev)                                                                        \
	(((n) == (f) ? (f) = (n)->next : (0)),                                                                             \
	 ((n) == (l) ? (l) = (l)->prev : (0)),                                                                             \
	 (CheckNil(nil, (n)->prev) ? (0) : ((n)->prev->next = (n)->next)),                                                 \
	 (CheckNil(nil, (n)->next) ? (0) : ((n)->next->prev = (n)->prev)))

// singly-linked, doubly-headed lists (queues)
#define SLLQueuePush_NZ(nil, f, l, n, next)                                                                            \
	(CheckNil(nil, f) ? ((f) = (l) = (n), SetNil(nil, (n)->next))                                                      \
					  : ((l)->next = (n), (l) = (n), SetNil(nil, (n)->next)))

#define SLLQueuePushFront_NZ(nil, f, l, n, next)                                                                       \
	(CheckNil(nil, f) ? ((f) = (l) = (n), SetNil(nil, (n)->next)) : ((n)->next = (f), (f) = (n)))

#define SLLQueuePop_NZ(nil, f, l, next) ((f) == (l) ? (SetNil(nil, f), SetNil(nil, l)) : ((f) = (f)->next))

// singly-linked, singly-headed lists (stacks)
#define SLLStackPush_N(f, n, next) ((n)->next = (f), (f) = (n))
#define SLLStackPop_N(f, next) ((f) = (f)->next)

// doubly-linked-list helpers
#define DLLInsert_NP(f, l, p, n, next, prev) DLLInsert_NPZ(0, f, l, p, n, next, prev)
#define DLLPushBack_NP(f, l, n, next, prev) DLLPushBack_NPZ(0, f, l, n, next, prev)
#define DLLPushFront_NP(f, l, n, next, prev) DLLPushFront_NPZ(0, f, l, n, next, prev)
#define DLLRemove_NP(f, l, n, next, prev) DLLRemove_NPZ(0, f, l, n, next, prev)
#define DLLInsert(f, l, p, n) DLLInsert_NPZ(0, f, l, p, n, next, prev)
#define DLLPushBack(f, l, n) DLLPushBack_NPZ(0, f, l, n, next, prev)
#define DLLPushFront(f, l, n) DLLPushFront_NPZ(0, f, l, n, next, prev)
#define DLLRemove(f, l, n) DLLRemove_NPZ(0, f, l, n, next, prev)

// singly-linked, doubly-headed list helpers
#define SLLQueuePush_N(f, l, n, next) SLLQueuePush_NZ(0, f, l, n, next)
#define SLLQueuePushFront_N(f, l, n, next) SLLQueuePushFront_NZ(0, f, l, n, next)
#define SLLQueuePop_N(f, l, next) SLLQueuePop_NZ(0, f, l, next)
#define SLLQueuePush(f, l, n) SLLQueuePush_NZ(0, f, l, n, next)
#define SLLQueuePushFront(f, l, n) SLLQueuePushFront_NZ(0, f, l, n, next)
#define SLLQueuePop(f, l) SLLQueuePop_NZ(0, f, l, next)

// singly-linked, singly-headed list helpers
#define SLLStackPush(f, n) SLLStackPush_N(f, n, next)
#define SLLStackPop(f) SLLStackPop_N(f, next)

////////////////////////////////
// Singly-linked, singly-headed lists (stacks)

#define SLLStackPush_N(f, n, next) ((n)->next = (f), (f) = (n))
#define SLLStackPop_N(f, next) ((f) = (f)->next)

// Singly-linked, singly-headed list helpers
#define SLLStackPush(f, n) SLLStackPush_N(f, n, next)
#define SLLStackPop(f) SLLStackPop_N(f, next)

////////////////////////////////
// Versions
#define Version(major, minor, patch)                                                                                   \
	(U64)((((U64)(major) & 0xffff) << 32) | ((((U64)(minor) & 0xffff) << 16)) | ((((U64)(patch) & 0xffff) << 0)))
#define MajorFromVersion(version) (((version) & 0xffff00000000ull) >> 32)
#define MinorFromVersion(version) (((version) & 0x0000ffff0000ull) >> 16)
#define PatchFromVersion(version) (((version) & 0x00000000ffffull) >> 0)

////////////////////////////////
// For-Loop Construct Macros

#define DeferLoop(begin, end) for (int _i_ = ((begin), 0); !_i_; _i_ += 1, (end))
#define DeferLoopChecked(begin, end) for (int _i_ = 2 * !(begin); (_i_ == 2 ? ((end), 0) : !_i_); _i_ += 1, (end))

#define EachIndex(it, count) (U64 it = 0; it < (count); it += 1)
#define EachElement(it, array) (U64 it = 0; it < ArrayCount(array); it += 1)
#define EachEnumVal(type, it) (type it = (type)0; it < type##_COUNT; it = (type)(it + 1))
#define EachNonZeroEnumVal(type, it) (type it = (type)1; it < type##_COUNT; it = (type)(it + 1))
#define EachInRange(it, range) (U64 it = (range).min; it < (range).max; it += 1)
#define EachNode(it, T, first) (T* it = first; it != 0; it = it->next)
#define EachBit(it, flags)                                                                                             \
	(U64(_i_) = (flags), it = (flags) & -(flags); (_i_) != 0; (_i_) &= ((_i_) - 1), it = (flags) & -(flags))

////////////////////////////////
//~ rjf: Memory Operation Macros

#define MemoryCopy(dst, src, size) memmove((dst), (src), (size))
#define MemorySet(dst, byte, size) memset((dst), (byte), (size))
#define MemoryCompare(a, b, size) memcmp((a), (b), (size))

#define MemoryCopyStruct(d, s) MemoryCopy((d), (s), sizeof(*(d)))
#define MemoryCopyArray(d, s) MemoryCopy((d), (s), sizeof(d))
#define MemoryCopyTyped(d, s, c) MemoryCopy((d), (s), sizeof(*(d)) * (c))
#define MemoryCopyStr8(dst, s) MemoryCopy(dst, (s).str, (s).size)

#define MemoryZero(s, z) memset((s), 0, (z))
#define MemoryZeroStruct(s) MemoryZero((s), sizeof(*(s)))
#define MemoryZeroArray(a) MemoryZero((a), sizeof(a))
#define MemoryZeroTyped(m, c) MemoryZero((m), sizeof(*(m)) * (c))

#define MemoryMatch(a, b, z) (MemoryCompare((a), (b), (z)) == 0)
#define MemoryMatchStruct(a, b) MemoryMatch((a), (b), sizeof(*(a)))
#define MemoryMatchArray(a, b) MemoryMatch((a), (b), sizeof(a))

#define MemoryIsZeroStruct(ptr) memory_is_zero((ptr), sizeof(*(ptr)))

////////////////////////////////
//~ rjf: Asserts

#if COMPILER_MSVC
#define Trap() __debugbreak()
#elif COMPILER_CLANG || COMPILER_GCC
#define Trap() __builtin_trap()
#else
#error Unknown trap intrinsic for this compiler.
#endif

#define AssertAlways(x)                                                                                                \
	do                                                                                                                 \
	{                                                                                                                  \
		if (!(x))                                                                                                      \
		{                                                                                                              \
			Trap();                                                                                                    \
		}                                                                                                              \
	} while (0)
#if BUILD_DEBUG
#define Assert(x) AssertAlways(x)
#else
#define Assert(x) (void)(x)
#endif
#define InvalidPath Assert(!"Invalid Path!")
#define NotImplemented Assert(!"Not Implemented!")
#define NoOp ((void)0)
#define StaticAssert(C, ID) global U8 Glue(ID, __LINE__)[(C) ? 1 : -1]

#endif // CORE_H
