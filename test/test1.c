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
 * test1.c - Test upsampling
 */


#include "hdsp.h"

int main(int argc, char **argv) {

    #define Fs_x 8000
    #define f_x 200
    #define Fs_y 48000
    #define FRAME_LEN_MS 20
    #define X_LEN_SAMPLES (FRAME_LEN_MS * Fs_x / 1000)
    #define Y_LEN_SAMPLES (FRAME_LEN_MS * Fs_y / 1000)
    #define UPSAMPLE_FACTOR (Fs_y / Fs_x)

    int16_t x[X_LEN_SAMPLES] = {0};
    int16_t y[Y_LEN_SAMPLES] = {0};

    int i = 0;
    while (i < X_LEN_SAMPLES) {
        x[i] = 100 * sin((double)i * 2 * M_PI * f_x / Fs_x);
        printf("%d\n",x[i]);
        i = i + 1;
    }

    hdsp_test(HDSP_STATUS_OK == hdsp_upsample(x, X_LEN_SAMPLES, UPSAMPLE_FACTOR, y, Y_LEN_SAMPLES), "It did not work\n");

    i = 0;
    while (i < X_LEN_SAMPLES)
    {
        int j = 0;
        while (j < UPSAMPLE_FACTOR) {
            if (j == 0) {
                hdsp_test(y[UPSAMPLE_FACTOR * i + j] == x[i], "Wrong sample");
            } else {
                hdsp_test(y[UPSAMPLE_FACTOR * i + j] == 0, "Wrong zero");
            }
            j = j + 1;
        }
        i = i + 1;
    }
    return 0;
}