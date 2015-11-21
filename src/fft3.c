
/* CAMERA: convex accelerated maximum entropy reconstruction algorithm.
 * Copyright (C) 2015  Bradley Worley  <geekysuavo@gmail.com>.
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

/* rrr3() .. iii3(): macros to access the coefficients of
 * a three-dimensional hypercomplex array.
 */
#define rrr3(a,i)  ((a)[8 * (i)])
#define rri3(a,i)  ((a)[8 * (i) + 1])
#define rir3(a,i)  ((a)[8 * (i) + 2])
#define rii3(a,i)  ((a)[8 * (i) + 3])
#define irr3(a,i)  ((a)[8 * (i) + 4])
#define iri3(a,i)  ((a)[8 * (i) + 5])
#define iir3(a,i)  ((a)[8 * (i) + 6])
#define iii3(a,i)  ((a)[8 * (i) + 7])

/* fftrev3(): permute the elements of a three-dimensional hypercomplex
 * array into a bit-reversed order.
 *
 * arguments:
 *  @x: pointer to the array of H3 scalars to modify.
 *  @logn: base-two logarithm of the number of scalars to permute.
 *  @stride: spacing between permuted scalars within the input array.
 */
void fftrev3 (hx3 *x, int logn, int stride) {
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
      const hx3 tmp = x[i * stride];
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

/* fftcore3d0(): perform the core butterfly operations of the radix-two
 * fast Fourier transform on the first dimension of a three-dimensional
 * hypercomplex array.
 *
 * arguments:
 *  @x: pointer to the array of coefficients to transform.
 *  @dir: integer indicating the transform direction.
 *  @logn: number of butterfly passes to perform.
 *  @stride: spacing between scalars within the array.
 */
void fftcore3d0 (hx0 *x, int dir, int logn, int stride) {
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
      const hx0 wd_rrr = rrr3(x, j);
      const hx0 wd_rri = rri3(x, j);
      const hx0 wd_rir = rir3(x, j);
      const hx0 wd_rii = rii3(x, j);
      const hx0 wd_irr = irr3(x, j);
      const hx0 wd_iri = iri3(x, j);
      const hx0 wd_iir = iir3(x, j);
      const hx0 wd_iii = iii3(x, j);

      /* perform the subtraction half of the butterfly operation. */
      rrr3(x, j) = rrr3(x, i) - wd_rrr;
      rri3(x, j) = rri3(x, i) - wd_rri;
      rir3(x, j) = rir3(x, i) - wd_rir;
      rii3(x, j) = rii3(x, i) - wd_rii;
      irr3(x, j) = irr3(x, i) - wd_irr;
      iri3(x, j) = iri3(x, i) - wd_iri;
      iir3(x, j) = iir3(x, i) - wd_iir;
      iii3(x, j) = iii3(x, i) - wd_iii;

      /* perform the addition half of the butterfly operation. */
      rrr3(x, i) += wd_rrr;
      rri3(x, i) += wd_rri;
      rir3(x, i) += wd_rir;
      rii3(x, i) += wd_rii;
      irr3(x, i) += wd_irr;
      iri3(x, i) += wd_iri;
      iir3(x, i) += wd_iir;
      iii3(x, i) += wd_iii;
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
        const hx0 z1_rrr = rrr3(x, j);
        const hx0 z1_rri = rri3(x, j);
        const hx0 z1_rir = rir3(x, j);
        const hx0 z1_rii = rii3(x, j);
        const hx0 z1_irr = irr3(x, j);
        const hx0 z1_iri = iri3(x, j);
        const hx0 z1_iir = iir3(x, j);
        const hx0 z1_iii = iii3(x, j);

        /* multiply the coefficients by the u1-twiddle factor. */
        const hx0 wd_rrr = w_real * z1_rrr - w_imag * z1_rri;
        const hx0 wd_rri = w_real * z1_rri + w_imag * z1_rrr;
        const hx0 wd_rir = w_real * z1_rir - w_imag * z1_rii;
        const hx0 wd_rii = w_real * z1_rii + w_imag * z1_rir;
        const hx0 wd_irr = w_real * z1_irr - w_imag * z1_iri;
        const hx0 wd_iri = w_real * z1_iri + w_imag * z1_irr;
        const hx0 wd_iir = w_real * z1_iir - w_imag * z1_iii;
        const hx0 wd_iii = w_real * z1_iii + w_imag * z1_iir;

        /* perform the subtraction half of the butterfly operation. */
        rrr3(x, j) = rrr3(x, i) - wd_rrr;
        rri3(x, j) = rri3(x, i) - wd_rri;
        rir3(x, j) = rir3(x, i) - wd_rir;
        rii3(x, j) = rii3(x, i) - wd_rii;
        irr3(x, j) = irr3(x, i) - wd_irr;
        iri3(x, j) = iri3(x, i) - wd_iri;
        iir3(x, j) = iir3(x, i) - wd_iir;
        iii3(x, j) = iii3(x, i) - wd_iii;

        /* perform the addition half of the butterfly operation. */
        rrr3(x, i) += wd_rrr;
        rri3(x, i) += wd_rri;
        rir3(x, i) += wd_rir;
        rii3(x, i) += wd_rii;
        irr3(x, i) += wd_irr;
        iri3(x, i) += wd_iri;
        iir3(x, i) += wd_iir;
        iii3(x, i) += wd_iii;
      }
    }

    /* update the dual index. */
    dual *= 2;
  }
}

/* fftcore3d1(): perform the core butterfly operations of the radix-two
 * fast Fourier transform on the second dimension of a three-dimensional
 * hypercomplex array.
 *
 * arguments:
 *  @x: pointer to the array of coefficients to transform.
 *  @dir: integer indicating the transform direction.
 *  @logn: number of butterfly passes to perform.
 *  @stride: spacing between scalars within the array.
 */
void fftcore3d1 (hx0 *x, int dir, int logn, int stride) {
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
      const hx0 wd_rrr = rrr3(x, j);
      const hx0 wd_rri = rri3(x, j);
      const hx0 wd_rir = rir3(x, j);
      const hx0 wd_rii = rii3(x, j);
      const hx0 wd_irr = irr3(x, j);
      const hx0 wd_iri = iri3(x, j);
      const hx0 wd_iir = iir3(x, j);
      const hx0 wd_iii = iii3(x, j);

      /* perform the subtraction half of the butterfly operation. */
      rrr3(x, j) = rrr3(x, i) - wd_rrr;
      rri3(x, j) = rri3(x, i) - wd_rri;
      rir3(x, j) = rir3(x, i) - wd_rir;
      rii3(x, j) = rii3(x, i) - wd_rii;
      irr3(x, j) = irr3(x, i) - wd_irr;
      iri3(x, j) = iri3(x, i) - wd_iri;
      iir3(x, j) = iir3(x, i) - wd_iir;
      iii3(x, j) = iii3(x, i) - wd_iii;

      /* perform the addition half of the butterfly operation. */
      rrr3(x, i) += wd_rrr;
      rri3(x, i) += wd_rri;
      rir3(x, i) += wd_rir;
      rii3(x, i) += wd_rii;
      irr3(x, i) += wd_irr;
      iri3(x, i) += wd_iri;
      iir3(x, i) += wd_iir;
      iii3(x, i) += wd_iii;
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
        const hx0 z1_rrr = rrr3(x, j);
        const hx0 z1_rri = rri3(x, j);
        const hx0 z1_rir = rir3(x, j);
        const hx0 z1_rii = rii3(x, j);
        const hx0 z1_irr = irr3(x, j);
        const hx0 z1_iri = iri3(x, j);
        const hx0 z1_iir = iir3(x, j);
        const hx0 z1_iii = iii3(x, j);

        /* multiply the coefficients by the u2-twiddle factor. */
        const hx0 wd_rrr = w_real * z1_rrr - w_imag * z1_rir;
        const hx0 wd_rri = w_real * z1_rri - w_imag * z1_rii;
        const hx0 wd_rir = w_real * z1_rir + w_imag * z1_rrr;
        const hx0 wd_rii = w_real * z1_rii + w_imag * z1_rri;
        const hx0 wd_irr = w_real * z1_irr - w_imag * z1_iir;
        const hx0 wd_iri = w_real * z1_iri - w_imag * z1_iii;
        const hx0 wd_iir = w_real * z1_iir + w_imag * z1_irr;
        const hx0 wd_iii = w_real * z1_iii + w_imag * z1_iri;

        /* perform the subtraction half of the butterfly operation. */
        rrr3(x, j) = rrr3(x, i) - wd_rrr;
        rri3(x, j) = rri3(x, i) - wd_rri;
        rir3(x, j) = rir3(x, i) - wd_rir;
        rii3(x, j) = rii3(x, i) - wd_rii;
        irr3(x, j) = irr3(x, i) - wd_irr;
        iri3(x, j) = iri3(x, i) - wd_iri;
        iir3(x, j) = iir3(x, i) - wd_iir;
        iii3(x, j) = iii3(x, i) - wd_iii;

        /* perform the addition half of the butterfly operation. */
        rrr3(x, i) += wd_rrr;
        rri3(x, i) += wd_rri;
        rir3(x, i) += wd_rir;
        rii3(x, i) += wd_rii;
        irr3(x, i) += wd_irr;
        iri3(x, i) += wd_iri;
        iir3(x, i) += wd_iir;
        iii3(x, i) += wd_iii;
      }
    }

    /* update the dual index. */
    dual *= 2;
  }
}

/* fftcore3d2(): perform the core butterfly operations of the radix-two
 * fast Fourier transform on the third dimension of a three-dimensional
 * hypercomplex array.
 *
 * arguments:
 *  @x: pointer to the array of coefficients to transform.
 *  @dir: integer indicating the transform direction.
 *  @logn: number of butterfly passes to perform.
 *  @stride: spacing between scalars within the array.
 */
void fftcore3d2 (hx0 *x, int dir, int logn, int stride) {
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
      const hx0 wd_rrr = rrr3(x, j);
      const hx0 wd_rri = rri3(x, j);
      const hx0 wd_rir = rir3(x, j);
      const hx0 wd_rii = rii3(x, j);
      const hx0 wd_irr = irr3(x, j);
      const hx0 wd_iri = iri3(x, j);
      const hx0 wd_iir = iir3(x, j);
      const hx0 wd_iii = iii3(x, j);

      /* perform the subtraction half of the butterfly operation. */
      rrr3(x, j) = rrr3(x, i) - wd_rrr;
      rri3(x, j) = rri3(x, i) - wd_rri;
      rir3(x, j) = rir3(x, i) - wd_rir;
      rii3(x, j) = rii3(x, i) - wd_rii;
      irr3(x, j) = irr3(x, i) - wd_irr;
      iri3(x, j) = iri3(x, i) - wd_iri;
      iir3(x, j) = iir3(x, i) - wd_iir;
      iii3(x, j) = iii3(x, i) - wd_iii;

      /* perform the addition half of the butterfly operation. */
      rrr3(x, i) += wd_rrr;
      rri3(x, i) += wd_rri;
      rir3(x, i) += wd_rir;
      rii3(x, i) += wd_rii;
      irr3(x, i) += wd_irr;
      iri3(x, i) += wd_iri;
      iir3(x, i) += wd_iir;
      iii3(x, i) += wd_iii;
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
        const hx0 z1_rrr = rrr3(x, j);
        const hx0 z1_rri = rri3(x, j);
        const hx0 z1_rir = rir3(x, j);
        const hx0 z1_rii = rii3(x, j);
        const hx0 z1_irr = irr3(x, j);
        const hx0 z1_iri = iri3(x, j);
        const hx0 z1_iir = iir3(x, j);
        const hx0 z1_iii = iii3(x, j);

        /* multiply the coefficients by the u3-twiddle factor. */
        const hx0 wd_rrr = w_real * z1_rrr - w_imag * z1_irr;
        const hx0 wd_rri = w_real * z1_rri - w_imag * z1_iri;
        const hx0 wd_rir = w_real * z1_rir - w_imag * z1_iir;
        const hx0 wd_rii = w_real * z1_rii - w_imag * z1_iii;
        const hx0 wd_irr = w_real * z1_irr + w_imag * z1_rrr;
        const hx0 wd_iri = w_real * z1_iri + w_imag * z1_rri;
        const hx0 wd_iir = w_real * z1_iir + w_imag * z1_rir;
        const hx0 wd_iii = w_real * z1_iii + w_imag * z1_rii;

        /* perform the subtraction half of the butterfly operation. */
        rrr3(x, j) = rrr3(x, i) - wd_rrr;
        rri3(x, j) = rri3(x, i) - wd_rri;
        rir3(x, j) = rir3(x, i) - wd_rir;
        rii3(x, j) = rii3(x, i) - wd_rii;
        irr3(x, j) = irr3(x, i) - wd_irr;
        iri3(x, j) = iri3(x, i) - wd_iri;
        iir3(x, j) = iir3(x, i) - wd_iir;
        iii3(x, j) = iii3(x, i) - wd_iii;

        /* perform the addition half of the butterfly operation. */
        rrr3(x, i) += wd_rrr;
        rri3(x, i) += wd_rri;
        rir3(x, i) += wd_rir;
        rii3(x, i) += wd_rii;
        irr3(x, i) += wd_irr;
        iri3(x, i) += wd_iri;
        iir3(x, i) += wd_iir;
        iii3(x, i) += wd_iii;
      }
    }

    /* update the dual index. */
    dual *= 2;
  }
}

/* fftfn3(): apply a radix-two fast Fourier transform
 * to a three-dimensional hypercomplex array.
 *
 * arguments:
 *  @x: pointer to the hypercomplex array structure to modify.
 *  @dir: integer indicating the transform direction.
 */
void fftfn3 (arr3 *x, int dir) {
  /* declare required variables:
   *  @i: array scalar offset input index.
   *  @di1, @di2: array scalar offset strides.
   *  @ni: array scalar offset input extent.
   *  @j: array scalar offset actual index.
   *  @stride: stride for coefficient indexing.
   *  @logn: current array dimension butterfly count.
   */
  int i, di1, di2, ni, j, stride, logn;

  /* prepare for first-dimension transforms. */
  stride = 1;
  logn = x->l1;
  ni = x->n2 * x->n3;
  di1 = 1;
  di2 = x->n1;

  /* loop over all first-dimension vectors. */
  for (i = 0; i < ni; i++) {
    /* compute the linear array offset. */
    j = di2 * (i / di1) + i % di1;

    /* run the bit-reversal and butterflies. */
    fftrev3(x->x + j, logn, stride);
    fftcore3d0((hx0*) (x->x + j), dir, logn, stride);
  }

  /* prepare for second-dimension transforms. */
  stride = x->n1;
  logn = x->l2;
  ni = x->n1 * x->n3;
  di1 = x->n1;
  di2 = x->n1 * x->n2;

  /* loop over all second-dimension vectors. */
  for (i = 0; i < ni; i++) {
    /* compute the linear array offset. */
    j = di2 * (i / di1) + i % di1;

    /* run the bit-reversal and butterflies. */
    fftrev3(x->x + j, logn, stride);
    fftcore3d1((hx0*) (x->x + j), dir, logn, stride);
  }

  /* prepare for third-dimension transforms. */
  stride = x->n1 * x->n2;
  logn = x->l3;
  ni = x->n1 * x->n2;
  di1 = x->n1 * x->n2;
  di2 = x->n;

  /* loop over all third-dimension vectors. */
  for (i = 0; i < ni; i++) {
    /* compute the linear array offset. */
    j = di2 * (i / di1) + i % di1;

    /* run the bit-reversal and butterflies. */
    fftrev3(x->x + j, logn, stride);
    fftcore3d2((hx0*) (x->x + j), dir, logn, stride);
  }
}


