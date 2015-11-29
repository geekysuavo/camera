
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
 *  @TASK_RUN: name of the high-level reconstruction function.
 *  @ARR: data type of the input multidimensional arrays.
 *  @ARR_ALLOC: name of the array allocation function.
 *  @ARR_FREE: name of the array deallocation function.
 *  @ARR_COPY: name of the array copy function.
 *  @ARR_ZERO: name of the array zero function.
 *  @HX_SUMSQ: name of the scalar squared norm function.
 *  @HX_FUNC: name of the scalar objective function.
 *  @HX_GRAD: name of the scalar gradient function.
 *  @PIPESRC_READ: name of the pipe source reading function.
 *  @PIPESINK_WRITE: name of the pipe sink writing function.
 *  @FFT: name of the forward Fourier transform function.
 *  @IFFT: name of the backward Fourier transform function.
 */
#define TASK_CORE(dims)       CONCAT(task_core, dims)
#define TASK_RUN(dims)        CONCAT(task_run, dims)
#define ARR(dims)             CONCAT(arr, dims)
#define ARR_ALLOC(dims)       CONCAT(arr_alloc, dims)
#define ARR_FREE(dims)        CONCAT(arr_free, dims)
#define ARR_COPY(dims)        CONCAT(arr_copy, dims)
#define ARR_ZERO(dims)        CONCAT(arr_zero, dims)
#define HX_SUMSQ(dims)        CONCAT(hx_sumsq, dims)
#define HX_FUNC(dims)         CONCAT(hx_func, dims)
#define HX_GRAD(dims)         CONCAT(hx_grad, dims)
#define PIPESRC_READ(dims)    CONCAT(pipesrc_read, dims)
#define PIPESINK_WRITE(dims)  CONCAT(pipesink_write, dims)
#define FFT(dims)             CONCAT(fft, dims)
#define IFFT(dims)            CONCAT(ifft, dims)

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
   *  @n: schedule index count.
   *  @N: array point count.
   *  @K: schedule index array.
   *  @W: schedule weight array.
   */
  hx0 ly, lz, alpha, tau, kf, fobj = 0.0f;
  int i, iter, n, N, *K;
  hx0 *W;

  /* compute the inverse Lipschitz constant. */
  const hx0 Linv = 1.0f / L;

  /* initialize the output and weighted gradient arrays. */
  ARR_COPY(D)(x, b);
  ARR_ZERO(D)(h);

  /* initialize the schedule index and weight variables. */
  K = sch->idx;
  W = sch->w;

  /* initialize the schedule and array sizes. */
  n = sch->n;
  N = g->n;

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
      for (i = 0, fobj = 0.0f; i < N; i++)
        fobj += HX_FUNC(D)(g->x[i], L);
    }

    /* calculate the frequency-domain gradient. */
    for (i = 0; i < N; i++)
      g->x[i] = HX_GRAD(D)(g->x[i], L);

    /* compute the time-domain gradient. */
    IFFT(D)(g);

    /* determine whether constant-aim mode is enabled. */
    if (lmb <= 0.0f) {
      /* compute the sum of squares portion of the @y Lagrange multiplier. */
      for (i = 0, ly = 0.0f; i < n; i++)
        ly += HX_SUMSQ(D)(b->x[K[i]] - W[i] * x->x[K[i]]
                              + Linv * W[i] * g->x[K[i]]);

      /* finish computing the @y Lagrange multiplier. */
      ly = (L / eps) * hx_sqrt0(ly) - L;
    }
    else {
      /* use a constant Lagrange multiplier. */
      ly = lmb;
    }

    /* compute the unconstrained part of the @y update. */
    for (i = 0; i < N; i++)
      y->x[i] = x->x[i] - Linv * g->x[i];

    /* determine whether to constrain the @y update. */
    if (ly > 0.0f) {
      /* yes. compute the constraint contributions to the update. */
      kf = Linv * ly;
      for (i = 0; i < n; i++) {
        y->x[K[i]] += kf * W[i] * b->x[K[i]];
        y->x[K[i]] /= (1.0f + kf * W[i] * W[i]);
      }
    }

    /* update the weighted sum of prior gradients. */
    for (i = 0; i < N; i++)
      h->x[i] += alpha * g->x[i];

    /* determine whether constant-aim mode is enabled. */
    if (lmb <= 0.0f) {
      /* compute the sum of squares portion of the @z Lagrange multiplier. */
      for (i = 0, lz = 0.0f; i < n; i++)
        lz += HX_SUMSQ(D)((1.0f - W[i]) * b->x[K[i]]
                          + Linv * W[i] * h->x[K[i]]);

      /* finish computing the @z Lagrange multiplier. */
      lz = (L / eps) * hx_sqrt0(lz) - L;
    }
    else {
      /* use a constant Lagrange multiplier. */
      lz = lmb;
    }

    /* compute the unconstrained part of the @z update. */
    for (i = 0; i < N; i++)
      z->x[i] = b->x[i] - Linv * h->x[i];

    /* determine whether to constrain the @z update. */
    if (lz > 0.0f) {
      /* yes. compute the constraint contributions to the update. */
      kf = Linv * lz;
      for (i = 0; i < n; i++) {
        z->x[K[i]] += kf * W[i] * b->x[K[i]];
        z->x[K[i]] /= (1.0f + kf * W[i] * W[i]);
      }
    }

    /* compute the new coupled time-domain estimate. */
    kf = 1.0f - tau;
    for (i = 0; i < N; i++)
      x->x[i] = tau * z->x[i] + kf * y->x[i];

    /* output a log message, if necessary. */
    if (flog)
      fprintf(flog, "%6d %6d %.4le %.4le %.4le\n", ilog, iter, ly, lz, fobj);
  }
}

/* task_run[123](): execute a set of n-dimensional reconstructions.
 *
 * arguments:
 *  @T: task data structure pointer.
 *
 * returns:
 *  integer indicating whether (1) or not (0) reconstruction succeeded.
 */
int TASK_RUN(D) (task *T) {
  /* declare required reconstruction variables:
   *  @b, @x, @y, @z, @g, @h: arrays of hypercomplex array pointers.
   *  @i: reconstruction loop counter.
   *  @ntot: total number of reconstructions.
   *  @nrem: number of remaining reconstructions.
   *  @ncur: current number of parallel reconstructions.
   */
  ARR(D) **b, **x, **y, **z, **g, **h;
  int i, ntot, nrem, ncur;

  /* allocate arrays to hold parallel data structures. */
  b = (ARR(D)**) malloc(T->threads * sizeof(ARR(D)*));
  x = (ARR(D)**) malloc(T->threads * sizeof(ARR(D)*));
  y = (ARR(D)**) malloc(T->threads * sizeof(ARR(D)*));
  z = (ARR(D)**) malloc(T->threads * sizeof(ARR(D)*));
  g = (ARR(D)**) malloc(T->threads * sizeof(ARR(D)*));
  h = (ARR(D)**) malloc(T->threads * sizeof(ARR(D)*));

  /* check that the arrays were allocated successfully. */
  if (!b || !x || !y || !z || !g || !h) {
    /* if not, output an error message and return failure. */
    failf("failed to allocate arrays");
    return 0;
  }

  /* loop over the arrays of parallel data structures. */
  for (i = 0; i < T->threads; i++) {
    /* allocate the core hypercomplex arrays. */
    b[i] = ARR_ALLOC(D)(T->nx << 1, T->ny << 1, T->nz << 1);
    x[i] = ARR_ALLOC(D)(T->nx << 1, T->ny << 1, T->nz << 1);
    y[i] = ARR_ALLOC(D)(T->nx << 1, T->ny << 1, T->nz << 1);
    z[i] = ARR_ALLOC(D)(T->nx << 1, T->ny << 1, T->nz << 1);
    g[i] = ARR_ALLOC(D)(T->nx << 1, T->ny << 1, T->nz << 1);
    h[i] = ARR_ALLOC(D)(T->nx << 1, T->ny << 1, T->nz << 1);

    /* check that the arrays were allocated successfully. */
    if (!b[i] || !x[i] || !y[i] || !z[i] || !g[i] || !h[i]) {
      /* if not, output an error message and return failure. */
      failf("failed to allocate arrays");
      return 0;
    }
  }

  /* compute the tolerance and Lipschitz constant. */
  T->epsilon = sqrt(pow(2.0f, D) * ((hx0) T->sch->n)) * T->sigma;
  T->L = 0.5f / T->delta;

  /* initialize the reconstruction loop counters. */
  ntot = pipesrc_nread(T->Pin);
  nrem = ntot;

  /* loop until no more reconstructions remain. */
  while (nrem > 0) {
    /* compute the current number of parallel reconstructions. */
    ncur = (nrem < T->threads ? nrem : T->threads);

    /* read in a new set of cubes to reconstruct. */
    for (i = 0; i < ncur; i++) {
      if (!PIPESRC_READ(D)(T->Pin, b[i])) {
        failf("failed to read cube %d", ntot - nrem + i + 1);
        return 0;
      }
    }

    /* reconstruct the cubes in parallel. */
    #pragma omp parallel for private(i)
    for (i = 0; i < ncur; i++) {
      TASK_CORE(D)(b[i], x[i], y[i], z[i], g[i], h[i], T->sch, T->iters,
                   T->L, T->epsilon, T->lambda, T->fh_log,
                   ntot - nrem + i + 1);
    }

    /* write out the set of reconstructed cubes. */
    for (i = 0; i < ncur; i++) {
      if (!PIPESINK_WRITE(D)(T->Pout, x[i])) {
        failf("failed to write cube %d", ntot - nrem + i + 1);
        return 0;
      }
    }

    /* update the number of remaining reconstructions. */
    nrem -= ncur;
  }

  /* free all allocated hypercomplex arrays. */
  for (i = 0; i < T->threads; i++) {
    ARR_FREE(D)(b[i]);
    ARR_FREE(D)(x[i]);
    ARR_FREE(D)(y[i]);
    ARR_FREE(D)(z[i]);
    ARR_FREE(D)(g[i]);
    ARR_FREE(D)(h[i]);
  }

  /* free the array arrays. */
  free(b);
  free(x);
  free(y);
  free(z);
  free(g);
  free(h);

  /* return success. */
  return 1;
}

/* undefine all previously define'd macros above, so that this file
 * may be included again for other dimensionalities.
 */
#undef TASK_CORE
#undef TASK_RUN
#undef ARR
#undef ARR_ALLOC
#undef ARR_FREE
#undef ARR_COPY
#undef ARR_ZERO
#undef HX_SUMSQ
#undef HX_FUNC
#undef HX_GRAD
#undef PIPESRC_READ
#undef PIPESINK_WRITE
#undef FFT
#undef IFFT

