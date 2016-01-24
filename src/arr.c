
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

/* planv: array of globally available fftw plans. the number of
 * plans that will be stored in the array will depend on the task
 * dimensionality.
 */
fftwf_plan *planv;
unsigned int planc;

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

/* arr_plans_init(): initialize the global fftw plan array for use on
 * {one,two,three}-dimensional hypercomplex arrays.
 *
 * arguments:
 *  @n1: first-dimension size of the arrays.
 *  @n2: second-dimension size of the arrays, or zero.
 *  @n3: third-dimension size of the arrays, or zero.
 */
int arr_plans_init (int n1, int n2, int n3) {
  /* declare variables required at any dimensionality:
   *  @dims: transform size and stride data structure.
   *  @vdims: vector size and stride data structure.
   *  @ax, @bx: pointers to raw coefficient data in @a and @b.
   */
  fftwf_iodim dims[3], vdims[3];
  hx0 *ax, *bx;

  /* construct plans based on dimensionality. */
  if (n1 > 1 && n2 > 1 && n3 > 1) {
    /* allocate two temporary arrays. */
    arr3 *a = arr_alloc3(n1, n2, n3);
    arr3 *b = arr_alloc3(n1, n2, n3);
    if (!a || !b)
      return 0;

    /* set up the array data pointers. */
    ax = (hx0*) a->x;
    bx = (hx0*) b->x;

    /* FIXME: set up fftwf_iodim structures. */

    /* allocate the plan array. */
    planc = 24;
    planv = (fftwf_plan*) malloc(sizeof(fftwf_plan) * planc);
    if (!planv)
      return 0;

    /* FIXME construct a plan for dim=1 (a,b) fft. */
    /* FIXME construct a plan for dim=1 (c,d) fft. */
    /* FIXME construct a plan for dim=1 (e,f) fft. */
    /* FIXME construct a plan for dim=1 (g,h) fft. */

    /* FIXME construct a plan for dim=2 (a,c) fft. */
    /* FIXME construct a plan for dim=2 (b,d) fft. */
    /* FIXME construct a plan for dim=2 (e,g) fft. */
    /* FIXME construct a plan for dim=2 (f,h) fft. */

    /* FIXME construct a plan for dim=3 (a,e) fft. */
    /* FIXME construct a plan for dim=3 (b,f) fft. */
    /* FIXME construct a plan for dim=3 (c,g) fft. */
    /* FIXME construct a plan for dim=3 (d,h) fft. */

    /* FIXME construct a plan for dim=1 (a,b) ifft. */
    /* FIXME construct a plan for dim=1 (c,d) ifft. */
    /* FIXME construct a plan for dim=1 (e,f) ifft. */
    /* FIXME construct a plan for dim=1 (g,h) ifft. */

    /* FIXME construct a plan for dim=2 (a,c) ifft. */
    /* FIXME construct a plan for dim=2 (b,d) ifft. */
    /* FIXME construct a plan for dim=2 (e,g) ifft. */
    /* FIXME construct a plan for dim=2 (f,h) ifft. */

    /* FIXME construct a plan for dim=3 (a,e) ifft. */
    /* FIXME construct a plan for dim=3 (b,f) ifft. */
    /* FIXME construct a plan for dim=3 (c,g) ifft. */
    /* FIXME construct a plan for dim=3 (d,h) ifft. */

    /* check that the plans were successfully created. */
    if (!planv[0]  || !planv[1]  || !planv[2]  || !planv[3]  ||
        !planv[4]  || !planv[5]  || !planv[6]  || !planv[7]  ||
        !planv[8]  || !planv[9]  || !planv[10] || !planv[11] ||
        !planv[12] || !planv[13] || !planv[14] || !planv[15] ||
        !planv[16] || !planv[17] || !planv[18] || !planv[19] ||
        !planv[20] || !planv[21] || !planv[22] || !planv[23])
      return 0;

    /* free the temporary arrays. */
    arr_free3(a);
    arr_free3(b);
  }
  else if (n1 > 1 && n2 > 1) {
    /* allocate two temporary arrays. */
    arr2 *a = arr_alloc2(n1, n2, n3);
    arr2 *b = arr_alloc2(n1, n2, n3);
    if (!a || !b)
      return 0;

    /* set up the array data pointers. */
    ax = (hx0*) a->x;
    bx = (hx0*) b->x;

    /* initialize the first-dimension transform size. */
    dims[0].n = n1;
    dims[0].is = 4;
    dims[0].os = 4;

    /* initialize the second-dimension transform size. */
    dims[1].n = n2;
    dims[1].is = 4 * n1;
    dims[1].os = 4 * n1;

    /* initialize the first-dimension vector size. */
    vdims[0].n = n2;
    vdims[0].is = 4 * n1;
    vdims[0].os = 4 * n1;

    /* initialize the second-dimension vector size. */
    vdims[1].n = n1;
    vdims[1].is = 4;
    vdims[1].os = 4;

    /* allocate the plan array. */
    planc = 8;
    planv = (fftwf_plan*) malloc(sizeof(fftwf_plan) * planc);
    if (!planv)
      return 0;

    /* construct a plan for dim=1 (a,b) fft. */
    planv[0] = fftwf_plan_guru_split_dft(
                 1, dims, 1, vdims,
                 ax + 0, ax + 1,
                 bx + 0, bx + 1,
                 FFTW_MEASURE);

    /* construct a plan for dim=1 (c,d) fft. */
    planv[1] = fftwf_plan_guru_split_dft(
                 1, dims, 1, vdims,
                 ax + 2, ax + 3,
                 bx + 2, bx + 3,
                 FFTW_MEASURE);

    /* construct a plan for dim=2 (a,c) fft. */
    planv[2] = fftwf_plan_guru_split_dft(
                 1, dims + 1, 1, vdims + 1,
                 bx + 0, bx + 2,
                 bx + 0, bx + 2,
                 FFTW_MEASURE);

    /* construct a plan for dim=2 (b,d) fft. */
    planv[3] = fftwf_plan_guru_split_dft(
                 1, dims + 1, 1, vdims + 1,
                 bx + 1, bx + 3,
                 bx + 1, bx + 3,
                 FFTW_MEASURE);

    /* construct a plan for dim=1 (a,b) ifft. */
    planv[4] = fftwf_plan_guru_split_dft(
                 1, dims, 1, vdims,
                 ax + 1, ax + 0,
                 bx + 1, bx + 0,
                 FFTW_MEASURE);

    /* construct a plan for dim=1 (c,d) ifft. */
    planv[5] = fftwf_plan_guru_split_dft(
                 1, dims, 1, vdims,
                 ax + 3, ax + 2,
                 bx + 3, bx + 2,
                 FFTW_MEASURE);

    /* construct a plan for dim=2 (a,c) ifft. */
    planv[6] = fftwf_plan_guru_split_dft(
                 1, dims + 1, 1, vdims + 1,
                 bx + 2, bx + 0,
                 bx + 2, bx + 0,
                 FFTW_MEASURE);

    /* construct a plan for dim=2 (b,d) ifft. */
    planv[7] = fftwf_plan_guru_split_dft(
                 1, dims + 1, 1, vdims + 1,
                 bx + 3, bx + 1,
                 bx + 3, bx + 1,
                 FFTW_MEASURE);

    /* check that the plans were successfully created. */
    if (!planv[0] || !planv[1] || !planv[2] || !planv[3] ||
        !planv[4] || !planv[5] || !planv[6] || !planv[7])
      return 0;

    /* free the temporary arrays. */
    arr_free2(a);
    arr_free2(b);
  }
  else if (n1 > 1) {
    /* allocate two temporary arrays. */
    arr1 *a = arr_alloc1(n1, n2, n3);
    arr1 *b = arr_alloc1(n1, n2, n3);
    if (!a || !b)
      return 0;

    /* set up the array data pointers. */
    ax = (hx0*) a->x;
    bx = (hx0*) b->x;

    /* initialize the transform size. */
    dims[0].n = n1;
    dims[0].is = 2;
    dims[0].os = 2;

    /* allocate the plan array. */
    planc = 2;
    planv = (fftwf_plan*) malloc(sizeof(fftwf_plan) * planc);
    if (!planv)
      return 0;

    /* construct a plan for dim=1 (a,b) fft. */
    planv[0] = fftwf_plan_guru_split_dft(
                 1, dims, 0, NULL,
                 ax + 0, ax + 1,
                 bx + 0, bx + 1,
                 FFTW_MEASURE);

    /* construct a plan for dim=1 (a,b) ifft. */
    planv[1] = fftwf_plan_guru_split_dft(
                 1, dims, 0, NULL,
                 ax + 1, ax + 0,
                 bx + 1, bx + 0,
                 FFTW_MEASURE);

    /* check that the plans were successfully created. */
    if (!planv[0] || !planv[1])
      return 0;

    /* free the temporary arrays. */
    arr_free1(a);
    arr_free1(b);
  }
  else {
    /* invalid dimensionality. */
    return 0;
  }

  /* return success. */
  return 1;
}

/* arr_plans_free(): deallocate the global fftw plan array.
 */
void arr_plans_free (void) {
  /* declare required variables:
   *  @i: plan array index.
   */
  int i;

  /* loop over the plans in the plan array. */
  for (i = 0; i < planc; i++)
    fftwf_destroy_plan(planv[i]);

  /* free the plan array pointer. */
  free(planv);
  planc = 0;
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
  /* declare required variables:
   *  @ax, @bx: pointers to raw real coefficient data.
   */
  hx0 *ax, *bx;

  /* obtain raw coefficient data pointers. */
  ax = (hx0*) asrc->x;
  bx = (hx0*) adest->x;

  /* act based on transform direction. */
  switch (sign) {
    /* forward: */
    case FFTW_FORWARD:
      /* dim=1 (a,b) */
      fftwf_execute_split_dft(planv[0], ax + 0, ax + 1, bx + 0, bx + 1);
      break;

    /* backward: */
    case FFTW_BACKWARD:
      /* dim=1 (a,b) */
      fftwf_execute_split_dft(planv[1], ax + 1, ax + 0, bx + 1, bx + 0);
      break;
  }
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
  /* declare required variables:
   *  @ax, @bx: pointers to raw real coefficient data.
   */
  hx0 *ax, *bx;

  /* obtain raw coefficient data pointers. */
  ax = (hx0*) asrc->x;
  bx = (hx0*) adest->x;

  /* act based on transform direction. */
  switch (sign) {
    /* forward: */
    case FFTW_FORWARD:
      /* dim=1 (a,b),(c,d) */
      fftwf_execute_split_dft(planv[0], ax + 0, ax + 1, bx + 0, bx + 1);
      fftwf_execute_split_dft(planv[1], ax + 2, ax + 3, bx + 2, bx + 3);

      /* dim=2 (a,c),(b,d) */
      fftwf_execute_split_dft(planv[2], bx + 0, bx + 2, bx + 0, bx + 2);
      fftwf_execute_split_dft(planv[3], bx + 1, bx + 3, bx + 1, bx + 3);
      break;

    /* backward: */
    case FFTW_BACKWARD:
      /* dim=1 (a,b),(c,d) */
      fftwf_execute_split_dft(planv[4], ax + 1, ax + 0, bx + 1, bx + 0);
      fftwf_execute_split_dft(planv[5], ax + 3, ax + 2, bx + 3, bx + 2);

      /* dim=2 (a,c),(b,d) */
      fftwf_execute_split_dft(planv[6], bx + 2, bx + 0, bx + 2, bx + 0);
      fftwf_execute_split_dft(planv[7], bx + 3, bx + 1, bx + 3, bx + 1);
      break;
  }
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
  /* declare required variables:
   *  @ax, @bx: pointers to raw real coefficient data.
   */
  hx0 *ax, *bx;

  /* obtain raw coefficient data pointers. */
  ax = (hx0*) asrc->x;
  bx = (hx0*) adest->x;

  /* act based on transform direction. */
  switch (sign) {
    /* forward: */
    case FFTW_FORWARD:
      /* FIXME dim=1 (a,b),(c,d),(e,f),(g,h) */
      /* FIXME dim=2 (a,c),(b,d),(e,g),(f,h) */
      /* FIXME dim=3 (a,e),(b,f),(c,g),(d,h) */
      break;

    /* backward: */
    case FFTW_BACKWARD:
      /* FIXME dim=1 (a,b),(c,d),(e,f),(g,h) */
      /* FIXME dim=2 (a,c),(b,d),(e,g),(f,h) */
      /* FIXME dim=3 (a,e),(b,f),(c,g),(d,h) */
      break;
  }
}

