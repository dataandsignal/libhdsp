/*
 * This file is part of libhdsp - Handy DSP routines library.
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

hdsp_status_t hdsp_upsample(int16_t *x, size_t x_len, int upsample_factor, int16_t *y, size_t y_len)
{
    if (!x || x_len < 1 || upsample_factor < 1 || !y || y_len < 1) {
        return HDSP_STATUS_FALSE;
    }

    if (x_len * upsample_factor != y_len) {
        return HDSP_STATUS_FALSE;
    }

    if (upsample_factor == 1) {
        memcpy(y, x, x_len * sizeof(int16_t));
        return HDSP_STATUS_OK;
    }

    size_t i = 0;
    while (i < x_len)
    {
        size_t j = 0;
        while (j < upsample_factor) {
            if (j == 0) {
                y[upsample_factor * i + j] = x[i];
            } else {
                y[upsample_factor * i + j] = 0;
            }
            j = j + 1;
        }
        i = i + 1;
    }

    return HDSP_STATUS_OK;
}

hdsp_status_t hdsp_filter(int16_t *x, size_t x_len, hdsp_filter_t *fltr, int16_t *y, size_t *y_len)
{
    return HDSP_STATUS_OK;
}