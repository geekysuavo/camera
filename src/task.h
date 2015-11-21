
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

/* function declarations: */

int task_run1 (char *fname_in, char *fname_out,
               char *fname_log, char *fname_sched,
               const int iters, const int threads,
               hx0 delta, hx0 sigma, hx0 lambda,
               int n);

int task_run2 (char *fname_in, char *fname_out,
               char *fname_log, char *fname_sched,
               const int iters, const int threads,
               hx0 delta, hx0 sigma, hx0 lambda,
               int n1, int n2);

int task_run3 (char *fname_in, char *fname_out,
               char *fname_log, char *fname_sched,
               const int iters, const int threads,
               hx0 delta, hx0 sigma, hx0 lambda,
               int n1, int n2, int n3);

#endif /* !__CAMERA_TASK_H__ */

