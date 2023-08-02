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

#define PROGRAM_NAME argv[0]
#define TARGET_SAMPLE_RATE 48000

static void usage(const char *name) {
    if (name == NULL)
        return;

    fprintf(stderr, "\nusage:\t %s upsample <input file raw> <input file sample rate> <output file upsampled raw>\n\n", name);
}

int main(int argc, char **argv) {

    const char *cmd = NULL;
    const char *f_in_name = NULL, *f_out_name = NULL;
    int sample_rate_in = 0;
    FILE *f_in = NULL;
    FILE *f_out_x = NULL, *f_out_x_upsampled = NULL,
        *f_out_x_upsampled_filtered = NULL, *f_out_x_upsampled_filtered_downsampled = NULL;
    size_t n = 0, n_total = 0;
    int16_t frame_in[TARGET_SAMPLE_RATE] = {0};
    double frame_out[TARGET_SAMPLE_RATE] = {0};
    double frame_out_downsampled[TARGET_SAMPLE_RATE] = {0};
    int k = 0;
    hdsp_filter_t filter = {0};
    int upsample_factor = 0;

    if (argc != 5) {
        usage(PROGRAM_NAME);
        exit(EXIT_FAILURE);
    }

    cmd = argv[1];
    if (strcmp(cmd, "upsample") != 0) {
        usage(PROGRAM_NAME);
        exit(EXIT_FAILURE);
    }

    f_in_name = argv[2];
    f_out_name = argv[4];
    sample_rate_in = atoi(argv[3]);
    upsample_factor = TARGET_SAMPLE_RATE / sample_rate_in;

    f_in = fopen(f_in_name, "rb");
    f_out_x = fopen("x.raw", "wb");
    f_out_x_upsampled = fopen("x_upsampled.raw", "wb");
    f_out_x_upsampled_filtered = fopen("x_upsampled_filtered.raw", "wb");
    f_out_x_upsampled_filtered_downsampled = fopen("x_upsampled_filtered_downsampled.raw", "wb");

    if (!f_in || !f_out_x || !f_out_x_upsampled || !f_out_x_upsampled_filtered || !f_out_x_upsampled_filtered_downsampled) {
        fprintf(stderr, "Cannot open files\n");
        goto fail;
    }

    if (sample_rate_in != 8000 && sample_rate_in != 16000) {
        fprintf(stderr, "Only 8000 and 16000 sampling rate is supported\n");
        goto fail;
    }

    if (HDSP_STATUS_OK != hdsp_fir_filter_init_lowpass_kaiser_opt(&filter, 48000, sample_rate_in / 2)) {
        fprintf(stderr, "Failed to create filter\n");
        goto fail;
    }

    while (sample_rate_in == (n = fread(frame_in, sizeof(int16_t), sample_rate_in, f_in))) {
        int m = 0;
        int16_t buffer[TARGET_SAMPLE_RATE] = {0};

        k = k + 1;
        n_total = n_total + n;

        // write input
        if (sample_rate_in < fwrite(frame_in, sizeof(int16_t), sample_rate_in, f_out_x)) {
            fprintf(stderr, "Failed to write x (input)\n");
            goto fail;
        }

        // process frame_in
        if (HDSP_STATUS_OK != hdsp_upsample_int16(frame_in, sample_rate_in, upsample_factor, buffer, TARGET_SAMPLE_RATE)) {
            fprintf(stderr, "Failed to upsample\n");
            goto fail;
        }

        // write upsampled
        if (TARGET_SAMPLE_RATE < fwrite(buffer, sizeof(int16_t), TARGET_SAMPLE_RATE, f_out_x_upsampled)) {
            fprintf(stderr, "Failed to write\n");
            goto fail;
        }

        if (HDSP_STATUS_OK != hdsp_fir_filter(buffer, TARGET_SAMPLE_RATE, &filter, frame_out, TARGET_SAMPLE_RATE)) {
            fprintf(stderr, "Failed to filter\n");
            goto fail;
        }

        m = 0;
        while (m < TARGET_SAMPLE_RATE) {
            buffer[m] = (int16_t) frame_out[m];
            m = m + 1;
        }

        // write upsampled, filtered
        if (TARGET_SAMPLE_RATE < fwrite(buffer, sizeof(int16_t), TARGET_SAMPLE_RATE, f_out_x_upsampled_filtered)) {
            fprintf(stderr, "Failed to write\n");
            goto fail;
        }

        if (HDSP_STATUS_OK != hdsp_downsample_double(frame_out, TARGET_SAMPLE_RATE, upsample_factor, frame_out_downsampled, sample_rate_in)) {
            fprintf(stderr, "Failed to downsample\n");
            goto fail;
        }

        m = 0;
        while (m < sample_rate_in) {
            buffer[m] = (int16_t) frame_out_downsampled[m];
            m = m + 1;
        }

        // write upsampled, filtered, downsampled
        if (TARGET_SAMPLE_RATE < fwrite(buffer, sizeof(int16_t), sample_rate_in, f_out_x_upsampled_filtered_downsampled)) {
            fprintf(stderr, "Failed to write\n");
            goto fail;
        }

        printf("Frame %d (bytes total: %zu)\n", k, n_total * sizeof(int16_t));
    }

    printf("Done. (frames: %d, bytes total: %zu)\n", k, n_total * sizeof(int16_t));
    return 0;

fail:
    if (f_in) {
        fclose(f_in);
    }
    if (f_out_x) {
        fclose(f_out_x);
    }
    if (f_out_x_upsampled) {
        fclose(f_out_x_upsampled);
    }
    if (f_out_x_upsampled_filtered) {
        fclose(f_out_x_upsampled_filtered);
    }
    if (f_out_x_upsampled_filtered_downsampled) {
        fclose(f_out_x_upsampled_filtered_downsampled);
    }
    exit(EXIT_FAILURE);
}
