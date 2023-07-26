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
    #define W256_LEN 256

    // Input

    // Output
    double w3[W3_LEN] = {0};
    double w4[W4_LEN] = {0};
    double w256[W256_LEN] = {0};

    // Reference, using MATLAB's hamming():
    // hamming(3).'
    double w3_ref[W3_LEN] = {0.08, 1, 0.08};
    // hamming(4).'
    double w4_ref[W4_LEN] = {0.08, 0.77, 0.77, 0.08};
    // hamming(256).'
    double w256_ref[W256_LEN] = {
            0.080000, 0.080140, 0.080558, 0.081256, 0.082232, 0.083487,0.085018,
            0.086825, 0.088908, 0.091264, 0.093893, 0.096793, 0.099962, 0.103398,
            0.107099, 0.111063, 0.115287, 0.119769, 0.124506, 0.129496, 0.134734,
            0.140219, 0.145946, 0.151913, 0.158115, 0.164549, 0.171211, 0.178097,
            0.185203, 0.192524, 0.200056, 0.207794, 0.215734, 0.223871, 0.232200,
            0.240716, 0.249413, 0.258287, 0.267332, 0.276542, 0.285912, 0.295437,
            0.305110, 0.314925, 0.324878, 0.334960, 0.345168, 0.355493, 0.365931,
            0.376474, 0.387117, 0.397852, 0.408674, 0.419575, 0.430550, 0.441591,
            0.452691, 0.463845, 0.475045, 0.486285, 0.497557, 0.508854, 0.520171,
            0.531500, 0.542834, 0.554166, 0.565489, 0.576797, 0.588083, 0.599340,
            0.610560, 0.621738, 0.632866, 0.643938, 0.654946, 0.665885, 0.676747,
            0.687527, 0.698217, 0.708810, 0.719302, 0.729684, 0.739951, 0.750097,
            0.760115, 0.770000, 0.779745, 0.789345, 0.798793, 0.808084, 0.817212,
            0.826172, 0.834958, 0.843565, 0.851988, 0.860222, 0.868261, 0.876100,
            0.883736, 0.891163, 0.898377, 0.905373, 0.912148, 0.918696,
            0.925015, 0.931100, 0.936947, 0.942554, 0.947916, 0.953030,
            0.957894, 0.962504, 0.966858, 0.970952, 0.974785, 0.978353,
            0.981656, 0.984690, 0.987455, 0.989948, 0.992168, 0.994113,
            0.995782, 0.997175, 0.998290, 0.999128, 0.999686, 0.999965,
            0.999965, 0.999686, 0.999128, 0.998290, 0.997175, 0.995782,
            0.994113, 0.992168, 0.989948, 0.987455, 0.984690, 0.981656,
            0.978353, 0.974785, 0.970952, 0.966858, 0.962504, 0.957894,
            0.953030, 0.947916, 0.942554, 0.936947, 0.931100, 0.925015,
            0.918696, 0.912148, 0.905373, 0.898377, 0.891163, 0.883736,
            0.876100, 0.868261, 0.860222, 0.851988, 0.843565, 0.834958,
            0.826172, 0.817212, 0.808084, 0.798793, 0.789345, 0.779745,
            0.770000, 0.760115, 0.750097, 0.739951, 0.729684, 0.719302,
            0.708810, 0.698217, 0.687527, 0.676747, 0.665885, 0.654946,
            0.643938, 0.632866, 0.621738, 0.610560, 0.599340, 0.588083,
            0.576797, 0.565489, 0.554166, 0.542834, 0.531500, 0.520171,
            0.508854, 0.497557, 0.486285, 0.475045, 0.463845, 0.452691,
            0.441591, 0.430550, 0.419575, 0.408674, 0.397852, 0.387117,
            0.376474, 0.365931, 0.355493, 0.345168, 0.334960, 0.324878,
            0.314925, 0.305110, 0.295437, 0.285912, 0.276542, 0.267332,
            0.258287, 0.249413, 0.240716, 0.232200, 0.223871, 0.215734,
            0.207794, 0.200056, 0.192524, 0.185203, 0.178097, 0.171211,
            0.164549, 0.158115, 0.151913, 0.145946, 0.140219, 0.134734,
            0.129496, 0.124506, 0.119769, 0.115287, 0.111063, 0.107099,
            0.103398, 0.099962, 0.096793, 0.093893, 0.091264, 0.088908,
            0.086825, 0.085018, 0.083487, 0.082232, 0.081256, 0.080558,
            0.080140, 0.080000};

    hdsp_test_output_vector_with_newline_double(w3, W3_LEN);

    // Test N=3
    hdsp_hamming(w3, W3_LEN);
    fprintf(stderr, "N=3:\n");
    hdsp_test_output_vector_with_newline_double(w3, W3_LEN);
    hdsp_test_vectors_equal_double(w3, w3_ref, W3_LEN);

    // Test N=4
    hdsp_hamming(w4, W4_LEN);
    fprintf(stderr, "N=4:\n");
    hdsp_test_output_vector_with_newline_double(w4, W4_LEN);
    hdsp_test_vectors_equal_double(w4, w4_ref, W4_LEN);

    // Test N=256
    hdsp_hamming(w256, W256_LEN);
    printf("%f, %f, %d, %d, %d\n", DBL_EPSILON, fabs(w256[1] - w256_ref[1]), DBL_EPSILON < fabs(w256[1] - w256_ref[1]), DBL_EPSILON == fabs(w256[1] - w256_ref[1]), DBL_EPSILON > fabs(w256[1] - w256_ref[1]));
    fprintf(stderr, "N=256:\n");
    hdsp_test_output_vector_with_newline_double(w256, W256_LEN);
    hdsp_test_vectors_equal_almost_double(w256, w256_ref, W256_LEN);

    return 0;
}