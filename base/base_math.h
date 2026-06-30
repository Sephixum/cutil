#ifndef BASE_MATH_H
#define BASE_MATH_H

#include "base_typedefs.h"
#include <math.h>

#define VEC_NORMALIZE_EPSILON 1e-6f

#define PI32 3.14159265359f
#define PI64 3.14159265358979311600

#define TAU32 6.28318530718f
#define TAU64 6.28318530717958647693

#define PiDeg2RadF32 (PI32 / 180.0f)
#define PiRad2DegF32 (180.0f / PI32)

#define PiDeg2RadF64 (PI64 / 180.0)
#define PiRad2DegF64 (180.0 / PI64)

#define Deg2RadF32(x) (x / 180.0f)
#define Rad2DegF32(x) (180.0f / x)

#define Deg2RadF64(x) (x / 180)
#define Rad2DegF64(x) (180 / x)

////////////////////////
// F32 variant
typedef union Vec2F32 Vec2F32;
union Vec2F32
{
	struct
	{
		F32 x, y;
	};

	struct
	{
		F32 u, v;
	};

	F32 e[2];
};

typedef union Vec3F32 Vec3F32;
union Vec3F32
{
	struct
	{
		F32 x, y, z;
	};

	struct
	{
		F32 u, v, s;
	};

	F32 e[3];
};

typedef union Vec4F32 Vec4F32;
union Vec4F32
{
	struct
	{
		F32 x, y, z, w;
	};

	struct
	{
		F32 u, v, s, t;
	};

	F32 e[4];
};

typedef union Mat4x4F32 Mat4x4F32;
union Mat4x4F32
{
	F32		e[4][4];	  // e[col][row] -- column-major
	F32		elements[16]; // flat, column-major order, directly uploadable
	Vec4F32 columns[4];
};

internal Vec2F32 Vec2F32_Make(F32 x, F32 y);
internal Vec3F32 Vec3F32_Make(F32 x, F32 y, F32 z);
internal Vec4F32 Vec4F32_Make(F32 x, F32 y, F32 z, F32 w);

internal Vec2F32 Vec2F32_Add(Vec2F32 a, Vec2F32 b);
internal Vec3F32 Vec3F32_Add(Vec3F32 a, Vec3F32 b);
internal Vec4F32 Vec4F32_Add(Vec4F32 a, Vec4F32 b);

internal Vec2F32 Vec2F32_Sub(Vec2F32 a, Vec2F32 b);
internal Vec3F32 Vec3F32_Sub(Vec3F32 a, Vec3F32 b);
internal Vec4F32 Vec4F32_Sub(Vec4F32 a, Vec4F32 b);

internal Vec2F32 Vec2F32_Mul(Vec2F32 a, Vec2F32 b);
internal Vec3F32 Vec3F32_Mul(Vec3F32 a, Vec3F32 b);
internal Vec4F32 Vec4F32_Mul(Vec4F32 a, Vec4F32 b);

internal F32 Vec2F32_Dot(Vec2F32 a, Vec2F32 b);
internal F32 Vec3F32_Dot(Vec3F32 a, Vec3F32 b);
internal F32 Vec4F32_Dot(Vec4F32 a, Vec4F32 b);

internal Vec2F32 Vec2F32_Scale(Vec2F32 a, F32 b);
internal Vec3F32 Vec3F32_Scale(Vec3F32 a, F32 b);
internal Vec4F32 Vec4F32_Scale(Vec4F32 a, F32 b);

internal F32 Vec2F32_Length(Vec2F32 v);
internal F32 Vec3F32_Length(Vec3F32 v);
internal F32 Vec4F32_Length(Vec4F32 v);

internal F32 Vec2F32_LengthSquared(Vec2F32 v);
internal F32 Vec3F32_LengthSquared(Vec3F32 v);
internal F32 Vec4F32_LengthSquared(Vec4F32 v);

internal Vec2F32 Vec2F32_Normalize(Vec2F32 v);
internal Vec3F32 Vec3F32_Normalize(Vec3F32 v);
internal Vec4F32 Vec4F32_Normalize(Vec4F32 v);

internal Mat4x4F32 Mat4x4F32_MakeIdentity(void);
internal Mat4x4F32 Mat4x4F32_MakeFromVec3Columns(Vec3F32 c0, Vec3F32 c1, Vec3F32 c2, Vec3F32 c3);
internal Mat4x4F32 Mat4x4F32_MakeFromVec3Rows(Vec3F32 r0, Vec3F32 r1, Vec3F32 r2, Vec3F32 r3);

internal Mat4x4F32 Mat4x4F32_Mul(Mat4x4F32 a, Mat4x4F32 b);
internal Vec4F32   Mat4x4F32_MulVec4F32(Mat4x4F32 m, Vec4F32 v);
internal Mat4x4F32 Mat4x4F32_Transpose(Mat4x4F32 m);
internal Mat4x4F32 Mat4x4F32_Inverse(Mat4x4F32 m);

internal Mat4x4F32 Mat4x4F32_Translate(Vec3F32 t);
internal Mat4x4F32 Mat4x4F32_Scale(Vec3F32 s);
internal Mat4x4F32 Mat4x4F32_RotateX(F32 radians);
internal Mat4x4F32 Mat4x4F32_RotateY(F32 radians);
internal Mat4x4F32 Mat4x4F32_RotateZ(F32 radians);
internal Mat4x4F32 Mat4x4F32_RotateAxisAngle(Vec3F32 axis, F32 radians); // arbitrary-axis rotation

internal Vec3F32 Mat4x4F32_GetTranslation(Mat4x4F32 m);
internal Vec3F32 Mat4x4F32_GetRotation(Mat4x4F32 m);
internal Vec3F32 Mat4x4F32_GetScale(Mat4x4F32 m);

////////////////////////
// F64 variant
typedef union Vec2F64 Vec2F64;
union Vec2F64
{
	struct
	{
		F64 x, y;
	};

	struct
	{
		F64 u, v;
	};

	F64 e[2];
};

typedef union Vec3F64 Vec3F64;
union Vec3F64
{
	struct
	{
		F64 x, y, z;
	};

	struct
	{
		F64 u, v, s;
	};

	F64 e[3];
};

typedef union Vec4F64 Vec4F64;
union Vec4F64
{
	struct
	{
		F64 x, y, z, w;
	};

	struct
	{
		F64 u, v, s, t;
	};

	F64 e[4];
};

typedef union Mat4x4F64 Mat4x4F64;
union Mat4x4F64
{
	F64		e[4][4];	  // e[col][row] -- column-major
	F64		elements[16]; // flat, column-major order, directly uploadable
	Vec4F64 columns[4];
};

internal Vec2F64 Vec2F64_Make(F64 x, F64 y);
internal Vec3F64 Vec3F64_Make(F64 x, F64 y, F64 z);
internal Vec4F64 Vec4F64_Make(F64 x, F64 y, F64 z, F64 w);

internal Vec2F64 Vec2F64_Add(Vec2F64 a, Vec2F64 b);
internal Vec3F64 Vec3F64_Add(Vec3F64 a, Vec3F64 b);
internal Vec4F64 Vec4F64_Add(Vec4F64 a, Vec4F64 b);

internal Vec2F64 Vec2F64_Sub(Vec2F64 a, Vec2F64 b);
internal Vec3F64 Vec3F64_Sub(Vec3F64 a, Vec3F64 b);
internal Vec4F64 Vec4F64_Sub(Vec4F64 a, Vec4F64 b);

internal Vec2F64 Vec2F64_Mul(Vec2F64 a, Vec2F64 b);
internal Vec3F64 Vec3F64_Mul(Vec3F64 a, Vec3F64 b);
internal Vec4F64 Vec4F64_Mul(Vec4F64 a, Vec4F64 b);

internal F64 Vec2F64_Dot(Vec2F64 a, Vec2F64 b);
internal F64 Vec3F64_Dot(Vec3F64 a, Vec3F64 b);
internal F64 Vec4F64_Dot(Vec4F64 a, Vec4F64 b);

internal Vec2F64 Vec2F64_Scale(Vec2F64 a, F64 b);
internal Vec3F64 Vec3F64_Scale(Vec3F64 a, F64 b);
internal Vec4F64 Vec4F64_Scale(Vec4F64 a, F64 b);

internal F64 Vec2F64_Length(Vec2F64 v);
internal F64 Vec3F64_Length(Vec3F64 v);
internal F64 Vec4F64_Length(Vec4F64 v);

internal F64 Vec2F64_LengthSquared(Vec2F64 v);
internal F64 Vec3F64_LengthSquared(Vec3F64 v);
internal F64 Vec4F64_LengthSquared(Vec4F64 v);

internal Vec2F64 Vec2F64_Normalize(Vec2F64 v);
internal Vec3F64 Vec3F64_Normalize(Vec3F64 v);
internal Vec4F64 Vec4F64_Normalize(Vec4F64 v);

internal Mat4x4F64 Mat4x4F64_MakeIdentity(void);
internal Mat4x4F64 Mat4x4F64_MakeFromVec3Columns(Vec3F64 c0, Vec3F64 c1, Vec3F64 c2, Vec3F64 c3);
internal Mat4x4F64 Mat4x4F64_MakeFromVec3Rows(Vec3F64 r0, Vec3F64 r1, Vec3F64 r2, Vec3F64 r3);

internal Mat4x4F64 Mat4x4F64_Mul(Mat4x4F64 a, Mat4x4F64 b);
internal Vec4F64   Mat4x4F64_MulVec4F64(Mat4x4F64 m, Vec4F64 v);
internal Mat4x4F64 Mat4x4F64_Transpose(Mat4x4F64 m);
internal Mat4x4F64 Mat4x4F64_Inverse(Mat4x4F64 m);
internal Mat4x4F64 Mat4x4F64_Translate(Vec3F64 t);
internal Mat4x4F64 Mat4x4F64_Scale(Vec3F64 s);
internal Mat4x4F64 Mat4x4F64_RotateX(F64 radians);
internal Mat4x4F64 Mat4x4F64_RotateY(F64 radians);
internal Mat4x4F64 Mat4x4F64_RotateZ(F64 radians);
internal Mat4x4F64 Mat4x4F64_RotateAxisAngle(Vec3F64 axis, F64 radians);

#endif // BASE_MATH_H
