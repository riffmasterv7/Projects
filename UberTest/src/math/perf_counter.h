#ifndef __PERFOMANCE_COUNTER__
#define __PERFOMANCE_COUNTER__

#include <windows.h>
//#include <intrin.h>

#include "../types_v7.h"

static LARGE_INTEGER s_freq;

// Manually defined function pointers
typedef float (FUNC_CALL_TYPE * FP_float_Vector4_Vector4)(float4 v1, float4 v2);
typedef float (FUNC_CALL_TYPE * FP_float_Vector4)(float4 v1);
typedef void (FUNC_CALL_TYPE * FP_void_Vector4_Vector4_Vector4)(float4 v1, float4 v2, float4 v3);
typedef void (FUNC_CALL_TYPE * FP_void_Vector4_Vector4_float)(float4 v1, float4 v2, float f);
typedef void (FUNC_CALL_TYPE * FP_void_Vector4_Vector4)(float4 v1, float4 v2);
typedef void (FUNC_CALL_TYPE * FP_void_Vector4_float)(float4 v1, float f);

typedef void (FUNC_CALL_TYPE * FP_void_Matrix_Matrix_Matrix)(float44 res, float44 m1, float44 m2);
typedef void (FUNC_CALL_TYPE * FP_void_Matrix_Matrix)(float44 res, float44 m);
typedef void (FUNC_CALL_TYPE * FP_void_Vector4_Matrix_Vector4)(float4 res, float44 m, float4 v);

typedef float (FUNC_CALL_TYPE * FP_float_float)(float v);
typedef void (FUNC_CALL_TYPE * FP_float2_float)(float * res1, float * res2, float v);

typedef double (__cdecl * FP_d_d)(double v);

static unsigned short s_random = 3383;

unsigned rand8()
{
    unsigned r = s_random;
    unsigned hr = r >> 1;
    unsigned rr = (((r >> 8) + hr) & 0xFF) << 8;
    hr &= 0xFF;
    rr |= hr;
    if (rr == r)
    {
        rr += 0x101;
    }
    s_random = (unsigned short)rr;
    return ((rr >> 8) & 0xFF);
}

float frand01()
{
    union
    {
        float f32;
        unsigned u32;
    } d32;
    unsigned r0 = rand8();
    unsigned r1 = rand8();
    unsigned r2 = rand8();
    d32.f32 = 1.0f;
    d32.u32 += (r0 | (r1 << 8) | ((r2 & 0x7F) << 16));
    d32.f32 -= 1.0f;
    return d32.f32;
}

float frand1_1()
{
    float r = frand01() - 0.5f;
    return (r + r);
}

static __inline int64 get_time_stamp()
{
    int64 res = 0;
    /*
    __asm {
    	sub eax, eax
    		cpuid
    }
    res = __rdtsc();
    __asm {
    	sub eax, eax
    		cpuid
    }
    */
    return res;
}

double __cdecl pcTestFloatFloat_Special(int tests,  float * f, FP_d_d fpTestFunc, float v, float * res)
{

    int64 before, after;
    double elapsed = 0.0;
    int i;
    *res = 0.0f;
    before = get_time_stamp();

    for (i = 0; i < tests; i++)
    {

        *res += (float)fpTestFunc(f[i]);
    }

    after = get_time_stamp();

    elapsed = (double)after - (double)before;

    return elapsed / (double)tests;
}

double __cdecl pcTestFloatFloat(int tests, float * f, FP_float_float fpTestFunc, float v, float * res)
{

    int64 before, after;
    double elapsed = 0.0;
    int i;
    *res = 0.0f;
    before = get_time_stamp();

    for (i = 0; i < tests; i++)
    {

        *res += (float)fpTestFunc(f[i]);
    }

    after = get_time_stamp();

    elapsed = (double)after - (double)before;

    return elapsed / (double)tests;
}

double __cdecl pcTestFloat2Float(int tests,  float * f, FP_float2_float fpTestFunc, float v, float * res1, float * res2)
{

    int64 before, after;
    double elapsed = 0.0;
    int i;
    float r1 = 0.0f;
    float r2 = 0.0f;

    *res1 = *res2 = 0.0f;

    before = get_time_stamp();

    for (i = 0; i < tests; i++)
    {

        fpTestFunc(&r1, &r2, f[i]);
        *res1 += r1;
        *res2 += r2;

    }

    after = get_time_stamp();

    elapsed = (double)after - (double)before;

    return elapsed / (double)tests;
}

double __cdecl pcTestFloatVector4(int tests, FP_float_Vector4 fpTestFunc, float4 v0, float * res)
{

    LARGE_INTEGER before, after;
    double elapsed = 0.0;
    int i;

    QueryPerformanceFrequency(&s_freq);
    QueryPerformanceCounter(&before);

    for (i = 0; i < tests; i++)
    {

        *res = fpTestFunc(v0);

        //elapsed+=v0[0]+v0[1]+v0[2];
    }

    QueryPerformanceCounter(&after);

    //printf("%f\n", elapsed);

    elapsed = (double)after.QuadPart - (double)before.QuadPart;
    elapsed /= (double)s_freq.QuadPart;

    return elapsed / (double)tests;
}

double __cdecl pcTestFloatVector4Vector4(int tests, FP_float_Vector4_Vector4 fpTestFunc, float4 v0, float4 v1, float * res)
{

    LARGE_INTEGER before, after;
    double elapsed = 0.0;
    int i;

    QueryPerformanceFrequency(&s_freq);
    QueryPerformanceCounter(&before);

    for (i = 0; i < tests; i++)
    {

        *res = fpTestFunc(v0, v1);

        //elapsed+=v0[0]+v0[1]+v0[2];
    }

    QueryPerformanceCounter(&after);

    //printf("%f\n", elapsed);

    elapsed = (double)after.QuadPart - (double)before.QuadPart;
    elapsed /= (double)s_freq.QuadPart;

    return elapsed / (double)tests;
}

double __cdecl pcTestVoidVector4Vector4Vector4(int tests, FP_void_Vector4_Vector4_Vector4 fpTestFunc, float4 v0, float4 v1, float4 v2)
{

    LARGE_INTEGER before, after;
    double elapsed = 0.0;
    int i;

    QueryPerformanceFrequency(&s_freq);
    QueryPerformanceCounter(&before);

    for (i = 0; i < tests; i++)
    {

        fpTestFunc(v0, v1, v2);

        //elapsed+=v0[0]+v0[1]+v0[2];
    }

    QueryPerformanceCounter(&after);

    //printf("%f\n", elapsed);

    elapsed = (double)after.QuadPart - (double)before.QuadPart;
    elapsed /= (double)s_freq.QuadPart;

    return elapsed / (double)tests;
}

double __cdecl pcTestVoidVector4Vector4(int tests, FP_void_Vector4_Vector4 fpTestFunc, float4 v0, float4 v1)
{

    LARGE_INTEGER before, after;
    double elapsed = 0.0;
    int i;

    QueryPerformanceFrequency(&s_freq);
    QueryPerformanceCounter(&before);

    for (i = 0; i < tests; i++)
    {

        fpTestFunc(v0, v1);

        //elapsed+=v0[0]+v0[1]+v0[2];
    }

    QueryPerformanceCounter(&after);

    //printf("%f\n", elapsed);

    elapsed = (double)after.QuadPart - (double)before.QuadPart;
    elapsed /= (double)s_freq.QuadPart;

    return elapsed / (double)tests;
}

double __cdecl pcTestVoidVector4Vector4Float(int tests, FP_void_Vector4_Vector4_float fpTestFunc, float4 v0, float4 v1, float f)
{

    LARGE_INTEGER before, after;
    double elapsed = 0.0;
    int i;

    QueryPerformanceFrequency(&s_freq);
    QueryPerformanceCounter(&before);

    for (i = 0; i < tests; i++)
    {

        fpTestFunc(v0, v1, f);

        //elapsed+=v0[0]+v0[1]+v0[2];
    }

    QueryPerformanceCounter(&after);

    //printf("%f\n", elapsed);

    elapsed = (double)after.QuadPart - (double)before.QuadPart;
    elapsed /= (double)s_freq.QuadPart;

    return elapsed / (double)tests;
}

double __cdecl pcTestVoidVector4Float(int tests, FP_void_Vector4_float fpTestFunc, float4 v0, float f)
{

    LARGE_INTEGER before, after;
    double elapsed = 0.0;
    int i;

    QueryPerformanceFrequency(&s_freq);
    QueryPerformanceCounter(&before);

    for (i = 0; i < tests; i++)
    {

        fpTestFunc(v0, f);

        //elapsed+=v0[0]+v0[1]+v0[2];
    }

    QueryPerformanceCounter(&after);

    //printf("%f\n", elapsed);

    elapsed = (double)after.QuadPart - (double)before.QuadPart;
    elapsed /= (double)s_freq.QuadPart;

    return elapsed / (double)tests;
}

double __cdecl pcTestVoidMatrixMatrixMatrix(int tests, FP_void_Matrix_Matrix_Matrix fpTestFunc, float44 res, float44 m1, float44 m2)
{

    LARGE_INTEGER before, after;
    double elapsed = 0.0;
    int i;

    QueryPerformanceFrequency(&s_freq);
    QueryPerformanceCounter(&before);

    for (i = 0; i < tests; ++i)
    {

        fpTestFunc(res, m1, m2);

        //elapsed+=v0[0]+v0[1]+v0[2];
    }

    QueryPerformanceCounter(&after);

    //printf("%f\n", elapsed);

    elapsed = (double)after.QuadPart - (double)before.QuadPart;
    elapsed /= (double)s_freq.QuadPart;

    return elapsed / (double)tests;
}

double __cdecl pcTestVoidMatrixMatrix(int tests, FP_void_Matrix_Matrix fpTestFunc, float44 res, float44 m1)
{

    LARGE_INTEGER before, after;
    double elapsed = 0.0;
    int i;

    QueryPerformanceFrequency(&s_freq);
    QueryPerformanceCounter(&before);

    for (i = 0; i < tests; ++i)
    {

        fpTestFunc(res, m1);

        //elapsed+=v0[0]+v0[1]+v0[2];
    }

    QueryPerformanceCounter(&after);

    //printf("%f\n", elapsed);

    elapsed = (double)after.QuadPart - (double)before.QuadPart;
    elapsed /= (double)s_freq.QuadPart;

    return elapsed / (double)tests;
}

double __cdecl pcTestVoidVector4MatrixVector4(int tests, FP_void_Vector4_Matrix_Vector4 fpTestFunc, float4 res, float44 m, float4 v)
{

    LARGE_INTEGER before, after;
    double elapsed = 0.0;
    int i;

    QueryPerformanceFrequency(&s_freq);
    QueryPerformanceCounter(&before);

    for (i = 0; i < tests; ++i)
    {

        fpTestFunc(res, m, v);

        //elapsed+=v0[0]+v0[1]+v0[2];
    }

    QueryPerformanceCounter(&after);

    //printf("%f\n", elapsed);

    elapsed = (double)after.QuadPart - (double)before.QuadPart;
    elapsed /= (double)s_freq.QuadPart;

    return elapsed / (double)tests;
}

#endif/*__PERFOMANCE_COUNTER__*/
