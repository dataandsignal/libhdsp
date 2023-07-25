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

    // Reference, using MATLAB's conv:
    // x = x(1:8)
    // h = 0 : 1 : 2
    //conv(x,h,"full")
    int16_t ref[X_LEN + H_LEN - 1] = {0, 0, 1, 4, 7, 10, 13, 16, 19, 14};

    fprintf(stderr, "x:\n");
    hdsp_test_output_vector_with_newline(x,X_LEN);
    fprintf(stderr, "h:\n");
    hdsp_test_output_vector_with_newline(h,H_LEN);

    hdsp_test(X_LEN + H_LEN - 1 == hdsp_conv_full(x, X_LEN, h, H_LEN, y), "It did not work\n");
    fprintf(stderr, "y:\n");
    hdsp_test_output_vector_with_newline(y, X_LEN + H_LEN - 1);

    hdsp_test_vectors_equal(y, ref, X_LEN + H_LEN - 1);

    return 0;
}