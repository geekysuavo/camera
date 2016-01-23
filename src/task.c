
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

/* construct the one-dimensional reconstruction core function. */
#define D 1
#include "task-core.c"
#undef D

/* construct the two-dimensional reconstruction core function. */
#define D 2
#include "task-core.c"
#undef D

/* construct the three-dimensional reconstruction core function. */
#define D 3
#include "task-core.c"
#undef D

/* TASK_PARSE_STR(): preprocessor macro to parse a string argument
 * from the command-line argument string array into a duplicate
 * location.
 */
#define TASK_PARSE_STR(opt, dest) \
  if (strcmp(argv[i], opt) == 0) { \
    if (i + 1 >= argc) { \
      failf("'" opt "' argument requires a value"); \
      return NULL; \
    } \
    if (dest) free(dest); \
    dest = strdup(argv[++i]); \
  }

/* TASK_PARSE(): preprocessor macro to parse an integer or float argument
 * from the command-line argument string array into a new location.
 */
#define TASK_PARSE(opt, fmt, dest) \
  if (strcmp(argv[i], opt) == 0) { \
    if (i + 1 >= argc || sscanf(argv[++i], fmt, &(dest)) != 1) { \
      failf("'" opt "' argument requires a value"); \
      return NULL; \
    } \
  }

/* TASK_PARSE_FLAG(): preprocessor macro to parse a boolean flag argument
 * from the command-line argument string array.
 */
#define TASK_PARSE_FLAG(opt, dest) \
  if (strcmp(argv[i], opt) == 0) { \
    dest = 1; \
  }

/* TASK_PARSE_ERR(): preprocessor macro to catch command-line argument
 * string array parsing errors.
 */
#define TASK_PARSE_ERR() \
  { \
    failf("unsupported argument '%s'", argv[i]); \
    return NULL; \
  }

/* function declarations for dimensionality-specific task_run() functions.
 */
int task_run1 (task *T);
int task_run2 (task *T);
int task_run3 (task *T);

/* task_alloc(): allocate and initialize a task structure pointer based
 * on an argument string array.
 *
 * arguments:
 *  @argc: size of the argument string array.
 *  @argv: values in the argument string array.
 *
 * returns:
 *  newly allocated task structure pointer.
 */
task *task_alloc (int argc, char **argv) {
  /* declare required variables:
   *  @T: task structure pointer to allocate and initialize.
   *  @i: general-purpose loop counter.
   */
  task *T;
  int i;

  /* allocate a new structure pointer. */
  T = (task*) malloc(sizeof(task));
  if (!T)
    return NULL;

  /* initialize the filename strings. */
  T->fname_in = NULL;
  T->fname_out = NULL;
  T->fname_log = NULL;
  T->fname_sched = strdup("nuslist");

  /* initialize the logging file handle. */
  T->fh_log = NULL;

  /* initialize the input/output data structure pointers. */
  T->Pin = NULL;
  T->Pout = NULL;
  T->sch = NULL;

  /* initialize the integer variables. */
  T->nx = 0;
  T->ny = 0;
  T->nz = 0;
  T->help = 0;
  T->dims = 0;
  T->iters = 250;
  T->threads = 1;

  /* initialize the float variables. */
  T->accel = 500.0f;
  T->delta = 1.0f;
  T->sigma = 1.0f;
  T->lambda = 0.0f;
  T->wx = 0.0f;
  T->wy = 0.0f;
  T->wz = 0.0f;
  T->jx = 0.0f;
  T->jy = 0.0f;
  T->jz = 0.0f;

  /* loop over all command-line arguments. */
  i = 1;
  while (i < argc) {
    /* check for a recognized argument flag. */
         TASK_PARSE_STR("-in",        T->fname_in)
    else TASK_PARSE_STR("-out",       T->fname_out)
    else TASK_PARSE_STR("-log",       T->fname_log)
    else TASK_PARSE_STR("-sched",     T->fname_sched)
    else TASK_PARSE("-dims",    "%d", T->dims)
    else TASK_PARSE("-xN",      "%d", T->nx)
    else TASK_PARSE("-yN",      "%d", T->ny)
    else TASK_PARSE("-zN",      "%d", T->nz)
    else TASK_PARSE("-xJ",      "%f", T->jx)
    else TASK_PARSE("-yJ",      "%f", T->jy)
    else TASK_PARSE("-zJ",      "%f", T->jz)
    else TASK_PARSE("-xW",      "%f", T->wx)
    else TASK_PARSE("-yW",      "%f", T->wy)
    else TASK_PARSE("-zW",      "%f", T->wz)
    else TASK_PARSE("-iters",   "%d", T->iters)
    else TASK_PARSE("-threads", "%d", T->threads)
    else TASK_PARSE("-accel",   "%f", T->accel)
    else TASK_PARSE("-delta",   "%f", T->delta)
    else TASK_PARSE("-sigma",   "%f", T->sigma)
    else TASK_PARSE("-lambda",  "%f", T->lambda)
    else TASK_PARSE_FLAG("-help", T->help)
    else TASK_PARSE_ERR()

    /* increment the argument index. */
    i++;
  }

  /* check if zero arguments were supplied. */
  if (argc == 1) {
    /* if so, indicate that the task has become much simpler. */
    T->help = 1;
  }

  /* return the task structure pointer. */
  return T;
}

/* task_free(): free all heap-allocated memory with a task structure.
 *
 * arguments:
 *  @T: pointer to the data structure to free.
 */
void task_free (task *T) {
  /* return if the structure pointer is null. */
  if (!T)
    return;

  /* free the input filename string, if necessary. */
  if (T->fname_in)
    free(T->fname_in);

  /* free the output filename string, if necessary. */
  if (T->fname_out)
    free(T->fname_out);

  /* free the log filename string, if necessary. */
  if (T->fname_log)
    free(T->fname_log);

  /* free the schedule filename string, if necessary. */
  if (T->fname_sched)
    free(T->fname_sched);

  /* close the logging file handle, if necessary. */
  if (T->fh_log)
    fclose(T->fh_log);

  /* free the input pipe, if necessary. */
  if (T->Pin)
    nmrpipe_close(T->Pin);

  /* free the output pipe, if necessary. */
  if (T->Pout)
    nmrpipe_close(T->Pout);

  /* free the schedule structure. */
  if (T->sch)
    sched_free(T->sch);

  /* free the structure pointer. */
  free(T);
}

/* task_memcheck_humanize(): convert a byte count into a human-readable
 * size. this is just a little extra fluff to make very rare error
 * messages from task_memcheck() more tangible.
 *
 * arguments:
 *  @n: input byte count to convert.
 *  @f: pointer to the output floating-point value.
 *  @s: string in which to store the output units.
 */
void task_memcheck_humanize (long n, float *f, char *s) {
  /* initialize the output values. */
  *f = (float) n;
  strcpy(s, "B");

  /* convert to kB, if necessary. */
  if (*f >= 1000.0) {
    *f /= 1000.0;
    strcpy(s, "kB");
  }

  /* convert to MB, if necessary. */
  if (*f >= 1000.0) {
    *f /= 1000.0;
    strcpy(s, "MB");
  }

  /* convert to GB, if necessary. */
  if (*f >= 1000.0) {
    *f /= 1000.0;
    strcpy(s, "GB");
  }

  /* convert to TB, if necessary. */
  if (*f >= 1000.0) {
    *f /= 1000.0;
    strcpy(s, "TB");
  }
}

/* task_memcheck(): determine whether a task, as configured, will exceed
 * the physical memory capacity of the system.
 *
 * arguments:
 *  @T: pointer to a task data structure.
 *
 * returns:
 *  integer indicating whether (1) or not (0) the currently configured
 *  task will exceed the total physical memory capacity of the system.
 */
int task_memcheck (task *T) {
  /* declare required variables:
   *  @n_task: number of bytes required by the task.
   *  @n_avail: number of bytes available.
   */
  char s_task[4], s_avail[4];
  float f_task, f_avail;
  long n_task, n_avail;

  /* obtain the available memory limit. */
  n_avail = sysconf(_SC_PHYS_PAGES);
  n_avail *= sysconf(_SC_PAGE_SIZE);

  /* compute the required memory size. */
  n_task = sizeof(float) * ((long) pow(2.0, (float) T->dims));
  n_task *= (T->dims >= 1 ? T->nx : 1);
  n_task *= (T->dims >= 2 ? T->ny : 1);
  n_task *= (T->dims >= 3 ? T->nz : 1);
  n_task *= 6 * T->threads;

  /* check if the required amount exceeds the limit. */
  if (n_task >= n_avail) {
    /* compute human-readable values. */
    task_memcheck_humanize(n_task, &f_task, s_task);
    task_memcheck_humanize(n_avail, &f_avail, s_avail);

    /* output an error message ... */
    failf("task exceeds available memory (%.2f %s >= %.2f %s)",
          f_task, s_task, f_avail, s_avail);

    /* ... and return failure. */
    return 0;
  }

  /* return success. */
  return 1;
}

/* task_run(): execute a reconstruction based on the contents of a
 * task structure pointer.
 *
 * arguments:
 *  @T: task structure pointer to access.
 *
 * returns:
 *  integer indicating whether (1) or not (0) task execution succeeded.
 */
int task_run (task *T) {
  /* declare required variables:
   *  @status: return status from the task execution.
   */
  int status = 0;

  /* check if the task pointer is null. */
  if (!T) {
    /* output an error message and return failure. */
    failf("task structure pointer is null");
    return 0;
  }

  /* check if the help flag has been raised. */
  if (T->help) {
    /* if so, output the usage message and return success. */
    usagef();
    return 1;
  }

  /* check that the iteration count is in bounds. */
  if (T->iters < 1) {
    /* if not, output an error message and return failure. */
    failf("unsupported iteration count %d", T->iters);
    return 0;
  }

  /* check that the thread count is in bounds. */
  if (T->threads < 1 || T->threads > 8192) {
    /* if not, output an error message and return failure. */
    failf("unsupported thread count %d", T->threads);
    return 0;
  }

  /* check that the acceleration factor is in bounds. */
  if (T->accel < 1.0f) {
    /* if not, output an error message and return failure. */
    failf("unsupported acceleration factor %.3f", T->accel);
    return 0;
  }

  /* check that the background is in bounds. */
  if (T->delta <= 0.0f) {
    /* if not, output an error message and return failure. */
    failf("unsupported background %.3f", T->delta);
    return 0;
  }

  /* check that the noise estimate is in bounds. */
  if (T->sigma <= 0.0f) {
    /* if not, output an error message and return failure. */
    failf("unsupported noise estimate %.3f", T->sigma);
    return 0;
  }

  /* check that the Lagrange multiplier is in bounds. */
  if (T->lambda < 0.0f) {
    /* if not, output an error message and return failure. */
    failf("unsupported Lagrange multiplier %.3f", T->lambda);
    return 0;
  }

  /* read the schedule file. */
  T->sch = sched_alloc(T->fname_sched);

  /* check if the schedule file was read successfully. */
  if (!T->sch) {
    /* if not, output an error message and return failure. */
    failf("failed to read schedule file");
    return 0;
  }

  /* check if the dimension count was explicitly set. */
  if (T->dims == 0) {
    /* if not, use the value from the schedule file. */
    T->dims = T->sch->d;
  }
  else {
    /* if so, check the value against the schedule file. */
    if (T->dims != T->sch->d) {
      /* on mismatch, output an error message ... */
      failf("schedule dimension count mismatch (%d != %d)",
            T->dims, T->sch->d);

      /* ... and return failure. */
      return 0;
    }
  }

  /* check that the dimension count is in bounds. */
  if (T->dims < 1 || T->dims > 3) {
    /* if not, output an error message and return failure. */
    failf("unsupported dimension count %d", T->dims);
    return 0;
  }

  /* if the first-dimension size was not explicitly set, use
   * the value guessed from the schedule file.
   */
  if (T->dims >= 1 && T->nx == 0)
    T->nx = T->sch->n1;

  /* if the second-dimension size was not explicitly set, use
   * the value guessed from the schedule file.
   */
  if (T->dims >= 2 && T->ny == 0)
    T->ny = T->sch->n2;

  /* if the third-dimension size was not explicitly set, use
   * the value guessed from the schedule file.
   */
  if (T->dims >= 3 && T->nz == 0)
    T->nz = T->sch->n3;

  /* update the schedule dimensionality and sizes for packing. */
  T->sch->d = T->dims;
  T->sch->n1 = 2 * T->nx;
  T->sch->n2 = 2 * T->ny;
  T->sch->n3 = 2 * T->nz;

  /* pack the schedule array values into linear indices. */
  if (!sched_pack(T->sch)) {
    /* if unsuccessful, output an error message and return failure. */
    failf("failed to pack schedule into linear indices");
    return 0;
  }

  /* check that the parameters will not exceed the total
   * physical memory capacity of the system.
   */
  if (!task_memcheck(T))
    return 0;

  /* check if a log filename was provided. */
  if (T->fname_log) {
    /* attempt to open the log file handle. */
    T->fh_log = fopen(T->fname_log, "w");

    /* check if the file handle was opened. */
    if (!T->fh_log) {
      /* if not, output an error message and return failure. */
      failf("failed to open '%s' for logging", T->fname_log);
      return 0;
    }
  }

  /* open the input pipe source. */
  T->Pin = pipesrc_open(T->fname_in, T->sch);
  if (!T->Pin) {
    /* if unsuccessful, output an error message and return failure. */
    failf("failed to open pipe source");
    return 0;
  }

  /* adjust the pipe header values to match the reconstructed data. */
  switch (T->dims) {
    /* one-dimensional data. */
    case 1:
      T->Pin->hdr.sz = T->nx;
      T->Pin->hdr.tdsz_f1 = T->nx;
      break;

    /* two-dimensional data. */
    case 2:
      T->Pin->hdr.sz = T->nx;
      T->Pin->hdr.specnum = 2 * T->ny;
      T->Pin->hdr.apod_f1 = T->nx;
      T->Pin->hdr.apod_f3 = T->ny;
      T->Pin->hdr.quad = PIPE_QUAD_COMPLEX;
      T->Pin->hdr.quad_f1 = PIPE_QUAD_COMPLEX;
      T->Pin->hdr.quad_f3 = PIPE_QUAD_COMPLEX;
      break;

    /* three-dimensional data. */
    case 3:
      T->Pin->hdr.sz = T->nx;
      T->Pin->hdr.specnum = 2 * T->ny;
      T->Pin->hdr.apod_f1 = T->nx;
      T->Pin->hdr.apod_f3 = T->ny;
      T->Pin->hdr.apod_f4 = T->nz;
      T->Pin->hdr.size_f3 = 2 * T->nz;
      T->Pin->hdr.quad = PIPE_QUAD_COMPLEX;
      T->Pin->hdr.quad_f1 = PIPE_QUAD_COMPLEX;
      T->Pin->hdr.quad_f3 = PIPE_QUAD_COMPLEX;
      T->Pin->hdr.quad_f4 = PIPE_QUAD_COMPLEX;
      break;
  }

  /* open the output pipe sink. */
  T->Pout = pipesink_open(T->fname_out, &T->Pin->hdr);
  if (!T->Pout) {
    /* if unsuccessful, output an error message and return failure. */
    failf("failed to open pipe sink");
    return 0;
  }

  /* compute the convolution coefficients. */
  status = sched_kernel(T->sch, T->dims,
                        T->nx, T->jx, T->wx, T->Pin->hdr.sw_f1,
                        T->ny, T->jy, T->wy, T->Pin->hdr.sw_f3,
                        T->nz, T->jz, T->wz, T->Pin->hdr.sw_f4);

  /* check whether the computation succeeded. */
  if (!status) {
    /* if unsuccessful, output an error message and return failure. */
    failf("failed to compute convolution weights");
    return 0;
  }

  /* set the number of threads to be used during reconstruction. */
  omp_set_num_threads(T->threads);

  /* execute a reconstruction task of the appropriate dimensionality. */
  switch (T->dims) {
    /* one-dimensional reconstruction. */
    case 1:
      status = task_run1(T);
      break;

    /* two-dimensional reconstruction. */
    case 2:
      status = task_run2(T);
      break;

    /* three-dimensional reconstruction. */
    case 3:
      status = task_run3(T);
      break;
  }

  /* return the status value. */
  return status;
}

