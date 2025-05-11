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

double hdsp_factorial[HDSP_FACTORIAL_MAX + 1] = {
    1.0,1.0,2.0,6.0,24.0,120.0,720.0,
    5040.0,40320.0,362880.0,3628800.0,39916800.0,
    479001600.0,6227020800.0,87178291200.0,1307674368000.0,
    20922789888000.0,355687428096000.0,6402373705728000.0,
    121645100408832000.0,2432902008176640000.0,51090942171709440000.0,
    1124000727777607680000.0,25852016738884978212864.0,620448401733239409999872.0,
    15511210043330986055303168.0,403291461126605650322784256.0,10888869450418351940239884288.0,
    304888344611713836734530715648.0,8841761993739700772720181510144.0,
    265252859812191032188804700045312.0,8222838654177922430198509928972288.0,
    263130836933693517766352317727113216.0,8683317618811885938715673895318323200.0,
    295232799039604119555149671006000381952.0,10333147966386144222209170348167175077888.0,
    371993326789901177492420297158468206329856.0,13763753091226343102992036262845720547033088.0,
    523022617466601037913697377988137380787257344.0,20397882081197441587828472941238084160318341120.0,
    815915283247897683795548521301193790359984930816.0
};

/**
* Returns value of the modified Bessel function of the first kind for argument x, I_zero(x).
* Value is approximated using series with k = 0 to HDSP_FACTORIAL_MAX.
* https://mathworld.wolfram.com/ModifiedBesselFunctionoftheFirstKind.html
*/
double hdsp_modified_bessel_1st_kind_zero(double x);

double hdsp_fir_ls_57_4000_48000[HDSP_FIR_LS_KAISER_57_4000_48000_LEN] = {
    0.0113680544, 0.0087507903, 0.0013115806, -0.0074163364,
    -0.0129434981, -0.0120838026, -0.0047224621, 0.0058555156,
    0.0143650910, 0.0160818140, 0.0093579393, -0.0031950720,
    -0.0155910860, -0.0211838542, -0.0160349323, -0.0013134718,
    0.0165850766, 0.0285383186, 0.0269203527, 0.0097338420,
    -0.0173172952, -0.0420341698, -0.0500548749, -0.0307423907,
    0.0177656798, 0.0865681269, 0.1581475346, 0.2120603162,
    0.2320833333, 0.2120603162, 0.1581475346, 0.0865681269,
    0.0177656798, -0.0307423907, -0.0500548749, -0.0420341698,
    -0.0173172952, 0.0097338420, 0.0269203527, 0.0285383186,
    0.0165850766, -0.0013134718, -0.0160349323, -0.0211838542,
    -0.0155910860, -0.0031950720, 0.0093579393, 0.0160818140,
    0.0143650910, 0.0058555156, -0.0047224621, -0.0120838026,
    -0.0129434981, -0.0074163364, 0.0013115806, 0.0087507903,
    0.0113680544
};
double hdsp_fir_ls_kaiser_57_4000_48000[HDSP_FIR_LS_KAISER_57_4000_48000_LEN] = {
    0.0002317719, 0.0002933296, 0.0000653322, -0.0005152687,
    -0.0011996204, -0.0014443471, -0.0007086233, 0.0010787318,
    0.0031880267, 0.0042290905, 0.0028736999, -0.0011307916,
    -0.0062838011, -0.0096164325, -0.0081152050, -0.0007340415,
    0.0101430054, 0.0189368961, 0.0192239944, 0.0074220577,
    -0.0139930822, -0.0357312183, -0.0444435302, -0.0283134534,
    0.0168563899, 0.0840521946, 0.1560909305, 0.2113680921,
    0.2320833333, 0.2113680921, 0.1560909305, 0.0840521946,
    0.0168563899, -0.0283134534, -0.0444435302, -0.0357312183,
    -0.0139930822, 0.0074220577, 0.0192239944, 0.0189368961,
    0.0101430054, -0.0007340415, -0.0081152050, -0.0096164325,
    -0.0062838011, -0.0011307916, 0.0028736999, 0.0042290905,
    0.0031880267, 0.0010787318, -0.0007086233, -0.0014443471,
    -0.0011996204, -0.0005152687, 0.0000653322, 0.0002933296,
    0.0002317719
};

double hdsp_fir_ls_75_8000_48000[HDSP_FIR_LS_KAISER_75_8000_48000_LEN] = {
    0.0064470000, -0.0031510000, -0.0090940000, -0.0032450000,
    0.0072950000, 0.0087370000, -0.0014250000, -0.0103550000,
    -0.0060070000, 0.0067210000, 0.0113520000, 0.0010500000,
    -0.0114920000, -0.0095530000, 0.0054830000, 0.0144520000,
    0.0046390000, -0.0124730000, -0.0143570000, 0.0032040000,
    0.0184320000, 0.0101630000, -0.0132680000, -0.0216090000,
    -0.0010510000, 0.0244100000, 0.0199630000, -0.0138540000,
    -0.0352210000, -0.0105390000, 0.0368160000, 0.0442490000,
    -0.0142130000, -0.0788640000, -0.0502370000, 0.1047500000,
    0.2979960000, 0.3856670000, 0.2979960000, 0.1047500000,
    -0.0502370000, -0.0788640000, -0.0142130000, 0.0442490000,
    0.0368160000, -0.0105390000, -0.0352210000, -0.0138540000,
    0.0199630000, 0.0244100000, -0.0010510000, -0.0216090000,
    -0.0132680000, 0.0101630000, 0.0184320000, 0.0032040000,
    -0.0143570000, -0.0124730000, 0.0046390000, 0.0144520000,
    0.0054830000, -0.0095530000, -0.0114920000, 0.0010500000,
    0.0113520000, 0.0067210000, -0.0060070000, -0.0103550000,
    -0.0014250000, 0.0087370000, 0.0072950000, -0.0032450000,
    -0.0090940000, -0.0031510000, 0.0064470000
};
double hdsp_fir_ls_kaiser_75_8000_48000[HDSP_FIR_LS_KAISER_75_8000_48000_LEN] = {
    0.0001314414, -0.0000946699, -0.0003771918, -0.0001777816,
    0.0005110697, 0.0007632490, -0.0001520994, -0.0013277041,
    -0.0009119127, 0.0011928743, 0.0023294453, 0.0002466303,
    -0.0030619300, -0.0028634022, 0.0018347777, 0.0053608132,
    0.0018948703, -0.0055752556, -0.0069812104, 0.0016853716,
    0.0104324659, 0.0061577390, -0.0085634654, -0.0147862994,
    -0.0007589469, 0.0185192198, 0.0158431490, -0.0114528143,
    -0.0302037540, -0.0093372058, 0.0335644092, 0.0413488592,
    -0.0135605853, -0.0765310018, -0.0493965138, 0.1039680895,
    0.2974386077, 0.3856670000, 0.2974386077, 0.1039680895,
    -0.0493965138, -0.0765310018, -0.0135605853, 0.0413488592,
    0.0335644092, -0.0093372058, -0.0302037540, -0.0114528143,
    0.0158431490, 0.0185192198, -0.0007589469, -0.0147862994,
    -0.0085634654, 0.0061577390, 0.0104324659, 0.0016853716,
    -0.0069812104, -0.0055752556, 0.0018948703, 0.0053608132,
    0.0018347777, -0.0028634022, -0.0030619300, 0.0002466303,
    0.0023294453, 0.0011928743, -0.0009119127, -0.0013277041,
    -0.0001520994, 0.0007632490, 0.0005110697, -0.0001777816,
    -0.0003771918, -0.0000946699, 0.0001314414
};

hdsp_status_t hdsp_upsample_int16(int16_t *x, size_t x_len, int upsample_factor, int16_t *y, size_t y_len)
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

hdsp_status_t hdsp_downsample_int16(int16_t *x, size_t x_len, int downsample_factor, int16_t *y, size_t y_len)
{
    if (!x || x_len < 1 || downsample_factor < 1 || !y || y_len < 1) {
        return HDSP_STATUS_FALSE;
    }

    if (x_len / downsample_factor != y_len) {
        return HDSP_STATUS_FALSE;
    }

    if (downsample_factor == 1) {
        memcpy(y, x, x_len * sizeof(int16_t));
        return HDSP_STATUS_OK;
    }

    size_t i = 0;
    size_t j = 0;
    while (i < x_len)
    {
        y[j] = x[i];
        j = j + 1;
        i = i + downsample_factor;
    }

    return HDSP_STATUS_OK;
}

hdsp_status_t hdsp_downsample_double(double *x, size_t x_len, int downsample_factor, double *y, size_t y_len)
{
    if (!x || x_len < 1 || downsample_factor < 1 || !y || y_len < 1) {
        return HDSP_STATUS_FALSE;
    }

    if (x_len / downsample_factor != y_len) {
        return HDSP_STATUS_FALSE;
    }

    if (downsample_factor == 1) {
        memcpy(y, x, x_len * sizeof(double));
        return HDSP_STATUS_OK;
    }

    size_t i = 0;
    size_t j = 0;
    while (i < x_len)
    {
        y[j] = x[i];
        j = j + 1;
        i = i + downsample_factor;
    }

    return HDSP_STATUS_OK;
}

hdsp_status_t hdsp_downsample_float(float *x, size_t x_len, int downsample_factor, float *y, size_t y_len)
{
    if (!x || x_len < 1 || downsample_factor < 1 || !y || y_len < 1) {
        return HDSP_STATUS_FALSE;
    }

    if (x_len / downsample_factor != y_len) {
        return HDSP_STATUS_FALSE;
    }

    if (downsample_factor == 1) {
        memcpy(y, x, x_len * sizeof(float));
        return HDSP_STATUS_OK;
    }

    size_t i = 0;
    size_t j = 0;
    while (i < x_len)
    {
        y[j] = x[i];
        j = j + 1;
        i = i + downsample_factor;
    }

    return HDSP_STATUS_OK;
}

void hdsp_int16_2_float(int16_t *x, size_t x_len, float *y)
{
    size_t k = 0;
    while (k < x_len) {
        y[k] = (float) x[k];
        k = k + 1;
    }
}

void hdsp_double_2_int16(double *x, size_t x_len, int16_t *y)
{
    size_t k = 0;
    while (k < x_len) {
        y[k] = (int16_t) x[k];
        k = k + 1;
    }
}

void hdsp_double_2_float(double *x, size_t x_len, float *y)
{
    size_t k = 0;
    while (k < x_len) {
        y[k] = (float) x[k];
        k = k + 1;
    }
}

void hdsp_float_2_int16(float *x, size_t x_len, int16_t *y)
{
    size_t k = 0;
    while (k < x_len) {
        y[k] = (int16_t) x[k];
        k = k + 1;
    }
}

#define DEBUG 0
uint16_t hdsp_conv_full(int16_t *x, uint16_t x_len, double *h, uint16_t h_len, double *y)
{
    uint32_t t = 0, tau = 0;
    uint32_t c_len = x_len + h_len - 1;
    uint32_t tau_min = 0, tau_max = 0;

    if (!x || !h || !y || x_len < 1 || h_len < 1) {
        return 0;
    }

    while (t < c_len) {

        y[t] = 0.0;
        tau_min = (t < h_len - 1) ? 0 : (t - (h_len - 1));
        tau_max = (t < h_len - 1) ? hdsp_min(x_len - 1, t) : t;

        tau_max = hdsp_min(tau_max, x_len - 1);

        if (DEBUG) {
            fprintf(stderr, "t,tau_min,tau_max: %u/%u/%u\n", t, tau_min, tau_max);
        }

        for (tau = tau_min; tau <= tau_max; tau++) {
            int16_t x_v = x[tau];
            double h_v = h[t-tau];
            y[t] += x_v * h_v;
            if (DEBUG) {
                fprintf(stderr, "-> t=%u,tau_min=%u,tau_max=%u\ttau=%u: x[%u]=%d h[%u]=%f y[%u]=%f\n",
                        t, tau_min, tau_max, tau, tau, x_v, t - tau, h_v, t, y[t]);
            }
        }

        t = t + 1;
    }

    return t;
}

uint16_t hdsp_conv(int16_t *x, uint16_t x_len, double *h, uint16_t h_len, hdsp_conv_type_t type,
                   double *y, int32_t *idx_start, int32_t *idx_end)
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

void hdsp_hamming_window(double *w, uint16_t n)
{
    uint16_t half = 0;
    int i = 0;

    if (n % 2) {
        // odd length window
        half = (n + 1) / 2;
    } else {
        // even length window
        half = floor((double)n / 2);
    }

    while (i < half) {
        w[i] = 0.54 - 0.46 * cos(2 * M_PI * ((double) i / (double) (n - 1)));
        w[n - 1 - i] = w[i];
        i = i + 1;
    }
}

void hdsp_kaiser_window(double *w, uint16_t n, double beta)
{
    double bes = fabs(hdsp_modified_bessel_1st_kind_zero(beta));
    uint16_t half = 0;
    int i = 0;

    if (n % 2) {
        // odd length window
        half = (n + 1) / 2;
    } else {
        // even length window
        half = floor((double)n / 2);
    }

    while (i < half) {
        w[i] = hdsp_modified_bessel_1st_kind_zero(beta *
                sqrt(1 - ((2 * (double)(i) / (double)(n - 1)) - 1) * ((2 * (double)(i) / (double)(n - 1)) - 1))) / bes;
        w[n - 1 - i] = w[i];
        i = i + 1;
    }
}

double hdsp_kaiser_beta(double attenuation_db)
{
    return 0.1102 * (attenuation_db - 8.7) * (attenuation_db > 50.0 ? 1 : 0) +
        (0.5842 * pow(attenuation_db - 21.0, 0.4) + 0.07886 * (attenuation_db - 21.0))
            * (attenuation_db >= 21.0 && attenuation_db <= 50.0 ? 1 : 0);
}

void hdsp_design_kaiser_n_beta(uint16_t passband_freq, uint16_t fs_hz, double stopband_attenuation_db, double passband_ripple_db,
                               uint16_t *n, double *beta)
{
    double passband_freq_normalized = (double) passband_freq / ((double)fs_hz / 2.0);
    double stopband_attenuation_linear = HDSP_KAISER_FILTER_STOPBAND_ATTENUATION_DB_TO_LINEAR(stopband_attenuation_db);
    double passband_ripple_linear = HDSP_KAISER_FILTER_PASSBAND_RIPPLE_DB_TO_LINEAR(passband_ripple_db);
    double tw_percentage = -0.98 * HDSP_KAISER_FILTER_STEEPNES + 0.99;
    double tw = tw_percentage * (1.0 - passband_freq_normalized);
    double stopband_freq_normalized = passband_freq_normalized + tw;
    //double stopband_freq = stopband_freq_normalized * ((double)fs_hz / 2.0);

    double passband_freq_normalized_2pi = passband_freq_normalized / 2.0;
    double stopband_freq_normalized_2pi = stopband_freq_normalized / 2.0;

    double delta = hdsp_min(passband_ripple_linear, stopband_attenuation_linear);
    double attenuation_db = -20.0 * log10(delta);
    double D = (attenuation_db - 7.95) / (2.0 * M_PI * 2.285);   // 7.95 was in Kaiser's original paper
    double df = fabs(stopband_freq_normalized_2pi - passband_freq_normalized_2pi);

    if (DEBUG) {
        fprintf(stderr, "->passband_ripple_linear: %f\n", passband_ripple_linear);
        fprintf(stderr, "->stopband_attenuation_linear: %f\n", stopband_attenuation_linear);
        fprintf(stderr, "->attenuation_db: %f\n", attenuation_db);
    }

    if (n) {
        if (DEBUG) {
            fprintf(stderr, "->D: %f\n", D);
            fprintf(stderr, "->df: %f\n", df);
        }
        *n = ceil(D / df + 1);
    }

    if (beta) {
        *beta = hdsp_kaiser_beta(attenuation_db);
    }
}

double hdsp_sinc(double x, uint16_t fs_hz) {
    if (fabs(x) < 1.0 / (double)fs_hz) {
        return 1.0;
    }

    return sin(M_PI * x) / (M_PI * x);
}

hdsp_status_t hdsp_fir_filter_init_lowpass_by_spectrum_sampling(hdsp_filter_t *filter, size_t n,
                                                                uint16_t fs_hz, uint16_t passband_freq_hz) {

    int32_t k = 0;
    int32_t L2 = (n - 1) / 2;

    if (!filter || (n > HDSP_FIR_FILTER_LEN_MAX) || (passband_freq_hz > fs_hz)) {
        return HDSP_STATUS_FALSE;
    }

    memset(filter, 0, sizeof(*filter));

    while (k < n) {
        filter->b[k] = (2.0 * passband_freq_hz / (double)fs_hz)
                * hdsp_sinc(2.0 * passband_freq_hz * (k - L2) / (double)fs_hz, fs_hz);
        k = k + 1;
    }

    filter->b_len = n;
    filter->passband_freq_hz = passband_freq_hz;
    filter->fs_hz = fs_hz;
    filter->design_method = HDSP_FILTER_DESIGN_METHOD_SPECTRUM_SAMPLING;

    return HDSP_STATUS_OK;
}

hdsp_status_t hdsp_fir_filter_init_lowpass_by_ls(hdsp_filter_t *filter, size_t n,
                                                                uint16_t fs_hz, uint16_t passband_freq_hz) {
    if (!filter || (n > HDSP_FIR_FILTER_LEN_MAX) || (passband_freq_hz > fs_hz)) {
        return HDSP_STATUS_FALSE;
    }

    memset(filter, 0, sizeof(*filter));

    if (n == HDSP_FIR_LS_KAISER_57_4000_48000_LEN && fs_hz == 48000 && passband_freq_hz == 4000) {
        memcpy(filter->b, hdsp_fir_ls_57_4000_48000,
               HDSP_FIR_LS_KAISER_57_4000_48000_LEN * sizeof(hdsp_fir_ls_57_4000_48000[0]));
    } else if (n == HDSP_FIR_LS_KAISER_75_8000_48000_LEN && fs_hz == 48000 && passband_freq_hz == 8000) {
        memcpy(filter->b, hdsp_fir_ls_75_8000_48000,
               HDSP_FIR_LS_KAISER_75_8000_48000_LEN * sizeof(hdsp_fir_ls_75_8000_48000[0]));
    } else {
        return HDSP_STATUS_FALSE;
    }

    filter->b_len = n;
    filter->passband_freq_hz = passband_freq_hz;
    filter->fs_hz = fs_hz;
    filter->design_method = HDSP_FILTER_DESIGN_METHOD_LEAST_SQUARES;

    return HDSP_STATUS_OK;
}

hdsp_status_t hdsp_fir_filter_init_lowpass(hdsp_filter_t *filter, size_t n,
                                           uint16_t fs_hz, uint16_t passband_freq_hz,
                                           hdsp_filter_design_method_t method)
{
    switch (method) {
        case HDSP_FILTER_DESIGN_METHOD_LEAST_SQUARES:
            return hdsp_fir_filter_init_lowpass_by_ls(filter, n, fs_hz, passband_freq_hz);
        case HDSP_FILTER_DESIGN_METHOD_SPECTRUM_SAMPLING:
        default:
            return hdsp_fir_filter_init_lowpass_by_spectrum_sampling(filter, n, fs_hz, passband_freq_hz);
    }
}

hdsp_status_t hdsp_fir_filter_shape(hdsp_filter_t *filter, double *w, uint16_t w_len)
{
    int32_t k = 0;

    if (!filter || !w || filter->b_len != w_len) {
        return HDSP_STATUS_FALSE;
    }

    while (k < w_len) {
        filter->b[k] *= w[k];
        k = k + 1;
    }
    return HDSP_STATUS_OK;
}

hdsp_status_t hdsp_fir_filter_init_lowpass_kaiser_opt(hdsp_filter_t *filter, uint16_t fs_hz, uint16_t passband_freq_hz)
{
    uint16_t n = 0;
    double beta = 0.0;
    double w[HDSP_FIR_LS_KAISER_57_4000_48000_LEN + HDSP_FIR_LS_KAISER_75_8000_48000_LEN] = {0};

    if (!filter || (passband_freq_hz > fs_hz)) {
        return HDSP_STATUS_FALSE;
    }

    memset(filter, 0, sizeof(*filter));

    if (fs_hz == 48000 && passband_freq_hz == 4000) {
        hdsp_design_kaiser_n_beta(4000, 48000, HDSP_KAISER_FILTER_STOPBAND_ATTENUATION_DB, HDSP_KAISER_FILTER_PASSBAND_RIPPLE_DB, &n, &beta);
        n = HDSP_FIR_LS_KAISER_57_4000_48000_LEN;
        hdsp_kaiser_window(w, n, beta);
    } else if (fs_hz == 48000 && passband_freq_hz == 8000) {
        hdsp_design_kaiser_n_beta(8000, 48000, HDSP_KAISER_FILTER_STOPBAND_ATTENUATION_DB, HDSP_KAISER_FILTER_PASSBAND_RIPPLE_DB, &n, &beta);
        n = HDSP_FIR_LS_KAISER_75_8000_48000_LEN;
        hdsp_kaiser_window(w, n, beta);
    } else {
        return HDSP_STATUS_FALSE;
    }

    hdsp_kaiser_window(w, n, beta);
    if (HDSP_STATUS_OK != hdsp_fir_filter_init_lowpass_by_ls(filter, n, fs_hz, passband_freq_hz)) {
        return HDSP_STATUS_FALSE;
    }
    if (HDSP_STATUS_OK != hdsp_fir_filter_shape(filter, w, n)) {
        return HDSP_STATUS_FALSE;
    }

    return HDSP_STATUS_OK;
}

hdsp_status_t hdsp_fir_filter(int16_t *x, size_t x_len, hdsp_filter_t *filter, double *y, size_t y_len)
{
    int32_t idx_start = 0;
    int32_t idx_end = 0;
    double *y_tmp = NULL;

    if (!x || x_len == 0 || !filter || filter->b_len == 0 || !y || y_len < x_len) {
        return HDSP_STATUS_FALSE;
    }

    y_tmp = malloc(sizeof(double)*(x_len + filter->b_len - 1));
    if (!y_tmp) {
        goto fail;
    }
    memset(y_tmp, 0, sizeof(double)*(x_len + filter->b_len - 1));

    if (x_len + filter->b_len - 1 != hdsp_conv(x, x_len, filter->b, filter->b_len,
                                               HDSP_CONV_TYPE_SAME, y_tmp, &idx_start, &idx_end)) {
        goto fail;
    }

    memcpy(y, &y_tmp[idx_start], sizeof(y_tmp[0]) *(idx_end - idx_start));

    free(y_tmp);
    y_tmp = NULL;

    return HDSP_STATUS_OK;

fail:
    if (y_tmp) {
        free(y_tmp);
        y_tmp = NULL;
    }
    return HDSP_STATUS_FALSE;
}

double hdsp_modified_bessel_1st_kind_zero(double x)
{
    int k = 0;
    double v = 0.0, nominator = 1.0, denominator = 1.0;
    double factor = 1.0/4.0 * x * x;
    while (k < HDSP_FACTORIAL_MAX) {
        nominator *= (k > 0 ? factor : 1);
        denominator = hdsp_factorial[k] * hdsp_factorial[k];
        v += nominator / denominator;
        k = k + 1;
    }
    return v;
}

void hdsp_die(const char *file, int line, const char *reason)
{
    fprintf(stderr, "Failure: %s:%d %s\n", file, line, reason);
    exit(EXIT_FAILURE);
}
