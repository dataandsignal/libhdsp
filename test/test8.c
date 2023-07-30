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
 * test8.c - Test FIR lowpass filter
 */


#include "hdsp.h"

int main(int argc, char **argv) {

    #define PASSBAND_FREQ_HZ 8000
    #define FS_HZ 48000
    #define NUMBER_OF_POINTS 71

    // Input

    // Output
    hdsp_filter_t filter = {0};


    // Reference, using MATLAB's x_fir(8000, 48000, 71, 0)
    double filter_ref[NUMBER_OF_POINTS] = {
            -0.007876, -0.008108, -0.000000, 0.008615, 0.008892, -0.000000, -0.009506,
            -0.009845, 0.000000, 0.010602, 0.011027, -0.000000, -0.011985, -0.012530,
            -0.000000, 0.013783, 0.014509, 0.000000, -0.016216, -0.017229,
            -0.000000, 0.019690, 0.021205, 0.000000, -0.025060, -0.027566,
            -0.000000, 0.034458, 0.039381, -0.000000, -0.055133, -0.068916,
            -0.000000, 0.137832, 0.275664, 0.333333, 0.275664, 0.137832,
            -0.000000, -0.068916, -0.055133, -0.000000, 0.039381, 0.034458,
            -0.000000, -0.027566, -0.025060, 0.000000, 0.021205, 0.019690,
            -0.000000, -0.017229, -0.016216, 0.000000, 0.014509, 0.013783,
            -0.000000, -0.012530, -0.011985, -0.000000, 0.011027, 0.010602,
            0.000000, -0.009845, -0.009506, -0.000000, 0.008892, 0.008615,
            -0.000000, -0.008108, -0.007876
    };

    double v = 0.0;
    uint16_t n = 0;
    double beta = 0.0;


    // 1. Test implementation methods

    // Test lowpass unwindowed filter created by sampling the spectrum
    hdsp_test(HDSP_STATUS_OK == hdsp_fir_filter_init_lowpass_by_spectrum_sampling(&filter, NUMBER_OF_POINTS,
                                                                                  FS_HZ, PASSBAND_FREQ_HZ),
              "Filter initialisation failed");
    hdsp_test(filter.b_len == NUMBER_OF_POINTS, "Wrong filter length");
    hdsp_test(filter.design_method == HDSP_FILTER_DESIGN_METHOD_SPECTRUM_SAMPLING, "Wrong design method");
    fprintf(stderr, "b:\n");
    hdsp_test_output_vector_with_newline_double(filter.b, filter.b_len);
    hdsp_test_vectors_equal_almost_double(filter.b, filter_ref, NUMBER_OF_POINTS);

    // Test lowpass unwindowed filter created by least squares method
    hdsp_test(HDSP_STATUS_FALSE == hdsp_fir_filter_init_lowpass_by_ls(&filter, 256, FS_HZ, PASSBAND_FREQ_HZ),
              "Filter initialisation should fail");

    // Fs=48000, Wpass=4000
    hdsp_test(HDSP_STATUS_OK == hdsp_fir_filter_init_lowpass_by_ls(&filter, HDSP_FIR_LS_KAISER_57_4000_48000_LEN,
                                                                      FS_HZ, 4000),
              "Filter initialisation failed");
    hdsp_test(filter.b_len == HDSP_FIR_LS_KAISER_57_4000_48000_LEN, "Wrong filter length");
    hdsp_test(filter.design_method == HDSP_FILTER_DESIGN_METHOD_LEAST_SQUARES, "Wrong design method");
    fprintf(stderr, "b:\n");
    hdsp_test_output_vector_with_newline_double(filter.b, filter.b_len);
    hdsp_test_vectors_equal_almost_double(filter.b, hdsp_fir_ls_kaiser_57_4000_48000, filter.b_len);

    // Fs=48000, Wpass=8000
    hdsp_test(HDSP_STATUS_OK == hdsp_fir_filter_init_lowpass_by_ls(&filter, HDSP_FIR_LS_KAISER_75_8000_48000_LEN,
                                                                   FS_HZ, 8000),
              "Filter initialisation failed");
    hdsp_test(filter.b_len == HDSP_FIR_LS_KAISER_75_8000_48000_LEN, "Wrong filter length");
    hdsp_test(filter.design_method == HDSP_FILTER_DESIGN_METHOD_LEAST_SQUARES, "Wrong design method");
    fprintf(stderr, "b:\n");
    hdsp_test_output_vector_with_newline_double(filter.b, filter.b_len);
    hdsp_test_vectors_equal_almost_double(filter.b, hdsp_fir_ls_kaiser_75_8000_48000, filter.b_len);

    // Test lowpass windowed filter created by least squares method

    // Fs=48000, Wpass=4000
    hdsp_test(HDSP_STATUS_OK == hdsp_fir_filter_init_lowpass_by_ls(&filter, HDSP_FIR_LS_KAISER_57_4000_48000_LEN,
                                                                   FS_HZ, 4000),
              "Filter initialisation failed");

    // Fs=48000, Wpass=8000
    hdsp_test(HDSP_STATUS_OK == hdsp_fir_filter_init_lowpass_by_ls(&filter, HDSP_FIR_LS_KAISER_75_8000_48000_LEN,
                                                                   FS_HZ, 8000),
              "Filter initialisation failed");

    // 2. Test interface

    // Test lowpass unwindowed filter created by sampling the spectrum
    hdsp_test(HDSP_STATUS_OK == hdsp_fir_filter_init_lowpass(&filter, NUMBER_OF_POINTS, FS_HZ, PASSBAND_FREQ_HZ,
                                                             HDSP_FILTER_DESIGN_METHOD_SPECTRUM_SAMPLING),
              "Filter initialisation failed");
    hdsp_test(filter.b_len == NUMBER_OF_POINTS, "Wrong filter length");
    hdsp_test(filter.design_method == HDSP_FILTER_DESIGN_METHOD_SPECTRUM_SAMPLING, "Wrong design method");
    fprintf(stderr, "b:\n");
    hdsp_test_output_vector_with_newline_double(filter.b, filter.b_len);
    hdsp_test_vectors_equal_almost_double(filter.b, filter_ref, NUMBER_OF_POINTS);

    // Test lowpass unwindowed filter created by least squares method
    hdsp_test(HDSP_STATUS_FALSE == hdsp_fir_filter_init_lowpass(&filter, 256, FS_HZ, PASSBAND_FREQ_HZ,
                                                                      HDSP_FILTER_DESIGN_METHOD_LEAST_SQUARES),
              "Filter initialisation should fail");

    // Fs=48000, Wpass=4000
    hdsp_test(HDSP_STATUS_OK == hdsp_fir_filter_init_lowpass(&filter, HDSP_FIR_LS_KAISER_57_4000_48000_LEN, FS_HZ,
                                                                   4000, HDSP_FILTER_DESIGN_METHOD_LEAST_SQUARES),
              "Filter initialisation failed");
    hdsp_test(filter.b_len == HDSP_FIR_LS_KAISER_57_4000_48000_LEN, "Wrong filter length");
    hdsp_test(filter.design_method == HDSP_FILTER_DESIGN_METHOD_LEAST_SQUARES, "Wrong design method");
    fprintf(stderr, "b:\n");
    hdsp_test_output_vector_with_newline_double(filter.b, filter.b_len);
    hdsp_test_vectors_equal_almost_double(filter.b, hdsp_fir_ls_kaiser_57_4000_48000, filter.b_len);

    // Fs=48000, Wpass=8000
    hdsp_test(HDSP_STATUS_OK == hdsp_fir_filter_init_lowpass(&filter, HDSP_FIR_LS_KAISER_75_8000_48000_LEN, FS_HZ,
                                                                   8000, HDSP_FILTER_DESIGN_METHOD_LEAST_SQUARES),
              "Filter initialisation failed");
    hdsp_test(filter.b_len == HDSP_FIR_LS_KAISER_75_8000_48000_LEN, "Wrong filter length");
    hdsp_test(filter.design_method == HDSP_FILTER_DESIGN_METHOD_LEAST_SQUARES, "Wrong design method");
    fprintf(stderr, "b:\n");
    hdsp_test_output_vector_with_newline_double(filter.b, filter.b_len);
    hdsp_test_vectors_equal_almost_double(filter.b, hdsp_fir_ls_kaiser_75_8000_48000, filter.b_len);

    return 0;
}