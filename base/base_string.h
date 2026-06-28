#ifndef BASE_STRING_H
#define BASE_STRING_H

#include "base_typedefs.h"
#include "base_arena.h"

typedef struct String8 String8;
struct String8
{
	U8 *str;
	U64 size;
};

typedef struct String16 String16;
struct String16
{
	U16 *str;
	U64	 size;
};

typedef struct String32 String32;
struct String32
{
	U32 *str;
	U64	 size;
};

typedef struct String8Node String8Node;
struct String8Node
{
	String8Node *next;
	String8		 string;
};

typedef struct String8List String8List;
struct String8List
{
	String8Node *first;
	String8Node *last;
	U64			 node_count;
	U64			 total_size;
};

typedef struct String8Array String8Array;
struct String8Array
{
	String8 *v;
	U64		 count;
	U64		 total_size;
};

typedef U32 StringMatchFlags;
enum
{
	StringMatchFlag_CaseInsensitive	 = Bit(1),
	StringMatchFlag_RightSideSloppy	 = Bit(2),
	StringMatchFlag_SlashInsensitive = Bit(3),
};

typedef U32 StringSplitFlags;
enum
{
	StringSplitFlag_KeepEmpties = Bit(1)
};

typedef U32 PathStyle;
enum
{
	PathStyle_Null			  = Bit(1),
	PathStyle_Relative		  = Bit(2),
	PathStyle_WindowsAbsolute = Bit(3),
	PathStyle_UnixAbsolute,

#if OS_WINDOWS
	PathStyle_SystemAbsolute = PathStyle_WindowsAbsolute
#elif OS_LINUX
	PathStyle_SystemAbsolute = PathStyle_UnixAbsolute
#else
#error Absolute path style is undefined for this OS.
#endif
};

typedef struct StringJoin StringJoin;
struct StringJoin
{
	String8 pre;
	String8 sep;
	String8 post;
};

////////////////////////////////
// Character Classification & Conversion Functions

internal B32 CharIsSpace(U8 c);
internal B32 CharIsUpper(U8 c);
internal B32 CharIsLower(U8 c);
internal B32 CharIsAlpha(U8 c);
internal B32 CharIsSlash(U8 c);
internal B32 CharIsDigit(U8 c, U32 base);
internal U8	 LowerFromChar(U8 c);
internal U8	 UpperFromChar(U8 c);
internal U8	 CorrectSlashFromChar(U8 c);

////////////////////////////////
// C-String Measurement

internal U64 Cstring8Length(U8 *c);
internal U64 Cstring16Length(U16 *c);
internal U64 Cstring32Length(U32 *c);

////////////////////////////////
// String Constructors

#define s(S) Str8LitComp(S)

#define Str8Lit(S) Str8((U8 *)(S), sizeof(S) - 1)
#define Str8LitComp(S)                                                                                                 \
	(String8)                                                                                                          \
	{                                                                                                                  \
		(U8 *)(S), sizeof(S) - 1,                                                                                      \
	}
#define Str8LitCstr(S) Str8((U8 *)(S), sizeof(S))
#define Str8Varg(S) (int)((S).size), ((S).str)

#define Str8Array(S, C) Str8((U8 *)(S), sizeof(*(S)) * (C))
#define Str8ArrayFixed(S) Str8((U8 *)(S), sizeof(S))
#define Str8Struct(S) Str8((U8 *)(S), sizeof(*(S)))

internal String8 Str8(U8 *str, U64 size);
internal String8 Str8Range(U8 *first, U8 *one_past_last);
internal String8 Str8Zero(void);

internal String8 Str8Cstring(char *c);
internal String8 Str8CstringCapped(void *cstr, void *cap);
internal String8 Str8CstringCappedReverse(void *raw_start, void *raw_cap);

////////////////////////////////
// String Formatting & Copying

internal String8 Str8Cat(Arena *arena, String8 s1, String8 s2);
internal String8 Str8Copy(Arena *arena, String8 s);
internal String8 Str8fv(Arena *arena, char *fmt, va_list args);
internal String8 Str8f(Arena *arena, char *fmt, ...);

#define PushStr8Cat(arena, s1, s2) Str8Cat((arena), (s1), (s2))
#define PushStr8Copy(arena, s) Str8Copy((arena), (s))
#define PushStr8fv(arena, fmt, args) Str8fv((arena), (fmt), (args))
#define PushStr8f(arena, ...) Str8f((arena), __VA_ARGS__)

////////////////////////////////
// String Slicing

internal String8 Str8Substr(String8 str, U64 s, U64 f);
internal String8 Str8Prefix(String8 str, U64 size);
internal String8 Str8Skip(String8 str, U64 amt);
internal String8 Str8Postfix(String8 str, U64 size);
internal String8 Str8Chop(String8 str, U64 amt);
internal String8 Str8SkipChopWhitespace(String8 string);
internal String8 Str8SkipChopSlashes(String8 string);

////////////////////////////////
// String <=> Integer Conversions

// string -> integer
internal S64 SignFromStr8(String8 string, String8 *string_tail);
internal B32 Str8IsInteger(String8 string, U32 radix);
internal B32 Str8IsIntegerSigned(String8 string, U32 radix);
internal U64 U64FromStr8(String8 string, U32 radix);
internal S64 S64FromStr8(String8 string, U32 radix);
internal U32 U32FromStr8(String8 string, U32 radix);
internal S32 S32FromStr8(String8 string, U32 radix);
internal B32 TryU64FromStr8CRules(String8 string, U64 *x);
internal B32 TryS64FromStr8CRules(String8 string, S64 *x);

// integer -> string
internal String8 Str8FromMemorySize(Arena *arena, U64 size);
internal String8 Str8FromCount(Arena *arena, U64 count);
internal String8 Str8FromBitsU32(Arena *arena, U32 x);
internal String8 Str8FromBitsU64(Arena *arena, U64 x);
internal String8 Str8FromU64(Arena *arena, U64 u64, U32 radix, U8 min_digits, U8 digit_group_separator);
internal String8 Str8FromS64(Arena *arena, S64 s64, U32 radix, U8 min_digits, U8 digit_group_separator);
internal String8 Str8FromF64(Arena *arena, F64);

////////////////////////////////
// String <=> Float Conversions

internal F64 F64FromStr8(String8 string);

////////////////////////////////
// String Matching

#define Str8MatchLit(a_lit, b, flags) Str8Match(Str8Lit(a_lit), (b), (flags))
#define Str8MatchCstr(a_cstr, b, flags) Str8Match(Str8Cstring(a_cstr), (b), (flags))
internal B32 Str8Match(String8 a, String8 b, StringMatchFlags flags);
internal B32 Str8MatchWildCard(String8 string, String8 pattern, StringMatchFlags flags);
internal U64 Str8FindNeedle(String8 string, U64 start_pos, String8 needle, StringMatchFlags flags);
internal U64 Str8FindNeedleReverse(String8 string, U64 start_pos, String8 needle, StringMatchFlags flags);
internal B32 Str8IsBefore(String8 a, String8 b);
internal B32 Str8CharMatch(U8 a, U8 b, StringMatchFlags flags);

#define Str8EndsWith(string, end, flags) Str8Match(Str8Postfix((string), (end).size), (end), (flags))

////////////////////////////////
// String List Construction Functions

internal String8Node *Str8ListPushNode(String8List *list, String8Node *node);
internal String8Node *Str8ListPushNodeSetString(String8List *list, String8Node *node, String8 string);
internal String8Node *Str8ListPushNodeFront(String8List *list, String8Node *node);
internal String8Node *Str8ListPushNodeFrontSetString(String8List *list, String8Node *node, String8 string);
internal String8Node *Str8ListPush(Arena *arena, String8List *list, String8 string);
internal String8Node *Str8ListPushFront(Arena *arena, String8List *list, String8 string);
internal void		  Str8ListConcatInPlace(String8List *list, String8List *to_push);
internal String8Node *Str8ListPushAligner(Arena *arena, String8List *list, U64 min, U64 align);
internal String8Node *Str8ListPushf(Arena *arena, String8List *list, char *fmt, ...);
internal String8Node *Str8ListPushFrontf(Arena *arena, String8List *list, char *fmt, ...);
internal String8Node *Str8ListPopFront(String8List *list);
internal String8List  Str8ListCopy(Arena *arena, String8List *list);
internal String8List  Str8ListSubstr(Arena *arena, String8List list, U64 s, U64 f);
#define Str8ListFirst(list) ((list)->first ? (list)->first->string : Str8Zero())

////////////////////////////////
// String Arrays

internal String8Array Str8ArrayZero(void);
internal String8Array Str8ArrayFrom_list(Arena *arena, String8List *list);
internal String8Array Str8ArrayReserve(Arena *arena, U64 count);
internal String8Array Str8ArrayCopy(Arena *arena, String8Array array);

#endif // BASE_STRING_H
