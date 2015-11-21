
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

/* define a macro to concatenate tokens together. */
#define CONCAT(a,b) a ## b

/* define a set of substitutions to make based on the value of TASK_DIMS
 * that has been define'd by the including source file:
 *  @TASK_CORE: name of the core reconstruction function.
 *  @ARR: data type of the input multidimensional arrays.
 *  @ARR_COPY: name of the array copy function.
 *  @ARR_ZERO: name of the array zero function.
 *  @HX_SUMSQ: name of the scalar squared norm function.
 *  @HX_FUNC: name of the scalar objective function.
 *  @HX_GRAD: name of the scalar gradient function.
 *  @FFT: name of the forward Fourier transform function.
 *  @IFFT: name of the backward Fourier transform function.
 */
#define TASK_CORE(dims)  CONCAT(task_core, dims)
#define ARR(dims)        CONCAT(arr, dims)
#define ARR_COPY(dims)   CONCAT(arr_copy, dims)
#define ARR_ZERO(dims)   CONCAT(arr_zero, dims)
#define HX_SUMSQ(dims)   CONCAT(hx_sumsq, dims)
#define HX_FUNC(dims)    CONCAT(hx_func, dims)
#define HX_GRAD(dims)    CONCAT(hx_grad, dims)
#define FFT(dims)        CONCAT(fft, dims)
#define IFFT(dims)       CONCAT(ifft, dims)

/* task_core[123](): reconstruct an n-dimensional hypercomplex array,
 * given a complete set of pre-prepared data structures.
 *
 * arguments:
 *  @b: input array of measured data, infilled with zeros.
 *  @x: output array of reconstructed values.
 *  @y: array to hold gradient descent updates.
 *  @z: array to hold mirror descent updates.
 *  @g: array to hold computed gradients.
 *  @h: array to hold weighted gradient sums.
 *  @sch: pointer to the schedule data structure.
 *  @iters: number of iterations to perform.
 *  @L: Lipschitz constant for regularization.
 *  @eps: measured data inequality tolerance.
 *  @lmb: constant-lambda value, or zero.
 *  @flog: logging output file handle, or null.
 *  @ilog: slice index for logging.
 */
void TASK_CORE(D) (ARR(D) *b, ARR(D) *x, ARR(D) *y,
                   ARR(D) *z, ARR(D) *g, ARR(D) *h,
                   sched *sch, const int iters, const hx0 L,
                   const hx0 eps, const hx0 lmb,
                   FILE *flog, const int ilog) {
  /* declare required local variables:
   *  @ly, @lz: Lagrange multipliers for @y and @z updates.
   *  @alpha, @tau: coupling constants for acceleration.
   *  @kf: variable used for scaling in array updates.
   *  @fobj: current value of the objective function.
   *  @i: general-purpose loop counter.
   *  @iter: iteration loop counter.
   *  @nS: schedule index count.
   *  @S: schedule index array.
   */
  hx0 ly, lz, alpha, tau, kf, fobj = 0.0f;
  int i, iter, nS, *S;

  /* compute the inverse Lipschitz constant. */
  const hx0 Linv = 1.0f / L;

  /* initialize the output and weighted gradient arrays. */
  ARR_COPY(D)(x, b);
  ARR_ZERO(D)(h);

  /* initialize the schedule variables. */
  S = sch->idx;
  nS = sch->n;

  /* loop over the number of iterations. */
  for (iter = 1; iter <= iters; iter++) {
    /* compute the coupling constants of the current iteration. */
    alpha = 0.5f * ((hx0) (iter + 1));
    tau = 2.0f / ((hx0) (iter + 3));

    /* compute the current frequency-domain estimate. */
    ARR_COPY(D)(g, x);
    FFT(D)(g);

    /* check if the objective function value is required. */
    if (flog) {
      /* compute the current objective. */
      for (i = 0, fobj = 0.0f; i < g->n; i++)
        fobj += HX_FUNC(D)(g->x[i], L);
    }

    /* calculate the frequency-domain gradient. */
    for (i = 0; i < g->n; i++)
      g->x[i] = HX_GRAD(D)(g->x[i], L);

    /* compute the time-domain gradient. */
    IFFT(D)(g);

    /* determine whether constant-aim mode is enabled. */
    if (lmb <= 0.0f) {
      /* compute the sum of squares portion of the @y Lagrange multiplier. */
      for (i = 0, ly = 0.0f; i < nS; i++)
        ly += HX_SUMSQ(D)(b->x[S[i]] - x->x[S[i]] + Linv * g->x[S[i]]);

      /* finish computing the @y Lagrange multiplier. */
      ly = (L / eps) * hx_sqrt0(ly) - L;
    }
    else {
      /* use a constant Lagrange multiplier. */
      ly = lmb;
    }

    /* determine whether the @y-update constraint is active. */
    if (ly <= 0.0f) {
      /* inactive. */
      for (i = 0; i < g->n; i++)
        y->x[i] = x->x[i] - Linv * g->x[i];
    }
    else {
      /* active. compute the right half of the update. */
      kf = Linv * ly;
      for (i = 0; i < g->n; i++)
        y->x[i] = x->x[i] + kf * b->x[i] - Linv * g->x[i];

      /* compute the left half of the update. */
      kf = 1.0f - ly / (ly + L);
      for (i = 0; i < nS; i++)
        y->x[S[i]] *= kf;
    }

    /* update the weighted sum of prior gradients. */
    for (i = 0; i < g->n; i++)
      h->x[i] += alpha * g->x[i];

    /* determine whether constant-aim mode is enabled. */
    if (lmb <= 0.0f) {
      /* compute the sum of squares portion of the @z Lagrange multiplier. */
      for (i = 0, lz = 0.0f; i < nS; i++)
        lz += HX_SUMSQ(D)(h->x[S[i]]);

      /* finish computing the @z Lagrange multiplier. */
      lz = (L / eps) * hx_sqrt0(Linv * Linv * lz) - L;
    }
    else {
      /* use a constant Lagrange multiplier. */
      lz = lmb;
    }

    /* determine whether the @z-update constraint is active. */
    if (lz <= 0.0f) {
      /* inactive. */
      for (i = 0; i < g->n; i++)
        z->x[i] = b->x[i] - Linv * h->x[i];
    }
    else {
      /* active. compute the right half of the update. */
      kf = Linv * lz;
      for (i = 0; i < g->n; i++)
        z->x[i] = b->x[i] + kf * b->x[i] - Linv * h->x[i];

      /* compute the left half of the update. */
      kf = 1.0f - lz / (lz + L);
      for (i = 0; i < nS; i++)
        z->x[S[i]] *= kf;
    }

    /* compute the new coupled time-domain estimate. */
    kf = 1.0f - tau;
    for (i = 0; i < x->n; i++)
      x->x[i] = tau * z->x[i] + kf * y->x[i];

    /* output a log message, if necessary. */
    if (flog)
      fprintf(flog, "%6d %6d %.4le %.4le %.4le\n", ilog, iter, ly, lz, fobj);
  }
}

/* undefine all previously define'd macros above, so that this file
 * may be included again for other dimensionalities.
 */
#undef TASK_CORE
#undef ARR
#undef ARR_COPY
#undef ARR_ZERO
#undef HX_SUMSQ
#undef HX_FUNC
#undef FFT
#undef IFFT

