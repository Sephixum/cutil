#include "base/base_include.h"
#include <stdio.h>
#include <math.h>

#define TEST_EPSILON 1e-5f
#define ASSERT(cond)                                                                                                   \
	if (!(cond))                                                                                                       \
	{                                                                                                                  \
		printf("FAIL: %s:%d\n", __FILE__, __LINE__);                                                                   \
		return 0;                                                                                                      \
	}
#define ASSERTF(cond, fmt, ...)                                                                                        \
	if (!(cond))                                                                                                       \
	{                                                                                                                  \
		printf("FAIL: %s:%d - " fmt "\n", __FILE__, __LINE__, __VA_ARGS__);                                            \
		return 0;                                                                                                      \
	}

global S32 test_count  = 0;
global S32 test_passed = 0;

#define RUN_TEST(name)                                                                                                 \
	test_count++;                                                                                                      \
	if (name())                                                                                                        \
	{                                                                                                                  \
		test_passed++;                                                                                                 \
		printf("✓ %s\n", #name);                                                                                       \
	}                                                                                                                  \
	else                                                                                                               \
	{                                                                                                                  \
		printf("✗ %s\n", #name);                                                                                       \
	}

typedef S32 (*TestFn)(void);

////////////////////////////////
// Helpers

internal S32 Vec3F32_ApproxEqual(Vec3F32 a, Vec3F32 b, F32 epsilon)
{
	return fabsf(a.x - b.x) < epsilon && fabsf(a.y - b.y) < epsilon && fabsf(a.z - b.z) < epsilon;
}

internal S32 Quat4F32_ApproxEqual(Quat4F32 a, Quat4F32 b, F32 epsilon)
{
	return fabsf(a.x - b.x) < epsilon && fabsf(a.y - b.y) < epsilon && fabsf(a.z - b.z) < epsilon &&
		   fabsf(a.w - b.w) < epsilon;
}

internal S32 Mat4x4F32_ApproxEqual(Mat4x4F32 a, Mat4x4F32 b, F32 epsilon)
{
	for (S32 i = 0; i < 16; i++)
	{
		if (fabsf(a.elements[i] - b.elements[i]) >= epsilon)
			return 0;
	}
	return 1;
}

////////////////////////////////
// Vec3F32 Tests

internal S32 test_Vec3F32_Make(void)
{
	Vec3F32 v = Vec3F32_Make(1, 2, 3);
	ASSERT(v.x == 1 && v.y == 2 && v.z == 3);
	return 1;
}

internal S32 test_Vec3F32_Add(void)
{
	Vec3F32 a	   = Vec3F32_Make(1, 2, 3);
	Vec3F32 b	   = Vec3F32_Make(4, 5, 6);
	Vec3F32 result = Vec3F32_Add(a, b);
	ASSERT(result.x == 5 && result.y == 7 && result.z == 9);
	return 1;
}

internal S32 test_Vec3F32_Dot(void)
{
	Vec3F32 a	= Vec3F32_Make(1, 2, 3);
	Vec3F32 b	= Vec3F32_Make(4, 5, 6);
	F32		dot = Vec3F32_Dot(a, b);
	// 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
	ASSERT(fabsf(dot - 32.0f) < TEST_EPSILON);
	return 1;
}

internal S32 test_Vec3F32_Length(void)
{
	Vec3F32 v	= Vec3F32_Make(3, 4, 0);
	F32		len = Vec3F32_Length(v);
	// sqrt(9 + 16) = sqrt(25) = 5
	ASSERT(fabsf(len - 5.0f) < TEST_EPSILON);
	return 1;
}

internal S32 test_Vec3F32_Normalize(void)
{
	Vec3F32 v	 = Vec3F32_Make(3, 4, 0);
	Vec3F32 norm = Vec3F32_Normalize(v);
	F32		len	 = Vec3F32_Length(norm);
	ASSERT(fabsf(len - 1.0f) < TEST_EPSILON);
	return 1;
}

////////////////////////////////
// Mat4x4F32 Tests

internal S32 test_Mat4x4F32_Identity(void)
{
	Mat4x4F32 identity = Mat4x4F32_MakeIdentity();
	for (S32 i = 0; i < 4; i++)
	{
		for (S32 j = 0; j < 4; j++)
		{
			F32 expected = (i == j) ? 1.0f : 0.0f;
			if (fabsf(identity.e[i][j] - expected) >= TEST_EPSILON)
				return 0;
		}
	}
	return 1;
}

internal S32 test_Mat4x4F32_Mul(void)
{
	Mat4x4F32 a		 = Mat4x4F32_MakeIdentity();
	Mat4x4F32 b		 = Mat4x4F32_MakeIdentity();
	Mat4x4F32 result = Mat4x4F32_Mul(a, b);
	ASSERT(Mat4x4F32_ApproxEqual(result, a, TEST_EPSILON));
	return 1;
}

internal S32 test_Mat4x4F32_Translate(void)
{
	Vec3F32	  t		= Vec3F32_Make(5, 10, 15);
	Mat4x4F32 trans = Mat4x4F32_Translate(t);

	Vec3F32 v	   = Vec3F32_Make(1, 2, 3);
	Vec4F32 v4	   = Vec4F32_Make(v.x, v.y, v.z, 1.0f);
	Vec4F32 result = Mat4x4F32_MulVec4F32(trans, v4);

	// translation should add to the vector
	ASSERT(fabsf(result.x - 6.0f) < TEST_EPSILON);
	ASSERT(fabsf(result.y - 12.0f) < TEST_EPSILON);
	ASSERT(fabsf(result.z - 18.0f) < TEST_EPSILON);
	return 1;
}

internal S32 test_Mat4x4F32_Scale(void)
{
	Vec3F32	  s		= Vec3F32_Make(2, 3, 4);
	Mat4x4F32 scale = Mat4x4F32_Scale(s);

	Vec4F32 v	   = Vec4F32_Make(1, 1, 1, 1);
	Vec4F32 result = Mat4x4F32_MulVec4F32(scale, v);

	ASSERT(fabsf(result.x - 2.0f) < TEST_EPSILON);
	ASSERT(fabsf(result.y - 3.0f) < TEST_EPSILON);
	ASSERT(fabsf(result.z - 4.0f) < TEST_EPSILON);
	return 1;
}

internal S32 test_Mat4x4F32_Transpose(void)
{
	Mat4x4F32 m = Mat4x4F32_MakeIdentity();
	m.e[0][1]	= 5.0f;
	m.e[1][0]	= 0.0f;

	Mat4x4F32 t = Mat4x4F32_Transpose(m);
	ASSERT(fabsf(t.e[1][0] - 5.0f) < TEST_EPSILON);
	ASSERT(fabsf(t.e[0][1] - 0.0f) < TEST_EPSILON);
	return 1;
}

internal S32 test_Mat4x4F32_RotateX(void)
{
	Mat4x4F32 rot = Mat4x4F32_RotateX(PI32 / 2.0f); // 90 degrees

	Vec4F32 v	   = Vec4F32_Make(0, 1, 0, 1);
	Vec4F32 result = Mat4x4F32_MulVec4F32(rot, v);

	// rotating (0, 1, 0) 90 degrees around X should give approximately (0, 0, 1)
	ASSERT(fabsf(result.x - 0.0f) < TEST_EPSILON);
	ASSERT(fabsf(result.y - 0.0f) < TEST_EPSILON);
	ASSERT(fabsf(result.z - 1.0f) < TEST_EPSILON);
	return 1;
}

internal S32 test_Mat4x4F32_GetTranslation(void)
{
	Vec3F32	  t			= Vec3F32_Make(5, 10, 15);
	Mat4x4F32 trans		= Mat4x4F32_Translate(t);
	Vec3F32	  extracted = Mat4x4F32_GetTranslation(trans);

	ASSERT(Vec3F32_ApproxEqual(extracted, t, TEST_EPSILON));
	return 1;
}

internal S32 test_Mat4x4F32_Inverse_Identity(void)
{
	Mat4x4F32 identity = Mat4x4F32_MakeIdentity();
	Mat4x4F32 inv	   = Mat4x4F32_Inverse(identity);

	ASSERT(Mat4x4F32_ApproxEqual(inv, identity, TEST_EPSILON));
	return 1;
}

internal S32 test_Mat4x4F32_Inverse_Scale(void)
{
	Vec3F32	  s		= Vec3F32_Make(2, 3, 4);
	Mat4x4F32 scale = Mat4x4F32_Scale(s);
	Mat4x4F32 inv	= Mat4x4F32_Inverse(scale);

	Mat4x4F32 product  = Mat4x4F32_Mul(scale, inv);
	Mat4x4F32 identity = Mat4x4F32_MakeIdentity();

	ASSERT(Mat4x4F32_ApproxEqual(product, identity, TEST_EPSILON));
	return 1;
}

////////////////////////////////
// Quat4F32 Tests

internal S32 test_Quat4F32_Make(void)
{
	Quat4F32 q = Quat4F32_Make(1, 2, 3, 4);
	ASSERT(q.x == 1 && q.y == 2 && q.z == 3 && q.w == 4);
	return 1;
}

internal S32 test_Quat4F32_Identity(void)
{
	Quat4F32 q = Quat4F32_Identity();
	ASSERT(q.x == 0 && q.y == 0 && q.z == 0 && q.w == 1);
	return 1;
}

internal S32 test_Quat4F32_Mul(void)
{
	Quat4F32 q1		= Quat4F32_Identity();
	Quat4F32 q2		= Quat4F32_Make(1, 2, 3, 4);
	Quat4F32 result = Quat4F32_Mul(q1, q2);

	ASSERT(Quat4F32_ApproxEqual(result, q2, TEST_EPSILON));
	return 1;
}

internal S32 test_Quat4F32_Normalize(void)
{
	Quat4F32 q	  = Quat4F32_Make(1, 2, 3, 4);
	Quat4F32 norm = Quat4F32_Normalize(q);
	F32		 len  = Quat4F32_Length(norm);

	ASSERT(fabsf(len - 1.0f) < TEST_EPSILON);
	return 1;
}

internal S32 test_Quat4F32_Conjugate(void)
{
	Quat4F32 q	  = Quat4F32_Make(1, 2, 3, 4);
	Quat4F32 conj = Quat4F32_Conjugate(q);

	ASSERT(conj.x == -1 && conj.y == -2 && conj.z == -3 && conj.w == 4);
	return 1;
}

internal S32 test_Quat4F32_FromAxisAngle(void)
{
	Vec3F32 axis  = Vec3F32_Make(0, 1, 0); // Y axis
	F32		angle = PI32 / 2.0f;		   // 90 degrees

	Quat4F32 q = Quat4F32_FromAxisAngle(axis, angle);

	// should be normalized
	F32 len = Quat4F32_Length(q);
	ASSERT(fabsf(len - 1.0f) < TEST_EPSILON);
	return 1;
}

internal S32 test_Quat4F32_ToAxisAngle(void)
{
	Vec3F32 axis  = Vec3F32_Make(0, 1, 0);
	F32		angle = PI32 / 4.0f; // 45 degrees

	Quat4F32 q = Quat4F32_FromAxisAngle(axis, angle);

	Vec3F32 extracted_axis;
	F32		extracted_angle;
	Quat4F32_ToAxisAngle(q, &extracted_axis, &extracted_angle);

	ASSERT(Vec3F32_ApproxEqual(extracted_axis, axis, TEST_EPSILON));
	ASSERT(fabsf(extracted_angle - angle) < TEST_EPSILON);
	return 1;
}

internal S32 test_Quat4F32_FromEuler(void)
{
	Vec3F32	 euler = Vec3F32_Make(0, PI32 / 4.0f, 0); // 45 degree pitch
	Quat4F32 q	   = Quat4F32_FromEuler(euler);

	// should be normalized
	F32 len = Quat4F32_Length(q);
	ASSERT(fabsf(len - 1.0f) < TEST_EPSILON);
	return 1;
}

internal S32 test_Quat4F32_ToEuler(void)
{
	Vec3F32	 euler	   = Vec3F32_Make(0, PI32 / 6.0f, 0);
	Quat4F32 q		   = Quat4F32_FromEuler(euler);
	Vec3F32	 extracted = Quat4F32_ToEuler(q);

	ASSERT(Vec3F32_ApproxEqual(extracted, euler, TEST_EPSILON));
	return 1;
}

internal S32 test_Quat4F32_ToMat4x4(void)
{
	Quat4F32  q		   = Quat4F32_Identity();
	Mat4x4F32 m		   = Quat4F32_ToMat4x4(q);
	Mat4x4F32 identity = Mat4x4F32_MakeIdentity();

	ASSERT(Mat4x4F32_ApproxEqual(m, identity, TEST_EPSILON));
	return 1;
}

internal S32 test_Quat4F32_FromMat4x4(void)
{
	Mat4x4F32 m		   = Mat4x4F32_MakeIdentity();
	Quat4F32  q		   = Quat4F32_FromMat4x4(m);
	Quat4F32  identity = Quat4F32_Identity();

	ASSERT(Quat4F32_ApproxEqual(q, identity, TEST_EPSILON));
	return 1;
}

internal S32 test_Quat4F32_RotateVec3(void)
{
	// Create 90 degree rotation around Y axis
	Quat4F32 q = Quat4F32_FromAxisAngle(Vec3F32_Make(0, 1, 0), PI32 / 2.0f);

	Vec3F32 v		= Vec3F32_Make(1, 0, 0);
	Vec3F32 rotated = Quat4F32_RotateVec3(q, v);

	// (1, 0, 0) rotated 90 degrees around Y should be approximately (0, 0, -1)
	ASSERT(fabsf(rotated.x - 0.0f) < TEST_EPSILON);
	ASSERT(fabsf(rotated.y - 0.0f) < TEST_EPSILON);
	ASSERT(fabsf(rotated.z - (-1.0f)) < TEST_EPSILON);
	return 1;
}

internal S32 test_Quat4F32_Slerp(void)
{
	Quat4F32 q1 = Quat4F32_Identity();
	Quat4F32 q2 = Quat4F32_FromAxisAngle(Vec3F32_Make(0, 1, 0), PI32 / 2.0f);

	Quat4F32 mid = Quat4F32_Slerp(q1, q2, 0.5f);

	// should be normalized
	F32 len = Quat4F32_Length(mid);
	ASSERT(fabsf(len - 1.0f) < TEST_EPSILON);

	// at t=0 should be q1
	Quat4F32 start = Quat4F32_Slerp(q1, q2, 0.0f);
	ASSERT(Quat4F32_ApproxEqual(start, q1, TEST_EPSILON));

	// at t=1 should be q2
	Quat4F32 end = Quat4F32_Slerp(q1, q2, 1.0f);
	ASSERT(Quat4F32_ApproxEqual(end, q2, TEST_EPSILON));

	return 1;
}

internal S32 test_Quat4F32_Inverse(void)
{
	Quat4F32 q	 = Quat4F32_FromAxisAngle(Vec3F32_Make(1, 1, 1), PI32 / 3.0f);
	Quat4F32 inv = Quat4F32_Inverse(q);

	Quat4F32 product  = Quat4F32_Mul(q, inv);
	Quat4F32 identity = Quat4F32_Identity();

	ASSERT(Quat4F32_ApproxEqual(product, identity, TEST_EPSILON));
	return 1;
}

////////////////////////////////
// Main

S32 main(void)
{
	printf("\n=== Math Library Tests ===\n\n");

	printf("Vec3F32:\n");
	RUN_TEST(test_Vec3F32_Make);
	RUN_TEST(test_Vec3F32_Add);
	RUN_TEST(test_Vec3F32_Dot);
	RUN_TEST(test_Vec3F32_Length);
	RUN_TEST(test_Vec3F32_Normalize);

	printf("\nMat4x4F32:\n");
	RUN_TEST(test_Mat4x4F32_Identity);
	RUN_TEST(test_Mat4x4F32_Mul);
	RUN_TEST(test_Mat4x4F32_Translate);
	RUN_TEST(test_Mat4x4F32_Scale);
	RUN_TEST(test_Mat4x4F32_Transpose);
	RUN_TEST(test_Mat4x4F32_RotateX);
	RUN_TEST(test_Mat4x4F32_GetTranslation);
	RUN_TEST(test_Mat4x4F32_Inverse_Identity);
	RUN_TEST(test_Mat4x4F32_Inverse_Scale);

	printf("\nQuat4F32:\n");
	RUN_TEST(test_Quat4F32_Make);
	RUN_TEST(test_Quat4F32_Identity);
	RUN_TEST(test_Quat4F32_Mul);
	RUN_TEST(test_Quat4F32_Normalize);
	RUN_TEST(test_Quat4F32_Conjugate);
	RUN_TEST(test_Quat4F32_FromAxisAngle);
	RUN_TEST(test_Quat4F32_ToAxisAngle);
	RUN_TEST(test_Quat4F32_FromEuler);
	RUN_TEST(test_Quat4F32_ToEuler);
	RUN_TEST(test_Quat4F32_ToMat4x4);
	RUN_TEST(test_Quat4F32_FromMat4x4);
	RUN_TEST(test_Quat4F32_RotateVec3);
	RUN_TEST(test_Quat4F32_Slerp);
	RUN_TEST(test_Quat4F32_Inverse);

	printf("\n=== Summary ===\n");
	printf("%d / %d tests passed\n", test_passed, test_count);

	return (test_passed == test_count) ? 0 : 1;
}

#include "base/base_include.c"
