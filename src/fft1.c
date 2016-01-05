
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

/* include the core camera header. */
#include "camera.h"

/* real1(), imag1(): macros to access the coefficients of a one-dimensional
 * hypercomplex array.
 */
#define real1(a,i)  ((a)[2 * (i)])
#define imag1(a,i)  ((a)[2 * (i) + 1])

/* fftrev1(): permute the elements of a one-dimensional hypercomplex
 * array into a bit-reversed order.
 *
 * arguments:
 *  @x: pointer to the array of H1 scalars to modify.
 *  @n: number of scalars to permute.
 */
void fftrev1 (hx1 *x, int n) {
  /* declare required variables:
   *  @i, @j: loop indices of swapped array elements.
   */
  int i, j;

  /* loop over each element in the array. */
  for (i = 0, j = 0; i < n - 1; i++) {
    /* initialize a third counter to half the array length. */
    int k = n >> 1;

    /* swap all appropriate indices. */
    if (i < j) {
      const hx1 tmp = x[i];
      x[i] = x[j];
      x[j] = tmp;
    }

    /* update the third counter. */
    while (k <= j) {
      j -= k;
      k >>= 1;
    }

    /* update the second swap index. */
    j += k;
  }
}

/* fftcore1(): perform the core butterfly operations of the radix-two
 * fast Fourier transform on a one-dimensional hypercomplex array.
 *
 * arguments:
 *  @x: pointer to the array of coefficients to transform.
 *  @dir: integer indicating the transform direction.
 *  @logn: number of butterfly passes to perform.
 */
void fftcore1 (hx0 *x, int dir, int logn) {
  /* declare required variables:
   *  @dual: dual/opposing index of each pass.
   *  @bit: butterfly pass loop counter, goes from [0,logn).
   *  @n: vector length; the number of scalars in the transform.
   */
  int dual, bit, n;

  /* compute the vector length and initialize the dual counter. */
  n = (1 << logn);
  dual = 1;

  /* loop for the number of butterfly passes to execute. */
  for (bit = 0; bit < logn; bit++) {
    /* initialize the twiddle factor. */
    hx0 w_real = 1.0f;
    hx0 w_imag = 0.0f;

    /* compute the trigonometric recurrence phase angle. */
    const double phi = 2.0 * dir * M_PI / (2.0 * (double) dual);

    /* compute the trigonometric recurrence factors. */
    const hx0 s = sin(phi);
    const hx0 t = sin(phi / 2.0);
    const hx0 s2 = 2.0f * t * t;

    /* declare the element index pair. */
    int a, b;

    /* loop over the case (a==0), which needs no twiddle factor updates. */
    for (b = 0; b < n; b += 2 * dual) {
      /* compute the element indices to operate on. */
      const int i = b;
      const int j = b + dual;

      /* read in the first input coefficients. */
      const hx0 z1_real = real1(x, j);
      const hx0 z1_imag = imag1(x, j);

      /* no twiddle factor multiplication is required in this case. */
      const hx0 wd_real = z1_real;
      const hx0 wd_imag = z1_imag;

      /* perform the butterfly operation. */
      real1(x, j) = real1(x, i) - wd_real;
      imag1(x, j) = imag1(x, i) - wd_imag;
      real1(x, i) += wd_real;
      imag1(x, i) += wd_imag;
    }

    /* loop over the cases (a>0). */
    for (a = 1; a < dual; a++) {
      {
        /* update the twiddle factor via the recurrence relation. */
        const hx0 tmp_real = w_real - s * w_imag - s2 * w_real;
        const hx0 tmp_imag = w_imag + s * w_real - s2 * w_imag;
        w_real = tmp_real;
        w_imag = tmp_imag;
      }

      for (b = 0; b < n; b += 2 * dual) {
        /* compute the element indices to operate on. */
        const int i = b + a;
        const int j = b + a + dual;

        /* read in the first input coefficients. */
        const hx0 z1_real = real1(x, j);
        const hx0 z1_imag = imag1(x, j);

        /* multiply the coefficients by the u1-twiddle factor. */
        const hx0 wd_real = w_real * z1_real - w_imag * z1_imag;
        const hx0 wd_imag = w_real * z1_imag + w_imag * z1_real;

        /* perform the butterfly operation. */
        real1(x, j) = real1(x, i) - wd_real;
        imag1(x, j) = imag1(x, i) - wd_imag;
        real1(x, i) += wd_real;
        imag1(x, i) += wd_imag;
      }
    }

    /* update the dual index. */
    dual *= 2;
  }
}

/* fftfn1(): apply a radix-two fast Fourier transform
 * to a one-dimensional hypercomplex array.
 *
 * arguments:
 *  @x: pointer to the hypercomplex array structure to modify.
 *  @dir: integer indicating the transform direction.
 */
void fftfn1 (arr1 *x, int dir) {
  /* run the bit-reversal and butterflies. */
  fftrev1(x->x, x->n);
  fftcore1((hx0*) x->x, dir, x->l);
}

