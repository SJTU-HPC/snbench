
#pragma once
#include <immintrin.h>
#include "papi.h"
#include "include.h"

namespace op
{
const std::string NAME="SQRT";

double run_serial(double a, double b, int repeat) {
    asm volatile (
        "vmovsd %[a_value], %%xmm0\n\t" 
        "vmovsd %[b_value], %%xmm1\n\t" 
        : 
        : [a_value] "m" (a), [b_value] "m" (b)
        : "%xmm0", "%xmm1"
    );

    for (int i = 0; i < repeat; i+=UNROOL) {
        asm volatile (
            DUP_X("vsqrtsd %%xmm0, %%xmm1, %%xmm2\n\t")
            : 
            : 
            : "%xmm0", "%xmm1", "%xmm2" 
        );
    }
    return 0;
}

double run_sse_128(double a, double b, int repeat) {
    __m128d v_a, v_b;
    v_a = _mm_set1_pd(a);
    v_b = _mm_set1_pd(b);

    for (int i = 0; i < repeat; i+=UNROOL) {
        asm volatile (
            DUP_X("vsqrtpd %0, %%xmm2\n\t")
            : 
            : "v" (v_a)
            : "%xmm2" 
        );
    }
    return 0;
}

double run_avx_256(double a, double b, int repeat) {
    __m256d v_a, v_b;
    v_a = _mm256_set1_pd(a);
    v_b = _mm256_set1_pd(b);

    for (int i = 0; i < repeat; i+=UNROOL) {
        asm volatile (
            DUP_X("vsqrtpd %0, %%ymm2\n\t")
            : 
            : "v" (v_a)
            : "%ymm2" 
        );
    }
    return 0;
}

double run_avx_512(double a, double b, int repeat) {
    __m512d v_a, v_b;
    v_a = _mm512_set1_pd(a);
    v_b = _mm512_set1_pd(b);

    for (int i = 0; i < repeat; i+=UNROOL) {
        asm volatile (
            DUP_X("vsqrtpd %0, %%zmm2\n\t")
            : 
            : "v" (v_a)
            : "%zmm2" 
        );
    }
    return 0;
}

double comp_op(double a, double b) {
    return sqrt(a);
}

std::string get_input_normal_subnormal_format(double a, double b) {
    return fpclassify(comp_op(a, b)) + fpclassify(a);
};

}

