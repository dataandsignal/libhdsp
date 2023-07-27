/*
 * This file is part of libhdsp - Handy DSP routines library
 *
 * Copyright (c) 2023 Data And Signal - IT Solutions
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following
 *   disclaimer in the documentation and/or other materials provided
 *   with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Piotr Gregor <piotr@dataandsignal.com>
 * Data And Signal - IT Solutions
 *
 */


#ifndef LHDSP_LHDSP_H
#define LHDSP_LHDSP_H


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include <math.h>
#include <float.h>

enum hdsp_conv_type {
    HDSP_CONV_TYPE_FULL,
    HDSP_CONV_TYPE_SAME,
    HDSP_CONV_TYPE_VALID
};
typedef enum hdsp_conv_type hdsp_conv_type_t;

#define hdsp_min(x, y) ((x) < (y) ? (x) : (y))
#define hdsp_max(x, y) ((x) < (y) ? (y) : (x))

enum hdsp_status {
    HDSP_STATUS_OK,
    HDSP_STATUS_FALSE
};
typedef enum hdsp_status hdsp_status_t;

struct hdsp_filter {
    int16_t *a;
    int16_t *b;
    size_t a_len;
    size_t b_len;
};
typedef struct hdsp_filter hdsp_filter_t;

/**
 * Upsample by zero insertion.
 *      x - (in) input frame
 *      x_len - (in) input frame length in samples
 *      upsample_factor - (in) upsampling factor equal to ratio of output frequency to input frequency (Fy / Fx)
 *      y - (in/out) upsampled frame, memory should be pre-allocated
 *      y_len - (in) upsampled frame's length in samples, should be x_len*upsample_factor
 * Returns HDSP_STATUS_OK on success, HDSP_STATUS_FALSE on error.
 */
hdsp_status_t hdsp_upsample(int16_t *x, size_t x_len, int upsample_factor, int16_t *y, size_t y_len);

/**
 * Compute full-length convolution of input signal x and filter h: x*h=Sum{x[tau]h[t-tau]}.
 * Result is of length x_len + h_len - 1, written to a vector pointed to by y (y must be allocated).
 *      x - (in) input signal
 *      x_len - (in) input signal length in samples
 *      h - (in) input filter
 *      h_len - (in) number of filter's coefficients
 *      y - (out) output, must point to a valid memory of at least sizeof(int16_t)*(x_len + h_len - 1) bytes
 * Returns the number of elements written to y (x_len + h_len - 1).
 */
uint16_t hdsp_conv_full(int16_t *x, uint16_t x_len, int16_t *h, uint16_t h_len, int16_t *y);

/**
 * Compute convolution of input signal x and filter h: x*h=Sum{x[tau]h[t-tau]}.
 * Convolution types match those from MATLAB's conv function https://www.mathworks.com/help/fixedpoint/ref/conv.html
 * Convolution types:
 *  full - (HDSP_CONV_TYPE_FULL) full-length convolution, (x_len + h_len - 1) elements
 *  same - (HDSP_CONV_TYPE_SAME) central part of full-length convolution truncated to length of x
 *  valid - (HDSP_CONV_TYPE_VALID) segment of full-length convolution elements, which have been computed without
 *          a need to zero-pad (missing) elements, i.e. only such elements for which computation all coefficients
 *          of filter h were multiplied by samples of x (none of the coefficients of h fallen outside of x)
 * Result is a full-length convolution of size x_len + h_len - 1, written to a vector pointed to by y
 * (y must be allocated). A part of the full-length convolution determined by convolution type is defined
 * by idx_start and idx_end.
 *      x - (in) input signal
 *      x_len - (in) input frame length in samples
 *      h - (in) input filter
 *      h_len - (in) number of filter's coefficients
 *      type - convolution type (HDSP_CONV_TYPE_FULL, HDSP_CONV_TYPE_SAME, HDSP_CONV_TYPE_VALID)
 *      y - (out) output, must point to a valid memory of at least sizeof(int16_t)*(x_len + h_len - 1) bytes
 *      Full-length convolution is written to y.
 *      idx_start - (out) index of first element in y for the required convolution type (must point to int32_t),
 *      starting from 0
 *      idx_end - (out) index of one past the last element in y for the required convolution type
 *      (must point to int32_t), starting from 0
 * Returns the number of elements written to y (x_len + h_len - 1) and indices pointing to first and one past the last
 * element of convolution result for a required convolution type. In case of 'valid' convolution type a result vector
 * may be empty, which is signalled by idx_start == -1 and idx_end == -1.
 */
uint16_t hdsp_conv(int16_t *x, uint16_t x_len, int16_t *h, uint16_t h_len, hdsp_conv_type_t type,
                   int16_t *y, int32_t *idx_start, int32_t *idx_end);

/**
 * Create N-point symmetric Hamming window.
 *      y - (out) result (must point to a valid memory of at least sizeof(double)*n bytes
 *      n - (in) number of points
 */
void hdsp_hamming_window(double *w, uint16_t n);

/**
 * Create N-point symmetric Kaiser window.
 *      y - (out) result (must point to a valid memory of at least sizeof(double)*n bytes
 *      n - (in) number of points
 *      beta - beta coefficient that determines the shape of the Kaiser window. In the frequency domain,
 *      it determines the trade-off between main-lobe width and side lobe level
 */
void hdsp_kaiser_window(double *w, uint16_t n, double beta);

/**
 * Filter frame x with filter.
 */
hdsp_status_t hdsp_filter(int16_t *x, size_t x_len, hdsp_filter_t *fltr, int16_t *y, size_t *y_len);

#define HDSP_FACTORIAL_MAX 40
double hdsp_factorial[HDSP_FACTORIAL_MAX + 1] = {
        1.0,1.0,2.0,6.0,24.0,120.0,720.0,
        5040.0,40320.0,362880.0,3628800.0,39916800.0,
        479001600.0,6227020800.0,87178291200.0,1307674368000.0,
        20922789888000.0,355687428096000.0,6402373705728000.0,
        121645100408832000.0,2432902008176640000.0,51090942171709440000.0,
        1124000727777607680000.0,25852016738884978212864.0,620448401733239409999872.0,
        15511210043330986055303168.0,403291461126605650322784256.0,10888869450418351940239884288.0,
        304888344611713836734530715648.0,8841761993739700772720181510144.0,
        265252859812191032188804700045312.0,8222838654177922430198509928972288.0,
        263130836933693517766352317727113216.0,8683317618811885938715673895318323200.0,
        295232799039604119555149671006000381952.0,10333147966386144222209170348167175077888.0,
        371993326789901177492420297158468206329856.0,13763753091226343102992036262845720547033088.0,
        523022617466601037913697377988137380787257344.0,20397882081197441587828472941238084160318341120.0,
        815915283247897683795548521301193790359984930816.0
};

/**
 * Returns value of the modified Bessel function of the first kind for argument x, I_zero(x).
 * Value is approximated using series with k = 0 to HDSP_FACTORIAL_MAX.
 * https://mathworld.wolfram.com/ModifiedBesselFunctionoftheFirstKind.html
 */
double hdsp_modified_bessel_1st_kind_zero(double x);

/* Tests */

void hdsp_die(const char *file, int line, const char *s);

#define HDSP_DOUBLE_ALMOST_EPSILON 0.000001
#define hdsp_test(x, m) if (!(x)) hdsp_die(__FILE__, __LINE__, m);
#define hdsp_test_output_vector_with_newline(v, v_len) \
    for (int i = 0; i < v_len; i++) {fprintf(stderr, "[%d]:%d\n",i,v[i]);}
#define hdsp_test_output_vector_with_tab(v, v_len) \
    for (int i = 0; i < v_len; i++) {fprintf(stderr, "[%d]:%d\t",i,v[i]);if(i + 1 == v_len){fprintf(stderr,"\n");}}
#define hdsp_test_vectors_equal(a, b, len) \
    for (int i = 0; i < len; i++) {if (a[i] != b[i]) {fprintf(stderr, "[%d]:%d!=%d\n",i,a[i],b[i]);exit(EXIT_FAILURE);}}
#define HDSP_EQUAL_DOUBLES(a,b) (fabs((a) - (b)) <= DBL_EPSILON)
#define HDSP_EQUAL_ALMOST_DOUBLES(a,b) (fabs((a) - (b)) <= HDSP_DOUBLE_ALMOST_EPSILON)
#define hdsp_test_output_vector_with_newline_double(v, v_len) \
    for (int i = 0; i < v_len; i++) {fprintf(stderr, "[%d]:%f\n",i,v[i]);}
#define hdsp_test_output_vector_with_tab_double(v, v_len) \
    for (int i = 0; i < v_len; i++) {fprintf(stderr, "[%d]:%f\t",i,v[i]);if(i + 1 == v_len){fprintf(stderr,"\n");}}
#define hdsp_test_vectors_equal_double(a, b, len) \
    for (int i = 0; i < len; i++) {if (!HDSP_EQUAL_DOUBLES(a[i],b[i])) {fprintf(stderr, "[%d]:%f!=%f\n",i,a[i],b[i]);exit(EXIT_FAILURE);}}
#define hdsp_test_vectors_equal_almost_double(a, b, len) \
    for (int i = 0; i < len; i++) {if (!HDSP_EQUAL_ALMOST_DOUBLES(a[i],b[i])) {fprintf(stderr, "[%d]:%f!=%f\n",i,a[i],b[i]);exit(EXIT_FAILURE);}}

#ifdef __cplusplus
}
#endif

#endif /* LHDSP_LHDSP_H */