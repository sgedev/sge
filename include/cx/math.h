/*
  Based on HandmadeMath.h v1.9.0

  =============================================================================

  LICENSE
  
  This software is in the public domain. Where that dedication is not
  recognized, you are granted a perpetual, irrevocable license to copy,
  distribute, and modify this file as you see fit.
  
  CREDITS
  
  Written by Zakary Strange (zak@strangedev.net && @strangezak)
  
  Functionality:
   Matt Mascarenhas (@miblo_)
   Aleph
   FieryDrake (@fierydrake)
   Gingerbill (@TheGingerBill)
   Ben Visness (@bvisness) 
   Trinton Bullard (@Peliex_Dev)
   @AntonDan
   
  Fixes:
   Jeroen van Rijn (@J_vanRijn)
   Kiljacken (@Kiljacken)
   Insofaras (@insofaras)
   Daniel Gibson (@DanielGibson)
*/


/* let's figure out if SSE is really available (unless disabled anyway)
   (it isn't on non-x86/x86_64 platforms or even x86 without explicit SSE support)
   => only use "#ifdef CX_MATH_USE_SSE" to check for SSE support below this block! */
#ifndef CX_MATH_NO_SSE

# ifdef _MSC_VER
   /* MSVC supports SSE in amd64 mode or _M_IX86_FP >= 1 (2 means SSE2) */
#  if defined(_M_AMD64) || ( defined(_M_IX86_FP) && _M_IX86_FP >= 1 )
#   define CX_MATH_USE_SSE 1
#  endif
# else /* not MSVC, probably GCC, clang, icc or something that doesn't support SSE anyway */
#  ifdef __SSE__ /* they #define __SSE__ if it's supported */
#   define CX_MATH_USE_SSE 1
#  endif /*  __SSE__ */
# endif /* not _MSC_VER */

#endif /* #ifndef CX_MATH_NO_SSE */

#include <stdint.h> // This is for types

#ifdef CX_MATH_USE_SSE
#include <xmmintrin.h>
#endif

#ifndef CX_MATH_H
#define CX_MATH_H

#ifdef _MSC_VER
#pragma warning(disable:4201)
#endif

#ifdef __clang__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wgnu-anonymous-struct"
#endif

#include <cx/common.h>

CX_BEGIN_C_DECLS

#if !defined(CX_SINF) || !defined(CX_COSF) || !defined(CX_TANF) || \
    !defined(CX_SQRTF) || !defined(CX_EXPF) || !defined(CX_LOGF) || \
    !defined(CX_ACOSF) || !defined(CX_ATANF)|| !defined(CX_ATAN2F)
#include <math.h>    
#endif
    
#ifndef CX_SINF
#define CX_SINF sinf
#endif    
        
#ifndef CX_COSF
#define CX_COSF cosf
#endif    
        
#ifndef CX_TANF
#define CX_TANF tanf
#endif        

#ifndef CX_SQRTF
#define CX_SQRTF sqrtf
#endif    
    
#ifndef CX_EXPF
#define CX_EXPF expf
#endif

#ifndef CX_LOGF
#define CX_LOGF logf
#endif

#ifndef CX_ACOSF
#define CX_ACOSF acosf
#endif

#ifndef CX_ATANF
#define CX_ATANF atanf
#endif

#ifndef CX_ATAN2F
#define CX_ATAN2F atan2f
#endif

#define CX_PI32 3.14159265359f
#define CX_PI 3.14159265358979323846

#define CX_MIN(a, b) (a) > (b) ? (b) : (a)
#define CX_MAX(a, b) (a) < (b) ? (b) : (a)
#define CX_ABS(a) ((a) > 0 ? (a) : -(a))
#define CX_MOD(a, m) ((a) % (m)) >= 0 ? ((a) % (m)) : (((a) % (m)) + (m))
#define CX_SQUARE(x) ((x) * (x))

typedef union {
    struct { float x, y; };
    struct { float u, v; };
    struct { float left, right; };
    struct { float width, height; };

    float elements[2];
} cx_vec2_t;

typedef union {
    struct { float x, y, z; };
    struct { float u, v, w; };
    struct { float r, g, b; };

    struct {
        cx_vec2_t xy;
        float ignored0_;
    };

    struct {
        float ignored1_;
        cx_vec2_t yz;
    };

    struct  {
        cx_vec2_t uv;
        float ignored2_;
    };

    struct {
        float ignored3_;
        cx_vec2_t vw;
    };

    float elements[3];
} cx_vec3_t;

typedef union {
    struct {
        union {
            cx_vec3_t xyz;
            struct { float x, y, z; };
        };

        float w;
    };

    struct {
        union {
            cx_vec3_t rgb;
            struct { float r, g, b; };
        };

        float a;
    };

    struct {
        cx_vec2_t xy;
        float ignored0_;
        float ignored1_;
    };

    struct {
        float ignored2_;
        cx_vec2_t yz;
        float ignored3_;
    };

    struct {
        float ignored4_;
        float ignored5_;
        cx_vec2_t ZW;
    };

    float elements[4];
    
#ifdef CX_MATH_USE_SSE    
    __m128 elements_SSE;
#endif
} cx_vec4_t;

typedef union {
    float elements[4][4];
        
#ifdef CX_MATH_USE_SSE
    __m128 columns[4];

    // DEPRECATED. Our matrices are column-major, so this was named
    // incorrectly. Use Columns instead.
    __m128 rows[4];
#endif
} cx_mat4_t;

typedef union {
    struct {
        union {
            cx_vec3_t xyz;
            struct { float x, y, z; };
        };
        
        float w;
    };
    
    float elements[4];

#ifdef CX_MATH_USE_SSE    
    __m128 elements_SSE;
#endif
} cx_quat_t;

/*
 * Floating-point math functions
 */

static CX_INLINE float cx_sinf(float x)
{
    return CX_SINF(x);
}

static CX_INLINE float cx_cosf(float x)
{
    return CX_COSF(x);
}

static CX_INLINE float cx_tanf(float x)
{
    return CX_TANF(x);
}

static CX_INLINE float cx_acosf(float x)
{
    return CX_ACOSF(x);
}

static CX_INLINE float cx_atanf(float x)
{
    return CX_ATANF(x);
}

static CX_INLINE float cx_atan2f(float left, float right)
{
    return CX_ATAN2F(left, right);
}

static CX_INLINE float cx_expf(float x)
{
    return CX_EXPF(x);
}

static CX_INLINE float cx_logf(float x)
{
    return CX_LOGF(x);
}

static CX_INLINE float cx_sqrtf(float x)
{
    float r;

#ifdef CX_MATH_USE_SSE
    __m128 In = _mm_set_ss(x);
    __m128 Out = _mm_sqrt_ss(In);
    r = _mm_cvtss_f32(Out);
#else
    r = CX_SQRTF(Float);
#endif 

    return(r);
}

static CX_INLINE float cx_rsqrtf(float x)
{
    float r;

#ifdef CX_MATH_USE_SSE
    __m128 In = _mm_set_ss(x);
    __m128 Out = _mm_rsqrt_ss(In);
    r = _mm_cvtss_f32(Out);
#else
    r = 1.0f / cx_sqrtf(Float);
#endif

    return(r);
}

float cx_power(float base, int exponent);

static CX_INLINE float cx_powerf(float base, float exponent)
{
    return CX_EXPF(exponent * CX_LOGF(base));
}

/*
 * Utility functions
 */

static CX_INLINE float cx_radians(float x)
{
    return x * (CX_PI32 / 180.0f);
}

static CX_INLINE float cx_lerp(float a, float time, float b)
{
    return (1.0f - time) * a + time * b;
}

static CX_INLINE float cx_clamp(float min, float value, float max)
{
    if (value < min)
        return min;

    if (value > max)
        return max;

    return value;
}

/*
 * Vector initialization
 */

static CX_INLINE cx_vec2_t cx_vec2(float x, float y)
{
    cx_vec2_t r;

    r.x = x;
    r.y = y;

    return (r);
}

static CX_INLINE cx_vec2_t cx_vec2i(int x, int y)
{
    cx_vec2_t r;

    r.x = (float)x;
    r.y = (float)y;

    return (r);
}

static CX_INLINE cx_vec3_t cx_vec3(float x, float y, float z)
{
    cx_vec3_t r;

    r.x = x;
    r.y = y;
    r.z = z;

    return (r);
}

static CX_INLINE cx_vec3_t cx_vec3i(int x, int y, int z)
{
    cx_vec3_t r;

    r.x = (float)x;
    r.y = (float)y;
    r.z = (float)z;

    return (r);
}

static CX_INLINE cx_vec4_t cx_vec4(float x, float y, float z, float w)
{
    cx_vec4_t r;

#ifdef CX_MATH_USE_SSE
    r.elements_SSE = _mm_setr_ps(x, y, z, w);
#else
    r.x = x;
    r.y = y;
    r.z = z;
    r.w = w;
#endif

    return (r);
}

static CX_INLINE cx_vec4_t cx_vec4i(int x, int y, int z, int w)
{
    cx_vec4_t r;

#ifdef CX_MATH_USE_SSE
    r.elements_SSE = _mm_setr_ps((float)x, (float)y, (float)z, (float)w);
#else
    r.x = (float)x;
    r.y = (float)y;
    r.z = (float)z;
    r.w = (float)w;
#endif

    return (r);
}

static CX_INLINE cx_vec4_t cx_vec4v(cx_vec3_t v, float w)
{
    cx_vec4_t r;
    
#ifdef CX_MATH_USE_SSE
    r.elements_SSE = _mm_setr_ps(v.x, v.y, v.z, w);
#else
    r.xyz = v;
    r.w = w;
#endif
    
    return (r);
}

/*
 * Binary vector operations
 */

static CX_INLINE cx_vec2_t cx_vec2_add(cx_vec2_t left, cx_vec2_t right)
{
    cx_vec2_t r;

    r.x = left.x + right.x;
    r.y = left.y + right.y;

    return (r);
}

static CX_INLINE cx_vec3_t cx_vec3_add(cx_vec3_t left, cx_vec3_t right)
{
    cx_vec3_t r;

    r.x = left.x + right.x;
    r.y = left.y + right.y;
    r.z = left.z + right.z;

    return (r);
}

static CX_INLINE cx_vec4_t cx_vec4_add(cx_vec4_t left, cx_vec4_t right)
{
    cx_vec4_t r;

#ifdef CX_MATH_USE_SSE
    r.elements_SSE = _mm_add_ps(left.elements_SSE, right.elements_SSE);
#else    
    r.x = left.x + right.x;
    r.y = left.y + right.y;
    r.z = left.z + right.z;
    r.w = left.w + right.w;    
#endif

    return (r);
}

static CX_INLINE cx_vec2_t cx_vec2_sub(cx_vec2_t left, cx_vec2_t right)
{
    cx_vec2_t r;

    r.x = left.x - right.x;
    r.y = left.y - right.y;

    return (r);
}

static CX_INLINE cx_vec3_t cx_vec3_sub(cx_vec3_t left, cx_vec3_t right)
{
    cx_vec3_t r;

    r.x = left.x - right.x;
    r.y = left.y - right.y;
    r.z = left.z - right.z;

    return (r);
}

static CX_INLINE cx_vec4_t cx_vec4_sub(cx_vec4_t left, cx_vec4_t right)
{
    cx_vec4_t r;
    
#ifdef CX_MATH_USE_SSE
    r.elements_SSE = _mm_sub_ps(left.elements_SSE, right.elements_SSE);
#else    
    r.x = left.x - right.x;
    r.y = left.y - right.y;
    r.z = left.z - right.z;
    r.w = left.w - right.w;    
#endif

    return (r);
}

static CX_INLINE cx_vec2_t cx_vec2_mul(cx_vec2_t left, cx_vec2_t right)
{
    cx_vec2_t r;

    r.x = left.x * right.x;
    r.y = left.y * right.y;

    return (r);
}

static CX_INLINE cx_vec2_t cx_vec2_mulf(cx_vec2_t left, float right)
{
    cx_vec2_t r;

    r.x = left.x * right;
    r.y = left.y * right;

    return (r);
}

static CX_INLINE cx_vec3_t cx_vec3_mul(cx_vec3_t left, cx_vec3_t right)
{
    cx_vec3_t r;

    r.x = left.x * right.x;
    r.y = left.y * right.y;
    r.z = left.z * right.z;

    return (r);
}

static CX_INLINE cx_vec3_t cx_vec3_mulf(cx_vec3_t left, float right)
{
    cx_vec3_t r;

    r.x = left.x * right;
    r.y = left.y * right;
    r.z = left.z * right;

    return (r);
}

static CX_INLINE cx_vec4_t cx_vec4_mul(cx_vec4_t left, cx_vec4_t right)
{
    cx_vec4_t r;

#ifdef CX_MATH_USE_SSE
    r.elements_SSE = _mm_mul_ps(left.elements_SSE, right.elements_SSE);
#else
    r.x = left.x * right.x;
    r.y = left.y * right.y;
    r.z = left.z * right.z;
    r.w = left.w * right.w;    
#endif

    return (r);
}

static CX_INLINE cx_vec4_t cx_vec4_mulf(cx_vec4_t left, float right)
{
    cx_vec4_t r;

#ifdef CX_MATH_USE_SSE
    __m128 Scalar = _mm_set1_ps(right);
    r.elements_SSE = _mm_mul_ps(left.elements_SSE, Scalar);
#else    
    r.x = left.x * right;
    r.y = left.y * right;
    r.z = left.z * right;
    r.w = left.w * right;
#endif
    
    return (r);
}

static CX_INLINE cx_vec2_t cx_vec2_div(cx_vec2_t left, cx_vec2_t right)
{
    cx_vec2_t r;

    r.x = left.x / right.x;
    r.y = left.y / right.y;

    return (r);
}

static CX_INLINE cx_vec2_t cx_vec2_divf(cx_vec2_t left, float right)
{
    cx_vec2_t r;

    r.x = left.x / right;
    r.y = left.y / right;

    return (r);
}

static CX_INLINE cx_vec3_t cx_vec3_div(cx_vec3_t left, cx_vec3_t right)
{
    cx_vec3_t r;

    r.x = left.x / right.x;
    r.y = left.y / right.y;
    r.z = left.z / right.z;

    return (r);
}

static CX_INLINE cx_vec3_t cx_vec3_divf(cx_vec3_t left, float right)
{
    cx_vec3_t r;

    r.x = left.x / right;
    r.y = left.y / right;
    r.z = left.z / right;

    return (r);
}

static CX_INLINE cx_vec4_t cx_vec4_div(cx_vec4_t left, cx_vec4_t right)
{
    cx_vec4_t r;
    
#ifdef CX_MATH_USE_SSE
    r.elements_SSE = _mm_div_ps(left.elements_SSE, right.elements_SSE);
#else
    r.x = left.x / right.x;
    r.y = left.y / right.y;
    r.z = left.z / right.z;
    r.w = left.w / right.w;
#endif
    
    return (r);
}

static CX_INLINE cx_vec4_t cx_vec4_divf(cx_vec4_t left, float right)
{
    cx_vec4_t r;

#ifdef CX_MATH_USE_SSE
    __m128 Scalar = _mm_set1_ps(right);
    r.elements_SSE = _mm_div_ps(left.elements_SSE, Scalar);
#else    
    r.x = left.x / right;
    r.y = left.y / right;
    r.z = left.z / right;
    r.w = left.w / right;
#endif
    
    return (r);
}

static CX_INLINE bool cx_vec2_eq(cx_vec2_t left, cx_vec2_t right)
{
    bool r = (left.x == right.x && left.y == right.y);

    return (r);
}

static CX_INLINE bool cx_vec3_eq(cx_vec3_t left, cx_vec3_t right)
{
    bool r = (left.x == right.x && left.y == right.y && left.z == right.z);

    return (r);
}

static CX_INLINE bool cx_vec4_eq(cx_vec4_t left, cx_vec4_t right)
{
    bool r = (left.x == right.x && left.y == right.y && left.z == right.z && left.w == right.w);

    return (r);
}

static CX_INLINE float cx_vec2_dot(cx_vec2_t a, cx_vec2_t b)
{
    float r = (a.x * b.x) + (a.y * b.y);
    
    return (r);
}

static CX_INLINE float cx_vec3_dot(cx_vec3_t a, cx_vec3_t b)
{
    float r = (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
    
    return (r);
}

static CX_INLINE float cx_vec4_dot(cx_vec4_t a, cx_vec4_t b)
{
    float r;

    // NOTE(zak): IN the future if we wanna check what version SSE is support 
    // we can use _mm_dp_ps (4.3) but for now we will use the old way. 
    // Or a r = _mm_mul_ps(v1, v2), r = _mm_hadd_ps(r, r), r = _mm_hadd_ps(r, r) for SSE3
#ifdef CX_MATH_USE_SSE
    __m128 SSErOne = _mm_mul_ps(a.elements_SSE, b.elements_SSE);
    __m128 SSErTwo = _mm_shuffle_ps(SSErOne, SSErOne, _MM_SHUFFLE(2, 3, 0, 1));
    SSErOne = _mm_add_ps(SSErOne, SSErTwo);
    SSErTwo = _mm_shuffle_ps(SSErOne, SSErOne, _MM_SHUFFLE(0, 1, 2, 3));
    SSErOne = _mm_add_ps(SSErOne, SSErTwo);       
    _mm_store_ss(&r, SSErOne);
#else
    r = (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
#endif
    
    return (r);
}

static CX_INLINE cx_vec3_t cx_cross(cx_vec3_t a, cx_vec3_t b)
{
    cx_vec3_t r;

    r.x = (a.y * b.z) - (a.z * b.y);
    r.y = (a.z * b.x) - (a.x * b.z);
    r.z = (a.x * b.y) - (a.y * b.x);

    return (r);
}

/*
 * Unary vector operations
 */

static CX_INLINE float cx_vec2_lsqrt(cx_vec2_t a)
{
    return cx_vec2_dot(a, a);
}

static CX_INLINE float cx_vec3_lsqrt(cx_vec3_t a)
{
    return cx_vec3_dot(a, a);
}

static CX_INLINE float cx_vec4_lsqrt(cx_vec4_t a)
{
    return cx_vec4_dot(a, a);
}

static CX_INLINE float cx_vec2_length(cx_vec2_t a)
{
    return cx_sqrtf(cx_vec2_lsqrt(a));
}

static CX_INLINE float cx_vec3_length(cx_vec3_t a)
{
    return cx_sqrtf(cx_vec3_lsqrt(a));
}

static CX_INLINE float cx_vec4_length(cx_vec4_t a)
{
    return cx_sqrtf(cx_vec4_lsqrt(a));
}

static CX_INLINE cx_vec2_t cx_vec2_normalize(cx_vec2_t a)
{
    cx_vec2_t r = { 0 };

    float len = cx_vec2_length(a);
    
    /* NOTE(kiljacken): We need a zero check to not divide-by-zero */
    if (len != 0.0f) {
        r.x = a.x * (1.0f / len);
        r.y = a.y * (1.0f / len);
    }
    
    return (r);
}

static CX_INLINE cx_vec3_t cx_vec3_normalize(cx_vec3_t a)
{
    cx_vec3_t r = { 0 };

    float len = cx_vec3_length(a);
    
    /* NOTE(kiljacken): We need a zero check to not divide-by-zero */
    if (len != 0.0f)
    {
        r.x = a.x * (1.0f / len);
        r.y = a.y * (1.0f / len);
        r.z = a.z * (1.0f / len);
    }
    
    return (r);
}

static CX_INLINE cx_vec4_t cx_vec4_normalize(cx_vec4_t a)
{
    cx_vec4_t r = { 0 };

    float len = cx_vec4_length(a);
    
    /* NOTE(kiljacken): We need a zero check to not divide-by-zero */
    if (len != 0.0f)
    {
        float v = 1.0f / len;
        
#ifdef CX_MATH_USE_SSE
        __m128 SSEMultiplier = _mm_set1_ps(v);
        r.elements_SSE = _mm_mul_ps(a.elements_SSE, SSEMultiplier);        
#else 
        r.x = A.x * Multiplier;
        r.y = A.y * Multiplier;
        r.z = A.z * Multiplier;
        r.w = A.w * Multiplier;
#endif
    }
    
    return (r);
}

static CX_INLINE cx_vec2_t cx_vec2_normalize_fast(cx_vec2_t a)
{
    return cx_vec2_mulf(a, cx_rsqrtf(cx_vec2_dot(a, a)));
}

static CX_INLINE cx_vec3_t cx_vec3_normalize_fast(cx_vec3_t a)
{
    return cx_vec3_mulf(a, cx_rsqrtf(cx_vec3_dot(a, a)));
}

static CX_INLINE cx_vec4_t cx_vec4_normalize_fast(cx_vec4_t a)
{
    return cx_vec4_mulf(a, cx_rsqrtf(cx_vec4_dot(a, a)));
}

/*
 * SSE stuff
 */

#ifdef CX_MATH_USE_SSE
static CX_INLINE __m128 cx_linear_combine_SSE(__m128 left, cx_mat4_t right)
{
    __m128 r;

    r = _mm_mul_ps(_mm_shuffle_ps(left, left, 0x00), right.columns[0]);
    r = _mm_add_ps(r, _mm_mul_ps(_mm_shuffle_ps(left, left, 0x55), right.columns[1]));
    r = _mm_add_ps(r, _mm_mul_ps(_mm_shuffle_ps(left, left, 0xaa), right.columns[2]));
    r = _mm_add_ps(r, _mm_mul_ps(_mm_shuffle_ps(left, left, 0xff), right.columns[3]));
    
    return (r);
}
#endif

/*
 * Matrix functions
 */

static CX_INLINE cx_mat4_t cx_mat4(void)
{
    cx_mat4_t r = { 0 };

    return (r);
}

static CX_INLINE cx_mat4_t cx_mat4d(float Diagonal)
{
    cx_mat4_t r = cx_mat4();
    
    r.elements[0][0] = Diagonal;
    r.elements[1][1] = Diagonal;
    r.elements[2][2] = Diagonal;
    r.elements[3][3] = Diagonal;

    return (r);
}

#ifdef CX_MATH_USE_SSE
static CX_INLINE cx_mat4_t cx_transpose(cx_mat4_t v)
{
    cx_mat4_t r = v;
    
    _MM_TRANSPOSE4_PS(r.columns[0], r.columns[1], r.columns[2], r.columns[3]);

    return (r);
}
#else
cx_mat4_t cx_transpose(cx_mat4_t v);
#endif

#ifdef CX_MATH_USE_SSE
static CX_INLINE cx_mat4_t cx_mat4_add(cx_mat4_t left, cx_mat4_t right)
{
    cx_mat4_t r;

    r.columns[0] = _mm_add_ps(left.columns[0], right.columns[0]);
    r.columns[1] = _mm_add_ps(left.columns[1], right.columns[1]);
    r.columns[2] = _mm_add_ps(left.columns[2], right.columns[2]);
    r.columns[3] = _mm_add_ps(left.columns[3], right.columns[3]);    

    return (r);
}
#else
cx_mat4_t cx_mat4_add(cx_mat4_t left, cx_mat4_t right);
#endif

#ifdef CX_MATH_USE_SSE
static CX_INLINE cx_mat4_t cx_mat4_sub(cx_mat4_t left, cx_mat4_t right)
{
    cx_mat4_t r;

    r.columns[0] = _mm_sub_ps(left.columns[0], right.columns[0]);
    r.columns[1] = _mm_sub_ps(left.columns[1], right.columns[1]);
    r.columns[2] = _mm_sub_ps(left.columns[2], right.columns[2]);
    r.columns[3] = _mm_sub_ps(left.columns[3], right.columns[3]);

    return (r);
}
#else
cx_mat4_t cx_mat4_sub(cx_mat4_t left, cx_mat4_t right);
#endif

cx_mat4_t cx_mat4_mul(cx_mat4_t left, cx_mat4_t right);

#ifdef CX_MATH_USE_SSE
static CX_INLINE cx_mat4_t cx_mat4_mulf(cx_mat4_t m, float s)
{
    cx_mat4_t r;

    __m128 SSEScalar = _mm_set1_ps(s);
    r.columns[0] = _mm_mul_ps(m.columns[0], SSEScalar);
    r.columns[1] = _mm_mul_ps(m.columns[1], SSEScalar);
    r.columns[2] = _mm_mul_ps(m.columns[2], SSEScalar);
    r.columns[3] = _mm_mul_ps(m.columns[3], SSEScalar);

    return (r);
}
#else
cx_mat4_t cx_mat4_mulf(cx_mat4_t m, float Scalar);
#endif

cx_vec4_t cx_mat4_mulv4(cx_mat4_t m, cx_vec4_t v);

#ifdef CX_MATH_USE_SSE
static CX_INLINE cx_mat4_t cx_mat4_divf(cx_mat4_t m, float s)
{
    cx_mat4_t r;
    
    __m128 SSEScalar = _mm_set1_ps(s);
    r.columns[0] = _mm_div_ps(m.columns[0], SSEScalar);
    r.columns[1] = _mm_div_ps(m.columns[1], SSEScalar);
    r.columns[2] = _mm_div_ps(m.columns[2], SSEScalar);
    r.columns[3] = _mm_div_ps(m.columns[3], SSEScalar);    

    return (r);
}
#else
CX_EXTERN cx_mat4_t cx_mat4_divf(cx_mat4_t m, float s);
#endif

/*
 * Common graphics transformations
 */

static CX_INLINE cx_mat4_t cx_mat4_ortho(float left, float right, float bottom, float top, float znear, float zfar)
{
    cx_mat4_t r = cx_mat4();

    r.elements[0][0] = 2.0f / (right - left);
    r.elements[1][1] = 2.0f / (top - bottom);
    r.elements[2][2] = 2.0f / (znear - zfar);
    r.elements[3][3] = 1.0f;

    r.elements[3][0] = (left + right) / (left - right);
    r.elements[3][1] = (bottom + top) / (bottom - top);
    r.elements[3][2] = (zfar + znear) / (znear - zfar);

    return (r);
}

static CX_INLINE cx_mat4_t cx_mat4_perspective(float fov, float ratio, float znear, float zfar)
{
    cx_mat4_t r = cx_mat4();

    // See https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml

    float cotangent = 1.0f / cx_tanf(fov * (CX_PI32 / 360.0f));
    
    r.elements[0][0] = cotangent / ratio;
    r.elements[1][1] = cotangent;
    r.elements[2][3] = -1.0f;
    r.elements[2][2] = (znear + zfar) / (znear - zfar);
    r.elements[3][2] = (2.0f * znear * zfar) / (znear - zfar);
    r.elements[3][3] = 0.0f;

    return (r);
}

static CX_INLINE cx_mat4_t cx_mat4_translate(cx_vec3_t translation)
{
    cx_mat4_t r = cx_mat4d(1.0f);

    r.elements[3][0] = translation.x;
    r.elements[3][1] = translation.y;
    r.elements[3][2] = translation.z;

    return (r);
}

cx_mat4_t cx_mat4_rotate(float angle, cx_vec3_t axis);

static CX_INLINE cx_mat4_t cx_mat4_scale(cx_vec3_t s)
{
    cx_mat4_t r = cx_mat4d(1.0f);

    r.elements[0][0] = s.x;
    r.elements[1][1] = s.y;
    r.elements[2][2] = s.z;

    return (r);
}

cx_mat4_t cx_mat4_look_at(cx_vec3_t eye, cx_vec3_t center, cx_vec3_t up);

/*
 * Quaternion operations
 */

static CX_INLINE cx_quat_t cx_quat(float x, float y, float z, float w)
{
    cx_quat_t r;

#ifdef CX_MATH_USE_SSE
    r.elements_SSE = _mm_setr_ps(x, y, z, w);
#else
    r.x = X;
    r.y = Y;
    r.z = Z;
    r.w = W;
#endif

    return (r);
}

static CX_INLINE cx_quat_t cx_quatv4(cx_vec4_t v)
{
    cx_quat_t r;

#ifdef CX_MATH_USE_SSE
    r.elements_SSE = v.elements_SSE;
#else
    r.x = v.x;
    r.y = v.y;
    r.z = v.z;
    r.w = v.w;
#endif

    return (r);
}

static CX_INLINE cx_quat_t cx_quat_add(cx_quat_t left, cx_quat_t right)
{
    cx_quat_t r;

#ifdef CX_MATH_USE_SSE
    r.elements_SSE = _mm_add_ps(left.elements_SSE, right.elements_SSE);
#else
    r.x = left.x + right.x;
    r.y = left.y + right.y;
    r.z = left.z + right.z;
    r.w = left.w + right.w;
#endif

    return (r);
}

static CX_INLINE cx_quat_t cx_quat_sub(cx_quat_t left, cx_quat_t right)
{
    cx_quat_t r;

#ifdef CX_MATH_USE_SSE
    r.elements_SSE = _mm_sub_ps(left.elements_SSE, right.elements_SSE);
#else
    r.x = left.x - right.x;
    r.y = left.y - right.y;
    r.z = left.z - right.z;
    r.w = left.w - right.w;
#endif

    return (r);
}

static CX_INLINE cx_quat_t cx_quat_mul(cx_quat_t left, cx_quat_t right)
{
    cx_quat_t r;

#ifdef CX_MATH_USE_SSE
    __m128 SSErOne = _mm_xor_ps(_mm_shuffle_ps(left.elements_SSE, left.elements_SSE, _MM_SHUFFLE(0, 0, 0, 0)), _mm_setr_ps(0.f, -0.f, 0.f, -0.f));
    __m128 SSErTwo = _mm_shuffle_ps(right.elements_SSE, right.elements_SSE, _MM_SHUFFLE(0, 1, 2, 3));
    __m128 SSErThree = _mm_mul_ps(SSErTwo, SSErOne);

    SSErOne = _mm_xor_ps(_mm_shuffle_ps(left.elements_SSE, left.elements_SSE, _MM_SHUFFLE(1, 1, 1, 1)) , _mm_setr_ps(0.f, 0.f, -0.f, -0.f));
    SSErTwo = _mm_shuffle_ps(right.elements_SSE, right.elements_SSE, _MM_SHUFFLE(1, 0, 3, 2));
    SSErThree = _mm_add_ps(SSErThree, _mm_mul_ps(SSErTwo, SSErOne));

    SSErOne = _mm_xor_ps(_mm_shuffle_ps(left.elements_SSE, left.elements_SSE, _MM_SHUFFLE(2, 2, 2, 2)), _mm_setr_ps(-0.f, 0.f, 0.f, -0.f));
    SSErTwo = _mm_shuffle_ps(right.elements_SSE, right.elements_SSE, _MM_SHUFFLE(2, 3, 0, 1));
    SSErThree = _mm_add_ps(SSErThree, _mm_mul_ps(SSErTwo, SSErOne));

    SSErOne = _mm_shuffle_ps(left.elements_SSE, left.elements_SSE, _MM_SHUFFLE(3, 3, 3, 3));
    SSErTwo = _mm_shuffle_ps(right.elements_SSE, right.elements_SSE, _MM_SHUFFLE(3, 2, 1, 0));
    r.elements_SSE = _mm_add_ps(SSErThree, _mm_mul_ps(SSErTwo, SSErOne));
#else
    r.x = (left.x * right.w) + (left.y * right.z) - (left.z * right.y) + (left.w * right.x);
    r.y = (-left.x * right.z) + (left.y * right.w) + (left.z * right.x) + (left.w * right.y);
    r.z = (left.x * right.y) - (left.y * right.x) + (left.z * right.w) + (left.w * right.z);
    r.w = (-left.x * right.x) - (left.y * right.y) - (left.z * right.z) + (left.w * right.w);
#endif

    return (r);
}

static CX_INLINE cx_quat_t cx_quat_mulf(cx_quat_t left, float s)
{
    cx_quat_t r;

#ifdef CX_MATH_USE_SSE
    __m128 Scalar = _mm_set1_ps(s);
    r.elements_SSE = _mm_mul_ps(left.elements_SSE, Scalar);
#else
    r.x = left.x * Multiplicative;
    r.y = left.y * Multiplicative;
    r.z = left.z * Multiplicative;
    r.w = left.w * Multiplicative;
#endif

    return (r);
}

static CX_INLINE cx_quat_t cx_quat_divf(cx_quat_t left, float s)
{
    cx_quat_t r;

#ifdef CX_MATH_USE_SSE
    __m128 Scalar = _mm_set1_ps(s);
    r.elements_SSE = _mm_div_ps(left.elements_SSE, Scalar);
#else
    r.x = left.x / Dividend;
    r.y = left.y / Dividend;
    r.z = left.z / Dividend;
    r.w = left.w / Dividend;
#endif

    return (r);
}

cx_quat_t cx_quat_inv(cx_quat_t left);

static CX_INLINE float cx_quat_dot(cx_quat_t left, cx_quat_t right)
{
    float r;

#ifdef CX_MATH_USE_SSE
    __m128 SSErOne = _mm_mul_ps(left.elements_SSE, right.elements_SSE);
    __m128 SSErTwo = _mm_shuffle_ps(SSErOne, SSErOne, _MM_SHUFFLE(2, 3, 0, 1));
    SSErOne = _mm_add_ps(SSErOne, SSErTwo);
    SSErTwo = _mm_shuffle_ps(SSErOne, SSErOne, _MM_SHUFFLE(0, 1, 2, 3));
    SSErOne = _mm_add_ps(SSErOne, SSErTwo);
    _mm_store_ss(&r, SSErOne);
#else
    r = (left.x * right.x) + (left.y * right.y) + (left.z * right.z) + (left.w * right.w);
#endif

    return (r);
}

static CX_INLINE cx_quat_t cx_quat_normalize(cx_quat_t left)
{
    cx_quat_t r;

    float Length = cx_sqrtf(cx_quat_dot(left, left));
    r = cx_quat_divf(left, Length);

    return (r);
}

static CX_INLINE cx_quat_t cx_quat_nlerp(cx_quat_t left, float Time, cx_quat_t right)
{
    cx_quat_t r;

#ifdef CX_MATH_USE_SSE
    __m128 Scalarleft = _mm_set1_ps(1.0f - Time);
    __m128 Scalarright = _mm_set1_ps(Time);
    __m128 SSErOne = _mm_mul_ps(left.elements_SSE, Scalarleft);
    __m128 SSErTwo = _mm_mul_ps(right.elements_SSE, Scalarright);
    r.elements_SSE = _mm_add_ps(SSErOne, SSErTwo);
#else
    r.x = HMM_Lerp(left.x, Time, right.x);
    r.y = HMM_Lerp(left.y, Time, right.y);
    r.z = HMM_Lerp(left.z, Time, right.z);
    r.w = HMM_Lerp(left.w, Time, right.w);
#endif
    r = cx_quat_normalize(r);

    return (r);
}

cx_quat_t cx_quat_clerp(cx_quat_t left, float Time, cx_quat_t right);
cx_mat4_t cx_quat_to_mat4(cx_quat_t left);
cx_quat_t cx_quat_from_axis_angle(cx_vec3_t Axis, float AngleOfRotation);

#ifdef __clang__
#pragma GCC diagnostic pop
#endif

CX_END_C_DECLS

#endif /* CX_MATH_H */
