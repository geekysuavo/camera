
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

/* N_BUF: size of the character buffer used while reading from
 * schedule files.
 */
#define N_BUF  256

/* sched_init(): allocate an empty schedule structure pointer.
 *
 * returns:
 *  newly allocated schedule structure pointer with empty index arrays,
 *  or NULL if unable to allocate a pointer.
 */
sched *sched_init (void) {
  /* declare required variables:
   *  @sch: pointer to the newly allocated schedule structure.
   */
  sched *sch;

  /* allocate a new structure pointer. */
  sch = (sched*) malloc(sizeof(sched));
  if (!sch)
    return NULL;

  /* initialize the contents of the structure. */
  sch->idx = NULL;
  sch->w = NULL;
  sch->n = 0;

  /* return the allocated and prepared structure pointer. */
  return sch;
}

/* sched_alloc1(): allocate and populate a one-dimensional schedule
 * from a file, specified with a filename string.
 *
 * arguments:
 *  @fname: filename of the input schedule to read.
 *  @n: size of the sampling grid.
 *
 * returns:
 *  pointer to a newly allocated and populated schedule structure,
 *  or NULL on failure.
 */
sched *sched_alloc1 (const char *fname, int n) {
  /* declare required variables:
   *  @buf: character buffer to hold each line of schedule text.
   *  @sch: pointer to the output sampling schedule.
   *  @fh: file handle of the schedule text file.
   *  @i: currently parsed grid index.
   */
  char buf[N_BUF];
  sched *sch;
  FILE *fh;
  int i;

  /* initialize the schedule structure pointer. */
  sch = sched_init();
  if (!sch)
    return NULL;

  /* attempt to open the input schedule file. */
  fh = fopen(fname, "r");
  if (!fh)
    return NULL;

  /* loop until the end of the schedule file is reached. */
  while (!feof(fh)) {
    /* read a new line of text from the schedule file. */
    if (fgets(buf, N_BUF, fh)) {
      /* parse the line of text into a grid index. */
      if (sscanf(buf, "%d", &i) == 1) {
        /* check that the grid index is in bounds. */
        if (i < 0 || i >= n) {
          /* if not, output an error message and return null. */
          failf("grid index out of bounds");
          return NULL;
        }

        /* increment the schedule size and reallocate its array. */
        sch->n++;
        sch->idx = (int*) realloc(sch->idx, sch->n * sizeof(int));
        if (!sch->idx)
          return NULL;

        /* store the new grid index into the schedule array. */
        sch->idx[sch->n - 1] = i;
      }
    }
  }

  /* close the input file and return the schedule structure pointer. */
  fclose(fh);
  return sch;
}

/* sched_alloc2(): allocate and populate a two-dimensional schedule
 * from a file, specified with a filename string.
 *
 * arguments:
 *  @fname: filename of the input schedule to read.
 *  @n1: first-dimension size of the sampling grid.
 *  @n2: second-dimension size of the sampling grid.
 *
 * returns:
 *  pointer to a newly allocated and populated schedule structure,
 *  or NULL on failure.
 */
sched *sched_alloc2 (const char *fname, int n1, int n2) {
  /* declare required variables:
   *  @buf: character buffer to hold each line of schedule text.
   *  @i1: currently parsed first-dimension grid index.
   *  @i2: currently parsed second-dimension grid index.
   *  @sch: pointer to the output sampling schedule.
   *  @fh: file handle of the schedule text file.
   */
  char buf[N_BUF];
  int i1, i2;
  sched *sch;
  FILE *fh;

  /* initialize the schedule structure pointer. */
  sch = sched_init();
  if (!sch)
    return NULL;

  /* attempt to open the input schedule file. */
  fh = fopen(fname, "r");
  if (!fh)
    return NULL;

  /* loop until the end of the schedule file is reached. */
  while (!feof(fh)) {
    /* read a new line of text from the schedule file. */
    if (fgets(buf, N_BUF, fh)) {
      /* parse the line of text into a grid index. */
      if (sscanf(buf, "%d %d", &i1, &i2) == 2) {
        /* check that the grid index is in bounds. */
        if (i1 < 0 || i1 >= n1 ||
            i2 < 0 || i2 >= n2) {
          /* if not, output an error message and return null. */
          failf("grid index out of bounds");
          return NULL;
        }

        /* increment the schedule size and reallocate its array. */
        sch->n++;
        sch->idx = (int*) realloc(sch->idx, sch->n * sizeof(int));
        if (!sch->idx)
          return NULL;

        /* pack and store the new grid index into the schedule array. */
        sch->idx[sch->n - 1] = i1 + n1 * i2;
      }
    }
  }

  /* close the input file and return the schedule structure pointer. */
  fclose(fh);
  return sch;
}

/* sched_alloc3(): allocate and populate a three-dimensional schedule
 * from a file, specified with a filename string.
 *
 * arguments:
 *  @fname: filename of the input schedule to read.
 *  @n1: first-dimension size of the sampling grid.
 *  @n2: second-dimension size of the sampling grid.
 *  @n3: third-dimension size of the sampling grid.
 *
 * returns:
 *  pointer to a newly allocated and populated schedule structure,
 *  or NULL on failure.
 */
sched *sched_alloc3 (const char *fname, int n1, int n2, int n3) {
  /* declare required variables:
   *  @buf: character buffer to hold each line of schedule text.
   *  @i1: currently parsed first-dimension grid index.
   *  @i2: currently parsed second-dimension grid index.
   *  @i3: currently parsed third-dimension grid index.
   *  @sch: pointer to the output sampling schedule.
   *  @fh: file handle of the schedule text file.
   */
  char buf[N_BUF];
  int i1, i2, i3;
  sched *sch;
  FILE *fh;

  /* initialize the schedule structure pointer. */
  sch = sched_init();
  if (!sch)
    return NULL;

  /* attempt to open the input schedule file. */
  fh = fopen(fname, "r");
  if (!fh)
    return NULL;

  /* loop until the end of the schedule file is reached. */
  while (!feof(fh)) {
    /* read a new line of text from the schedule file. */
    if (fgets(buf, N_BUF, fh)) {
      /* parse the line of text into a grid index. */
      if (sscanf(buf, "%d %d %d", &i1, &i2, &i3) == 3) {
        /* check that the grid index is in bounds. */
        if (i1 < 0 || i1 >= n1 ||
            i2 < 0 || i2 >= n2 ||
            i3 < 0 || i3 >= n3) {
          /* if not, output an error message and return null. */
          failf("grid index out of bounds");
          return NULL;
        }

        /* increment the schedule size and reallocate its array. */
        sch->n++;
        sch->idx = (int*) realloc(sch->idx, sch->n * sizeof(int));
        if (!sch->idx)
          return NULL;

        /* pack and store the new grid index into the schedule array. */
        sch->idx[sch->n - 1] = i1 + n1 * i2 + n1 * n2 * i3;
      }
    }
  }

  /* close the input file and return the schedule structure pointer. */
  fclose(fh);
  return sch;
}

/* sched_alloc(): allocate and populate a schedule data structure
 * from a file, specified with a filename string.
 *
 * arguments:
 *  @fname: filename of the input schedule to read.
 *  @dims: number of dimensions in the sampling grid.
 *  @n1: first-dimension size of the sampling grid.
 *  @n2: second-dimension size of the sampling grid.
 *  @n3: third-dimension size of the sampling grid.
 *
 * returns:
 *  pointer to a newly allocated and populated schedule structure,
 *  or NULL on failure.
 */
sched *sched_alloc (const char *fname, int dims,
                    int n1, int n2, int n3) {
  /* determine which dimensionality to use. */
  switch (dims) {
    /* one-dimensional. */
    case 1:
      return sched_alloc1(fname, n1);

    /* two-dimensional. */
    case 2:
      return sched_alloc2(fname, n1, n2);

    /* three-dimensional. */
    case 3:
      return sched_alloc3(fname, n1, n2, n3);
  }

  /* if an unsupported dimensionality was passed, return failure. */
  return NULL;
}

/* sched_free(): free all heap-allocated memory within a schedule.
 *
 * arguments:
 *  @sch: pointer to the data structure to free.
 */
void sched_free (sched *sch) {
  /* return if the structure pointer is null. */
  if (!sch)
    return;

  /* check if the index array is allocated. */
  if (sch->idx) {
    /* if so, free the index array. */
    free(sch->idx);
    sch->idx = NULL;
  }

  /* free the structure pointer. */
  free(sch);
  sch = NULL;
}

/* sched_kernel(): compute a set of convolution kernel coefficients for
 * use during reconstruction.
 *
 * arguments:
 *  @sch: pointer to the schedule structure pointer.
 *  @dims: number of grid dimensions.
 *  @n1, @J1, @w1, @sw1: first-dimension parameters.
 *  @n2, @J2, @w2, @sw2: second-dimension parameters.
 *  @n3, @J3, @w3, @sw3: third-dimension parameters.
 *
 * returns:
 *  integer indicating whether (1) or not (0) the computation succeeded.
 */
int sched_kernel (sched *sch, int dims,
                  int n1, hx0 J1, hx0 w1, hx0 sw1,
                  int n2, hx0 J2, hx0 w2, hx0 sw2,
                  int n3, hx0 J3, hx0 w3, hx0 sw3) {
  /* declare required variables:
   *  @i: general-purpose loop counter.
   *  @k: packed grid index.
   *  @k1, @k2, @k3: unpacked grid indices.
   *  @dt1, @dt2, @dt3: dwell times.
   */
  int i, k, k1, k2, k3;
  hx0 dt1, dt2, dt3;

  /* return if the structure pointer is null. */
  if (!sch)
    return 0;

  /* allocate an array of convolution coefficients. */
  sch->w = (hx0*) calloc(sch->n, sizeof(hx0));
  if (!sch->w)
    return 0;

  /* compute the dwell times from the spectral widths. */
  dt1 = (sw1 == 0.0f ? 1.0f : 1.0f / sw1);
  dt2 = (sw2 == 0.0f ? 1.0f : 1.0f / sw2);
  dt3 = (sw3 == 0.0f ? 1.0f : 1.0f / sw3);

  /* loop over the array of coefficients. */
  for (i = 0; i < sch->n; i++) {
    /* initialize the coefficient and get the packed index. */
    sch->w[i] = 1.0f;
    k = sch->idx[i];

    /* check if the schedule has at least one dimension. */
    if (dims >= 1) {
      /* compute the first-dimension index. */
      k1 = k % n1;

      /* multiply in the first-dimension contributions. */
      sch->w[i] *= cos(M_PI * J1 * dt1 * ((hx0) k1));
      sch->w[i] *= exp(-w1 * dt1 * ((hx0) k1));
    }

    /* check if the schedule has at least two dimensions. */
    if (dims >= 2) {
      /* compute the second-dimension index. */
      k2 = ((k - k1) / n1) % n2;

      /* multiply in the second-dimension contributions. */
      sch->w[i] *= cos(M_PI * J2 * dt2 * ((hx0) k2));
      sch->w[i] *= exp(-w2 * dt2 * ((hx0) k2));
    }

    /* check if the schedule has three dimensions. */
    if (dims >= 3) {
      /* compute the second-dimension index. */
      k3 = (k - k1 - n1 * k2) / (n1 * n2);

      /* multiply in the second-dimension contributions. */
      sch->w[i] *= cos(M_PI * J3 * dt3 * ((hx0) k3));
      sch->w[i] *= exp(-w3 * dt3 * ((hx0) k3));
    }
  }

  /* return success. */
  return 1;
}

