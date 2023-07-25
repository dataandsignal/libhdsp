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
 * by idx_start and idx_end (that point inside y, both).
 *      x - (in) input signal
 *      x_len - (in) input frame length in samples
 *      h - (in) input filter
 *      h_len - (in) number of filter's coefficients
 *      type - convolution type (HDSP_CONV_TYPE_FULL, HDSP_CONV_TYPE_SAME, HDSP_CONV_TYPE_VALID)
 *      y - (out) output, must point to a valid memory of at least sizeof(int16_t)*(x_len + h_len - 1) bytes.
 *      Full-length convolution is written to y.
 *      idx_start - (out) index of first element in y for the required convolution type (must point to uint16_t),
 *      starting from 0
 *      idx_end - (out) index of last element in y for the required convolution type (must point to uint16_t),
 *      starting from 0
 * Returns the number of elements written to y (x_len + h_len - 1).
 */
uint16_t hdsp_conv(int16_t *x, uint16_t x_len, int16_t *h, uint16_t h_len, hdsp_conv_type_t type,
                   int16_t *y, uint16_t *idx_start, uint16_t *idx_end);

/**
 * Filter frame x with filter.
 */
hdsp_status_t hdsp_filter(int16_t *x, size_t x_len, hdsp_filter_t *fltr, int16_t *y, size_t *y_len);

/* Tests */

void hdsp_die(const char *file, int line, const char *s);

#define hdsp_test(x, m) if (!(x)) hdsp_die(__FILE__, __LINE__, m);
#define hdsp_test_output_vector_with_newline(v, v_len) \
    for (int i = 0; i < v_len; i++) {fprintf(stderr, "[%d]:%d\n",i,v[i]);}
#define hdsp_test_output_vector_with_tab(v, v_len) \
    for (int i = 0; i < v_len; i++) {fprintf(stderr, "[%d]:%d\t",i,v[i]);if(i + 1 == v_len){fprintf(stderr,"\n");}}
#define hdsp_test_vectors_equal(a, b, len) \
    for (int i = 0; i < len; i++) {if (a[i] != b[i]) {fprintf(stderr, "[%d]:%d!=%d\n",i,a[i],b[i]);exit(EXIT_FAILURE);}}

#ifdef __cplusplus
}
#endif

#endif /* LHDSP_LHDSP_H */