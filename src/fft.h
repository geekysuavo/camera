
/* CAMERA: convex accelerated maximum entropy reconstruction algorithm.
 * Copyright (C) 2015-2016  Bradley Worley  <geekysuavo@gmail.com>.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to:
 *
 *   Free Software Foundation, Inc.
 *   51 Franklin Street, Fifth Floor
 *   Boston, MA  02110-1301, USA.
 */

/* ensure once-only inclusion. */
#ifndef __CAMERA_FFT_H__
#define __CAMERA_FFT_H__

/* FFT_FORWARD, FFT_REVERSE: signs of the phase factors used during
 * forward and reverse Fourier transformation, respectively.
 */
#define FFT_FORWARD  -1
#define FFT_REVERSE   1

/* function declarations: */

void fftfn1 (arr1 *x, int dir);
#define fft1(x)  fftfn1(x, FFT_FORWARD)
#define ifft1(x) fftfn1(x, FFT_REVERSE)

void fftfn2 (arr2 *x, int dir);
#define fft2(x)  fftfn2(x, FFT_FORWARD)
#define ifft2(x) fftfn2(x, FFT_REVERSE)

void fftfn3 (arr3 *x, int dir);
#define fft3(x)  fftfn3(x, FFT_FORWARD)
#define ifft3(x) fftfn3(x, FFT_REVERSE)

#endif /* !__CAMERA_FFT_H__ */

