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
 * hdsptool.c - Tool for testing some of the libhdsp features
 *
 * Command 'upsample' will test upsampling, filtering, downsampling of input file.
 * Syntax is:
 *      ./hdsptool upsample <input file raw> <input file sample rate> <frame ptime ms>
 * Input file needs to be in raw PCM 16 bit format and sampling rate must be 8 or 16 kHz.
 * Target output sampling rate is 48 kHz. Arbitrary ptime is supported.
 * 4 files will be created to test the process:
 *      PTms_x.raw - copy of the input, 16 bit PCM, 8 or 16 kHz (as the input)
 *      PTms_x_u.raw - upsampled to 48 kHz, 16 bit PCM
 *      PTms_x_u_f.raw - zero-phase filtered with minimum length Kaiser filter
 *          of passband frequency equal to half of the input file sampling rate (4 or 8 kHz), filter fixes
 *          frequency aliasing effect that is a result of upsampling
 *      PTms_x_u_f_d.raw - upsampled, filtered and downsampled to original sampling rate (8 or 16 kHz)
 * PTms_ will resolve to <ptime ms>ms_ where ptime is passed as a command parameter.
 */


#include "hdsp.h"

#define PROGRAM_NAME argv[0]
#define TARGET_SAMPLE_RATE 48000
#define SAMPLES_PER_10MS_FRAME_OF_48000HZ 480
#define BUFLEN 2000

static void usage(const char *name) {
    if (name == NULL)
        return;

    fprintf(stderr, "\nusage:\t %s upsample <input file raw> <input file sample rate> <ptime ms>\n\n", name);
}

int main(int argc, char **argv) {

    const char *cmd = NULL;
    const char *f_in_name = NULL, *f_out_name = NULL;
    int sample_rate_in = 0;
    int ptime_ms = 0;
    int samples_in = 0;
    int samples_per_ptime_of_48khz_frame = 0;
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
    char fname_x[BUFLEN] = {0};
    char fname_x_u[BUFLEN] = {0};
    char fname_x_u_f[BUFLEN] = {0};
    char fname_x_u_f_d[BUFLEN] = {0};

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
    sample_rate_in = atoi(argv[3]);
    ptime_ms = atoi(argv[4]);

    upsample_factor = TARGET_SAMPLE_RATE / sample_rate_in;
    samples_in = ptime_ms * sample_rate_in / 1000;
    samples_per_ptime_of_48khz_frame = samples_in * upsample_factor;

    snprintf(fname_x, BUFLEN - 1, "%dms_x.raw", ptime_ms);
    snprintf(fname_x_u, BUFLEN - 1, "%dms_x_u.raw", ptime_ms);
    snprintf(fname_x_u_f, BUFLEN - 1, "%dms_x_u_f.raw", ptime_ms);
    snprintf(fname_x_u_f_d, BUFLEN - 1, "%dms_x_u_f_d.raw", ptime_ms);

    f_in = fopen(f_in_name, "rb");
    f_out_x = fopen(fname_x, "wb");
    f_out_x_upsampled = fopen(fname_x_u, "wb");
    f_out_x_upsampled_filtered = fopen(fname_x_u_f, "wb");
    f_out_x_upsampled_filtered_downsampled = fopen(fname_x_u_f_d, "wb");

    if (sample_rate_in != 8000 && sample_rate_in != 16000) {
        fprintf(stderr, "Only 8000 and 16000 sampling rate is supported\n");
        goto fail;
    }

    if (!f_in || !f_out_x || !f_out_x_upsampled || !f_out_x_upsampled_filtered || !f_out_x_upsampled_filtered_downsampled) {
        fprintf(stderr, "Cannot open files\n");
        goto fail;
    }

    if (HDSP_STATUS_OK != hdsp_fir_filter_init_lowpass_kaiser_opt(&filter, 48000, sample_rate_in / 2)) {
        fprintf(stderr, "Failed to create filter\n");
        goto fail;
    }

    printf("sampling rate=%d, frame ms=%d, frame samples=%d, upsampling factor=%d\n", sample_rate_in, ptime_ms, samples_in, upsample_factor);

    while (samples_in == (n = fread(frame_in, sizeof(int16_t), samples_in, f_in))) {
        int m = 0;
        int16_t buffer[TARGET_SAMPLE_RATE] = {0};

        k = k + 1;
        n_total = n_total + n;

        // write input
        if (samples_in < fwrite(frame_in, sizeof(int16_t), samples_in, f_out_x)) {
            fprintf(stderr, "Failed to write x (input)\n");
            goto fail;
        }

        // process frame_in
        if (HDSP_STATUS_OK != hdsp_upsample_int16(frame_in, samples_in, upsample_factor, buffer, samples_per_ptime_of_48khz_frame)) {
            fprintf(stderr, "Failed to upsample\n");
            goto fail;
        }

        // write upsampled
        if (samples_per_ptime_of_48khz_frame < fwrite(buffer, sizeof(int16_t), samples_per_ptime_of_48khz_frame, f_out_x_upsampled)) {
            fprintf(stderr, "Failed to write\n");
            goto fail;
        }

        if (HDSP_STATUS_OK != hdsp_fir_filter(buffer, samples_per_ptime_of_48khz_frame, &filter, frame_out, samples_per_ptime_of_48khz_frame)) {
            fprintf(stderr, "Failed to filter\n");
            goto fail;
        }

        m = 0;
        while (m < samples_per_ptime_of_48khz_frame) {
            buffer[m] = (int16_t) frame_out[m];
            m = m + 1;
        }

        // write upsampled, filtered
        if (samples_per_ptime_of_48khz_frame < fwrite(buffer, sizeof(int16_t), samples_per_ptime_of_48khz_frame, f_out_x_upsampled_filtered)) {
            fprintf(stderr, "Failed to write\n");
            goto fail;
        }

        if (HDSP_STATUS_OK != hdsp_downsample_double(frame_out, samples_per_ptime_of_48khz_frame, upsample_factor, frame_out_downsampled, samples_in)) {
            fprintf(stderr, "Failed to downsample\n");
            goto fail;
        }

        m = 0;
        while (m < samples_in) {
            buffer[m] = (int16_t) frame_out_downsampled[m];
            m = m + 1;
        }

        // write upsampled, filtered, downsampled
        if (samples_in < fwrite(buffer, sizeof(int16_t), samples_in, f_out_x_upsampled_filtered_downsampled)) {
            fprintf(stderr, "Failed to write\n");
            goto fail;
        }

        // printf("Frame %d (bytes total: %zu)\n", k, n_total * sizeof(int16_t));
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
