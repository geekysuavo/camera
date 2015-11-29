
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
#ifndef __CAMERA_ARR_H__
#define __CAMERA_ARR_H__

/* include the c library memory allocation header. */
#include <malloc.h>

/* arr1: data structure for one-dimensional hypercomplex arrays.
 */
typedef struct {
  /* @x: linear array of scalar hypercomplex array elements. */
  hx1 *x;

  /* @n: total array size. */
  int n;

  /* @l: total array size, log2. */
  int l;
}
arr1;

/* arr2: data structure for two-dimensional hypercomplex arrays.
 */
typedef struct {
  /* @x: linear array of scalar hypercomplex array elements. */
  hx2 *x;

  /* @n: total array size.
   * @n1: first-dimension array size.
   * @n2: second-dimension array size
   */
  int n, n1, n2;

  /* @l: total array size, log2.
   * @l1: first-dimension array size, log2.
   * @l2: second-dimension array size, log2.
   */
  int l, l1, l2;
}
arr2;

/* arr3: data structure for three-dimensional hypercomplex arrays.
 */
typedef struct {
  /* @x: linear array of scalar hypercomplex array elements. */
  hx3 *x;

  /* @n: total array size.
   * @n1: first-dimension array size.
   * @n2: second-dimension array size.
   * @n3: third-dimension array size.
   */
  int n, n1, n2, n3;

  /* @l: total array size, log2.
   * @l1: first-dimension array size, log2.
   * @l2: second-dimension array size, log2.
   * @l3: third-dimension array size, log2.
   */
  int l, l1, l2, l3;
}
arr3;

/* function declarations: */

arr1 *arr_alloc1 (int n1, int n2, int n3);

arr2 *arr_alloc2 (int n1, int n2, int n3);

arr3 *arr_alloc3 (int n1, int n2, int n3);

void arr_free1 (arr1 *a);

void arr_free2 (arr2 *a);

void arr_free3 (arr3 *a);

void arr_zero1 (arr1 *a);

void arr_zero2 (arr2 *a);

void arr_zero3 (arr3 *a);

void arr_copy1 (arr1 *adest, arr1 *asrc);

void arr_copy2 (arr2 *adest, arr2 *asrc);

void arr_copy3 (arr3 *adest, arr3 *asrc);

#endif /* !__CAMERA_ARR_H__ */

