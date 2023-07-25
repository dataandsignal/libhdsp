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


#include "hdsp.h"

int main(int argc, char **argv) {

    #define X_LEN 8
    #define H_LEN 3

    // Input
    int16_t x[X_LEN] = {0, 1, 2, 3, 4, 5, 6, 7};
    int16_t h[H_LEN] = {0, 1, 2};

    // Output
    int16_t y[X_LEN + H_LEN - 1] = {0};
    int16_t z[X_LEN + H_LEN - 1] = {0};
    uint16_t idx_start = 0;
    uint16_t idx_end = 0;

    // Reference, using MATLAB's conv:
    // x = x(1:8)
    // h = 0 : 1 : 2
    // conv(x,h,"full")
    int16_t ref_conv_full[X_LEN + H_LEN - 1] = {0, 0, 1, 4, 7, 10, 13, 16, 19, 14};
    // conv(x,h,"same")
    int16_t ref_conv_same[X_LEN] = {0, 1, 4, 7, 10, 13, 16, 19};
    // conv(x,h,"valid")
    int16_t ref_conv_valid[6] = {1, 4, 7, 10, 13, 16};

    fprintf(stderr, "x:\n");
    hdsp_test_output_vector_with_newline(x,X_LEN);
    fprintf(stderr, "h:\n");
    hdsp_test_output_vector_with_newline(h,H_LEN);

    // Test x*h 'full'
    hdsp_test(X_LEN + H_LEN - 1 == hdsp_conv(x, X_LEN, h, H_LEN, HDSP_CONV_TYPE_FULL,
                                             y, &idx_start, &idx_end), "Conv x*h 'full' did not work");
    fprintf(stderr, "y:\n");
    hdsp_test_output_vector_with_newline(y, X_LEN + H_LEN - 1);

    hdsp_test_vectors_equal(y, ref_conv_full, X_LEN + H_LEN - 1);
    hdsp_test(idx_start == 0, "Wrong value for idx start");
    hdsp_test(idx_end == X_LEN + H_LEN - 2, "Wrong value for idx end");

    // Test x*h 'same'
    hdsp_test(X_LEN + H_LEN - 1 == hdsp_conv(x, X_LEN, h, H_LEN, HDSP_CONV_TYPE_SAME,
                                             y, &idx_start, &idx_end), "Conv x*h 'same' did not work");
    fprintf(stderr, "y:\n");
    hdsp_test_output_vector_with_newline(y, X_LEN + H_LEN - 1);

    hdsp_test(idx_start == 1, "Wrong value for idx start");
    hdsp_test(idx_end == 8, "Wrong value for idx end");
    memcpy(z, &y[idx_start], sizeof(int16_t) *(idx_end - idx_start + 1));
    hdsp_test_vectors_equal(z, ref_conv_same, idx_end - idx_start + 1);

    // Test x*h 'valid'
    hdsp_test(X_LEN + H_LEN - 1 == hdsp_conv(x, X_LEN, h, H_LEN, HDSP_CONV_TYPE_VALID,
                                             y, &idx_start, &idx_end), "Conv x*h 'valid' did not work");
    fprintf(stderr, "y:\n");
    hdsp_test_output_vector_with_newline(y, X_LEN + H_LEN - 1);

    hdsp_test(idx_start == 2, "Wrong value for idx start");
    hdsp_test(idx_end == 7, "Wrong value for idx end");
    memcpy(z, &y[idx_start], sizeof(int16_t) *(idx_end - idx_start + 1));
    hdsp_test_vectors_equal(z, ref_conv_valid, idx_end - idx_start + 1);

    return 0;
}