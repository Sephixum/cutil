#include "base_math.h"

////////////////////////
// F32 variant

internal Vec2F32 Vec2F32_Make(F32 x, F32 y)
{
	return (Vec2F32){x, y};
}

internal Vec3F32 Vec3F32_Make(F32 x, F32 y, F32 z)
{
	return (Vec3F32){x, y, z};
}

internal Vec4F32 Vec4F32_Make(F32 x, F32 y, F32 z, F32 w)
{
	return (Vec4F32){x, y, z, w};
}

internal Vec2F32 Vec2F32_Add(Vec2F32 a, Vec2F32 b)
{
	Vec2F32 result = {0};
	result.x	   = a.x + b.x;
	result.y	   = a.y + b.y;
	return result;
}

internal Vec3F32 Vec3F32_Add(Vec3F32 a, Vec3F32 b)
{
	Vec3F32 result = {0};
	result.x	   = a.x + b.x;
	result.y	   = a.y + b.y;
	result.z	   = a.z + b.z;
	return result;
}

internal Vec4F32 Vec4F32_Add(Vec4F32 a, Vec4F32 b)
{
	Vec4F32 result = {0};
	result.x	   = a.x + b.x;
	result.y	   = a.y + b.y;
	result.z	   = a.z + b.z;
	result.w	   = a.w + b.w;
	return result;
}

internal Vec2F32 Vec2F32_Sub(Vec2F32 a, Vec2F32 b)
{
	Vec2F32 result = {0};
	result.x	   = a.x - b.x;
	result.y	   = a.y - b.y;
	return result;
}

internal Vec3F32 Vec3F32_Sub(Vec3F32 a, Vec3F32 b)
{
	Vec3F32 result = {0};
	result.x	   = a.x - b.x;
	result.y	   = a.y - b.y;
	result.z	   = a.z - b.z;
	return result;
}

internal Vec4F32 Vec4F32_Sub(Vec4F32 a, Vec4F32 b)
{
	Vec4F32 result = {0};
	result.x	   = a.x - b.x;
	result.y	   = a.y - b.y;
	result.z	   = a.z - b.z;
	result.w	   = a.w - b.w;
	return result;
}

internal Vec2F32 Vec2F32_Mul(Vec2F32 a, Vec2F32 b)
{
	Vec2F32 result = {0};
	result.x	   = a.x * b.x;
	result.y	   = a.y * b.y;
	return result;
}

internal Vec3F32 Vec3F32_Mul(Vec3F32 a, Vec3F32 b)
{
	Vec3F32 result = {0};
	result.x	   = a.x * b.x;
	result.y	   = a.y * b.y;
	result.z	   = a.z * b.z;
	return result;
}

internal Vec4F32 Vec4F32_Mul(Vec4F32 a, Vec4F32 b)
{
	Vec4F32 result = {0};
	result.x	   = a.x * b.x;
	result.y	   = a.y * b.y;
	result.z	   = a.z * b.z;
	result.w	   = a.w * b.w;
	return result;
}

internal F32 Vec2F32_Dot(Vec2F32 a, Vec2F32 b)
{
	return (a.x * b.x) + (a.y * b.y);
}

internal F32 Vec3F32_Dot(Vec3F32 a, Vec3F32 b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

internal F32 Vec4F32_Dot(Vec4F32 a, Vec4F32 b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

internal Vec2F32 Vec2F32_Scale(Vec2F32 a, F32 b)
{
	a.x *= b;
	a.y *= b;
	return a;
}

internal Vec3F32 Vec3F32_Scale(Vec3F32 a, F32 b)
{
	a.x *= b;
	a.y *= b;
	a.z *= b;
	return a;
}

internal Vec4F32 Vec4F32_Scale(Vec4F32 a, F32 b)
{
	a.x *= b;
	a.y *= b;
	a.z *= b;
	a.w *= b;
	return a;
}

internal F32 Vec2F32_Length(Vec2F32 v)
{
	return sqrtf(Vec2F32_Dot(v, v));
}

internal F32 Vec3F32_Length(Vec3F32 v)
{
	return sqrtf(Vec3F32_Dot(v, v));
}

internal F32 Vec4F32_Length(Vec4F32 v)
{
	return sqrtf(Vec4F32_Dot(v, v));
}

internal F32 Vec2F32_LengthSquared(Vec2F32 v)
{
	return Vec2F32_Dot(v, v);
}
internal F32 Vec3F32_LengthSquared(Vec3F32 v)
{
	return Vec3F32_Dot(v, v);
}
internal F32 Vec4F32_LengthSquared(Vec4F32 v)
{
	return Vec4F32_Dot(v, v);
}

internal Vec2F32 Vec2F32_Normalize(Vec2F32 v)
{
	F32 len = Vec2F32_Length(v);
	if (len < F32_EPSILON)
	{
		return Vec2F32_Make(0, 0);
	}
	F32 inverse_len = 1 / len;
	return Vec2F32_Scale(v, inverse_len);
}

internal Vec3F32 Vec3F32_Normalize(Vec3F32 v)
{
	F32 len = Vec3F32_Length(v);
	if (len < F32_EPSILON)
	{
		return Vec3F32_Make(0, 0, 0);
	}
	F32 inverse_len = 1 / len;
	return Vec3F32_Scale(v, inverse_len);
}

internal Vec4F32 Vec4F32_Normalize(Vec4F32 v)
{
	F32 len = Vec4F32_Length(v);
	if (len < F32_EPSILON)
	{
		return Vec4F32_Make(0, 0, 0, 0);
	}
	F32 inverse_len = 1 / len;
	return Vec4F32_Scale(v, inverse_len);
}

internal Mat4x4F32 Mat4x4F32_MakeIdentity(void)
{
	Mat4x4F32 result = {0};
	result.e[0][0]	 = 1.0f;
	result.e[1][1]	 = 1.0f;
	result.e[2][2]	 = 1.0f;
	result.e[3][3]	 = 1.0f;
	return result;
}

internal Mat4x4F32 Mat4x4F32_MakeFromVec3Columns(Vec3F32 c0, Vec3F32 c1, Vec3F32 c2, Vec3F32 c3)
{
	// c0, c1, c2 = rotation/scale basis vectors (X, Y, Z axes)
	// c3         = translation
	// 4th component (w) filled in implicitly: 0 for basis columns, 1 for translation
	Mat4x4F32 result;
	result.columns[0] = Vec4F32_Make(c0.x, c0.y, c0.z, 0.0f);
	result.columns[1] = Vec4F32_Make(c1.x, c1.y, c1.z, 0.0f);
	result.columns[2] = Vec4F32_Make(c2.x, c2.y, c2.z, 0.0f);
	result.columns[3] = Vec4F32_Make(c3.x, c3.y, c3.z, 1.0f);
	return result;
}

////////////////////////////////
// Mat4x4F32 core ops

internal Mat4x4F32 Mat4x4F32_Mul(Mat4x4F32 a, Mat4x4F32 b)
{
	Mat4x4F32 result;
	for (S32 col = 0; col < 4; col += 1)
	{
		for (S32 row = 0; row < 4; row += 1)
		{
			F32 sum = 0;
			for (S32 k = 0; k < 4; k += 1)
			{
				sum += a.e[k][row] * b.e[col][k];
			}
			result.e[col][row] = sum;
		}
	}
	return result;
}

internal Vec4F32 Mat4x4F32_MulVec4F32(Mat4x4F32 m, Vec4F32 v)
{
	Vec4F32 result;
	for (S32 row = 0; row < 4; row += 1)
	{
		F32 sum = 0;
		for (S32 col = 0; col < 4; col += 1)
		{
			sum += m.e[col][row] * v.e[col];
		}
		result.e[row] = sum;
	}
	return result;
}

internal Mat4x4F32 Mat4x4F32_Transpose(Mat4x4F32 m)
{
	Mat4x4F32 result;
	for (S32 col = 0; col < 4; col += 1)
	{
		for (S32 row = 0; row < 4; row += 1)
		{
			result.e[col][row] = m.e[row][col];
		}
	}
	return result;
}

internal Mat4x4F32 Mat4x4F32_Inverse(Mat4x4F32 m)
{
	// classic cofactor/adjugate expansion, operating directly on the
	// flat column-major elements array (no row/col bookkeeping needed)
	F32		 *a		 = m.elements;
	Mat4x4F32 result = {0};
	F32		 *inv	 = result.elements;

	inv[0]	= a[5] * a[10] * a[15] - a[5] * a[11] * a[14] - a[9] * a[6] * a[15] + a[9] * a[7] * a[14] +
			  a[13] * a[6] * a[11] - a[13] * a[7] * a[10];
	inv[4]	= -a[4] * a[10] * a[15] + a[4] * a[11] * a[14] + a[8] * a[6] * a[15] - a[8] * a[7] * a[14] -
			  a[12] * a[6] * a[11] + a[12] * a[7] * a[10];
	inv[8]	= a[4] * a[9] * a[15] - a[4] * a[11] * a[13] - a[8] * a[5] * a[15] + a[8] * a[7] * a[13] +
			  a[12] * a[5] * a[11] - a[12] * a[7] * a[9];
	inv[12] = -a[4] * a[9] * a[14] + a[4] * a[10] * a[13] + a[8] * a[5] * a[14] - a[8] * a[6] * a[13] -
			  a[12] * a[5] * a[10] + a[12] * a[6] * a[9];

	inv[1]	= -a[1] * a[10] * a[15] + a[1] * a[11] * a[14] + a[9] * a[2] * a[15] - a[9] * a[3] * a[14] -
			  a[13] * a[2] * a[11] + a[13] * a[3] * a[10];
	inv[5]	= a[0] * a[10] * a[15] - a[0] * a[11] * a[14] - a[8] * a[2] * a[15] + a[8] * a[3] * a[14] +
			  a[12] * a[2] * a[11] - a[12] * a[3] * a[10];
	inv[9]	= -a[0] * a[9] * a[15] + a[0] * a[11] * a[13] + a[8] * a[1] * a[15] - a[8] * a[3] * a[13] -
			  a[12] * a[1] * a[11] + a[12] * a[3] * a[9];
	inv[13] = a[0] * a[9] * a[14] - a[0] * a[10] * a[13] - a[8] * a[1] * a[14] + a[8] * a[2] * a[13] +
			  a[12] * a[1] * a[10] - a[12] * a[2] * a[9];

	inv[2]	= a[1] * a[6] * a[15] - a[1] * a[7] * a[14] - a[5] * a[2] * a[15] + a[5] * a[3] * a[14] +
			  a[13] * a[2] * a[7] - a[13] * a[3] * a[6];
	inv[6]	= -a[0] * a[6] * a[15] + a[0] * a[7] * a[14] + a[4] * a[2] * a[15] - a[4] * a[3] * a[14] -
			  a[12] * a[2] * a[7] + a[12] * a[3] * a[6];
	inv[10] = a[0] * a[5] * a[15] - a[0] * a[7] * a[13] - a[4] * a[1] * a[15] + a[4] * a[3] * a[13] +
			  a[12] * a[1] * a[7] - a[12] * a[3] * a[5];
	inv[14] = -a[0] * a[5] * a[14] + a[0] * a[6] * a[13] + a[4] * a[1] * a[14] - a[4] * a[2] * a[13] -
			  a[12] * a[1] * a[6] + a[12] * a[2] * a[5];

	inv[3]	= -a[1] * a[6] * a[11] + a[1] * a[7] * a[10] + a[5] * a[2] * a[11] - a[5] * a[3] * a[10] -
			  a[9] * a[2] * a[7] + a[9] * a[3] * a[6];
	inv[7]	= a[0] * a[6] * a[11] - a[0] * a[7] * a[10] - a[4] * a[2] * a[11] + a[4] * a[3] * a[10] +
			  a[8] * a[2] * a[7] - a[8] * a[3] * a[6];
	inv[11] = -a[0] * a[5] * a[11] + a[0] * a[7] * a[9] + a[4] * a[1] * a[11] - a[4] * a[3] * a[9] -
			  a[8] * a[1] * a[7] + a[8] * a[3] * a[5];
	inv[15] = a[0] * a[5] * a[10] - a[0] * a[6] * a[9] - a[4] * a[1] * a[10] + a[4] * a[2] * a[9] + a[8] * a[1] * a[6] -
			  a[8] * a[2] * a[5];

	F32 det = a[0] * inv[0] + a[1] * inv[4] + a[2] * inv[8] + a[3] * inv[12];
	if (det != 0.f)
	{
		F32 inv_det = 1.f / det;
		for (S32 i = 0; i < 16; i += 1)
		{
			inv[i] *= inv_det;
		}
	}
	return result;
}

////////////////////////////////
// Construction

internal Mat4x4F32 Mat4x4F32_Translate(Vec3F32 t)
{
	Mat4x4F32 result = {
		.e =
			{
				{1, 0, 0, 0},
				{0, 1, 0, 0},
				{0, 0, 1, 0},
				{t.x, t.y, t.z, 1},
			},
	};
	return result;
}

internal Mat4x4F32 Mat4x4F32_Scale(Vec3F32 s)
{
	Mat4x4F32 result = {
		.e =
			{
				{s.x, 0, 0, 0},
				{0, s.y, 0, 0},
				{0, 0, s.z, 0},
				{0, 0, 0, 1},
			},
	};
	return result;
}

internal Mat4x4F32 Mat4x4F32_RotateX(F32 radians)
{
	F32		  c		 = cosf(radians);
	F32		  s		 = sinf(radians);
	Mat4x4F32 result = {
		.e =
			{
				{1, 0, 0, 0},
				{0, c, s, 0},
				{0, -s, c, 0},
				{0, 0, 0, 1},
			},
	};
	return result;
}

internal Mat4x4F32 Mat4x4F32_RotateY(F32 radians)
{
	F32		  c		 = cosf(radians);
	F32		  s		 = sinf(radians);
	Mat4x4F32 result = {
		.e =
			{
				{c, 0, -s, 0},
				{0, 1, 0, 0},
				{s, 0, c, 0},
				{0, 0, 0, 1},
			},
	};
	return result;
}

internal Mat4x4F32 Mat4x4F32_RotateZ(F32 radians)
{
	F32		  c		 = cosf(radians);
	F32		  s		 = sinf(radians);
	Mat4x4F32 result = {
		.e =
			{
				{c, s, 0, 0},
				{-s, c, 0, 0},
				{0, 0, 1, 0},
				{0, 0, 0, 1},
			},
	};
	return result;
}

internal Mat4x4F32 Mat4x4F32_RotateAxisAngle(Vec3F32 axis, F32 radians)
{
	// normalize axis defensively
	F32 len		= sqrtf(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
	F32 inv_len = (len != 0.f) ? (1.f / len) : 0.f;
	F32 x		= axis.x * inv_len;
	F32 y		= axis.y * inv_len;
	F32 z		= axis.z * inv_len;

	F32 c = cosf(radians);
	F32 s = sinf(radians);
	F32 t = 1.f - c;

	Mat4x4F32 result = {
		.e =
			{
				{t * x * x + c, t * x * y + s * z, t * x * z - s * y, 0},
				{t * x * y - s * z, t * y * y + c, t * y * z + s * x, 0},
				{t * x * z + s * y, t * y * z - s * x, t * z * z + c, 0},
				{0, 0, 0, 1},
			},
	};
	return result;
}

////////////////////////////////
// Decomposition

internal Vec3F32 Mat4x4F32_GetTranslation(Mat4x4F32 m)
{
	Vec3F32 result = {m.e[3][0], m.e[3][1], m.e[3][2]};
	return result;
}

internal Vec3F32 Mat4x4F32_GetScale(Mat4x4F32 m)
{
	Vec3F32 result = {
		sqrtf(m.e[0][0] * m.e[0][0] + m.e[0][1] * m.e[0][1] + m.e[0][2] * m.e[0][2]),
		sqrtf(m.e[1][0] * m.e[1][0] + m.e[1][1] * m.e[1][1] + m.e[1][2] * m.e[1][2]),
		sqrtf(m.e[2][0] * m.e[2][0] + m.e[2][1] * m.e[2][1] + m.e[2][2] * m.e[2][2]),
	};
	return result;
}

internal Vec3F32 Mat4x4F32_GetRotation(Mat4x4F32 m)
{
	// strip scale so we're decomposing a pure rotation matrix
	Vec3F32 scale = Mat4x4F32_GetScale(m);
	F32		sx	  = (scale.x != 0.f) ? (1.f / scale.x) : 0.f;
	F32		sy	  = (scale.y != 0.f) ? (1.f / scale.y) : 0.f;
	F32		sz	  = (scale.z != 0.f) ? (1.f / scale.z) : 0.f;

	F32 r00 = m.e[0][0] * sx, r01 = m.e[0][1] * sx, r02 = m.e[0][2] * sx;
	F32 r12 = m.e[1][2] * sy;
	F32 r22 = m.e[2][2] * sz;

	// Tait-Bryan ZYX extraction (matches R = RotateZ * RotateY * RotateX
	// composition order). Clamp guards against asin domain errors from
	// floating point drift at the gimbal-lock poles.
	F32 sin_y = -r02;
	sin_y	  = (sin_y < -1.f) ? -1.f : ((sin_y > 1.f) ? 1.f : sin_y);

	Vec3F32 result = {
		atan2f(r12, r22), // x (roll)
		asinf(sin_y),	  // y (pitch)
		atan2f(r01, r00), // z (yaw)
	};
	return result;
}

////////////////////////
// F64 variant
internal Vec2F64 Vec2F64_Make(F64 x, F64 y)
{
	return (Vec2F64){x, y};
}
internal Vec3F64 Vec3F64_Make(F64 x, F64 y, F64 z)
{
	return (Vec3F64){x, y, z};
}
internal Vec4F64 Vec4F64_Make(F64 x, F64 y, F64 z, F64 w)
{
	return (Vec4F64){x, y, z, w};
}

internal Vec2F64 Vec2F64_Add(Vec2F64 a, Vec2F64 b)
{
	Vec2F64 result = {0};
	result.x	   = a.x + b.x;
	result.y	   = a.y + b.y;
	return result;
}
internal Vec3F64 Vec3F64_Add(Vec3F64 a, Vec3F64 b)
{
	Vec3F64 result = {0};
	result.x	   = a.x + b.x;
	result.y	   = a.y + b.y;
	result.z	   = a.z + b.z;
	return result;
}
internal Vec4F64 Vec4F64_Add(Vec4F64 a, Vec4F64 b)
{
	Vec4F64 result = {0};
	result.x	   = a.x + b.x;
	result.y	   = a.y + b.y;
	result.z	   = a.z + b.z;
	result.w	   = a.w + b.w;
	return result;
}

internal Vec2F64 Vec2F64_Sub(Vec2F64 a, Vec2F64 b)
{
	Vec2F64 result = {0};
	result.x	   = a.x - b.x;
	result.y	   = a.y - b.y;
	return result;
}
internal Vec3F64 Vec3F64_Sub(Vec3F64 a, Vec3F64 b)
{
	Vec3F64 result = {0};
	result.x	   = a.x - b.x;
	result.y	   = a.y - b.y;
	result.z	   = a.z - b.z;
	return result;
}
internal Vec4F64 Vec4F64_Sub(Vec4F64 a, Vec4F64 b)
{
	Vec4F64 result = {0};
	result.x	   = a.x - b.x;
	result.y	   = a.y - b.y;
	result.z	   = a.z - b.z;
	result.w	   = a.w - b.w;
	return result;
}

internal Vec2F64 Vec2F64_Mul(Vec2F64 a, Vec2F64 b)
{
	Vec2F64 result = {0};
	result.x	   = a.x * b.x;
	result.y	   = a.y * b.y;
	return result;
}
internal Vec3F64 Vec3F64_Mul(Vec3F64 a, Vec3F64 b)
{
	Vec3F64 result = {0};
	result.x	   = a.x * b.x;
	result.y	   = a.y * b.y;
	result.z	   = a.z * b.z;
	return result;
}
internal Vec4F64 Vec4F64_Mul(Vec4F64 a, Vec4F64 b)
{
	Vec4F64 result = {0};
	result.x	   = a.x * b.x;
	result.y	   = a.y * b.y;
	result.z	   = a.z * b.z;
	result.w	   = a.w * b.w;
	return result;
}

internal F64 Vec2F64_Dot(Vec2F64 a, Vec2F64 b)
{
	return (a.x * b.x) + (a.y * b.y);
}
internal F64 Vec3F64_Dot(Vec3F64 a, Vec3F64 b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}
internal F64 Vec4F64_Dot(Vec4F64 a, Vec4F64 b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

internal Vec2F64 Vec2F64_Scale(Vec2F64 a, F64 b)
{
	a.x *= b;
	a.y *= b;
	return a;
}
internal Vec3F64 Vec3F64_Scale(Vec3F64 a, F64 b)
{
	a.x *= b;
	a.y *= b;
	a.z *= b;
	return a;
}
internal Vec4F64 Vec4F64_Scale(Vec4F64 a, F64 b)
{
	a.x *= b;
	a.y *= b;
	a.z *= b;
	a.w *= b;
	return a;
}

internal F64 Vec2F64_Length(Vec2F64 v)
{
	return sqrt(Vec2F64_Dot(v, v));
}
internal F64 Vec3F64_Length(Vec3F64 v)
{
	return sqrt(Vec3F64_Dot(v, v));
}
internal F64 Vec4F64_Length(Vec4F64 v)
{
	return sqrt(Vec4F64_Dot(v, v));
}

internal F64 Vec2F64_LengthSquared(Vec2F64 v)
{
	return Vec2F64_Dot(v, v);
}
internal F64 Vec3F64_LengthSquared(Vec3F64 v)
{
	return Vec3F64_Dot(v, v);
}
internal F64 Vec4F64_LengthSquared(Vec4F64 v)
{
	return Vec4F64_Dot(v, v);
}

internal Vec2F64 Vec2F64_Normalize(Vec2F64 v)
{
	F64 len = Vec2F64_Length(v);
	if (len < F64_EPSILON)
	{
		return Vec2F64_Make(0, 0);
	}
	F64 inverse_len = 1.0 / len;
	return Vec2F64_Scale(v, inverse_len);
}

internal Vec3F64 Vec3F64_Normalize(Vec3F64 v)
{
	F64 len = Vec3F64_Length(v);
	if (len < F64_EPSILON)
	{
		return Vec3F64_Make(0, 0, 0);
	}
	F64 inverse_len = 1.0 / len;
	return Vec3F64_Scale(v, inverse_len);
}

internal Vec4F64 Vec4F64_Normalize(Vec4F64 v)
{
	F64 len = Vec4F64_Length(v);
	if (len < F64_EPSILON)
	{
		return Vec4F64_Make(0, 0, 0, 0);
	}
	F64 inverse_len = 1.0 / len;
	return Vec4F64_Scale(v, inverse_len);
}

////////////////////////////////
// Construction helpers

internal Mat4x4F64 Mat4x4F64_MakeIdentity(void)
{
	Mat4x4F64 result = {
		.e =
			{
				{1, 0, 0, 0},
				{0, 1, 0, 0},
				{0, 0, 1, 0},
				{0, 0, 0, 1},
			},
	};
	return result;
}

internal Mat4x4F64 Mat4x4F64_MakeFromVec3Columns(Vec3F64 c0, Vec3F64 c1, Vec3F64 c2, Vec3F64 c3)
{
	// c0/c1/c2 are basis vectors (w=0), c3 is translation (w=1)
	Mat4x4F64 result = {
		.e =
			{
				{c0.x, c0.y, c0.z, 0},
				{c1.x, c1.y, c1.z, 0},
				{c2.x, c2.y, c2.z, 0},
				{c3.x, c3.y, c3.z, 1},
			},
	};
	return result;
}

////////////////////////////////
// Mat4x4F64 core ops

internal Mat4x4F64 Mat4x4F64_Mul(Mat4x4F64 a, Mat4x4F64 b)
{
	Mat4x4F64 result;
	for (S32 col = 0; col < 4; col += 1)
	{
		for (S32 row = 0; row < 4; row += 1)
		{
			F64 sum = 0;
			for (S32 k = 0; k < 4; k += 1)
			{
				sum += a.e[k][row] * b.e[col][k];
			}
			result.e[col][row] = sum;
		}
	}
	return result;
}

internal Vec4F64 Mat4x4F64_MulVec4F64(Mat4x4F64 m, Vec4F64 v)
{
	Vec4F64 result;
	for (S32 row = 0; row < 4; row += 1)
	{
		F64 sum = 0;
		for (S32 col = 0; col < 4; col += 1)
		{
			sum += m.e[col][row] * v.e[col];
		}
		result.e[row] = sum;
	}
	return result;
}

internal Mat4x4F64 Mat4x4F64_Transpose(Mat4x4F64 m)
{
	Mat4x4F64 result;
	for (S32 col = 0; col < 4; col += 1)
	{
		for (S32 row = 0; row < 4; row += 1)
		{
			result.e[col][row] = m.e[row][col];
		}
	}
	return result;
}

internal Mat4x4F64 Mat4x4F64_Inverse(Mat4x4F64 m)
{
	F64		 *a		 = m.elements;
	Mat4x4F64 result = {0};
	F64		 *inv	 = result.elements;

	inv[0]	= a[5] * a[10] * a[15] - a[5] * a[11] * a[14] - a[9] * a[6] * a[15] + a[9] * a[7] * a[14] +
			  a[13] * a[6] * a[11] - a[13] * a[7] * a[10];
	inv[4]	= -a[4] * a[10] * a[15] + a[4] * a[11] * a[14] + a[8] * a[6] * a[15] - a[8] * a[7] * a[14] -
			  a[12] * a[6] * a[11] + a[12] * a[7] * a[10];
	inv[8]	= a[4] * a[9] * a[15] - a[4] * a[11] * a[13] - a[8] * a[5] * a[15] + a[8] * a[7] * a[13] +
			  a[12] * a[5] * a[11] - a[12] * a[7] * a[9];
	inv[12] = -a[4] * a[9] * a[14] + a[4] * a[10] * a[13] + a[8] * a[5] * a[14] - a[8] * a[6] * a[13] -
			  a[12] * a[5] * a[10] + a[12] * a[6] * a[9];

	inv[1]	= -a[1] * a[10] * a[15] + a[1] * a[11] * a[14] + a[9] * a[2] * a[15] - a[9] * a[3] * a[14] -
			  a[13] * a[2] * a[11] + a[13] * a[3] * a[10];
	inv[5]	= a[0] * a[10] * a[15] - a[0] * a[11] * a[14] - a[8] * a[2] * a[15] + a[8] * a[3] * a[14] +
			  a[12] * a[2] * a[11] - a[12] * a[3] * a[10];
	inv[9]	= -a[0] * a[9] * a[15] + a[0] * a[11] * a[13] + a[8] * a[1] * a[15] - a[8] * a[3] * a[13] -
			  a[12] * a[1] * a[11] + a[12] * a[3] * a[9];
	inv[13] = a[0] * a[9] * a[14] - a[0] * a[10] * a[13] - a[8] * a[1] * a[14] + a[8] * a[2] * a[13] +
			  a[12] * a[1] * a[10] - a[12] * a[2] * a[9];

	inv[2]	= a[1] * a[6] * a[15] - a[1] * a[7] * a[14] - a[5] * a[2] * a[15] + a[5] * a[3] * a[14] +
			  a[13] * a[2] * a[7] - a[13] * a[3] * a[6];
	inv[6]	= -a[0] * a[6] * a[15] + a[0] * a[7] * a[14] + a[4] * a[2] * a[15] - a[4] * a[3] * a[14] -
			  a[12] * a[2] * a[7] + a[12] * a[3] * a[6];
	inv[10] = a[0] * a[5] * a[15] - a[0] * a[7] * a[13] - a[4] * a[1] * a[15] + a[4] * a[3] * a[13] +
			  a[12] * a[1] * a[7] - a[12] * a[3] * a[5];
	inv[14] = -a[0] * a[5] * a[14] + a[0] * a[6] * a[13] + a[4] * a[1] * a[14] - a[4] * a[2] * a[13] -
			  a[12] * a[1] * a[6] + a[12] * a[2] * a[5];

	inv[3]	= -a[1] * a[6] * a[11] + a[1] * a[7] * a[10] + a[5] * a[2] * a[11] - a[5] * a[3] * a[10] -
			  a[9] * a[2] * a[7] + a[9] * a[3] * a[6];
	inv[7]	= a[0] * a[6] * a[11] - a[0] * a[7] * a[10] - a[4] * a[2] * a[11] + a[4] * a[3] * a[10] +
			  a[8] * a[2] * a[7] - a[8] * a[3] * a[6];
	inv[11] = -a[0] * a[5] * a[11] + a[0] * a[7] * a[9] + a[4] * a[1] * a[11] - a[4] * a[3] * a[9] -
			  a[8] * a[1] * a[7] + a[8] * a[3] * a[5];
	inv[15] = a[0] * a[5] * a[10] - a[0] * a[6] * a[9] - a[4] * a[1] * a[10] + a[4] * a[2] * a[9] + a[8] * a[1] * a[6] -
			  a[8] * a[2] * a[5];

	F64 det = a[0] * inv[0] + a[1] * inv[4] + a[2] * inv[8] + a[3] * inv[12];
	if (det != 0.0)
	{
		F64 inv_det = 1.0 / det;
		for (S32 i = 0; i < 16; i += 1)
		{
			inv[i] *= inv_det;
		}
	}
	return result;
}

////////////////////////////////
// Transform construction

internal Mat4x4F64 Mat4x4F64_Translate(Vec3F64 t)
{
	Mat4x4F64 result = {
		.e =
			{
				{1, 0, 0, 0},
				{0, 1, 0, 0},
				{0, 0, 1, 0},
				{t.x, t.y, t.z, 1},
			},
	};
	return result;
}

internal Mat4x4F64 Mat4x4F64_Scale(Vec3F64 s)
{
	Mat4x4F64 result = {
		.e =
			{
				{s.x, 0, 0, 0},
				{0, s.y, 0, 0},
				{0, 0, s.z, 0},
				{0, 0, 0, 1},
			},
	};
	return result;
}

internal Mat4x4F64 Mat4x4F64_RotateX(F64 radians)
{
	F64		  c		 = cos(radians);
	F64		  s		 = sin(radians);
	Mat4x4F64 result = {
		.e =
			{
				{1, 0, 0, 0},
				{0, c, s, 0},
				{0, -s, c, 0},
				{0, 0, 0, 1},
			},
	};
	return result;
}

internal Mat4x4F64 Mat4x4F64_RotateY(F64 radians)
{
	F64		  c		 = cos(radians);
	F64		  s		 = sin(radians);
	Mat4x4F64 result = {
		.e =
			{
				{c, 0, -s, 0},
				{0, 1, 0, 0},
				{s, 0, c, 0},
				{0, 0, 0, 1},
			},
	};
	return result;
}

internal Mat4x4F64 Mat4x4F64_RotateZ(F64 radians)
{
	F64		  c		 = cos(radians);
	F64		  s		 = sin(radians);
	Mat4x4F64 result = {
		.e =
			{
				{c, s, 0, 0},
				{-s, c, 0, 0},
				{0, 0, 1, 0},
				{0, 0, 0, 1},
			},
	};
	return result;
}

internal Mat4x4F64 Mat4x4F64_RotateAxisAngle(Vec3F64 axis, F64 radians)
{
	F64 len		= sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
	F64 inv_len = (len != 0.0) ? (1.0 / len) : 0.0;
	F64 x		= axis.x * inv_len;
	F64 y		= axis.y * inv_len;
	F64 z		= axis.z * inv_len;

	F64 c = cos(radians);
	F64 s = sin(radians);
	F64 t = 1.0 - c;

	Mat4x4F64 result = {
		.e =
			{
				{t * x * x + c, t * x * y + s * z, t * x * z - s * y, 0},
				{t * x * y - s * z, t * y * y + c, t * y * z + s * x, 0},
				{t * x * z + s * y, t * y * z - s * x, t * z * z + c, 0},
				{0, 0, 0, 1},
			},
	};
	return result;
}

////////////////////////////////
// Decomposition

internal Vec3F64 Mat4x4F64_GetTranslation(Mat4x4F64 m)
{
	Vec3F64 result = {m.e[3][0], m.e[3][1], m.e[3][2]};
	return result;
}

internal Vec3F64 Mat4x4F64_GetScale(Mat4x4F64 m)
{
	Vec3F64 result = {
		sqrt(m.e[0][0] * m.e[0][0] + m.e[0][1] * m.e[0][1] + m.e[0][2] * m.e[0][2]),
		sqrt(m.e[1][0] * m.e[1][0] + m.e[1][1] * m.e[1][1] + m.e[1][2] * m.e[1][2]),
		sqrt(m.e[2][0] * m.e[2][0] + m.e[2][1] * m.e[2][1] + m.e[2][2] * m.e[2][2]),
	};
	return result;
}

internal Vec3F64 Mat4x4F64_GetRotation(Mat4x4F64 m)
{
	Vec3F64 scale = Mat4x4F64_GetScale(m);
	F64		sx	  = (scale.x != 0.0) ? (1.0 / scale.x) : 0.0;
	F64		sy	  = (scale.y != 0.0) ? (1.0 / scale.y) : 0.0;
	F64		sz	  = (scale.z != 0.0) ? (1.0 / scale.z) : 0.0;

	F64 r00 = m.e[0][0] * sx, r01 = m.e[0][1] * sx, r02 = m.e[0][2] * sx;
	F64 r12 = m.e[1][2] * sy;
	F64 r22 = m.e[2][2] * sz;

	F64 sin_y = -r02;
	sin_y	  = (sin_y < -1.0) ? -1.0 : ((sin_y > 1.0) ? 1.0 : sin_y);

	Vec3F64 result = {
		atan2(r12, r22), // x (roll)
		asin(sin_y),	 // y (pitch)
		atan2(r01, r00), // z (yaw)
	};
	return result;
}

////////////////////////////////
// Quaternion F32 variant

internal Quat4F32 Quat4F32_Make(F32 x, F32 y, F32 z, F32 w)
{
	return (Quat4F32){x, y, z, w};
}

internal Quat4F32 Quat4F32_Identity(void)
{
	return (Quat4F32){0, 0, 0, 1};
}

internal Quat4F32 Quat4F32_Add(Quat4F32 a, Quat4F32 b)
{
	return (Quat4F32){a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

internal Quat4F32 Quat4F32_Sub(Quat4F32 a, Quat4F32 b)
{
	return (Quat4F32){a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

internal Quat4F32 Quat4F32_Mul(Quat4F32 a, Quat4F32 b)
{
	// quaternion product: (a0 + a1*i + a2*j + a3*k)(b0 + b1*i + b2*j + b3*k)
	// where a.w is scalar, a.xyz is vector part
	Quat4F32 result;
	result.w = a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z;
	result.x = a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y;
	result.y = a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x;
	result.z = a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w;
	return result;
}

internal Quat4F32 Quat4F32_Scale(Quat4F32 q, F32 s)
{
	return (Quat4F32){q.x * s, q.y * s, q.z * s, q.w * s};
}

internal Quat4F32 Quat4F32_Conjugate(Quat4F32 q)
{
	return (Quat4F32){-q.x, -q.y, -q.z, q.w};
}

internal F32 Quat4F32_LengthSquared(Quat4F32 q)
{
	return q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w;
}

internal F32 Quat4F32_Length(Quat4F32 q)
{
	return sqrtf(Quat4F32_LengthSquared(q));
}

internal F32 Quat4F32_Dot(Quat4F32 a, Quat4F32 b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

internal Quat4F32 Quat4F32_Normalize(Quat4F32 q)
{
	F32 len = Quat4F32_Length(q);
	if(len < F32_EPSILON)
	{
		return Quat4F32_Identity();
	}
	F32 inv_len = 1.0f / len;
	return Quat4F32_Scale(q, inv_len);
}

internal Quat4F32 Quat4F32_Inverse(Quat4F32 q)
{
	F32 len_sq = Quat4F32_LengthSquared(q);
	if(len_sq < F32_EPSILON)
	{
		return Quat4F32_Identity();
	}
	Quat4F32 conj = Quat4F32_Conjugate(q);
	return Quat4F32_Scale(conj, 1.0f / len_sq);
}

internal Quat4F32 Quat4F32_FromAxisAngle(Vec3F32 axis, F32 radians)
{
	F32 len = sqrtf(axis.x*axis.x + axis.y*axis.y + axis.z*axis.z);
	F32 inv_len = (len != 0.f) ? (1.f / len) : 0.f;
	
	F32 half_angle = radians * 0.5f;
	F32 sin_half = sinf(half_angle);
	F32 cos_half = cosf(half_angle);
	
	Quat4F32 result;
	result.x = axis.x * inv_len * sin_half;
	result.y = axis.y * inv_len * sin_half;
	result.z = axis.z * inv_len * sin_half;
	result.w = cos_half;
	return result;
}

internal void Quat4F32_ToAxisAngle(Quat4F32 q, Vec3F32 *out_axis, F32 *out_radians)
{
	q = Quat4F32_Normalize(q);
	
	F32 sin_half_sq = q.x*q.x + q.y*q.y + q.z*q.z;
	F32 sin_half = sqrtf(sin_half_sq);
	
	if(sin_half < F32_EPSILON)
	{
		// no rotation or 360-degree rotation
		out_axis->x = 1.f;
		out_axis->y = 0.f;
		out_axis->z = 0.f;
		*out_radians = 0.f;
	}
	else
	{
		F32 inv_sin = 1.f / sin_half;
		out_axis->x = q.x * inv_sin;
		out_axis->y = q.y * inv_sin;
		out_axis->z = q.z * inv_sin;
		*out_radians = 2.f * atan2f(sin_half, q.w);
	}
}

internal Quat4F32 Quat4F32_FromEuler(Vec3F32 euler)
{
	// Tait-Bryan convention: ZYX (yaw-pitch-roll)
	// euler.x = roll (rotation around X), euler.y = pitch (Y), euler.z = yaw (Z)
	F32 roll = euler.x * 0.5f;
	F32 pitch = euler.y * 0.5f;
	F32 yaw = euler.z * 0.5f;
	
	F32 sr = sinf(roll), cr = cosf(roll);
	F32 sp = sinf(pitch), cp = cosf(pitch);
	F32 sy = sinf(yaw), cy = cosf(yaw);
	
	Quat4F32 result;
	result.x = sr*cp*cy - cr*sp*sy;
	result.y = cr*sp*cy + sr*cp*sy;
	result.z = cr*cp*sy - sr*sp*cy;
	result.w = cr*cp*cy + sr*sp*sy;
	return result;
}

internal Vec3F32 Quat4F32_ToEuler(Quat4F32 q)
{
	// Tait-Bryan ZYX extraction
	q = Quat4F32_Normalize(q);
	
	F32 x2 = q.x*q.x, y2 = q.y*q.y, z2 = q.z*q.z;
	F32 xy = q.x*q.y, xz = q.x*q.z, yz = q.y*q.z;
	F32 wx = q.w*q.x, wy = q.w*q.y, wz = q.w*q.z;
	
	// sin(pitch) clamped to avoid asin domain errors
	F32 sin_pitch = 2.0f * (wy - xz);
	sin_pitch = (sin_pitch < -1.f) ? -1.f : ((sin_pitch > 1.f) ? 1.f : sin_pitch);
	
	Vec3F32 result;
	result.x = atan2f(2.0f * (wz + xy), 1.0f - 2.0f * (x2 + y2));  // roll
	result.y = asinf(sin_pitch);                                     // pitch
	result.z = atan2f(2.0f * (wx + yz), 1.0f - 2.0f * (y2 + z2));  // yaw
	return result;
}

internal Quat4F32 Quat4F32_FromMat4x4(Mat4x4F32 m)
{
	// Shepperd's method for robust conversion
	Quat4F32 result;
	
	F32 trace = m.e[0][0] + m.e[1][1] + m.e[2][2];
	
	if(trace > 0.f)
	{
		F32 s = sqrtf(trace + 1.f) * 2.f;
		F32 inv_s = 1.f / s;
		result.w = 0.25f * s;
		result.x = (m.e[2][1] - m.e[1][2]) * inv_s;
		result.y = (m.e[0][2] - m.e[2][0]) * inv_s;
		result.z = (m.e[1][0] - m.e[0][1]) * inv_s;
	}
	else if(m.e[0][0] > m.e[1][1] && m.e[0][0] > m.e[2][2])
	{
		F32 s = sqrtf(1.f + m.e[0][0] - m.e[1][1] - m.e[2][2]) * 2.f;
		F32 inv_s = 1.f / s;
		result.w = (m.e[2][1] - m.e[1][2]) * inv_s;
		result.x = 0.25f * s;
		result.y = (m.e[0][1] + m.e[1][0]) * inv_s;
		result.z = (m.e[0][2] + m.e[2][0]) * inv_s;
	}
	else if(m.e[1][1] > m.e[2][2])
	{
		F32 s = sqrtf(1.f + m.e[1][1] - m.e[0][0] - m.e[2][2]) * 2.f;
		F32 inv_s = 1.f / s;
		result.w = (m.e[0][2] - m.e[2][0]) * inv_s;
		result.x = (m.e[0][1] + m.e[1][0]) * inv_s;
		result.y = 0.25f * s;
		result.z = (m.e[1][2] + m.e[2][1]) * inv_s;
	}
	else
	{
		F32 s = sqrtf(1.f + m.e[2][2] - m.e[0][0] - m.e[1][1]) * 2.f;
		F32 inv_s = 1.f / s;
		result.w = (m.e[1][0] - m.e[0][1]) * inv_s;
		result.x = (m.e[0][2] + m.e[2][0]) * inv_s;
		result.y = (m.e[1][2] + m.e[2][1]) * inv_s;
		result.z = 0.25f * s;
	}
	
	return result;
}

internal Mat4x4F32 Quat4F32_ToMat4x4(Quat4F32 q)
{
	q = Quat4F32_Normalize(q);
	
	F32 x2 = q.x*q.x, y2 = q.y*q.y, z2 = q.z*q.z;
	F32 xy = q.x*q.y, xz = q.x*q.z, yz = q.y*q.z;
	F32 wx = q.w*q.x, wy = q.w*q.y, wz = q.w*q.z;
	
	Mat4x4F32 result = {
		.e = {
			{1.f - 2.f*(y2 + z2), 2.f*(xy + wz), 2.f*(xz - wy), 0.f},
			{2.f*(xy - wz), 1.f - 2.f*(x2 + z2), 2.f*(yz + wx), 0.f},
			{2.f*(xz + wy), 2.f*(yz - wx), 1.f - 2.f*(x2 + y2), 0.f},
			{0.f, 0.f, 0.f, 1.f},
		},
	};
	return result;
}

internal Vec3F32 Quat4F32_RotateVec3(Quat4F32 q, Vec3F32 v)
{
	// v' = q * v * q^-1
	Quat4F32 v_quat = {v.x, v.y, v.z, 0.f};
	Quat4F32 q_inv = Quat4F32_Inverse(q);
	Quat4F32 result_quat = Quat4F32_Mul(Quat4F32_Mul(q, v_quat), q_inv);
	return (Vec3F32){result_quat.x, result_quat.y, result_quat.z};
}

internal Quat4F32 Quat4F32_Slerp(Quat4F32 a, Quat4F32 b, F32 t)
{
	a = Quat4F32_Normalize(a);
	b = Quat4F32_Normalize(b);
	
	F32 dot = Quat4F32_Dot(a, b);
	
	// clamp dot to avoid numerical issues with acos
	dot = (dot < -1.f) ? -1.f : ((dot > 1.f) ? 1.f : dot);
	
	// if dot < 0, negate b to take the shorter path
	if(dot < 0.f)
	{
		b = Quat4F32_Scale(b, -1.f);
		dot = -dot;
	}
	
	F32 theta = acosf(dot);
	F32 sin_theta = sinf(theta);
	
	if(sin_theta < F32_EPSILON)
	{
		// quaternions are nearly parallel, use linear interpolation
		return Quat4F32_Normalize(Quat4F32_Add(
			Quat4F32_Scale(a, 1.f - t),
			Quat4F32_Scale(b, t)
		));
	}
	
	F32 inv_sin = 1.f / sin_theta;
	F32 w_a = sinf((1.f - t) * theta) * inv_sin;
	F32 w_b = sinf(t * theta) * inv_sin;
	
	return Quat4F32_Normalize(Quat4F32_Add(
		Quat4F32_Scale(a, w_a),
		Quat4F32_Scale(b, w_b)
	));
}

////////////////////////////////
// Quaternion F64 variant

internal Quat4F64 Quat4F64_Make(F64 x, F64 y, F64 z, F64 w)
{
	return (Quat4F64){x, y, z, w};
}

internal Quat4F64 Quat4F64_Identity(void)
{
	return (Quat4F64){0, 0, 0, 1};
}

internal Quat4F64 Quat4F64_Add(Quat4F64 a, Quat4F64 b)
{
	return (Quat4F64){a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

internal Quat4F64 Quat4F64_Sub(Quat4F64 a, Quat4F64 b)
{
	return (Quat4F64){a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

internal Quat4F64 Quat4F64_Mul(Quat4F64 a, Quat4F64 b)
{
	Quat4F64 result;
	result.w = a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z;
	result.x = a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y;
	result.y = a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x;
	result.z = a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w;
	return result;
}

internal Quat4F64 Quat4F64_Scale(Quat4F64 q, F64 s)
{
	return (Quat4F64){q.x * s, q.y * s, q.z * s, q.w * s};
}

internal Quat4F64 Quat4F64_Conjugate(Quat4F64 q)
{
	return (Quat4F64){-q.x, -q.y, -q.z, q.w};
}

internal F64 Quat4F64_LengthSquared(Quat4F64 q)
{
	return q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w;
}

internal F64 Quat4F64_Length(Quat4F64 q)
{
	return sqrt(Quat4F64_LengthSquared(q));
}

internal F64 Quat4F64_Dot(Quat4F64 a, Quat4F64 b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

internal Quat4F64 Quat4F64_Normalize(Quat4F64 q)
{
	F64 len = Quat4F64_Length(q);
	if(len < F64_EPSILON)
	{
		return Quat4F64_Identity();
	}
	F64 inv_len = 1.0 / len;
	return Quat4F64_Scale(q, inv_len);
}

internal Quat4F64 Quat4F64_Inverse(Quat4F64 q)
{
	F64 len_sq = Quat4F64_LengthSquared(q);
	if(len_sq < F64_EPSILON)
	{
		return Quat4F64_Identity();
	}
	Quat4F64 conj = Quat4F64_Conjugate(q);
	return Quat4F64_Scale(conj, 1.0 / len_sq);
}

internal Quat4F64 Quat4F64_FromAxisAngle(Vec3F64 axis, F64 radians)
{
	F64 len = sqrt(axis.x*axis.x + axis.y*axis.y + axis.z*axis.z);
	F64 inv_len = (len != 0.0) ? (1.0 / len) : 0.0;
	
	F64 half_angle = radians * 0.5;
	F64 sin_half = sin(half_angle);
	F64 cos_half = cos(half_angle);
	
	Quat4F64 result;
	result.x = axis.x * inv_len * sin_half;
	result.y = axis.y * inv_len * sin_half;
	result.z = axis.z * inv_len * sin_half;
	result.w = cos_half;
	return result;
}

internal void Quat4F64_ToAxisAngle(Quat4F64 q, Vec3F64 *out_axis, F64 *out_radians)
{
	q = Quat4F64_Normalize(q);
	
	F64 sin_half_sq = q.x*q.x + q.y*q.y + q.z*q.z;
	F64 sin_half = sqrt(sin_half_sq);
	
	if(sin_half < F64_EPSILON)
	{
		out_axis->x = 1.0;
		out_axis->y = 0.0;
		out_axis->z = 0.0;
		*out_radians = 0.0;
	}
	else
	{
		F64 inv_sin = 1.0 / sin_half;
		out_axis->x = q.x * inv_sin;
		out_axis->y = q.y * inv_sin;
		out_axis->z = q.z * inv_sin;
		*out_radians = 2.0 * atan2(sin_half, q.w);
	}
}

internal Quat4F64 Quat4F64_FromEuler(Vec3F64 euler)
{
	F64 roll = euler.x * 0.5;
	F64 pitch = euler.y * 0.5;
	F64 yaw = euler.z * 0.5;
	
	F64 sr = sin(roll), cr = cos(roll);
	F64 sp = sin(pitch), cp = cos(pitch);
	F64 sy = sin(yaw), cy = cos(yaw);
	
	Quat4F64 result;
	result.x = sr*cp*cy - cr*sp*sy;
	result.y = cr*sp*cy + sr*cp*sy;
	result.z = cr*cp*sy - sr*sp*cy;
	result.w = cr*cp*cy + sr*sp*sy;
	return result;
}

internal Vec3F64 Quat4F64_ToEuler(Quat4F64 q)
{
	q = Quat4F64_Normalize(q);
	
	F64 x2 = q.x*q.x, y2 = q.y*q.y, z2 = q.z*q.z;
	F64 xy = q.x*q.y, xz = q.x*q.z, yz = q.y*q.z;
	F64 wx = q.w*q.x, wy = q.w*q.y, wz = q.w*q.z;
	
	F64 sin_pitch = 2.0 * (wy - xz);
	sin_pitch = (sin_pitch < -1.0) ? -1.0 : ((sin_pitch > 1.0) ? 1.0 : sin_pitch);
	
	Vec3F64 result;
	result.x = atan2(2.0 * (wz + xy), 1.0 - 2.0 * (x2 + y2));
	result.y = asin(sin_pitch);
	result.z = atan2(2.0 * (wx + yz), 1.0 - 2.0 * (y2 + z2));
	return result;
}

internal Quat4F64 Quat4F64_FromMat4x4(Mat4x4F64 m)
{
	Quat4F64 result;
	
	F64 trace = m.e[0][0] + m.e[1][1] + m.e[2][2];
	
	if(trace > 0.0)
	{
		F64 s = sqrt(trace + 1.0) * 2.0;
		F64 inv_s = 1.0 / s;
		result.w = 0.25 * s;
		result.x = (m.e[2][1] - m.e[1][2]) * inv_s;
		result.y = (m.e[0][2] - m.e[2][0]) * inv_s;
		result.z = (m.e[1][0] - m.e[0][1]) * inv_s;
	}
	else if(m.e[0][0] > m.e[1][1] && m.e[0][0] > m.e[2][2])
	{
		F64 s = sqrt(1.0 + m.e[0][0] - m.e[1][1] - m.e[2][2]) * 2.0;
		F64 inv_s = 1.0 / s;
		result.w = (m.e[2][1] - m.e[1][2]) * inv_s;
		result.x = 0.25 * s;
		result.y = (m.e[0][1] + m.e[1][0]) * inv_s;
		result.z = (m.e[0][2] + m.e[2][0]) * inv_s;
	}
	else if(m.e[1][1] > m.e[2][2])
	{
		F64 s = sqrt(1.0 + m.e[1][1] - m.e[0][0] - m.e[2][2]) * 2.0;
		F64 inv_s = 1.0 / s;
		result.w = (m.e[0][2] - m.e[2][0]) * inv_s;
		result.x = (m.e[0][1] + m.e[1][0]) * inv_s;
		result.y = 0.25 * s;
		result.z = (m.e[1][2] + m.e[2][1]) * inv_s;
	}
	else
	{
		F64 s = sqrt(1.0 + m.e[2][2] - m.e[0][0] - m.e[1][1]) * 2.0;
		F64 inv_s = 1.0 / s;
		result.w = (m.e[1][0] - m.e[0][1]) * inv_s;
		result.x = (m.e[0][2] + m.e[2][0]) * inv_s;
		result.y = (m.e[1][2] + m.e[2][1]) * inv_s;
		result.z = 0.25 * s;
	}
	
	return result;
}

internal Mat4x4F64 Quat4F64_ToMat4x4(Quat4F64 q)
{
	q = Quat4F64_Normalize(q);
	
	F64 x2 = q.x*q.x, y2 = q.y*q.y, z2 = q.z*q.z;
	F64 xy = q.x*q.y, xz = q.x*q.z, yz = q.y*q.z;
	F64 wx = q.w*q.x, wy = q.w*q.y, wz = q.w*q.z;
	
	Mat4x4F64 result = {
		.e = {
			{1.0 - 2.0*(y2 + z2), 2.0*(xy + wz), 2.0*(xz - wy), 0.0},
			{2.0*(xy - wz), 1.0 - 2.0*(x2 + z2), 2.0*(yz + wx), 0.0},
			{2.0*(xz + wy), 2.0*(yz - wx), 1.0 - 2.0*(x2 + y2), 0.0},
			{0.0, 0.0, 0.0, 1.0},
		},
	};
	return result;
}

internal Vec3F64 Quat4F64_RotateVec3(Quat4F64 q, Vec3F64 v)
{
	Quat4F64 v_quat = {v.x, v.y, v.z, 0.0};
	Quat4F64 q_inv = Quat4F64_Inverse(q);
	Quat4F64 result_quat = Quat4F64_Mul(Quat4F64_Mul(q, v_quat), q_inv);
	return (Vec3F64){result_quat.x, result_quat.y, result_quat.z};
}

internal Quat4F64 Quat4F64_Slerp(Quat4F64 a, Quat4F64 b, F64 t)
{
	a = Quat4F64_Normalize(a);
	b = Quat4F64_Normalize(b);
	
	F64 dot = Quat4F64_Dot(a, b);
	
	dot = (dot < -1.0) ? -1.0 : ((dot > 1.0) ? 1.0 : dot);
	
	if(dot < 0.0)
	{
		b = Quat4F64_Scale(b, -1.0);
		dot = -dot;
	}
	
	F64 theta = acos(dot);
	F64 sin_theta = sin(theta);
	
	if(sin_theta < F64_EPSILON)
	{
		return Quat4F64_Normalize(Quat4F64_Add(
			Quat4F64_Scale(a, 1.0 - t),
			Quat4F64_Scale(b, t)
		));
	}
	
	F64 inv_sin = 1.0 / sin_theta;
	F64 w_a = sin((1.0 - t) * theta) * inv_sin;
	F64 w_b = sin(t * theta) * inv_sin;
	
	return Quat4F64_Normalize(Quat4F64_Add(
		Quat4F64_Scale(a, w_a),
		Quat4F64_Scale(b, w_b)
	));
}
