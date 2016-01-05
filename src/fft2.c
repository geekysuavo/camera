
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

/* rr2(), ri2(), ir2(), ii2(): macros to access the coefficients of
 * a two-dimensional hypercomplex array.
 */
#define rr2(a,i)  ((a)[4 * (i)])
#define ri2(a,i)  ((a)[4 * (i) + 1])
#define ir2(a,i)  ((a)[4 * (i) + 2])
#define ii2(a,i)  ((a)[4 * (i) + 3])

/* fftrev2(): permute the elements of a two-dimensional hypercomplex
 * array into a bit-reversed order.
 *
 * arguments:
 *  @x: pointer to the array of H2 scalars to modify.
 *  @logn: base-two logarithm of the number of scalars to permute.
 *  @stride: spacing between permuted scalars within the input array.
 */
void fftrev2 (hx2 *x, int logn, int stride) {
  /* declare required variables:
   *  @i, @j: loop indices of swapped array elements.
   *  @n: number of scalars to permute.
   */
  int i, j, n;

  /* compute the vector length from its base-two value. */
  n = (1 << logn);

  /* loop over each element in the array. */
  for (i = 0, j = 0; i < n - 1; i++) {
    /* initialize a third counter to half the array length. */
    int k = n >> 1;

    /* swap all appropriate indices. */
    if (i < j) {
      const hx2 tmp = x[i * stride];
      x[i * stride] = x[j * stride];
      x[j * stride] = tmp;
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

/* fftcore2d0(): perform the core butterfly operations of the radix-two
 * fast Fourier transform on the first dimension of a two-dimensional
 * hypercomplex array.
 *
 * arguments:
 *  @x: pointer to the array of coefficients to transform.
 *  @dir: integer indicating the transform direction.
 *  @logn: number of butterfly passes to perform.
 *  @stride: spacing between scalars within the array.
 */
void fftcore2d0 (hx0 *x, int dir, int logn, int stride) {
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
      const int i = stride * b;
      const int j = stride * (b + dual);

      /* read in the first input coefficients. */
      const hx0 wd_rr = rr2(x, j);
      const hx0 wd_ri = ri2(x, j);
      const hx0 wd_ir = ir2(x, j);
      const hx0 wd_ii = ii2(x, j);

      /* perform the subtraction half of the butterfly operation. */
      rr2(x, j) = rr2(x, i) - wd_rr;
      ri2(x, j) = ri2(x, i) - wd_ri;
      ir2(x, j) = ir2(x, i) - wd_ir;
      ii2(x, j) = ii2(x, i) - wd_ii;

      /* perform the addition half of the butterfly operation. */
      rr2(x, i) += wd_rr;
      ri2(x, i) += wd_ri;
      ir2(x, i) += wd_ir;
      ii2(x, i) += wd_ii;
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
        const int i = stride * (b + a);
        const int j = stride * (b + a + dual);

        /* read in the first input coefficients. */
        const hx0 z1_rr = rr2(x, j);
        const hx0 z1_ri = ri2(x, j);
        const hx0 z1_ir = ir2(x, j);
        const hx0 z1_ii = ii2(x, j);

        /* multiply the coefficients by the u1-twiddle factor. */
        const hx0 wd_rr = w_real * z1_rr - w_imag * z1_ri;
        const hx0 wd_ri = w_real * z1_ri + w_imag * z1_rr;
        const hx0 wd_ir = w_real * z1_ir - w_imag * z1_ii;
        const hx0 wd_ii = w_real * z1_ii + w_imag * z1_ir;

        /* perform the subtraction half of the butterfly operation. */
        rr2(x, j) = rr2(x, i) - wd_rr;
        ri2(x, j) = ri2(x, i) - wd_ri;
        ir2(x, j) = ir2(x, i) - wd_ir;
        ii2(x, j) = ii2(x, i) - wd_ii;

        /* perform the addition half of the butterfly operation. */
        rr2(x, i) += wd_rr;
        ri2(x, i) += wd_ri;
        ir2(x, i) += wd_ir;
        ii2(x, i) += wd_ii;
      }
    }

    /* update the dual index. */
    dual *= 2;
  }
}

/* fftcore2d1(): perform the core butterfly operations of the radix-two
 * fast Fourier transform on the second dimension of a two-dimensional
 * hypercomplex array.
 *
 * arguments:
 *  @x: pointer to the array of coefficients to transform.
 *  @dir: integer indicating the transform direction.
 *  @logn: number of butterfly passes to perform.
 *  @stride: spacing between scalars within the array.
 */
void fftcore2d1 (hx0 *x, int dir, int logn, int stride) {
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
      const int i = stride * b;
      const int j = stride * (b + dual);

      /* read in the first input coefficients. */
      const hx0 wd_rr = rr2(x, j);
      const hx0 wd_ri = ri2(x, j);
      const hx0 wd_ir = ir2(x, j);
      const hx0 wd_ii = ii2(x, j);

      /* perform the subtraction half of the butterfly operation. */
      rr2(x, j) = rr2(x, i) - wd_rr;
      ri2(x, j) = ri2(x, i) - wd_ri;
      ir2(x, j) = ir2(x, i) - wd_ir;
      ii2(x, j) = ii2(x, i) - wd_ii;

      /* perform the addition half of the butterfly operation. */
      rr2(x, i) += wd_rr;
      ri2(x, i) += wd_ri;
      ir2(x, i) += wd_ir;
      ii2(x, i) += wd_ii;
    }

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
        const int i = stride * (b + a);
        const int j = stride * (b + a + dual);

        /* read in the first input coefficients. */
        const hx0 z1_rr = rr2(x, j);
        const hx0 z1_ri = ri2(x, j);
        const hx0 z1_ir = ir2(x, j);
        const hx0 z1_ii = ii2(x, j);

        /* multiply the coefficients by the u2-twiddle factor. */
        const hx0 wd_rr = w_real * z1_rr - w_imag * z1_ir;
        const hx0 wd_ri = w_real * z1_ri - w_imag * z1_ii;
        const hx0 wd_ir = w_real * z1_ir + w_imag * z1_rr;
        const hx0 wd_ii = w_real * z1_ii + w_imag * z1_ri;

        /* perform the subtraction half of the butterfly operation. */
        rr2(x, j) = rr2(x, i) - wd_rr;
        ri2(x, j) = ri2(x, i) - wd_ri;
        ir2(x, j) = ir2(x, i) - wd_ir;
        ii2(x, j) = ii2(x, i) - wd_ii;

        /* perform the addition half of the butterfly operation. */
        rr2(x, i) += wd_rr;
        ri2(x, i) += wd_ri;
        ir2(x, i) += wd_ir;
        ii2(x, i) += wd_ii;
      }
    }

    /* update the dual index. */
    dual *= 2;
  }
}

/* fftfn2(): apply a radix-two fast Fourier transform
 * to a two-dimensional hypercomplex array.
 *
 * arguments:
 *  @x: pointer to the hypercomplex array structure to modify.
 *  @dir: integer indicating the transform direction.
 */
void fftfn2 (arr2 *x, int dir) {
  /* declare required variables:
   *  @i: array scalar offset index.
   *  @di: array scalar offset stride.
   *  @ni: array scalar offset extent.
   *  @stride: stride for coefficient indexing.
   *  @logn: current array dimension butterfly count.
   */
  int i, di, ni, stride, logn;

  /* prepare for first-dimension transforms. */
  stride = 1;
  logn = x->l1;
  ni = x->n1 * x->n2;
  di = x->n1;

  /* loop over all first-dimension vectors. */
  for (i = 0; i < ni; i += di) {
    /* run the bit-reversal and butterflies. */
    fftrev2(x->x + i, logn, stride);
    fftcore2d0((hx0*) (x->x + i), dir, logn, stride);
  }

  /* prepare for second-dimension transforms. */
  stride = x->n1;
  logn = x->l2;
  ni = x->n1;
  di = 1;

  /* loop over all second-dimension vectors. */
  for (i = 0; i < ni; i += di) {
    /* run the bit-reversal and butterflies. */
    fftrev2(x->x + i, logn, stride);
    fftcore2d1((hx0*) (x->x + i), dir, logn, stride);
  }
}

