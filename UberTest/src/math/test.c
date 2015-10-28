#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include "../math_v7.h"
#include "perf_counter.h"

//#define D_TEST_VECTOR
//#define D_TEST_MATRIX
//#define D_TEST_MATH
//#define D_TEST_MATH_CORRECTNESS

#define D_TESTS 20

void makePerspectiveMV(float44 mat, const float fovy, const float aspect, const float near_plane, const float far_plane)
{
   const float f = -tan((fovy + D_PI) * 0.5f);
   const float dif = 1.0f / (near_plane-far_plane);

   /*
   mat[ 0] = f / aspect;
   mat[ 4] = 0;
   mat[ 8] = 0;
   mat[12] = 0;

   mat[ 1] = 0;
   mat[ 5] = f;
   mat[ 9] = 0;
   mat[13] = 0;

   mat[ 2] = 0;
   mat[ 6] = 0;
   mat[10] = (far_plane + near_plane) * dif;
   mat[14] = 2.0f * far_plane * near_plane * dif;

   mat[ 3] = 0;
   mat[ 7] = 0;
   mat[11] = -1.0;
   mat[15] = 0;
   */
   mtxZero(mat);

   mat[0][0] = f / aspect;
   mat[1][1] = f;
   mat[2][2] = (far_plane + near_plane) * dif;
   mat[2][3] = -1.0;
   mat[3][2] = 2.0f * far_plane * near_plane * dif;
}

int32 main(int32 argc, char* argv[])
{

    int32 tests = D_TESTS;
    int32 i = 0, j = 0;

    double time;

    /// Vector tests
#if defined (D_TEST_VECTOR)
    for (i = 1; i <= 1; ++i)
    {
        float res = 0.0f;
        vec4 v0;
        vec4 v1 = {0.0f, i * 1.0f, i * 1.0f, 1.0f};
        vec4 v2 = {1.0f, 2.0f, i * 3.0f, 2.0f};

        time = 1000.0 * pcTestFloatVector4Vector4(tests, vecDot, v1, v2, &res);
        printf("vecDot				 %0.10f	 %f\n", time, res);

        time = 1000.0 * pcTestFloatVector4Vector4(tests, vecDot4, v1, v2, &res);
        printf("vecDot4				%0.10f	 %f\n", time, res);

        time = 1000.0 * pcTestVoidVector4Vector4Vector4(tests, vecCrossRH, v0, v1, v2);
        printf("vecCross			  %0.10f	 %f %f %f\n", time, v0[0], v0[1], v0[2]);

        time = 1000.0 * pcTestVoidVector4Vector4Vector4(tests, vecAdd, v0, v1, v2);
        printf("vecAdd				 %0.10f	 %f %f %f\n", time, v0[0], v0[1], v0[2]);

        time = 1000.0 * pcTestVoidVector4Vector4Vector4(tests, vecSub, v0, v1, v2);
        printf("vecSub				 %0.10f	 %f %f %f\n", time, v0[0], v0[1], v0[2]);

        time = 1000.0 * pcTestVoidVector4Vector4Vector4(tests, vecMul, v0, v1, v2);
        printf("vecMul				 %0.10f	 %f %f %f\n", time, v0[0], v0[1], v0[2]);

        time = 1000.0 * pcTestVoidVector4Vector4Vector4(tests, vecDiv, v0, v1, v2);
        printf("vecDiv				 %0.10f	 %f %f %f\n", time, v0[0], v0[1], v0[2]);

        time = 1000.0 * pcTestVoidVector4Vector4Float(tests, vecScale, v0, v1, 2.0);
        printf("vecScale			  %0.10f	 %f %f %f\n", time, v0[0], v0[1], v0[2]);

        time = 1000.0 * pcTestVoidVector4Vector4(tests, vecNeg, v0, v1);
        printf("vecNeg				 %0.10f	 %f %f %f\n", time, v0[0], v0[1], v0[2]);

        time = 1000.0 * pcTestVoidVector4Vector4(tests, vecNeg4, v0, v1);
        printf("vecNeg4				%0.10f	 %f %f %f\n", time, v0[0], v0[1], v0[2]);

        time = 1000.0 * pcTestFloatVector4(tests, vecMag, v1, &res);
        printf("vecMag				 %0.10f	 %f \n", time, res);

        time = 1000.0 * pcTestFloatVector4(tests, vecMagSq, v1, &res);
        printf("vecMagSq			  %0.10f	 %f \n", time, res);

        time = 1000.0 * pcTestFloatVector4(tests, vecMag4, v1, &res);
        printf("vecMag4				%0.10f	 %f \n", time, res);

        time = 1000.0 * pcTestFloatVector4Vector4(tests, vecNormalizeEx, v0, v1, &res);
        printf("vecNormalizeEx	  %0.10f	 %f %f %f %f\n", time, v0[0], v0[1], v0[2], res);

        time = 1000.0 * pcTestVoidVector4Vector4(tests, vecNormalize, v0, v1);
        printf("vecNormalize		 %0.10f	 %f %f %f\n", time, v0[0], v0[1], v0[2]);

        time = 1000.0 * pcTestFloatVector4Vector4(tests, vecDist, v2, v1, &res);
        printf("vecDist				%0.10f	 %f\n", time, res);

        time = 1000.0 * pcTestFloatVector4Vector4(tests, vecDistSq, v2, v1, &res);
        printf("vecDistSq			 %0.10f	 %f\n", time, res);

        time = 1000.0 * pcTestVoidVector4Vector4(tests, vecCopy, v0, v1);
        printf("vecCopy				%0.10f	 %f %f %f\n", time, v0[0], v0[1], v0[2]);

        time = 1000.0 * pcTestVoidVector4Vector4(tests, vecRcp, v0, v2);
        printf("vecRcp				 %0.10f	 %f %f %f\n", time, v0[0], v0[1], v0[2]);

        time = 1000.0 * pcTestVoidVector4Vector4(tests, vecSwap, v2, v1);
        printf("vecSwap				%0.10f	 %f %f %f\n", time, v0[0], v0[1], v0[2]);
    }
#endif
    /// float44 tests
#if defined (D_TEST_MATRIX)
#define CYCLES 200
    for (i = 1; i <= CYCLES; ++i)
    {
        vec4 v1 = { 1.0f * i, 0.0f, 0.0f, 1.0f};
        vec4 v2;
        mtx res;
        mtx m1;
        mtx m2;

        mtxUnit(res);
        mtxMakePerspectiveRH(m1, 0.23423f * i, 1.33f, 0.001f * i, 100000.f + i);
        mtxMakeRotationY(m2, 6.77423f * i);

// 		time = 1000.0 * pcTestVoidMatrixMatrix(tests, mtxCopy, res, m1);
// 		printf("mtxCopy				 %0.10f	 %f\n", time, res[0][0]);
// 		sum += time;
//
// 		time = 1000.0 * pcTestVoidMatrixMatrixMatrix(tests, mtxMul, res, m1, m2);
// 		printf("mtxMul				  %0.10f	 %f %f %f %f\n", time, res[0][0], res[0][1], res[0][2], res[0][3]);

        time = 1000.0 * pcTestVoidVector4MatrixVector4(tests, mtxApply, v2, m2, v1);
        printf("mtxApply				%0.10f	 %f %f %f %f\n", time, v2[0], v2[1], v2[2], v2[3]);
//
// 		time = 1000.0 * pcTestVoidMatrixMatrix(tests, mtxTranspose, res, m1);
// 		printf("mtxTranspose		  %0.10f	 %f\n", time, res[0][0]);

// 		time = 1000.0 * pcTestVoidMatrixMatrix(tests, mtxInvertFull, res, m1);
// 		printf("mtxInvertFull		 %0.10f	 %f\n", time, res[0][0]);
    }
    //printf("AVG %0.10f\n", sum / (float)CYCLES);
#endif

#if defined (D_TEST_MATH)

    for (j = 0; j < 1; ++j)
    {
        float res1;
        float res2;
        float start = 180.0f;
        int i;
        float * angle_array = malloc(D_TESTS * sizeof(float));

        for (i = 0; i < D_TESTS; ++i)
        {
            angle_array[i] = D_DEG2RAD(start * frand1_1());
        }

        time = 1000.0 * pcTestFloat2Float(tests, angle_array, fSinCos, i, &res1, &res2);
        printf("fSinCos		 %1.3f	  %1.7f  %1.7f \n", time, res1, res2);
        time = 1000.0 * pcTestFloatFloat(tests, angle_array, fSin, i, &res1);
        printf("fSin			 %1.3f	  %1.7f  \n", time, res1);
        time = 1000.0 * pcTestFloatFloat(tests, angle_array, fCos, i, &res1);
        printf("fCos			 %1.3f	  %1.7f  \n", time, res1);

        printf("\n");

        time = 1000.0 * pcTestFloat2Float(tests, angle_array, deadlySinCos, i, &res1, &res2);
        printf("deadlySinCos  %1.3f	  %1.7f  %1.7f \n", time, res1, res2);
        time = 1000.0 * pcTestFloatFloat(tests, angle_array, deadlySin, i, &res1);
        printf("deadlySin	  %1.3f	  %1.7f  \n", time, res1);
        time = 1000.0 * pcTestFloatFloat(tests, angle_array, deadlyCos, i, &res1);
        printf("deadlyCos	  %1.3f	  %1.7f  \n", time, res1);
        time = 1000.0 * pcTestFloatFloat(tests, angle_array, deadlyTan, i, &res1);
        printf("deadlyTan	  %1.3f	  %1.7f  \n", time, res1);

        printf("\n");

        time = 1000.0 * pcTestFloat2Float(tests, angle_array, deadlySinCosEx, i, &res1, &res2);
        printf("deadlySinCos1 %1.3f	  %1.7f  %1.7f \n", time, res1, res2);
        time = 1000.0 * pcTestFloatFloat(tests, angle_array, deadlySinEx, i, &res1);
        printf("deadlySin1	 %1.3f	  %1.7f  \n", time, res1);
        time = 1000.0 * pcTestFloatFloat(tests, angle_array, deadlyCosEx, i, &res1);
        printf("deadlyCos1	 %1.3f	  %1.7f  \n", time, res1);
        time = 1000.0 * pcTestFloatFloat(tests, angle_array, deadlyTanEx, i, &res1);
        printf("deadlyTan1	 %1.3f	  %1.7f  \n", time, res1);

        printf("\n");

        time = 1000.0 * pcTestFloatFloat_Special(tests, angle_array, sin, i, &res1);
        printf("sin			  %1.3f	  %1.7f  \n", time, res1);
        time = 1000.0 * pcTestFloatFloat_Special(tests, angle_array, cos, i, &res1);
        printf("cos			  %1.3f	  %1.7f  \n", time, res1);
        time = 1000.0 * pcTestFloatFloat_Special(tests, angle_array, tan, i, &res1);
        printf("tan			  %1.3f	  %1.7f  \n", time, res1);

        printf("\n\n");

        free(angle_array);
    }
#endif

#if defined(D_TEST_MATH_CORRECTNESS)
    {
        float g_angle_array[D_TESTS];
        float start = 180.0f;
        int i;
        for (i = 0; i < D_TESTS; ++i)
        {
            g_angle_array[i] = start * frand1_1();
        }

        for (i = 0; i < 1; ++i)
        {

            float angle = g_angle_array[i];

            float sinOriginal = 0.0f;
            float sinClamped = 0.0f;

            float s1 = 0.0f, s2 = 0.0f;
            float c1 = 0.0f, c2 = 0.0f;
            float error = 0.0f;

            sinOriginal = tan(D_DEG2RAD(angle));
            sinClamped = deadlyTan(D_DEG2RAD(angle));
            //printf(" A: %3.0f	TAN: %1.7f	%1.7f	ERROR: %1.12f \n", angle, sinOriginal, sinClamped, fAbs(sinOriginal - sinClamped));

            sinOriginal = sin(D_DEG2RAD(angle));
            sinClamped = deadlySin(D_DEG2RAD(angle));
            printf(" A: %3.0f	SIN: %1.7f	%1.7f	ERROR: %1.12f \n", angle, sinOriginal, sinClamped, fAbs(sinOriginal - sinClamped));

            sinOriginal = cos(D_DEG2RAD(angle));
            sinClamped = deadlyCos(D_DEG2RAD(angle));
            //printf(" A: %3.0f	COS: %1.7f	%1.7f	ERROR: %1.12f \n", angle, sinOriginal, sinClamped, fAbs(sinOriginal - sinClamped));

            fSinCos(&s1, &c1, D_DEG2RAD(angle));
            deadlySinCos(&s2, &c2, D_DEG2RAD(angle));

            error = s1 - s2;
            //printf(" A: %3.0f	SIN: %1.7f	%1.7f	ERROR: %1.12f \n", angle, s1, s2, fAbs(error));

            error = c1 - c2;
            //printf(" A: %3.0f	COS: %1.7f	%1.7f	ERROR: %1.12f \n", angle, c1, c2, fAbs(error));
        }
    }
#endif

	{
		float nearPlane = 10.0f;
		float farPlane = 110.0f;
		mtx projectionMatrix;
		int32 i = 0;

		//mtxMakePerspectiveRH(projectionMatrix, D_DEG2RAD(30.0f), 1.0f, nearPlane, farPlane);
      makePerspectiveMV(projectionMatrix, D_DEG2RAD(30.0f), 1.0f, nearPlane, farPlane);

		for (; i < D_TESTS + 1; ++i) {

			vec4 output;
			vec4 input;
         float linearZ;

			input[0] = 0.0f;
			input[1] = 0.0f;
			input[2] = -nearPlane - (float)i * ((farPlane - nearPlane) / (float)D_TESTS);
			input[3] = 1.0f;

			mtxApply(output, projectionMatrix, input);

			printf("CamZ %5.1f, projZ %5.5f, projW %5.5f, ZdivW %1.5f, ZdivFar %1.5f \n", input[2], output[2], output[3], output[2] / output[3], (output[2] + nearPlane) / (farPlane + nearPlane) );
		}
	}

    return 1;

}
