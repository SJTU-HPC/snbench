
#pragma once
#include <immintrin.h>
#include "papi.h"
#include "include.h"

#if defined(MATH_LIB_AMD)
#define AMD_LIBM_VEC_EXPERIMENTAL
#include "amdlibm.h"
#include "amdlibm_vec.h"
#elif defined(MATH_LIB_SLEEF_U10) || defined(MATH_LIB_SLEEF_U35)
#include "sleef.h"
#endif

namespace op
{
const std::string NAME="COS";
alignas(64) double out[MAX_N];

double run_serial(const double *a, const double *b, int n, int repeat) {
    for (int rp = 0; rp < repeat; rp++) {
        for (int i = 0; i < n; i++) {
#if defined(MATH_LIB_AMD)
            out[i] = amd_cos(a[i]);
#elif defined(MATH_LIB_SLEEF_U10)
            out[i] = Sleef_cos_u10(a[i]);
#elif defined(MATH_LIB_SLEEF_U35)
            out[i] = Sleef_cos_u35(a[i]);
#else 
            out[i] = cos(a[i]);
#endif
        }
    }
    return out[0];
}


#ifndef SCALAR_ONLY
double run_sse_128(const double *a, const double *b, int n, int repeat) {
    for (int rp = 0; rp < repeat; rp++) {
        for (int i = 0; i < n; i+=2) {
            __m128d v_a = _mm_load_pd(a + i);
#if defined(MATH_LIB_AMD)
            v_a = amd_vrd2_cos(v_a);
#elif defined(MATH_LIB_SLEEF_U10)
            v_a = Sleef_cosd2_u10(v_a);
#elif defined(MATH_LIB_SLEEF_U35)
            v_a = Sleef_cosd2_u35(v_a);
#else 
            v_a = _mm_cos_pd(v_a);
#endif
            _mm_store_pd(out + i, v_a);
        }
    }
    return out[0];
}

double run_avx_256(const double *a, const double *b, int n, int repeat) {
    for (int rp = 0; rp < repeat; rp++) {
        for (int i = 0; i < n; i+=4) {
            __m256d v_a = _mm256_load_pd(a + i);
#if defined(MATH_LIB_AMD)
            v_a = amd_vrd4_cos(v_a);
#elif defined(MATH_LIB_SLEEF_U10)
            v_a = Sleef_cosd4_u10(v_a);
#elif defined(MATH_LIB_SLEEF_U35)
            v_a = Sleef_cosd4_u35(v_a);
#else 
            v_a = _mm256_cos_pd(v_a);
#endif
            _mm256_store_pd(out + i, v_a);
        }
    }
    return out[0];
}

double run_avx_512(const double *a, const double *b, int n, int repeat) {
    for (int rp = 0; rp < repeat; rp++) {
        for (int i = 0; i < n; i+=8) {
            __m512d v_a = _mm512_load_pd(a + i);
#if defined(MATH_LIB_AMD)
            v_a = amd_vrd8_cos(v_a);
#elif defined(MATH_LIB_SLEEF_U10)
            v_a = Sleef_cosd8_u10(v_a);
#elif defined(MATH_LIB_SLEEF_U35)
            v_a = Sleef_cosd8_u35(v_a);
#else 
            v_a = _mm512_cos_pd(v_a);
#endif
            _mm512_store_pd(out + i, v_a);
        }
    }
    return out[0];
}
#endif

double comp_op(double a, double b) {
#if defined(MATH_LIB_AMD)
    a = amd_cos(a);
#elif defined(MATH_LIB_SLEEF_U10)
    a = Sleef_cos_u10(a);
#elif defined(MATH_LIB_SLEEF_U35)
    a = Sleef_cos_u35(a);
#else 
    a = cos(a);
#endif
    return a;
}

std::string get_input_normal_subnormal_format(double a, double b) {
    return fpclassify(comp_op(a, b)) + fpclassify(a);
};

}
