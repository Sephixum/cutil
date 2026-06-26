#include "base_string.h"
#include "base_arena.h"
#include "base_typedefs.h"
#include "base_thread_context.h"
#include "base_core.h"
#include <stdio.h>
#include <stdlib.h>

internal B32 CharIsSpace(U8 c)
{
	return (c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\f' || c == '\v');
}

internal B32 CharIsUpper(U8 c)
{
	return ('A' <= c && c <= 'Z');
}

internal B32 CharIsLower(U8 c)
{
	return ('a' <= c && c <= 'z');
}

internal B32 CharIsAlpha(U8 c)
{
	return (CharIsLower(c) || CharIsUpper(c));
}

internal B32 CharIsSlash(U8 c)
{
	return (c == '/' || c == '\\');
}

internal B32 CharIsDigit(U8 c, U32 base)
{
	B32 result = 0;
	if (0 < base && base <= 16)
	{
		U8 val = integer_symbol_reverse[c];
		if (val < base)
		{
			result = 1;
		}
	}
	return result;
}

internal U8 LowerFromChar(U8 c)
{
	if (CharIsUpper(c))
	{
		c += ('a' - 'A');
	}
	return c;
}

internal U8 UpperFromChar(U8 c)
{
	if (CharIsLower(c))
	{
		c += ('A' - 'a');
	}
	return c;
}

internal U8 CorrectSlashFromChar(U8 c)
{
	if (CharIsSlash(c))
	{
		c = '/';
	}
	return c;
}

internal U64 Cstring8Length(U8 *c)
{
	U64 length = 0;
	if (c)
	{
		U8 *p = c;
		while (*p != 0)
		{
			length = (U64)(p - c);
			p += 1;
		}
	}
	return length;
}

internal U64 Cstring16Length(U16 *c)
{
	U64 length = 0;
	if (c)
	{
		U16 *p = c;
		while (*p != 0)
		{
			length = (U64)(p - c);
			p += 1;
		}
	}
	return length;
}

internal U64 Cstring32Length(U32 *c)
{
	U64 length = 0;
	if (c)
	{
		U32 *p = c;
		while (*p != 0)
		{
			length = (U64)(p - c);
			p += 1;
		}
	}
	return length;
}

internal String8 Str8(U8 *str, U64 size)
{
	return (String8){str, size};
}

internal String8 Str8Range(U8 *first, U8 *one_past_last)
{
	return (String8){first, (U64)(one_past_last - first)};
}

internal String8 Str8Zero(void)
{
	return (String8){0};
}

internal String8 Str8Cstring(char *c)
{
	String8 result = {(U8 *)c, Cstring8Length((U8 *)c)};
	return result;
}

internal String8 Str8CstringCapped(void *cstr, void *cap)
{
	char *ptr = (char *)cstr;
	char *opl = (char *)cap;
	for (; ptr < opl && *ptr != 0; ptr += 1)
		;
	U64		size   = (U64)(ptr - (char *)cstr);
	String8 result = Str8((U8 *)cstr, size);
	return result;
}

internal String8 Str8CstringCappedReverse(void *raw_start, void *raw_cap)
{
	U8 *start = raw_start;
	U8 *ptr	  = raw_cap;
	for (; ptr > start;)
	{
		ptr -= 1;
		if (*ptr == '\0')
		{
			break;
		}
	}
	U64		size   = (U64)(ptr - start);
	String8 result = Str8(start, size);
	return result;
}

internal String8 Str8Substr(String8 str, U64 s, U64 f)
{
	s = ClampTop(s, str.size);
	f = ClampTop(f, str.size);
	str.str += s;
	str.size = ((f > s) ? (f - s) : 0);
	return str;
}

internal String8 Str8Prefix(String8 str, U64 size)
{
	str.size = ClampTop(size, str.size);
	return str;
}

internal String8 Str8Skip(String8 str, U64 amt)
{
	amt = ClampTop(amt, str.size);
	str.str += amt;
	str.size -= amt;
	return str;
}

internal String8 Str8Postfix(String8 str, U64 size)
{
	size	 = ClampTop(size, str.size);
	str.str	 = (str.str + str.size) - size;
	str.size = size;
	return str;
}

internal String8 Str8Chop(String8 str, U64 amt)
{
	amt = ClampTop(amt, str.size);
	str.size -= amt;
	return str;
}

internal String8 Str8SkipChopWhitespace(String8 string)
{
	U8 *first = string.str;
	U8 *opl	  = first + string.size;
	for (; first < opl; first += 1)
	{
		if (!CharIsSpace(*first))
		{
			break;
		}
	}
	for (; opl > first;)
	{
		opl -= 1;
		if (!CharIsSpace(*opl))
		{
			opl += 1;
			break;
		}
	}
	String8 result = Str8Range(first, opl);
	return result;
}

internal String8 Str8SkipChopSlashes(String8 string)
{
	U8 *first = string.str;
	U8 *opl	  = first + string.size;
	for (; first < opl; first += 1)
	{
		if (!CharIsSlash(*first))
		{
			break;
		}
	}
	for (; opl > first;)
	{
		opl -= 1;
		if (!CharIsSlash(*opl))
		{
			opl += 1;
			break;
		}
	}
	String8 result = Str8Range(first, opl);
	return result;
}

////////////////////////////////
// String Formatting & Copying

internal String8 Str8Cat(Arena *arena, String8 s1, String8 s2)
{
	String8 str;
	str.size = s1.size + s2.size;
	str.str	 = ArenaPushArrayNoZero(arena, U8, str.size + 1);
	memcpy(str.str, s1.str, s1.size);
	memcpy(str.str + s1.size, s2.str, s2.size);
	str.str[str.size] = 0;
	return str;
}

internal String8 Str8Copy(Arena *arena, String8 s)
{
	String8 str;
	str.size = s.size;
	str.str	 = ArenaPushArrayNoZero(arena, U8, str.size + 1);
	memcpy(str.str, s.str, s.size);
	str.str[str.size] = 0;
	return str;
}

internal String8 Str8fv(Arena *arena, char *fmt, va_list args)
{
	va_list args2;
	va_copy(args2, args);
	U32		needed_bytes	= vsnprintf(0, 0, fmt, args) + 1;
	String8 result			= {0};
	result.str				= ArenaPushArrayNoZero(arena, U8, needed_bytes);
	result.size				= vsnprintf((char *)result.str, needed_bytes, fmt, args2);
	result.str[result.size] = 0;
	va_end(args2);
	return result;
}

internal String8 Str8f(Arena *arena, char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	String8 result = Str8fv(arena, fmt, args);
	va_end(args);
	return result;
}

internal S64 SignFromStr8(String8 string, String8 *string_tail)
{
	// count negative signs
	U64 neg_count = 0;
	U64 i		  = 0;
	for (; i < string.size; i += 1)
	{
		if (string.str[i] == '-')
		{
			neg_count += 1;
		}
		else if (string.str[i] != '+')
		{
			break;
		}
	}

	// output part of string after signs
	*string_tail = Str8Skip(string, i);

	// output integer sign
	S64 sign = (neg_count & 1) ? -1 : +1;
	return sign;
}

internal B32 Str8IsInteger(String8 string, U32 radix)
{
	B32 result = 0;
	if (string.size > 0)
	{
		if (1 < radix && radix <= 16)
		{
			result = 1;
			for (U64 i = 0; i < string.size; i += 1)
			{
				U8 c = string.str[i];
				if (!(c < 0x80) || integer_symbol_reverse[c] >= radix)
				{
					result = 0;
					break;
				}
			}
		}
	}
	return result;
}

internal B32 Str8IsIntegerSigned(String8 string, U32 radix)
{
	B32		result = 0;
	String8 sign   = Str8Prefix(string, 1);
	if (Str8Match(sign, Str8Lit("-"), 0))
	{
		result = Str8IsInteger(Str8Skip(string, 1), radix);
	}
	else
	{
		result = Str8IsInteger(string, radix);
	}
	return result;
}

internal U64 U64FromStr8(String8 string, U32 radix)
{
	U64 x = 0;
	if (1 < radix && radix <= 16)
	{
		for (U64 i = 0; i < string.size; i += 1)
		{
			x *= radix;
			x += integer_symbol_reverse[string.str[i] & 0x7F];
		}
	}
	return x;
}

internal S64 S64FromStr8(String8 string, U32 radix)
{
	S64 sign = SignFromStr8(string, &string);
	S64 x	 = (S64)U64FromStr8(string, radix) * sign;
	return x;
}

internal U32 U32FromStr8(String8 string, U32 radix)
{
	U64 x64 = U64FromStr8(string, radix);
	AssertAlways(x64 <= max_U32);
	U32 x32 = (U32)x64;
	return x32;
}

internal S32 S32FromStr8(String8 string, U32 radix)
{
	S64 x64 = S64FromStr8(string, radix);
	AssertAlways(x64 <= max_S32);
	S32 x32 = (S32)x64;
	return x32;
}

internal B32 TryU64FromStr8CRules(String8 string, U64 *x)
{
	U64 radix		= 0;
	U64 prefix_size = 0;
	{
		// hex
		if (Str8Match(Str8Prefix(string, 2), Str8Lit("0x"), StringMatchFlag_CaseInsensitive))
		{
			radix = 0x10, prefix_size = 2;
		}
		// binary
		else if (Str8Match(Str8Prefix(string, 2), Str8Lit("0b"), StringMatchFlag_CaseInsensitive))
		{
			radix = 2, prefix_size = 2;
		}
		// octal
		else if (Str8Match(Str8Prefix(string, 1), Str8Lit("0"), StringMatchFlag_CaseInsensitive) && string.size > 1)
		{
			radix = 010, prefix_size = 1;
		}
		// decimal
		else
		{
			radix = 10, prefix_size = 0;
		}
	}

	// convert if we can
	String8 integer	   = Str8Skip(string, prefix_size);
	B32		is_integer = Str8IsInteger(integer, radix);
	if (is_integer)
	{
		*x = U64FromStr8(integer, radix);
	}

	return is_integer;
}

internal B32 TryS64FromStr8CRules(String8 string, S64 *x)
{
	String8 string_tail = {0};
	S64		sign		= SignFromStr8(string, &string_tail);
	U64		x_u64		= 0;
	B32		is_integer	= TryU64FromStr8CRules(string_tail, &x_u64);
	*x					= x_u64 * sign;
	return is_integer;
}

internal String8 Str8FromMemorySize(Arena *arena, U64 size)
{
	String8 result = {0};
	{
		if (size < KB(1))
		{
			result = PushStr8f(arena, "%llu byte%s", size, size == 1 ? "" : "s");
		}
		else if (size < MB(1))
		{
			result = PushStr8f(arena, "%llu.%02llu KiB", size / KB(1), ((size * 100) / KB(1)) % 100);
		}
		else if (size < GB(1))
		{
			result = PushStr8f(arena, "%llu.%02llu MiB", size / MB(1), ((size * 100) / MB(1)) % 100);
		}
		else if (size < TB(1))
		{
			result = PushStr8f(arena, "%llu.%02llu GiB", size / GB(1), ((size * 100) / GB(1)) % 100);
		}
		else
		{
			result = PushStr8f(arena, "%llu.%02llu TiB", size / TB(1), ((size * 100) / TB(1)) % 100);
		}
	}
	return result;
}

internal String8 Str8FromCount(Arena *arena, U64 count)
{
	String8 result = {0};
	{
		if (count < 1 * 1000)
		{
			result = PushStr8f(arena, "%llu", count);
		}
		else if (count < 1000000)
		{
			const U64 frac = ((count * 100) / 1000) % 100;
			if (frac > 0)
			{
				result = PushStr8f(arena, "%llu.%02lluK", count / 1000, frac);
			}
			else
			{
				result = PushStr8f(arena, "%lluK", count / 1000);
			}
		}
		else if (count < 1000000000)
		{
			const U64 frac = ((count * 100) / 1000000) % 100;
			if (frac > 0)
			{
				result = PushStr8f(arena, "%llu.%02lluM", count / 1000000, frac);
			}
			else
			{
				result = PushStr8f(arena, "%lluM", count / 1000000);
			}
		}
		else
		{
			const U64 frac = ((count * 100) * 1000000000) % 100;
			if (frac > 0)
			{
				result = PushStr8f(arena, "%llu.%02lluB", count / 1000000000, frac);
			}
			else
			{
				result = PushStr8f(arena, "%lluB", count / 1000000000, frac);
			}
		}
	}
	return result;
}

internal String8 Str8FromBitsU32(Arena *arena, U32 x)
{
	const U8 c0		= 'a' + ((x >> 28) & 0xf);
	const U8 c1		= 'a' + ((x >> 24) & 0xf);
	const U8 c2		= 'a' + ((x >> 20) & 0xf);
	const U8 c3		= 'a' + ((x >> 16) & 0xf);
	const U8 c4		= 'a' + ((x >> 12) & 0xf);
	const U8 c5		= 'a' + ((x >> 8) & 0xf);
	const U8 c6		= 'a' + ((x >> 4) & 0xf);
	const U8 c7		= 'a' + ((x >> 0) & 0xf);
	String8	 result = PushStr8f(arena, "%c%c%c%c%c%c%c%c", c0, c1, c2, c3, c4, c5, c6, c7);
	return result;
}

internal String8 Str8FromBitsU64(Arena *arena, U64 x)
{
	const U8 c0		= 'a' + ((x >> 60) & 0xf);
	const U8 c1		= 'a' + ((x >> 56) & 0xf);
	const U8 c2		= 'a' + ((x >> 52) & 0xf);
	const U8 c3		= 'a' + ((x >> 48) & 0xf);
	const U8 c4		= 'a' + ((x >> 44) & 0xf);
	const U8 c5		= 'a' + ((x >> 40) & 0xf);
	const U8 c6		= 'a' + ((x >> 36) & 0xf);
	const U8 c7		= 'a' + ((x >> 32) & 0xf);
	const U8 c8		= 'a' + ((x >> 28) & 0xf);
	const U8 c9		= 'a' + ((x >> 24) & 0xf);
	const U8 ca		= 'a' + ((x >> 20) & 0xf);
	const U8 cb		= 'a' + ((x >> 16) & 0xf);
	const U8 cc		= 'a' + ((x >> 12) & 0xf);
	const U8 cd		= 'a' + ((x >> 8) & 0xf);
	const U8 ce		= 'a' + ((x >> 4) & 0xf);
	const U8 cf		= 'a' + ((x >> 0) & 0xf);
	String8	 result = PushStr8f(arena,
								"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
								c0,
								c1,
								c2,
								c3,
								c4,
								c5,
								c6,
								c7,
								c8,
								c9,
								ca,
								cb,
								cc,
								cd,
								ce,
								cf);
	return result;
}
internal String8 Str8FromU64(Arena *arena, U64 u64, U32 radix, U8 min_digits, U8 digit_group_separator)
{
	String8 result = {0};
	{
		// prefix
		String8 prefix = {0};
		switch (radix)
		{
		case 16:
		{
			prefix = Str8Lit("0x");
		}
		break;
		case 8:
		{
			prefix = Str8Lit("0o");
		}
		break;
		case 2:
		{
			prefix = Str8Lit("0b");
		}
		break;
		}

		// determine # of chars between separators
		U8 digit_group_size = 3;
		switch (radix)
		{
		default:
			break;
		case 2:
		case 8:
		case 16:
		{
			digit_group_size = 4;
		}
		break;
		}

		// prep
		U64 needed_leading_0s = 0;
		{
			U64 needed_digits = 1;
			{
				U64 u64_reduce = u64;
				for (;;)
				{
					u64_reduce /= radix;
					if (u64_reduce == 0)
					{
						break;
					}
					needed_digits += 1;
				}
			}
			needed_leading_0s	  = (min_digits > needed_digits) ? min_digits - needed_digits : 0;
			U64 needed_separators = 0;
			if (digit_group_separator != 0)
			{
				needed_separators = (needed_digits + needed_leading_0s) / digit_group_size;
				if (needed_separators > 0 && (needed_digits + needed_leading_0s) % digit_group_size == 0)
				{
					needed_separators -= 1;
				}
			}
			result.size				= prefix.size + needed_leading_0s + needed_separators + needed_digits;
			result.str				= ArenaPushArrayNoZero(arena, U8, result.size + 1);
			result.str[result.size] = 0;
		}

		// fill contents
		{
			U64 u64_reduce			   = u64;
			U64 digits_until_separator = digit_group_size;
			for (U64 idx = 0; idx < result.size; idx += 1)
			{
				if (digits_until_separator == 0 && digit_group_separator != 0)
				{
					result.str[result.size - idx - 1] = digit_group_separator;
					digits_until_separator			  = digit_group_size + 1;
				}
				else
				{
					result.str[result.size - idx - 1] = LowerFromChar(integer_symbols[u64_reduce % radix]);
					u64_reduce /= radix;
				}
				digits_until_separator -= 1;
				if (u64_reduce == 0)
				{
					break;
				}
			}
			for (U64 leading_0_idx = 0; leading_0_idx < needed_leading_0s; leading_0_idx += 1)
			{
				result.str[prefix.size + leading_0_idx] = '0';
			}
		}

		// fill prefix
		if (prefix.size != 0)
		{
			memcpy(result.str, prefix.str, prefix.size);
		}
	}
	return result;
}

internal String8 Str8FromS64(Arena *arena, S64 s64, U32 radix, U8 min_digits, U8 digit_group_separator)
{
	String8 result = {0};
	if (s64 < 0)
	{
		Temp	scratch		 = ScratchBegin(&arena, 1);
		String8 numeric_part = Str8FromU64(scratch.arena, (U64)(-s64), radix, min_digits, digit_group_separator);
		result				 = PushStr8f(arena, "-%S", numeric_part);
		ScratchEnd(scratch);
	}
	else
	{
		result = Str8FromU64(arena, (U64)s64, radix, min_digits, digit_group_separator);
	}
	return result;
}

internal F64 F64FromString(String8 string)
{
	// TODO crappy implementation for now that just uses atof.
	F64 result = 0;
	if (string.size > 0)
	{
		// find starting pos of numeric string, as well as sign
		F64 sign = +1.0;
		if (string.str[0] == '-')
		{
			sign = -1.0;
		}
		else if (string.str[0] == '+')
		{
			sign = 1.0;
		}

		// gather numerics
		U64	 num_valid_chars = 0;
		char buffer[64];
		B32	 exp = 0;
		for (U64 idx = 0; idx < string.size && num_valid_chars < sizeof(buffer) - 1; idx += 1)
		{
			if (CharIsDigit(string.str[idx], 10) || string.str[idx] == '.' || string.str[idx] == 'e' ||
				(exp && (string.str[idx] == '+' || string.str[idx] == '-')))
			{
				buffer[num_valid_chars] = string.str[idx];
				num_valid_chars += 1;
				exp = 0;
				exp = (string.str[idx] == 'e');
			}
		}

		// null-terminate (the reason for all of this!!!!!!)
		buffer[num_valid_chars] = 0;

		// do final conversion
		result = sign * atof(buffer);
	}
	return result;
}

internal String8Node *Str8ListPushNode(String8List *list, String8Node *node)
{
	SLLQueuePush(list->first, list->last, node);
	list->node_count += 1;
	list->total_size += node->string.size;
	return node;
}

internal String8Node *Str8ListPushNodeSetString(String8List *list, String8Node *node, String8 string)
{
	SLLQueuePush(list->first, list->last, node);
	list->node_count += 1;
	list->total_size += string.size;
	node->string = string;
	return node;
}

internal String8Node *Str8ListPushNodeFront(String8List *list, String8Node *node)
{
	SLLQueuePushFront(list->first, list->last, node);
	list->node_count += 1;
	list->total_size += node->string.size;
	return node;
}

internal String8Node *Str8ListPushNodeFrontSetString(String8List *list, String8Node *node, String8 string)
{
	SLLQueuePushFront(list->first, list->last, node);
	list->node_count += 1;
	list->total_size += string.size;
	node->string = string;
	return node;
}

internal String8Node *Str8ListPush(Arena *arena, String8List *list, String8 string)
{
	String8Node *node = ArenaPushArrayNoZero(arena, String8Node, 1);
	Str8ListPushNodeSetString(list, node, string);
	return node;
}

internal String8Node *Str8ListPushFront(Arena *arena, String8List *list, String8 string)
{
	String8Node *node = ArenaPushArrayNoZero(arena, String8Node, 1);
	Str8ListPushNodeFrontSetString(list, node, string);
	return node;
}

internal void Str8ListConcatInPlace(String8List *list, String8List *to_push)
{
	if (to_push->node_count != 0)
	{
		if (list->last)
		{
			list->node_count += to_push->node_count;
			list->total_size += to_push->total_size;
			list->last->next = to_push->first;
			list->last		 = to_push->last;
		}
		else
		{
			*list = *to_push;
		}
		MemoryZeroStruct(to_push);
	}
}

internal String8Node *Str8ListPushAligner(Arena *arena, String8List *list, U64 min, U64 align)
{
	read_only local_persist U8 zeroes[64] = {0};
	AssertAlways(IsPow2OrZero(align));
	U64 pad = Max(min, AlignPadPow2(list->total_size, align));
	if (pad < sizeof(zeroes))
	{
		return Str8ListPush(arena, list, Str8(zeroes, pad));
	}
	else
	{
		return Str8ListPush(arena, list, Str8(ArenaPushArray(arena, U8, pad), pad));
	}
}

internal String8Node *Str8ListPushf(Arena *arena, String8List *list, char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	String8		 string = PushStr8fv(arena, fmt, args);
	String8Node *result = Str8ListPush(arena, list, string);
	va_end(args);
	return result;
}

internal String8Node *Str8ListPushFrontf(Arena *arena, String8List *list, char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	String8		 string = PushStr8fv(arena, fmt, args);
	String8Node *result = Str8ListPushFront(arena, list, string);
	va_end(args);
	return result;
}

internal String8Node *Str8ListPopFront(String8List *list)
{
	String8Node *node = 0;
	if (list->node_count)
	{
		node = list->first;
		AssertAlways(list->total_size >= list->first->string.size);
		list->node_count -= 1;
		list->total_size -= list->first->string.size;
		SLLQueuePop(list->first, list->last);
	}
	return node;
}

internal String8List Str8ListCopy(Arena *arena, String8List *list)
{
	String8List result = {0};
	for (String8Node *node = list->first; node != 0; node = node->next)
	{
		String8Node *new_node	= ArenaPushArrayNoZero(arena, String8Node, 1);
		String8		 new_string = PushStr8Copy(arena, node->string);
		Str8ListPushNodeSetString(&result, new_node, new_string);
	}
	return result;
}

internal String8List Str8ListSubstr(Arena *arena, String8List list, U64 s, U64 f)
{
	String8List	 result = {0};
	String8Node *n		= list.first;

	U64 front_min = 0;
	{
		U64 cursor = 0;
		for (; n != 0; cursor += n->string.size, n = n->next)
		{
			if (cursor + n->string.size > s)
			{
				front_min = s - cursor;
				break;
			}
		}
	}
#define Dim1U64(s, f) ((f > s) ? (f - s) : 0)
	if (front_min > 0)
	{
		U64 front_max = front_min + Min(Dim1U64(s, f), n->string.size);
		Str8ListPush(arena, &result, Str8Substr(n->string, front_min, front_max));
		n = n->next;
	}

	for (; n != 0; n = n->next)
	{
		if (result.total_size >= Dim1U64(s, f))
		{
			break;
		}
		U64 copy_max  = Dim1U64(s, f) - result.total_size;
		U64 copy_size = Min(copy_max, n->string.size);
		Str8ListPush(arena, &result, Str8Substr(n->string, 0, copy_size));
	}
#undef Dim1U64

	return result;
}

internal B32 Str8Match(String8 a, String8 b, StringMatchFlags flags)
{
	B32 result = 0;
	if (a.size == b.size && flags == 0)
	{
		result = MemoryMatch(a.str, b.str, b.size);
	}
	else if (a.size == b.size || (flags & StringMatchFlag_RightSideSloppy))
	{
		B32 case_insensitive  = (flags & StringMatchFlag_CaseInsensitive);
		B32 slash_insensitive = (flags & StringMatchFlag_SlashInsensitive);
		U64 size			  = Min(a.size, b.size);
		result				  = 1;
		for (U64 i = 0; i < size; i += 1)
		{
			U8 at = a.str[i];
			U8 bt = b.str[i];
			if (case_insensitive)
			{
				at = UpperFromChar(at);
				bt = UpperFromChar(bt);
			}
			if (slash_insensitive)
			{
				at = CorrectSlashFromChar(at);
				bt = CorrectSlashFromChar(bt);
			}
			if (at != bt)
			{
				result = 0;
				break;
			}
		}
	}
	return result;
}

internal B32 Str8MatchWildCard(String8 string, String8 pattern, StringMatchFlags flags)
{
	B32 matched = 0;

	U64 pattern_cursor = 0;
	U64 string_cursor  = 0;

	U64 pattern_start = max_U64;
	U64 string_start  = 0;

	for (;;)
	{
		if (pattern_cursor == pattern.size)
		{
			if (string_cursor == string.size || (flags & StringMatchFlag_RightSideSloppy))
			{
				matched = 1;
				break;
			}
		}

		if (string_cursor == string.size)
		{
			while (pattern_cursor < pattern.size && pattern.str[pattern_cursor] == '*')
			{
				pattern_cursor += 1;
			}
			matched = (pattern_cursor == pattern.size);
			break;
		}

		if (pattern_cursor < pattern.size && pattern.str[pattern_cursor] == '*')
		{
			pattern_start = pattern_cursor;
			string_start  = string_cursor;
			pattern_cursor += 1;
			continue;
		}

		if (pattern_cursor < pattern.size &&
			(pattern.str[pattern_cursor] == '?' ||
			 Str8CharMatch(string.str[string_cursor], pattern.str[pattern_cursor], flags)))
		{
			string_cursor += 1;
			pattern_cursor += 1;
			continue;
		}

		if (pattern_start != max_U64)
		{
			pattern_cursor = pattern_start + 1;
			string_start += 1;
			string_cursor = string_start;
			continue;
		}

		break;
	}

	return matched;
}

internal B32 Str8CharMatch(U8 a, U8 b, StringMatchFlags flags)
{
	U8 at = a;
	U8 bt = b;
	if (flags & StringMatchFlag_CaseInsensitive)
	{
		at = UpperFromChar(at);
		bt = UpperFromChar(bt);
	}
	if (flags & StringMatchFlag_SlashInsensitive)
	{
		if (at == '\\')
		{
			at = '/';
		}
		if (bt == '\\')
		{
			bt = '/';
		}
	}
	return (at == bt);
}

internal U64 Str8FindNeedle(String8 string, U64 start_pos, String8 needle, StringMatchFlags flags)
{
	U8 *p			= string.str + start_pos;
	U64 stop_offset = Max(string.size + 1, needle.size) - needle.size;
	U8 *stop_p		= string.str + stop_offset;
	if (needle.size > 0)
	{
		U8				*string_opl					= string.str + string.size;
		String8			 needle_tail				= Str8Skip(needle, 1);
		StringMatchFlags adjusted_flags				= flags | StringMatchFlag_RightSideSloppy;
		U8				 needle_first_char_adjusted = needle.str[0];
		if (adjusted_flags & StringMatchFlag_CaseInsensitive)
		{
			needle_first_char_adjusted = UpperFromChar(needle_first_char_adjusted);
		}
		for (; p < stop_p; p += 1)
		{
			U8 haystack_char_adjusted = *p;
			if (adjusted_flags & StringMatchFlag_CaseInsensitive)
			{
				haystack_char_adjusted = UpperFromChar(haystack_char_adjusted);
			}
			if (haystack_char_adjusted == needle_first_char_adjusted)
			{
				if (Str8Match(Str8Range(p + 1, string_opl), needle_tail, adjusted_flags))
				{
					break;
				}
			}
		}
	}
	U64 result = string.size;
	if (p < stop_p)
	{
		result = (U64)(p - string.str);
	}
	return result;
}

internal U64 Str8FindNeedleReverse(String8 string, U64 start_pos, String8 needle, StringMatchFlags flags)
{
	U64 result = 0;
	for (S64 i = string.size - start_pos - needle.size; i >= 0; --i)
	{
		String8 haystack = Str8Substr(string, i, i + needle.size);
		if (Str8Match(haystack, needle, flags))
		{
			result = (U64)i + needle.size;
			break;
		}
	}
	return result;
}

internal B32 Str8IsBefore(String8 a, String8 b)
{
	B32 result = 0;
	{
		U64 common_size = Min(a.size, b.size);
		for (U64 off = 0; off < common_size; off += 1)
		{
			if (a.str[off] < b.str[off])
			{
				result = 1;
				break;
			}
			else if (a.str[off] > b.str[off])
			{
				result = 0;
				break;
			}
			else if (off + 1 == common_size)
			{
				result = (a.size < b.size);
			}
		}
	}
	return result;
}

internal String8Array Str8ArrayZero(void)
{
	String8Array result = {0};
	return result;
}

internal String8Array Str8ArrayFromList(Arena *arena, String8List *list)
{
	String8Array array;
	array.count = list->node_count;
	array.v		= ArenaPushArrayNoZero(arena, String8, array.count);
	U64 idx		= 0;
	for (String8Node *n = list->first; n != 0; n = n->next, idx += 1)
	{
		array.v[idx] = n->string;
	}
	return array;
}

internal String8Array Str8ArrayReserve(Arena *arena, U64 count)
{
	String8Array arr;
	arr.count = 0;
	arr.v	  = ArenaPushArray(arena, String8, count);
	return arr;
}

internal String8Array Str8ArrayCopy(Arena *arena, String8Array array)
{
	String8Array result = {0};
	result.count		= array.count;
	result.v			= ArenaPushArray(arena, String8, result.count);
  for
	  EachIndex(idx, result.count)
	  {
		  result.v[idx] = PushStr8Copy(arena, array.v[idx]);
	  }
  return result;
}
