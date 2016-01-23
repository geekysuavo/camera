
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

/* arr_alloc1(): allocate a new one-dimensional hypercomplex array.
 *
 * arguments:
 *  @n1: size of the array.
 *  @n2: unused.
 *  @n3: unused.
 *
 * returns:
 *  pointer to a newly allocated one-dimensional hypercomplex
 *  array structure, or NULL on failure.
 */
arr1 *arr_alloc1 (int n1, int n2, int n3) {
  /* declare required variables:
   *  @a: pointer to the newly allocated structure.
   */
  arr1 *a;

  /* check that the array size is valid. */
  if (n1 < 1) {
    /* if not, output an error message and return null. */
    failf("size '%d' is invalid", n1);
    return NULL;
  }

  /* allocate a new structure pointer. */
  a = (arr1*) malloc(sizeof(arr1));
  if (!a)
    return NULL;

  /* store the array size in the structure. */
  a->n = n1;

  /* allocate a new array of coefficients. */
  a->x = (hx1*) fftwf_malloc(sizeof(hx1) * a->n);
  if (!a->x)
    return NULL;

  /* zero the contents of the array. */
  arr_zero1(a);

  /* return the newly allocated structure pointer. */
  return a;
}

/* arr_alloc2(): allocate a new two-dimensional hypercomplex array.
 *
 * arguments:
 *  @n1: first-dimension size of the array.
 *  @n2: second-dimension size of the array.
 *  @n3: unused.
 *
 * returns:
 *  pointer to a newly allocated two-dimensional hypercomplex
 *  array structure, or NULL on failure.
 */
arr2 *arr_alloc2 (int n1, int n2, int n3) {
  /* declare required variables:
   *  @a: pointer to the newly allocated structure.
   */
  arr2 *a;

  /* check that all array sizes are valid. */
  if (n1 < 1 || n2 < 1) {
    /* if not, output an error message and return null. */
    failf("size '%dx%d' is invalid", n1, n2);
    return NULL;
  }

  /* allocate a new structure pointer. */
  a = (arr2*) malloc(sizeof(arr2));
  if (!a)
    return NULL;

  /* store the array sizes in the structure. */
  a->n1 = n1;
  a->n2 = n2;

  /* compute the total size of the array. */
  a->n = n1 * n2;

  /* allocate a new array of coefficients. */
  a->x = (hx2*) fftwf_malloc(sizeof(hx2) * a->n);
  if (!a->x)
    return NULL;

  /* zero the contents of the array. */
  arr_zero2(a);

  /* return the newly allocated structure pointer. */
  return a;
}

/* arr_alloc3(): allocate a new three-dimensional hypercomplex array.
 *
 * arguments:
 *  @n1: first-dimension size of the array.
 *  @n2: second-dimension size of the array.
 *  @n3: third-dimension size of the array.
 *
 * returns:
 *  pointer to a newly allocated three-dimensional hypercomplex
 *  array structure, or NULL on failure.
 */
arr3 *arr_alloc3 (int n1, int n2, int n3) {
  /* declare required variables:
   *  @a: pointer to the newly allocated structure.
   */
  arr3 *a;

  /* check that all array sizes are valid. */
  if (n1 < 1 || n2 < 1 || n3 < 1) {
    /* if not, output an error message and return null. */
    failf("size '%dx%dx%d' is invalid", n1, n2, n3);
    return NULL;
  }

  /* allocate a new structure pointer. */
  a = (arr3*) malloc(sizeof(arr3));
  if (!a)
    return NULL;

  /* store the array sizes in the structure. */
  a->n1 = n1;
  a->n2 = n2;
  a->n3 = n3;

  /* compute the total size of the array. */
  a->n = n1 * n2 * n3;

  /* allocate a new array of coefficients. */
  a->x = (hx3*) fftwf_malloc(sizeof(hx3) * a->n);
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

/* arr_fftfn1(): apply a fast Fourier transform to a
 * one-dimensional hypercomplex array.
 *
 * arguments:
 *  @adest: pointer to the hypercomplex destination data structure.
 *  @asrc: pointer to the hypercomplex source data structure.
 *  @sign: sign/direction of the fast Fourier transform.
 */
void arr_fftfn1 (arr1 *adest, arr1 *asrc, int sign) {
  /* FIXME: implement arr_fftfn1() */
}

/* arr_fftfn2(): apply a fast Fourier transform to a
 * two-dimensional hypercomplex array.
 *
 * arguments:
 *  @adest: pointer to the hypercomplex destination data structure.
 *  @asrc: pointer to the hypercomplex source data structure.
 *  @sign: sign/direction of the fast Fourier transform.
 */
void arr_fftfn2 (arr2 *adest, arr2 *asrc, int sign) {
  /* FIXME: implement arr_fftfn2() */
}

/* arr_fftfn3(): apply a fast Fourier transform to a
 * three-dimensional hypercomplex array.
 *
 * arguments:
 *  @adest: pointer to the hypercomplex destination data structure.
 *  @asrc: pointer to the hypercomplex source data structure.
 *  @sign: sign/direction of the fast Fourier transform.
 */
void arr_fftfn3 (arr3 *adest, arr3 *asrc, int sign) {
  /* FIXME: implement arr_fftfn3() */
}

