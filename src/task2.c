
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

/* construct the two-dimensional reconstruction core function. */
#define D 2
#include "task.c"
#undef D

/* task_run2(): execute a set of two-dimensional reconstructions.
 *
 * arguments:
 *  @fname_in: input filename, or null to read from standard input.
 *  @fname_out: output filename, or null to write to standard output.
 *  @fname_log: logging filename, or null to skip log output.
 *  @fname_sched: schedule filename.
 *  @iters: number of iterations.
 *  @threads: number of threads.
 *  @delta: functional background.
 *  @sigma: data noise estimate.
 *  @lambda: constant-lambda value.
 *  @n1: first-dimension size of all arrays.
 *  @n2: second-dimension size of all arrays.
 *
 * returns:
 *  integer indicating whether (1) or not (0) reconstruction succeeded.
 */
int task_run2 (char *fname_in, char *fname_out,
               char *fname_log, char *fname_sched,
               const int iters, const int threads,
               hx0 delta, hx0 sigma, hx0 lambda,
               int n1, int n2) {
  /* declare required input/output data structures:
   *  @Pin: input pipe source structure pointer.
   *  @Pout: output pipe sink structure pointer.
   *  @sch: schedule data structure pointer.
   *  @flog: logging file handle.
   */
  pipe *Pin, *Pout;
  sched *sch;
  FILE *flog;

  /* declare required reconstruction variables:
   *  @b, @x, @y, @z, @g, @h: arrays of hypercomplex array pointers.
   *  @i: reconstruction loop counter.
   *  @ntot: total number of reconstructions.
   *  @nrem: number of remaining reconstructions.
   *  @ncur: current number of parallel reconstructions.
   *  @eps: epsilon value from the supplied noise estimate.
   *  @L: Lipschitz constant of the regularization functional.
   */
  arr2 **b, **x, **y, **z, **g, **h;
  int i, ntot, nrem, ncur;
  hx0 eps, L;

  /* check that the array first-dimension size is in bounds. */
  if (n1 < 4) {
    /* if not, output an error message and return failure. */
    fprintf(stderr, "Error: invalid x-dimension length %d\n", n1);
    return 0;
  }

  /* check that the array second-dimension size is in bounds. */
  if (n2 < 4) {
    /* if not, output an error message and return failure. */
    fprintf(stderr, "Error: invalid y-dimension length %d\n", n2);
    return 0;
  }

  /* check if a log filename was provided. */
  if (fname_log) {
    /* attempt to open the log file handle. */
    flog = fopen(fname_log, "w");
    if (!flog) {
      /* if unsuccessful, output an error message and return failure. */
      fprintf(stderr, "Error: failed to open '%s' for logging\n", fname_log);
      return 0;
    }
  }
  else {
    /* if not, nullify the log file handle. */
    flog = NULL;
  }

  /* read the schedule file and check that it was parsed successfully. */
  sch = sched_alloc2(fname_sched, n1 << 1, n2 << 1);
  if (!sch) {
    /* if unsuccessful, output an error message and return failure. */
    fprintf(stderr, "Error: failed to read schedule '%s'\n", fname_sched);
    return 0;
  }

  /* open the input pipe source. */
  Pin = pipesrc_open(fname_in, sch);
  if (!Pin) {
    /* if unsuccessful, output an error message and return failure. */
    fprintf(stderr, "Error: failed to open pipe source\n");
    return 0;
  }

  /* adjust the pipe header values to match the reconstructed data. */
  Pin->hdr.sz = n1;
  Pin->hdr.specnum = 2 * n2;
  Pin->hdr.apod_f1 = n1;
  Pin->hdr.apod_f3 = n2;
  Pin->hdr.quad = PIPE_QUAD_COMPLEX;
  Pin->hdr.quad_f1 = PIPE_QUAD_COMPLEX;
  Pin->hdr.quad_f3 = PIPE_QUAD_COMPLEX;

  /* open the output pipe sink. */
  Pout = pipesink_open(fname_out, &Pin->hdr);
  if (!Pout) {
    /* if unsuccessful, output an error message and return failure. */
    fprintf(stderr, "Error: failed to open pipe sink\n");
    return 0;
  }

  /* allocate arrays to hold parallel data structures. */
  b = (arr2**) malloc(threads * sizeof(arr2*));
  x = (arr2**) malloc(threads * sizeof(arr2*));
  y = (arr2**) malloc(threads * sizeof(arr2*));
  z = (arr2**) malloc(threads * sizeof(arr2*));
  g = (arr2**) malloc(threads * sizeof(arr2*));
  h = (arr2**) malloc(threads * sizeof(arr2*));

  /* check that the arrays were allocated successfully. */
  if (!b || !x || !y || !z || !g || !h) {
    /* if not, output an error message and return failure. */
    fprintf(stderr, "Error: failed to allocate arrays\n");
    return 0;
  }

  /* loop over the arrays of parallel data structures. */
  for (i = 0; i < threads; i++) {
    /* allocate the core hypercomplex arrays. */
    b[i] = arr_alloc2(n1 << 1, n2 << 1);
    x[i] = arr_alloc2(n1 << 1, n2 << 1);
    y[i] = arr_alloc2(n1 << 1, n2 << 1);
    z[i] = arr_alloc2(n1 << 1, n2 << 1);
    g[i] = arr_alloc2(n1 << 1, n2 << 1);
    h[i] = arr_alloc2(n1 << 1, n2 << 1);

    /* check that the arrays were allocated successfully. */
    if (!b[i] || !x[i] || !y[i] || !z[i] || !g[i] || !h[i]) {
      /* if not, output an error message and return failure. */
      fprintf(stderr, "Error: failed to allocate arrays\n");
      return 0;
    }
  }

  /* compute the tolerance and Lipschitz constant. */
  eps = sqrt(4.0f * ((hx0) sch->n)) * sigma;
  L = 0.5f / delta;

  /* initialize the reconstruction loop counters. */
  ntot = pipesrc_nread(Pin);
  nrem = ntot;

  /* loop until no more reconstructions remain. */
  while (nrem > 0) {
    /* compute the current number of parallel reconstructions. */
    ncur = (nrem < threads ? nrem : threads);

    /* read in a new set of planes to reconstruct. */
    for (i = 0; i < ncur; i++) {
      if (!pipesrc_read2(Pin, b[i])) {
        fprintf(stderr, "Error: failed to read plane %d\n",
                ntot - nrem + i + 1);
        return 0;
      }
    }

    /* reconstruct the planes in parallel. */
    #pragma omp parallel for private(i)
    for (i = 0; i < ncur; i++) {
      task_core2(b[i], x[i], y[i], z[i], g[i], h[i],
                 sch, iters, L, eps, lambda,
                 flog, ntot - nrem + i + 1);
    }

    /* write out the set of reconstructed planes. */
    for (i = 0; i < ncur; i++) {
      if (!pipesink_write2(Pout, x[i])) {
        fprintf(stderr, "Error: failed to write plane %d\n",
                ntot - nrem + i + 1);
        return 0;
      }
    }

    /* update the number of remaining reconstructions. */
    nrem -= ncur;
  }

  /* free all allocated hypercomplex arrays. */
  for (i = 0; i < threads; i++) {
    arr_free2(b[i]);
    arr_free2(x[i]);
    arr_free2(y[i]);
    arr_free2(z[i]);
    arr_free2(g[i]);
    arr_free2(h[i]);
  }

  /* free the array arrays. */
  free(b);
  free(x);
  free(y);
  free(z);
  free(g);
  free(h);

  /* free the input/output data structures. */
  sched_free(sch);
  pipe_close(Pin);
  pipe_close(Pout);

  /* free the log file handle, if open. */
  if (flog)
    fclose(flog);

  /* return success. */
  return 1;
}

