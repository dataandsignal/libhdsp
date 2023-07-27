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
 * test6.c - Test hdsp_kaiser_window (a Kaiser window)
 */


#include "hdsp.h"

int main(int argc, char **argv) {

    // MATLAB: besseli(0,10)
    #define BESSEL_0_10 2815.716628
    #define W3_LEN 3
    #define W4_LEN 4
    #define W74_LEN 74
    #define W75_LEN 75

    // Input

    // Output
    double w3[W3_LEN] = {0};
    double w4[W4_LEN] = {0};
    double w74[W74_LEN] = {0};
    double w75[W75_LEN] = {0};

    // Reference, using MATLAB's kaiser(3,10):
    // kaiser(3,10).'
    double w3_ref[W3_LEN] = {0.00035515,1.00000000,0.00035515};
    // kaiser(4,10).'
    double w4_ref[W4_LEN] = {0.00035515,0.58181016,0.58181016,0.00035515};
    // kaiser(74,10),'
    double w74_ref[W74_LEN] = {
            0.00035515,0.00102357,0.00215324,0.00391210,0.00649706,0.01013280,
            0.01506917,0.02157699,0.02994233,0.04045922,0.05342105,0.06911060,
            0.08778920,0.10968525,0.13498249,0.16380846,0.19622369,0.23221199,
            0.27167241,0.31441323,0.36014843,0.40849695,0.45898490,0.51105091,
            0.56405458,0.61728795,0.66998973,0.72136184,0.77058803,0.81685375,
            0.85936684,0.89737823,0.93020208,0.95723457,0.97797069,0.99201851,
            0.99911032,0.99911032,0.99201851,0.97797069,0.95723457,0.93020208,
            0.89737823,0.85936684,0.81685375,0.77058803,0.72136184,0.66998973,
            0.61728795,0.56405458,0.51105091,0.45898490,0.40849695,0.36014843,
            0.31441323,0.27167241,0.23221199,0.19622369,0.16380846,0.13498249,
            0.10968525,0.08778920,0.06911060,0.05342105,0.04045922,0.02994233,
            0.02157699,0.01506917,0.01013280,0.00649706,0.00391210,0.00215324,
            0.00102357,0.00035515};
    // kaiser(75,10).'
    double w75_ref[W75_LEN] = {
            0.00035515,0.00101181,0.00211526,0.00382615,0.00633260,0.00984910,
            0.01461414,0.02088640,0.02893950,0.03905535,0.05151614,0.06659526,
            0.08454720,0.10559693,0.12992890,0.15767622,0.18891043,0.22363212,
            0.26176313,0.30314043,0.34751224,0.39453671,0.44378317,0.49473638,
            0.54680356,0.59932426,0.65158286,0.70282340,0.75226632,0.79912669,
            0.84263329,0.88204811,0.91668539,0.94592978,0.96925292,0.98622786,
            0.99654083,1.00000000,0.99654083,0.98622786,0.96925292,0.94592978,
            0.91668539,0.88204811,0.84263329,0.79912669,0.75226632,0.70282340,
            0.65158286,0.59932426,0.54680356,0.49473638,0.44378317,0.39453671,
            0.34751224,0.30314043,0.26176313,0.22363212,0.18891043,0.15767622,
            0.12992890,0.10559693,0.08454720,0.06659526,0.05151614,0.03905535,
            0.02893950,0.02088640,0.01461414,0.00984910,0.00633260,0.00382615,
            0.00211526,0.00101181,0.00035515};

    double v = 0.0;


    // Test modified Bessel 1st kind at order n = 0
    v = hdsp_modified_bessel_1st_kind_zero(10);
    hdsp_test(HDSP_EQUAL_ALMOST_DOUBLES(BESSEL_0_10,v), "Wrong Bessel value");

    // Test N=3
    hdsp_kaiser_window(w3, W3_LEN, 10);
    fprintf(stderr, "N=3:\n");
    hdsp_test_output_vector_with_newline_double(w3, W3_LEN);
    hdsp_test_vectors_equal_almost_double(w3, w3_ref, W3_LEN);

    // Test N=4
    hdsp_kaiser_window(w4, W4_LEN, 10);
    fprintf(stderr, "N=4:\n");
    hdsp_test_output_vector_with_newline_double(w4, W4_LEN);
    hdsp_test_vectors_equal_almost_double(w4, w4_ref, W4_LEN);

    // Test N=74
    hdsp_kaiser_window(w74, W74_LEN, 10);
    fprintf(stderr, "N=74:\n");
    hdsp_test_output_vector_with_newline_double(w74, W74_LEN);
    hdsp_test_vectors_equal_almost_double(w74, w74_ref, W74_LEN);

    // Test N=75
    hdsp_kaiser_window(w75, W75_LEN, 10);
    fprintf(stderr, "N=75:\n");
    hdsp_test_output_vector_with_newline_double(w75, W75_LEN);
    hdsp_test_vectors_equal_almost_double(w75, w75_ref, W75_LEN);


    return 0;
}