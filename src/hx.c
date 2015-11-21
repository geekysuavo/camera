
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

/* hx_sqrt0(): compute the real square root of a real scalar.
 *
 * arguments:
 *  @a: the input value.
 *
 * returns:
 *  the square root of the input value.
 */
inline hx0 hx_sqrt0 (hx0 a) {
  /* for now, use the built-in square root function. */
  return sqrt(a);
}

/* hx_log0(): compute the real natural logarithm of a positive real scalar.
 *
 * arguments:
 *  @a: the input value.
 *
 * returns:
 *  the natural logarithm of the input value.
 */
inline hx0 hx_log0 (hx0 a) {
  union {
    hx0 f;
    int i;
  } vx = {a};

  float b = vx.i;
  b *= 8.2629582881927490e-8f;

  return b - 87.989971088f;
}

/* hx_sumsq1(): compute the sum of squared coefficients of an H1 scalar.
 *
 * arguments:
 *  @a: the input hypercomplex scalar.
 *
 * returns:
 *  the sum of squares of the coefficients within the scalar.
 */
inline hx0 hx_sumsq1 (hx1 a) {
  /* compute and return the result. */
  return creal(conj(a) * a);
}

/* hx_sumsq2(): compute the sum of squared coefficients of an H2 scalar.
 *
 * arguments:
 *  @a: the input hypercomplex scalar.
 *
 * returns:
 *  the sum of squares of the coefficients within the scalar.
 */
hx0 hx_sumsq2 (hx2 a) {
  /* square all the coefficients. */
  hx2 asq = a * a;

  /* construct two index masks for shuffling. */
  hx2i m1 = (hx2i) {1, 1, 3, 3};
  hx2i m2 = (hx2i) {2, 2, 2, 2};

  /* shuffle and add the coefficients to compute the result. */
  asq = asq + __builtin_shuffle(asq, m1);
  asq = asq + __builtin_shuffle(asq, m2);

  /* return the final result. */
  return asq[0];
}

/* hx_sumsq3(): compute the sum of squared coefficients of an H3 scalar.
 *
 * arguments:
 *  @a: the input hypercomplex scalar.
 *
 * returns:
 *  the sum of squares of the coefficients within the scalar.
 */
hx0 hx_sumsq3 (hx3 a) {
  /* square all the coefficients. */
  hx3 asq = a * a;

  /* construct three index masks for shuffling. */
  hx3i m1 = (hx3i) {1, 1, 3, 3, 5, 5, 7, 7};
  hx3i m2 = (hx3i) {2, 2, 2, 2, 6, 6, 6, 6};
  hx3i m3 = (hx3i) {4, 4, 4, 4, 4, 4, 4, 4};

  /* shuffle and add the coefficients to compute the result. */
  asq = asq + __builtin_shuffle(asq, m1);
  asq = asq + __builtin_shuffle(asq, m2);
  asq = asq + __builtin_shuffle(asq, m3);

  /* return the final result. */
  return asq[0];
}

/* hx_func1(): compute the value of the hoch/hore entropy regularization
 * functional of a given H1 hypercomplex scalar.
 *
 * arguments:
 *  @x: scalar value from which to compute the functional value.
 *  @L: Lipschitz constant of the functional, equal to 1/(2*def).
 *
 * returns:
 *  value of the regularization functional.
 */
hx0 hx_func1 (hx1 x, hx0 L) {
  /* compute the norm of the scalar value. */
  hx0 xnrm = hx_sqrt0(hx_sumsq1(x));
  hx0 lx = L * xnrm;

  /* compute the value of the functional. */
  hx0 a = xnrm * hx_log0(lx + hx_sqrt0(1.0f + lx * lx));
  a -= hx_sqrt0(xnrm * xnrm + 1.0f / (L * L));

  /* return the computed result. */
  return a;
}

/* hx_func2(): compute the value of the hoch/hore entropy regularization
 * functional of a given H2 hypercomplex scalar.
 *
 * arguments:
 *  @x: scalar value from which to compute the functional value.
 *  @L: Lipschitz constant of the functional, equal to 1/(2*def).
 *
 * returns:
 *  value of the regularization functional.
 */
hx0 hx_func2 (hx2 x, hx0 L) {
  /* compute the norm of the scalar value. */
  hx0 xnrm = hx_sqrt0(hx_sumsq2(x));
  hx0 lx = L * xnrm;

  /* compute the value of the functional. */
  hx0 a = xnrm * hx_log0(lx + hx_sqrt0(1.0f + lx * lx));
  a -= hx_sqrt0(xnrm * xnrm + 1.0f / (L * L));

  /* return the computed result. */
  return a;
}

/* hx_func3(): compute the value of the hoch/hore entropy regularization
 * functional of a given H3 hypercomplex scalar.
 *
 * arguments:
 *  @x: scalar value from which to compute the functional value.
 *  @L: Lipschitz constant of the functional, equal to 1/(2*def).
 *
 * returns:
 *  value of the regularization functional.
 */
hx0 hx_func3 (hx3 x, hx0 L) {
  /* compute the norm of the scalar value. */
  hx0 xnrm = hx_sqrt0(hx_sumsq3(x));
  hx0 lx = L * xnrm;

  /* compute the value of the functional. */
  hx0 a = xnrm * hx_log0(lx + hx_sqrt0(1.0f + lx * lx));
  a -= hx_sqrt0(xnrm * xnrm + 1.0f / (L * L));

  /* return the computed result. */
  return a;
}

/* hx_grad1(): compute the gradient of the hoch/hore entropy regularization
 * functional on a given H1 hypercomplex scalar.
 *
 * arguments:
 *  @x: scalar value from which to compute the gradient.
 *  @L: Lipschitz constant of the functional, equal to 1/(2*def).
 *
 * returns:
 *  value of the H1 hypercomplex gradient.
 */
hx1 hx_grad1 (hx1 x, hx0 L) {
  /* compute the norm of the scalar value. */
  hx0 xnrm = hx_sqrt0(hx_sumsq1(x));
  hx0 lx = L * xnrm;

  /* compute the gradient scaling factor. */
  hx0 a = hx_log0(lx + hx_sqrt0(1.0f + lx * lx)) / xnrm;

  /* return the computed result. */
  return a * x;
}

/* hx_grad2(): compute the gradient of the hoch/hore entropy regularization
 * functional on a given H2 hypercomplex scalar.
 *
 * arguments:
 *  @x: scalar value from which to compute the gradient.
 *  @L: Lipschitz constant of the functional, equal to 1/(2*def).
 *
 * returns:
 *  value of the H2 hypercomplex gradient.
 */
hx2 hx_grad2 (hx2 x, hx0 L) {
  /* compute the norm of the scalar value. */
  hx0 xnrm = hx_sqrt0(hx_sumsq2(x));
  hx0 lx = L * xnrm;

  /* compute the gradient scaling factor. */
  hx0 a = hx_log0(lx + hx_sqrt0(1.0f + lx * lx)) / xnrm;

  /* return the computed result. */
  return a * x;
}

/* hx_grad3(): compute the gradient of the hoch/hore entropy regularization
 * functional on a given H3 hypercomplex scalar.
 *
 * arguments:
 *  @x: scalar value from which to compute the gradient.
 *  @L: Lipschitz constant of the functional, equal to 1/(2*def).
 *
 * returns:
 *  value of the H3 hypercomplex gradient.
 */
hx3 hx_grad3 (hx3 x, hx0 L) {
  /* compute the norm of the scalar value. */
  hx0 xnrm = hx_sqrt0(hx_sumsq3(x));
  hx0 lx = L * xnrm;

  /* compute the gradient scaling factor. */
  hx0 a = hx_log0(lx + hx_sqrt0(1.0f + lx * lx)) / xnrm;

  /* return the computed result. */
  return a * x;
}

