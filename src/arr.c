
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

/* arr_binary_logn(): compute the base-two logarithm of an integer.
 *
 * arguments:
 *  @n: input value for the computation.
 *
 * returns:
 *  if the input value is a power of two, then its base-two logarithm
 *  is returned. otherwise, a negative value will be returned.
 */
int arr_binary_logn (int n) {
  /* declare required variables:
   *  @nnear: nearest power-of-two value to the input value.
   *  @blogn: current base-two logarithm of the bit counter.
   *  @k: bit counter. always a power of two.
   */
  int nnear, blogn, k;

  /* initialize the bit counter and log value. */
  blogn = 0;
  k = 1;

  /* loop until the counter is no longer less than the input value. */
  while (k < n) {
    /* left-shift the counter and increment the log value. */
    k <<= 1;
    blogn++;
  }

  /* compute the nearest power of two to the input value. */
  nnear = (1 << blogn);

  /* the input and nearest values will be equal only if the
   * input value is a power of two.
   */
  if (n != nnear)
    return -1;

  /* the input value was a power of two. return its logarithm. */
  return blogn;
}

/* arr_alloc1(): allocate a new one-dimensional hypercomplex array.
 *
 * arguments:
 *  @n: size of the array. must be a power of two.
 *
 * returns:
 *  pointer to a newly allocated one-dimensional hypercomplex
 *  array structure, or NULL on failure.
 */
arr1 *arr_alloc1 (int n) {
  /* declare required variables:
   *  @a: pointer to the newly allocated structure.
   *  @l: base-two logarithm of the array size.
   */
  arr1 *a;
  int l;

  /* compute the base-two logarithm of the array size. */
  l = arr_binary_logn(n);

  /* check that the array size is a power of two. */
  if (l < 0) {
    /* if not, output an error message and return null. */
    fprintf(stderr, "Error: size '%d' not a power of two\n", n);
    return NULL;
  }

  /* allocate a new structure pointer. */
  a = (arr1*) malloc(sizeof(arr1));
  if (!a)
    return NULL;

  /* store the array sizes in the structure. */
  a->n = n;
  a->l = l;

  /* allocate a new array of coefficients. */
  a->x = (hx1*) calloc(a->n, sizeof(hx1));
  if (!a->x)
    return NULL;

  /* return the newly allocated structure pointer. */
  return a;
}

/* arr_alloc2(): allocate a new two-dimensional hypercomplex array.
 *
 * arguments:
 *  @n1: first-dimension size of the array. must be a power of two.
 *  @n2: second-dimension size of the array. must be a power of two.
 *
 * returns:
 *  pointer to a newly allocated two-dimensional hypercomplex
 *  array structure, or NULL on failure.
 */
arr2 *arr_alloc2 (int n1, int n2) {
  /* declare required variables:
   *  @l1: base-two logarithm of the array first-dimension size.
   *  @l2: base-two logarithm of the array second-dimension size.
   *  @a: pointer to the newly allocated structure.
   */
  int l1, l2;
  arr2 *a;

  /* compute the base-two logarithms of each array size. */
  l1 = arr_binary_logn(n1);
  l2 = arr_binary_logn(n2);

  /* check that all array sizes are powers of two. */
  if (l1 < 0 || l2 < 0) {
    /* if not, output an error message and return null. */
    fprintf(stderr, "Error: size '%dx%d' not a power of two\n", n1, n2);
    return NULL;
  }

  /* allocate a new structure pointer. */
  a = (arr2*) malloc(sizeof(arr2));
  if (!a)
    return NULL;

  /* store the array first-dimension size in the structure. */
  a->n1 = n1;
  a->l1 = l1;

  /* store the array second-dimension size in the structure. */
  a->n2 = n2;
  a->l2 = l2;

  /* compute the total size of the array. */
  a->n = n1 * n2;
  a->l = a->l1 + a->l2;

  /* allocate a new array of coefficients. */
  a->x = (hx2*) calloc(a->n, sizeof(hx2));
  if (!a->x)
    return NULL;

  /* return the newly allocated structure pointer. */
  return a;
}

/* arr_alloc3(): allocate a new three-dimensional hypercomplex array.
 *
 * arguments:
 *  @n1: first-dimension size of the array. must be a power of two.
 *  @n2: second-dimension size of the array. must be a power of two.
 *  @n3: third-dimension size of the array. must be a power of two.
 *
 * returns:
 *  pointer to a newly allocated three-dimensional hypercomplex
 *  array structure, or NULL on failure.
 */
arr3 *arr_alloc3 (int n1, int n2, int n3) {
  /* declare required variables:
   *  @l1: base-two logarithm of the array first-dimension size.
   *  @l2: base-two logarithm of the array second-dimension size.
   *  @l3: base-two logarithm of the array third-dimension size.
   *  @a: pointer to the newly allocated structure.
   */
  int l1, l2, l3;
  arr3 *a;

  /* compute the base-two logarithms of each array size. */
  l1 = arr_binary_logn(n1);
  l2 = arr_binary_logn(n2);
  l3 = arr_binary_logn(n3);

  /* check that all array sizes are powers of two. */
  if (l1 < 0 || l2 < 0 || l3 < 0) {
    /* if not, output an error message... */
    fprintf(stderr, "Error: size '%dx%dx%d' not a power of two\n",
            n1, n2, n3);

    /* ... and return null. */
    return NULL;
  }

  /* allocate a new structure pointer. */
  a = (arr3*) malloc(sizeof(arr3));
  if (!a)
    return NULL;

  /* store the array first-dimension size in the structure. */
  a->n1 = n1;
  a->l1 = l1;

  /* store the array second-dimension size in the structure. */
  a->n2 = n2;
  a->l2 = l2;

  /* store the array third-dimension size in the structure. */
  a->n3 = n3;
  a->l3 = l3;

  /* compute the total size of the array. */
  a->n = n1 * n2 * n3;
  a->l = a->l1 + a->l2 + a->l3;

  /* allocate a new array of coefficients. */
  a->x = (hx3*) memalign(sizeof(hx3), a->n * sizeof(hx3));
  if (!a->x)
    return NULL;

  /* zero the contents of the array. */
  arr_zero3(a);

  /* return the newly allocated structure pointer. */
  return a;
}

/* arr_free1(): free all heap-allocated memory within
 * a one-dimensional hypercomplex array.
 *
 * arguments:
 *  @a: pointer to the data structure to free.
 */
void arr_free1 (arr1 *a) {
  /* return if the structure pointer is null. */
  if (!a)
    return;

  /* free the array coefficient data. */
  free(a->x);
  a->x = NULL;

  /* free the structure pointer. */
  free(a);
  a = NULL;
}

/* arr_free2(): free all heap-allocated memory within
 * a two-dimensional hypercomplex array.
 *
 * arguments:
 *  @a: pointer to the data structure to free.
 */
void arr_free2 (arr2 *a) {
  /* return if the structure pointer is null. */
  if (!a)
    return;

  /* free the array coefficient data. */
  free(a->x);
  a->x = NULL;

  /* free the structure pointer. */
  free(a);
  a = NULL;
}

/* arr_free3(): free all heap-allocated memory within
 * a three-dimensional hypercomplex array.
 *
 * arguments:
 *  @a: pointer to the data structure to free.
 */
void arr_free3 (arr3 *a) {
  /* return if the structure pointer is null. */
  if (!a)
    return;

  /* free the array coefficient data. */
  free(a->x);
  a->x = NULL;

  /* free the structure pointer. */
  free(a);
  a = NULL;
}

/* arr_zero1(): zero the data of a one-dimensional hypercomplex array.
 *
 * arguments:
 *  @a: pointer to the data structure to modify.
 */
inline void arr_zero1 (arr1 *a) {
  /* set all array coefficients to zero. */
  memset(a->x, 0, a->n * sizeof(hx1));
}

/* arr_zero2(): zero the data of a two-dimensional hypercomplex array.
 *
 * arguments:
 *  @a: pointer to the data structure to modify.
 */
inline void arr_zero2 (arr2 *a) {
  /* set all array coefficients to zero. */
  memset(a->x, 0, a->n * sizeof(hx2));
}

/* arr_zero3(): zero the data of a three-dimensional hypercomplex array.
 *
 * arguments:
 *  @a: pointer to the data structure to modify.
 */
inline void arr_zero3 (arr3 *a) {
  /* set all array coefficients to zero. */
  memset(a->x, 0, a->n * sizeof(hx3));
}

/* arr_copy1(): copy the data of a one-dimensional hypercomplex array
 * into another. it is assumed that the sizes of both arrays are equal.
 *
 * arguments:
 *  @adest: pointer to the destination data structure.
 *  @asrc: pointer to the source data structure.
 */
inline void arr_copy1 (arr1 *adest, arr1 *asrc) {
  /* copy all array coefficients from the source to the destination. */
  memcpy(adest->x, asrc->x, asrc->n * sizeof(hx1));
}

/* arr_copy2(): copy the data of a two-dimensional hypercomplex array
 * into another. it is assumed that the sizes of both arrays are equal.
 *
 * arguments:
 *  @adest: pointer to the destination data structure.
 *  @asrc: pointer to the source data structure.
 */
inline void arr_copy2 (arr2 *adest, arr2 *asrc) {
  /* copy all array coefficients from the source to the destination. */
  memcpy(adest->x, asrc->x, asrc->n * sizeof(hx2));
}

/* arr_copy3(): copy the data of a three-dimensional hypercomplex array
 * into another. it is assumed that the sizes of both arrays are equal.
 *
 * arguments:
 *  @adest: pointer to the destination data structure.
 *  @asrc: pointer to the source data structure.
 */
inline void arr_copy3 (arr3 *adest, arr3 *asrc) {
  /* copy all array coefficients from the source to the destination. */
  memcpy(adest->x, asrc->x, asrc->n * sizeof(hx3));
}

