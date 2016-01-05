
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
#ifndef __CAMERA_HX_H__
#define __CAMERA_HX_H__

/* include the standard c99 complex math header. */
#include <complex.h>

/* hx0: defined type for H0 (real) scalars (one coefficient).
 */
typedef float hx0;

/* hx1: defined type for H1 (complex) scalars (two coefficients).
 */
typedef float complex hx1;

/* hx2: defined type for H2 hypercomplex scalars (four coefficients).
 * hx2i: integer mask for shuffling elements of hx2 scalars.
 */
typedef float hx2 __attribute__((vector_size(16)));
typedef int hx2i __attribute((vector_size(16)));

/* hx3: defined type for H3 hypercomplex scalars (eight coefficients).
 * hx3i: integer mask for shuffling elements of hx3 scalars.
 */
typedef float hx3 __attribute__((vector_size(32)));
typedef int hx3i __attribute__((vector_size(32)));

/* function declarations: */

hx0 hx_sqrt0 (hx0 a);

hx0 hx_log0 (hx0 a);

hx0 hx_sumsq1 (hx1 a);

hx0 hx_sumsq2 (hx2 a);

hx0 hx_sumsq3 (hx3 a);

hx0 hx_func1 (hx1 x, hx0 L);

hx0 hx_func2 (hx2 x, hx0 L);

hx0 hx_func3 (hx3 x, hx0 L);

hx1 hx_grad1 (hx1 x, hx0 L);

hx2 hx_grad2 (hx2 x, hx0 L);

hx3 hx_grad3 (hx3 x, hx0 L);

#endif /* !__CAMERA_HX_H__ */

