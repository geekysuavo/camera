
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
   *  @fname_in: FIXME
   *  @fname_out: FIXME
   *  @fname_log: FIXME
   *  @fname_sched: FIXME
   *  @fh_log: FIXME
   */
  char *fname_in, *fname_out, *fname_log, *fname_sched;
  FILE *fh_log;

  /* sampling and weighting structure member:
   *  @sch: FIXME
   */
  sched *sch;

  /* floating-point structure members:
   *  @delta: FIXME
   *  @epsilon: FIXME
   *  @sigma: FIXME
   *  @lambda: FIXME
   *  @L: FIXME
   */
  hx0 delta, epsilon, sigma, lambda, L;

  /* integer structure members:
   *  @iters: FIXME
   *  @n1: FIXME
   *  @n2: FIXME
   *  @n3: FIXME
   */
  int iters, n1, n2, n3;
}
task;

/* function declarations: */

task *task_alloc (int argc, char **argv);

void task_free (task *T);

int task_run (task *T);

#endif /* !__CAMERA_TASK_H__ */

