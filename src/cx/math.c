/*
  HandmadeMath.h v1.9.0
  
  This is a single header file with a bunch of useful functions for game and
  graphics math operations.
  
  =============================================================================
  
  You MUST
  
     #define HANDMADE_MATH_IMPLEMENTATION
     
  in EXACTLY one C or C++ file that includes this header, BEFORE the
  include, like this:
  
     #define HANDMADE_MATH_IMPLEMENTATION
     #include "HandmadeMath.h"
     
  All other files should just #include "HandmadeMath.h" without the #define.
  
  =============================================================================
  
  To disable SSE intrinsics, you MUST
  
  #define HANDMADE_MATH_NO_SSE
  
  in EXACTLY one C or C++ file that includes this header, BEFORE the
  include, like this:
     
     #define HANDMADE_MATH_IMPLEMENTATION
     #define HANDMADE_MATH_NO_SSE
     #include "HandmadeMath.h"
  
  =============================================================================
  
  To use HandmadeMath without the CRT, you MUST 
  
     #define HMM_SINF MySinF
     #define HMM_COSF MyCosF
     #define HMM_TANF MyTanF
     #define HMM_SQRTF MySqrtF
     #define HMM_EXPF MyExpF
     #define HMM_LOGF MyLogF
     #define HMM_ACOSF MyACosF
     #define HMM_ATANF MyATanF
     #define HMM_ATAN2F MYATan2F
     
  Provide your own implementations of SinF, CosF, TanF, ACosF, ATanF, ATan2F, 
  ExpF, and LogF in EXACTLY one C or C++ file that includes this header,
  BEFORE the include, like this:     
  
     #define HMM_SINF MySinF
     #define HMM_COSF MyCosF
     #define HMM_TANF MyTanF
     #define HMM_SQRTF MySqrtF
     #define HMM_EXPF MyExpF
     #define HMM_LOGF MyLogF
     #define HMM_ACOSF MyACosF
     #define HMM_ATANF MyATanF
     #define HMM_ATAN2F MyATan2F
     #define HANDMADE_MATH_IMPLEMENTATION
     #include "HandmadeMath.h"
     
  If you do not define all of these, HandmadeMath.h will use the
  versions of these functions that are provided by the CRT.
  
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
#include <cx/math.h>

float cx_power(float Base, int Exponent)
{
    float Result = 1.0f;
    float Mul = Exponent < 0 ? 1.f / Base : Base;
    unsigned int x = Exponent < 0 ? -Exponent : Exponent;
    while (x)
    {
        if (x & 1)
        {
            Result *= Mul;
        }
        
        Mul *= Mul;
        x >>= 1;
    }
    
    return (Result);
}

#ifndef CX_MATH_USE_SSE
cx_mat4_t cx_transpose(cx_mat4_t Matrix)
{
    cx_mat4_t Result;

    int Columns;
    for(Columns = 0; Columns < 4; ++Columns)
    {
        int Rows;
        for(Rows = 0; Rows < 4; ++Rows)
        {
            Result.Elements[Rows][Columns] = Matrix.Elements[Columns][Rows];
        }
    }

    return (Result);
}
#endif

#ifndef CX_MATH_USE_SSE
cx_mat4_t cx_mat4_add(cx_mat4_t Left, cx_mat4_t Right)
{
    cx_mat4_t Result;

    int Columns;
    for(Columns = 0; Columns < 4; ++Columns)
    {
        int Rows;
        for(Rows = 0; Rows < 4; ++Rows)
        {
            Result.Elements[Columns][Rows] = Left.Elements[Columns][Rows] + Right.Elements[Columns][Rows];
        }
    }

    return (Result);
}
#endif

#ifndef CX_MATH_USE_SSE
cx_mat4_t cx_mat4_sub(cx_mat4_t Left, cx_mat4_t Right)
{
    cx_mat4_t Result;

    int Columns;
    for(Columns = 0; Columns < 4; ++Columns)
    {
        int Rows;
        for(Rows = 0; Rows < 4; ++Rows)
        {
            Result.Elements[Columns][Rows] = Left.Elements[Columns][Rows] - Right.Elements[Columns][Rows];
        }
    }

    return (Result);
}
#endif

cx_mat4_t cx_mat4_mul(cx_mat4_t left, cx_mat4_t right)
{
    cx_mat4_t Result;

#ifdef CX_MATH_USE_SSE

    Result.columns[0] = cx_linear_combine_SSE(right.columns[0], left);
    Result.columns[1] = cx_linear_combine_SSE(right.columns[1], left);
    Result.columns[2] = cx_linear_combine_SSE(right.columns[2], left);
    Result.columns[3] = cx_linear_combine_SSE(right.columns[3], left);
    
#else
    int Columns;
    for(Columns = 0; Columns < 4; ++Columns)
    {
        int Rows;
        for(Rows = 0; Rows < 4; ++Rows)
        {
            float Sum = 0;
            int CurrentMatrice;
            for(CurrentMatrice = 0; CurrentMatrice < 4; ++CurrentMatrice)
            {
                Sum += Left.Elements[CurrentMatrice][Rows] * Right.Elements[Columns][CurrentMatrice];
            }

            Result.Elements[Columns][Rows] = Sum;
        }
    }
#endif

    return (Result);
}

#ifndef CX_MATH_USE_SSE
cx_mat4_t cx_mat4_mulf(cx_mat4_t Matrix, float Scalar)
{
    cx_mat4_t Result;

    int Columns;
    for(Columns = 0; Columns < 4; ++Columns)
    {
        int Rows;
        for(Rows = 0; Rows < 4; ++Rows)
        {
            Result.Elements[Columns][Rows] = Matrix.Elements[Columns][Rows] * Scalar;
        }
    }

    return (Result);
}
#endif

cx_vec4_t cx_mat4_mulv4(cx_mat4_t Matrix, cx_vec4_t Vector)
{
    cx_vec4_t Result;
   
#ifdef CX_MATH_USE_SSE
	Result.elements_SSE = cx_linear_combine_SSE(Vector.elements_SSE, Matrix);
#else
    int Columns, Rows;
    for(Rows = 0; Rows < 4; ++Rows)
    {
        float Sum = 0;
        for(Columns = 0; Columns < 4; ++Columns)
        {
            Sum += Matrix.Elements[Columns][Rows] * Vector.Elements[Columns];
        }
        
        Result.Elements[Rows] = Sum;
    }
#endif

    return (Result);
}

#ifndef CX_MATH_USE_SSE
cx_mat4_t cx_mat4_divf(cx_mat4_t Matrix, float Scalar)
{
    cx_mat4_t Result;
    
    int Columns;
    for(Columns = 0; Columns < 4; ++Columns)
    {
        int Rows;
        for(Rows = 0; Rows < 4; ++Rows)
        {
            Result.Elements[Columns][Rows] = Matrix.Elements[Columns][Rows] / Scalar;
        }
    }

    return (Result);
}
#endif

cx_mat4_t cx_mat4_rotate(float Angle, cx_vec3_t Axis)
{
    cx_mat4_t Result = cx_mat4d(1.0f);
    
    Axis = cx_vec3_normalize(Axis);
    
    float SinTheta = cx_sinf(cx_radians(Angle));
    float CosTheta = cx_cosf(cx_radians(Angle));
    float CosValue = 1.0f - CosTheta;
    
    Result.elements[0][0] = (Axis.x * Axis.x * CosValue) + CosTheta;
    Result.elements[0][1] = (Axis.x * Axis.y * CosValue) + (Axis.z * SinTheta);
    Result.elements[0][2] = (Axis.x * Axis.z * CosValue) - (Axis.y * SinTheta);
    
    Result.elements[1][0] = (Axis.y * Axis.x * CosValue) - (Axis.z * SinTheta);
    Result.elements[1][1] = (Axis.y * Axis.y * CosValue) + CosTheta;
    Result.elements[1][2] = (Axis.y * Axis.z * CosValue) + (Axis.x * SinTheta);
    
    Result.elements[2][0] = (Axis.z * Axis.x * CosValue) + (Axis.y * SinTheta);
    Result.elements[2][1] = (Axis.z * Axis.y * CosValue) - (Axis.x * SinTheta);
    Result.elements[2][2] = (Axis.z * Axis.z * CosValue) + CosTheta;
    
    return (Result);
}

cx_mat4_t cx_mat4_look_at(cx_vec3_t Eye, cx_vec3_t Center, cx_vec3_t Up)
{
    cx_mat4_t Result;

    cx_vec3_t F = cx_vec3_normalize(cx_vec3_sub(Center, Eye));
    cx_vec3_t S = cx_vec3_normalize(cx_cross(F, Up));
    cx_vec3_t u = cx_cross(S, F);

    Result.elements[0][0] = S.x;
    Result.elements[0][1] = u.x;
    Result.elements[0][2] = -F.x;
    Result.elements[0][3] = 0.0f;

    Result.elements[1][0] = S.y;
    Result.elements[1][1] = u.y;
    Result.elements[1][2] = -F.y;
    Result.elements[1][3] = 0.0f;

    Result.elements[2][0] = S.z;
    Result.elements[2][1] = u.z;
    Result.elements[2][2] = -F.z;
    Result.elements[2][3] = 0.0f;

    Result.elements[3][0] = -cx_vec3_dot(S, Eye);
    Result.elements[3][1] = -cx_vec3_dot(u, Eye);
    Result.elements[3][2] = cx_vec3_dot(F, Eye);
    Result.elements[3][3] = 1.0f;

    return (Result);
}

cx_quat_t cx_quat_inv(cx_quat_t left)
{
    cx_quat_t Conjugate;
    cx_quat_t Result;
    float Norm = 0;
    float NormSquared = 0;

    Conjugate.x = -left.x;
    Conjugate.y = -left.y;
    Conjugate.z = -left.z;
    Conjugate.w = left.w;

    Norm = cx_sqrtf(cx_quat_dot(left, left));
    NormSquared = Norm * Norm;

    Result = cx_quat_divf(Conjugate, NormSquared);

    return (Result);
}

cx_quat_t cx_quat_slerp(cx_quat_t left, float Time, cx_quat_t right)
{
    cx_quat_t Result;
    cx_quat_t QuaternionLeft;
    cx_quat_t QuaternionRight;

    float Cos_Theta = cx_quat_dot(left, right);
    float Angle = cx_acosf(Cos_Theta);
    
    float S1 = cx_sinf((1.0f - Time) * Angle);
    float S2 = cx_sinf(Time * Angle);
    float Is = 1.0f / cx_sinf(Angle);

    QuaternionLeft = cx_quat_mulf(left, S1);
    QuaternionRight = cx_quat_mulf(right, S2);

    Result = cx_quat_add(QuaternionLeft, QuaternionRight);
    Result = cx_quat_mulf(Result, Is);

    return (Result);
}

cx_mat4_t cx_quat_to_mat4(cx_quat_t left)
{
    cx_mat4_t Result;
    Result = cx_mat4d(1);

    cx_quat_t NormalizedQuaternion = cx_quat_normalize(left);
    
    float XX, YY, ZZ,
          xy, XZ, yz,
          WX, WY, WZ;

    XX = NormalizedQuaternion.x * NormalizedQuaternion.x;
    YY = NormalizedQuaternion.y * NormalizedQuaternion.y;
    ZZ = NormalizedQuaternion.z * NormalizedQuaternion.z;
    xy = NormalizedQuaternion.x * NormalizedQuaternion.y;
    XZ = NormalizedQuaternion.x * NormalizedQuaternion.z;
    yz = NormalizedQuaternion.y * NormalizedQuaternion.z;
    WX = NormalizedQuaternion.w * NormalizedQuaternion.x;
    WY = NormalizedQuaternion.w * NormalizedQuaternion.y;
    WZ = NormalizedQuaternion.w * NormalizedQuaternion.z;

    Result.elements[0][0] = 1.0f - 2.0f * (YY + ZZ);
    Result.elements[0][1] = 2.0f * (xy + WZ);
    Result.elements[0][2] = 2.0f * (XZ - WY);

    Result.elements[1][0] = 2.0f * (xy - WZ);
    Result.elements[1][1] = 1.0f - 2.0f * (XX + ZZ);
    Result.elements[1][2] = 2.0f * (yz + WX);

    Result.elements[2][0] = 2.0f * (XZ + WY);
    Result.elements[2][1] = 2.0f * (yz - WX);
    Result.elements[2][2] = 1.0f - 2.0f * (XX + YY);

    return (Result);
}

cx_quat_t cx_quat_from_axis_angle(cx_vec3_t Axis, float AngleOfRotation)
{
    cx_quat_t Result;
    
    cx_vec3_t RotatedVector;
    
    float AxisNorm = 0;
    float SineOfRotation = 0;

    AxisNorm = cx_sqrtf(cx_vec3_dot(Axis, Axis));
    SineOfRotation = cx_sinf(AngleOfRotation / 2.0f);
    RotatedVector = cx_vec3_mulf(Axis, SineOfRotation);

    Result.w = cx_cosf(AngleOfRotation / 2.0f);
    Result.xyz = cx_vec3_divf(RotatedVector, AxisNorm);

    return (Result);
}
