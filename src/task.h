
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

/* ensure once-only inclusion. */
#ifndef __CAMERA_TASK_H__
#define __CAMERA_TASK_H__

/* task: data structure for organizing the massive amount of arguments
 * that must be sent to each task_run() and task_core() function.
 */
typedef struct {
  /* filename and file handle related structure members:
   *  @fname_in: input filename string, or NULL if reading from stdin.
   *  @fname_out: output filename string, or NULL if writing to stdout.
   *  @fname_log: output log filename string, or NULL if not logging.
   *  @fname_sched: required sampling schedule filename string.
   *  @fh_log: logging file handle.
   */
  char *fname_in, *fname_out, *fname_log, *fname_sched;
  FILE *fh_log;

  /* spectral data input/output related structure members:
   *  @Pin: input pipe source structure pointer.
   *  @Pout: output pipe source structure pointer.
   *  @sch: schedule data structure pointer.
   */
  nmrpipe *Pin, *Pout;
  sched *sch;

  /* floating-point structure members:
   *  @epsilon: noise power estimate for constant-aim mode.
   *  @delta: default parameter for the regularization functional.
   *  @sigma: noise amplitude estimate for constant-aim mode.
   *  @lambda: Lagrange multiplier for constant-lambda mode.
   *  @L: Lipschitz constant for the regularization functional.
   *  @wx, @wy, @wz: exponential deconvolution linewidths.
   *  @jx, @jy, @jz: J-coupling deconvolution frequencies.
   */
  hx0 epsilon, delta, sigma, lambda, L;
  hx0 wx, wy, wz, jx, jy, jz;

  /* integer structure members:
   *  @help: whether or not to print the usage message.
   *  @dims: number of dimensions to reconstruct.
   *  @iters: number of reconstruction iterations to perform.
   *  @threads: number of parallel reconstructions to perform.
   *  @nx, @ny, @nz: reconstruction array sizes.
   */
  int help, dims, iters, threads, nx, ny, nz;
}
task;

/* function declarations: */

task *task_alloc (int argc, char **argv);

void task_free (task *T);

int task_run (task *T);

#endif /* !__CAMERA_TASK_H__ */

