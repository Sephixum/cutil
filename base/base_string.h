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

internal B32 Char_IsSpace(U8 c);
internal B32 Char_IsUpper(U8 c);
internal B32 Char_IsLower(U8 c);
internal B32 Char_IsAlpha(U8 c);
internal B32 Char_IsSlash(U8 c);
internal B32 Char_IsDigit(U8 c, U32 base);
internal U8	 Char_LowerFrom(U8 c);
internal U8	 Char_UpperFrom(U8 c);
internal U8	 Char_CorrectSlashFrom(U8 c);

////////////////////////////////
// C-String Measurement

internal U64 CString8_Length(U8 *c);

////////////////////////////////
// String Constructors

#define s(S) String8_LitComp(S)

#define String8_Lit(S) String8_Make((U8 *)(S), sizeof(S) - 1)
#define String8_LitComp(S)                                                                                             \
	(String8)                                                                                                          \
	{                                                                                                                  \
		(U8 *)(S), sizeof(S) - 1,                                                                                      \
	}
#define String8_LitCstr(S) Str8((U8 *)(S), sizeof(S))
#define String8_Varg(S) (int)((S).size), ((S).str)

#define String8_Array(S, C) Str8((U8 *)(S), sizeof(*(S)) * (C))
#define String8_ArrayFixed(S) Str8((U8 *)(S), sizeof(S))
#define String8_Struct(S) Str8((U8 *)(S), sizeof(*(S)))

internal String8 String8_Make(U8 *str, U64 size);
internal String8 String8_Range(U8 *first, U8 *one_past_last);
internal String8 String8_Zero(void);

internal String8 String8_CString(char *c);
internal String8 String8_CStringCapped(void *cstr, void *cap);
internal String8 String8_CStringCappedReverse(void *raw_start, void *raw_cap);

////////////////////////////////
// String Formatting & Copying

internal String8 String8_Cat(Arena *arena, String8 s1, String8 s2);
internal String8 String8_Copy(Arena *arena, String8 s);
internal String8 String8_Fv(Arena *arena, char *fmt, va_list args);
internal String8 String8_F(Arena *arena, char *fmt, ...);

#define Arena_PushString8_Cat(arena, s1, s2) String8_Cat((arena), (s1), (s2))
#define Arena_PushString8_Copy(arena, s) String8_Copy((arena), (s))
#define Arena_PushString8_Fv(arena, fmt, args) String8_Fv((arena), (fmt), (args))
#define Arena_PushString8_F(arena, ...) String8_F((arena), __VA_ARGS__)

////////////////////////////////
// String Slicing

internal String8 String8_Substr(String8 str, U64 s, U64 f);
internal String8 String8_Prefix(String8 str, U64 size);
internal String8 String8_Skip(String8 str, U64 amt);
internal String8 String8_Postfix(String8 str, U64 size);
internal String8 String8_Chop(String8 str, U64 amt);
internal String8 String8_SkipChopWhitespace(String8 string);
internal String8 String8_SkipChopSlashes(String8 string);

////////////////////////////////
// String <=> Integer Conversions

// string -> integer
internal S64 String8_SignFrom(String8 string, String8 *string_tail);
internal B32 String8_IsInteger(String8 string, U32 radix);
internal B32 String8_IsIntegerSigned(String8 string, U32 radix);
internal U64 String8_U64From(String8 string, U32 radix);
internal S64 String8_S64From(String8 string, U32 radix);
internal U32 String8_U32From(String8 string, U32 radix);
internal S32 String8_S32From(String8 string, U32 radix);
internal B32 String8_TryU64FromCRules(String8 string, U64 *x);
internal B32 String8_TryS64FromCRules(String8 string, S64 *x);

// integer -> string
internal String8 String8_FromMemorySize(Arena *arena, U64 size);
internal String8 String8_FromCount(Arena *arena, U64 count);
internal String8 String8_FromBitsU32(Arena *arena, U32 x);
internal String8 String8_FromBitsU64(Arena *arena, U64 x);
internal String8 String8_FromU64(Arena *arena, U64 u64, U32 radix, U8 min_digits, U8 digit_group_separator);
internal String8 String8_FromS64(Arena *arena, S64 s64, U32 radix, U8 min_digits, U8 digit_group_separator);
internal String8 String8_FromF64(Arena *arena, F64);

////////////////////////////////
// String <=> Float Conversions

internal F64 String8_F64From(String8 string);

////////////////////////////////
// String Matching

#define String8_MatchLit(a_lit, b, flags) String8_Match(Str8Lit(a_lit), (b), (flags))
#define String8_MatchCString(a_cstr, b, flags) String8_Match(String8_CString(a_cstr), (b), (flags))
#define String8_EndsWith(string, end, flags) String8_Match(Str8Postfix((string), (end).size), (end), (flags))

internal B32 String8_Match(String8 a, String8 b, StringMatchFlags flags);
internal B32 String8_MatchWildCard(String8 string, String8 pattern, StringMatchFlags flags);
internal U64 String8_FindNeedle(String8 string, U64 start_pos, String8 needle, StringMatchFlags flags);
internal U64 String8_FindNeedleReverse(String8 string, U64 start_pos, String8 needle, StringMatchFlags flags);
internal B32 String8_IsBefore(String8 a, String8 b);
internal B32 String8_CharMatch(U8 a, U8 b, StringMatchFlags flags);

////////////////////////////////
// String List Construction Functions

internal String8Node *String8List_PushNode(String8List *list, String8Node *node);
internal String8Node *String8List_PushNodeSetString(String8List *list, String8Node *node, String8 string);
internal String8Node *String8List_PushNodeFront(String8List *list, String8Node *node);
internal String8Node *String8List_PushNodeFrontSetString(String8List *list, String8Node *node, String8 string);
internal String8Node *String8List_Push(Arena *arena, String8List *list, String8 string);
internal String8Node *String8List_PushFront(Arena *arena, String8List *list, String8 string);
internal void		  String8List_ConcatInPlace(String8List *list, String8List *to_push);
internal String8Node *String8List_PushAligner(Arena *arena, String8List *list, U64 min, U64 align);
internal String8Node *String8List_Pushf(Arena *arena, String8List *list, char *fmt, ...);
internal String8Node *String8List_PushFrontf(Arena *arena, String8List *list, char *fmt, ...);
internal String8Node *String8List_PopFront(String8List *list);
internal String8List  String8List_Copy(Arena *arena, String8List *list);
internal String8List  String8List_Substr(Arena *arena, String8List list, U64 s, U64 f);
#define String8_ListFirst(list) ((list)->first ? (list)->first->string : String8_Zero())

////////////////////////////////
// String Arrays

internal String8Array String8Array_Zero(void);
internal String8Array String8Array_FromList(Arena *arena, String8List *list);
internal String8Array String8Array_Reserve(Arena *arena, U64 count);
internal String8Array String8Array_Copy(Arena *arena, String8Array array);

#endif // BASE_STRING_H
