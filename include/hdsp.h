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


#ifndef LHDSP_LHDSP_H
#define LHDSP_LHDSP_H


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include <math.h>
#include <float.h>

#define HDSP_FIR_FILTER_LEN_MAX 4096
#define HDSP_KAISER_FILTER_STOPBAND_ATTENUATION_DB 60
#define HDSP_KAISER_FILTER_PASSBAND_RIPPLE 0.1
#define HDSP_KAISER_FILTER_STEEPNES 0.85
#define HDSP_KAISER_FILTER_STOPBAND_ATTENUATION_DB_TO_LINEAR(x) pow(10.0,(double)(-x)/20.0)
#define HDSP_KAISER_FILTER_PASSBAND_RIPPLE_DB_TO_LINEAR(x) \
    (pow(10.0, ((double)(x)/20.0)) - 1.0)/(pow(10.0,((double)(x)/20.0)) + 1.0)

enum hdsp_conv_type {
    HDSP_CONV_TYPE_FULL,
    HDSP_CONV_TYPE_SAME,
    HDSP_CONV_TYPE_VALID
};
typedef enum hdsp_conv_type hdsp_conv_type_t;

enum hdsp_filter_design_method {
    HDSP_FILTER_DESIGN_METHOD_SPECTRUM_SAMPLING,
    HDSP_FILTER_DESIGN_METHOD_LEAST_SQUARES
};
typedef enum hdsp_filter_design_method hdsp_filter_design_method_t;

#define hdsp_min(x, y) ((x) < (y) ? (x) : (y))
#define hdsp_max(x, y) ((x) < (y) ? (y) : (x))

enum hdsp_status {
    HDSP_STATUS_OK,
    HDSP_STATUS_FALSE
};
typedef enum hdsp_status hdsp_status_t;

struct hdsp_filter {
    double a[HDSP_FIR_FILTER_LEN_MAX]; // nominator
    size_t a_len;
    double b[HDSP_FIR_FILTER_LEN_MAX]; // denominator
    size_t b_len;
    uint16_t passband_freq_hz; // Passband frequency in Hertz
    uint16_t fs_hz; // Sampling rate in Hz
    hdsp_filter_design_method_t design_method;
};
typedef struct hdsp_filter hdsp_filter_t;

/**
 * Upsample by zero insertion.
 *      x - (in) input frame
 *      x_len - (in) input frame length in samples
 *      upsample_factor - (in) upsampling factor equal to ratio of output frequency to input frequency (Fy / Fx)
 *      y - (in/out) upsampled frame, memory should be pre-allocated
 *      y_len - (in) upsampled frame's length in samples, should be x_len*upsample_factor
 * Returns HDSP_STATUS_OK on success, HDSP_STATUS_FALSE on error.
 */
hdsp_status_t hdsp_upsample(int16_t *x, size_t x_len, int upsample_factor, int16_t *y, size_t y_len);

/**
 * Compute full-length convolution of input signal x and filter h: x*h=Sum{x[tau]h[t-tau]}.
 * Result is of length x_len + h_len - 1, written to a vector pointed to by y (y must be allocated).
 *      x - (in) input signal
 *      x_len - (in) input signal length in samples
 *      h - (in) input filter
 *      h_len - (in) number of filter's coefficients
 *      y - (out) output, must point to a valid memory of at least sizeof(int16_t)*(x_len + h_len - 1) bytes
 * Returns the number of elements written to y (x_len + h_len - 1).
 */
uint16_t hdsp_conv_full(int16_t *x, uint16_t x_len, int16_t *h, uint16_t h_len, int16_t *y);

/**
 * Compute convolution of input signal x and filter h: x*h=Sum{x[tau]h[t-tau]}.
 * Convolution types match those from MATLAB's conv function https://www.mathworks.com/help/fixedpoint/ref/conv.html
 * Convolution types:
 *  full - (HDSP_CONV_TYPE_FULL) full-length convolution, (x_len + h_len - 1) elements
 *  same - (HDSP_CONV_TYPE_SAME) central part of full-length convolution truncated to length of x
 *          This implements zero-phase filtering (filter delay compensation).
 *  valid - (HDSP_CONV_TYPE_VALID) segment of full-length convolution elements, which have been computed without
 *          a need to zero-pad (missing) elements, i.e. only such elements for which computation all coefficients
 *          of filter h were multiplied by samples of x (none of the coefficients of h fallen outside of x)
 * Result is a full-length convolution of size x_len + h_len - 1, written to a vector pointed to by y
 * (y must be allocated). A part of the full-length convolution determined by convolution type is defined
 * by idx_start and idx_end.
 *      x - (in) input signal
 *      x_len - (in) input frame length in samples
 *      h - (in) input filter
 *      h_len - (in) number of filter's coefficients
 *      type - convolution type (HDSP_CONV_TYPE_FULL, HDSP_CONV_TYPE_SAME, HDSP_CONV_TYPE_VALID)
 *      y - (out) output, must point to a valid memory of at least sizeof(int16_t)*(x_len + h_len - 1) bytes
 *      Full-length convolution is written to y.
 *      idx_start - (out) index of first element in y for the required convolution type (must point to int32_t),
 *      starting from 0
 *      idx_end - (out) index of one past the last element in y for the required convolution type
 *      (must point to int32_t), starting from 0
 * Returns the number of elements written to y (x_len + h_len - 1) and indices pointing to first and one past the last
 * element of convolution result for a required convolution type. In case of 'valid' convolution type a result vector
 * may be empty, which is signalled by idx_start == -1 and idx_end == -1.
 */
uint16_t hdsp_conv(int16_t *x, uint16_t x_len, int16_t *h, uint16_t h_len, hdsp_conv_type_t type,
                   int16_t *y, int32_t *idx_start, int32_t *idx_end);

/**
 * Create N-point symmetric Hamming window.
 *      w - (out) result (must point to a valid memory of at least sizeof(double)*n bytes
 *      n - (in) number of points
 */
void hdsp_hamming_window(double *w, uint16_t n);

/**
 * Create N-point symmetric Kaiser window.
 *      w - (out) result (must point to a valid memory of at least sizeof(double)*n bytes
 *      n - (in) number of points
 *      beta - beta coefficient that determines the shape of the Kaiser window. In the frequency domain,
 *      it determines the trade-off between main-lobe width and side lobe level
 */
void hdsp_kaiser_window(double *w, uint16_t n, double beta);

/**
 *
 * Design beta for lowpass Kaiser filter at desired attenuation (dB);
 */
double hdsp_kaiser_beta(double attenuation_db);

/**
 * Design optimal beta and number of points (filter length) for lowpass Kaiser filter characterised by desired values
 * of passband frequency in Hz, stopband attenuation in dB and passband ripple in dB, at a given sampling rate.
 *      passband_freq - (in) passband frequency of the lowpass filter in Hz
 *      fs - (in) sampling frequency in Hz
 *      stopband_attenuation_db - (in) stopband attenuation in dB
 *      passband_ripple_db - (in) passband ripple in dB
 *      n - (out) number of points (filter length)
 *      beta - (out) Kaiser beta
 */
void hdsp_design_kaiser_n_beta(uint16_t passband_freq, uint16_t fs, double stopband_attenuation_db,
                               double passband_ripple_db, uint16_t *n, double *beta);

/**
 * Returns sinc(x) value.
 */
double hdsp_sinc(double x, uint16_t fs_hz);

/**
 * Initializes filter to Finite Impulse Response lowpass filter by sampling inverse of the spectrum
 * of perfect rectangle lowpass filter.
 * Impulse response isn't windowed, use Hamming or Kaiser window to shape filter.
 */
hdsp_status_t hdsp_fir_filter_init_lowpass_by_spectrum_sampling(hdsp_filter_t *filter, size_t n,
                                                                uint16_t fs_hz, uint16_t passband_freq_hz);

/**
 * Initializes filter to Finite Impulse Response lowpass filter by a least squares method.
 * Impulse response isn't windowed, use Hamming or Kaiser window to shape filter.
 */
hdsp_status_t hdsp_fir_filter_init_lowpass_by_ls(hdsp_filter_t *filter, size_t n,
                                                 uint16_t fs_hz, uint16_t passband_freq_hz);

/**
 * Initializes filter to Finite Impulse Response lowpass filter by a specified design method.
 * Impulse response isn't windowed, use Hamming or Kaiser window to shape filter.
 */
hdsp_status_t hdsp_fir_filter_init_lowpass(hdsp_filter_t *filter, size_t n,
                                           uint16_t fs_hz, uint16_t passband_freq_hz,
                                           hdsp_filter_design_method_t method);
/**
 * Apply a window to a FIR filter.
 */
hdsp_status_t hdsp_fir_filter_shape(hdsp_filter_t *filter, double *w, uint16_t w_len);

/**
 * Filter frame x with filter.
 */
hdsp_status_t hdsp_filter(int16_t *x, size_t x_len, hdsp_filter_t *fltr, double *y, size_t *y_len);

#define HDSP_FACTORIAL_MAX 40
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

#define HDSP_FIR_LS_KAISER_57_4000_48000_LEN 57u
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

#define HDSP_FIR_LS_KAISER_75_8000_48000_LEN 75u
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

/* Tests */

void hdsp_die(const char *file, int line, const char *s);

#define HDSP_DOUBLE_ALMOST_EPSILON 0.000001
#define hdsp_test(x, m) if (!(x)) hdsp_die(__FILE__, __LINE__, m);
#define hdsp_test_output_vector_with_newline(v, v_len) \
    for (int i = 0; i < v_len; i++) {fprintf(stderr, "[%d]:%d\n",i,v[i]);}
#define hdsp_test_output_vector_with_tab(v, v_len) \
    for (int i = 0; i < v_len; i++) {fprintf(stderr, "[%d]:%d\t",i,v[i]);if(i + 1 == v_len){fprintf(stderr,"\n");}}
#define hdsp_test_vectors_equal(a, b, len) \
    for (int i = 0; i < len; i++) {if (a[i] != b[i]) {fprintf(stderr, "[%d]:%d!=%d\n",i,a[i],b[i]);exit(EXIT_FAILURE);}}
#define HDSP_EQUAL_DOUBLES(a,b) (fabs((a) - (b)) <= DBL_EPSILON)
#define HDSP_EQUAL_ALMOST_DOUBLES(a,b) (fabs((a) - (b)) <= HDSP_DOUBLE_ALMOST_EPSILON)
#define hdsp_test_output_vector_with_newline_double(v, v_len) \
    for (int i = 0; i < v_len; i++) {fprintf(stderr, "[%d]:%f\n",i,v[i]);}
#define hdsp_test_output_vector_with_tab_double(v, v_len) \
    for (int i = 0; i < v_len; i++) {fprintf(stderr, "[%d]:%f\t",i,v[i]);if(i + 1 == v_len){fprintf(stderr,"\n");}}
#define hdsp_test_vectors_equal_double(a, b, len) \
    for (int i = 0; i < len; i++) {if (!HDSP_EQUAL_DOUBLES(a[i],b[i])) {fprintf(stderr, "[%d]:%f!=%f\n",i,a[i],b[i]);exit(EXIT_FAILURE);}}
#define hdsp_test_vectors_equal_almost_double(a, b, len) \
    for (int i = 0; i < len; i++) {if (!HDSP_EQUAL_ALMOST_DOUBLES(a[i],b[i])) {fprintf(stderr, "[%d]:%f!=%f\n",i,a[i],b[i]);exit(EXIT_FAILURE);}}

#ifdef __cplusplus
}
#endif

#endif /* LHDSP_LHDSP_H */