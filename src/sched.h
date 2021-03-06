
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

/* ensure once-only inclusion. */
#ifndef __CAMERA_SCHED_H__
#define __CAMERA_SCHED_H__

/* sched: data structure for n-dimensional sampling schedules.
 */
typedef struct {
  /* @idx: array of sampled grid indices.
   * @w: array of deconvolution weights.
   * @n: number of sampled grid indices.
   */
  int *idx;
  hx0 *w;

  /* @n: number of sampled grid indices.
   * @d: number of grid dimensions.
   */
  int n, d;

  /* @n1, @n2, @n3: guesses of the schedule's underlying grid size.
   */
  int n1, n2, n3;
}
sched;

/* function declarations: */

sched *sched_alloc (const char *fname);

int sched_pack (sched *sch);

int sched_kernel (sched *sch, int dims,
                  int n1, hx0 J1, hx0 w1, hx0 sw1,
                  int n2, hx0 J2, hx0 w2, hx0 sw2,
                  int n3, hx0 J3, hx0 w3, hx0 sw3);

void sched_free (sched *sch);

#endif /* !__CAMERA_SCHED_H__ */

