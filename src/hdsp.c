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

uint16_t hdsp_conv_full(int16_t *x, uint16_t x_len, int16_t *h, uint16_t h_len, int16_t *y)
{
    int16_t t = 0, tau = 0;
    int16_t c_len = x_len + h_len - 1;
    int16_t tau_min = 0, tau_max = 0;

    if (!x || !h || !y || x_len < 1 || h_len < 1) {
        return 0;
    }

    while (t < c_len) {

        y[t] = 0;

        // When x_len > h_len

        tau_min = (t < h_len - 1) ? 0 : t - (h_len - 1);
        tau_max = (t < h_len - 1) ? hdsp_min(x_len - 1, t) : t;
        tau_min = hdsp_max(tau_min, t - (h_len - 1));
        tau_max = hdsp_min(tau_max, x_len - 1);

        fprintf(stderr, "t,tau_min,tau_max: %d/%d/%d\n", t, tau_min, tau_max);

        for (tau = tau_min; tau <= tau_max; tau++) {
            int16_t x_v = x[tau];
            int16_t h_v = h[t-tau];
            y[t] += x_v * h_v;
            fprintf(stderr, "-> t=%d,tau_min=%d,tau_max=%d\ttau=%d: x[%d]=%d h[%d]=%d y[%d]=%d\n", t, tau_min, tau_max, tau, tau, x_v, t-tau, h_v, t, y[t]);
        }

        t++;
    }

    return t;
}

uint16_t hdsp_conv(int16_t *x, uint16_t x_len, int16_t *h, uint16_t h_len, hdsp_conv_type_t type,
                   int16_t *y, int32_t *idx_start, int32_t *idx_end)
{
    uint16_t n = 0;

    n = hdsp_conv_full(x, x_len, h, h_len, y);
    if (n != x_len + h_len - 1) {
        return n;
    }
    switch (type) {
        case HDSP_CONV_TYPE_SAME:
            *idx_start = floor(h_len/2);
            *idx_end = *idx_start + x_len;
            break;
        case HDSP_CONV_TYPE_VALID:
            if (x_len >= h_len) {
                *idx_start = h_len - 1;
                *idx_end = x_len;
            } else {
                *idx_start = -1;
                *idx_end = -1;
            }
            break;
        case HDSP_CONV_TYPE_FULL:
        default:
            *idx_start = 0;
            *idx_end = *idx_start + n - 1;
            break;
    }
    return n;
}

hdsp_status_t hdsp_filter(int16_t *x, size_t x_len, hdsp_filter_t *fltr, int16_t *y, size_t *y_len)
{
    return HDSP_STATUS_OK;
}

void hdsp_die(const char *file, int line, const char *reason)
{
    fprintf(stderr, "Failure: %s:%d %s\n", file, line, reason);
    exit(EXIT_FAILURE);
}