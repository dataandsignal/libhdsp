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
 * test6.c - Test hdsp_hamming (a Hammng window)
 */


#include "hdsp.h"

int main(int argc, char **argv) {

    #define W3_LEN 3
    #define W4_LEN 4

    // Input

    // Output
    double w3[W3_LEN] = {0,0,0};
    double w4[W4_LEN] = {0};

    // Reference, using MATLAB's hamming():
    // hamming(3)
    double w3_ref[W3_LEN] = {0.08, 1, 0.08};
    // hamming(4)
    double w4_ref[W4_LEN] = {0.08, 0.77, 0.77, 0.08};

    hdsp_test_output_vector_with_newline_double(w3, W3_LEN);

    // Test N=3
    hdsp_hamming(w3, W3_LEN);
    fprintf(stderr, "N=3:\n");
    hdsp_test_output_vector_with_newline_double(w3, W3_LEN);

    hdsp_test_vectors_equal_double(w3, w3_ref, W3_LEN);

    return 0;
}