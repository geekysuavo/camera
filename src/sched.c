
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

/* FMT_[123]D: format parsing string for reading schedule files.
 */
#define FMT_1D "%d"
#define FMT_2D "%d %d"
#define FMT_3D "%d %d %d"

/* sched_alloc(): allocate and populate a schedule data structure
 * from a file, specified with a filename string.
 *
 * arguments:
 *  @fname: filename of the input schedule to read.
 *
 * returns:
 *  pointer to a newly allocated and populated schedule structure,
 *  or NULL on failure.
 */
sched *sched_alloc (const char *fname) {
  /* declare required variables:
   *  @sch: pointer to the newly allocated schedule structure.
   *  @buf: character buffer to hold each line of schedule text.
   *  @i1, @i2, @i3: parsed grid indices.
   *  @dscan: number of parsed indices.
   *  @fh: input file handle.
   */
  int dscan, i1, i2, i3;
  char buf[N_BUF];
  sched *sch;
  FILE *fh;

  /* allocate a new structure pointer. */
  sch = (sched*) malloc(sizeof(sched));
  if (!sch) {
    /* if unsuccessful, output an error message and return. */
    failf("failed to allocate schedule pointer");
    return NULL;
  }

  /* initialize the contents of the structure. */
  sch->idx = NULL;
  sch->w = NULL;
  sch->n = 0;
  sch->d = 0;
  sch->n1 = 0;
  sch->n2 = 0;
  sch->n3 = 0;

  /* check that the input filename is defined. */
  if (!fname) {
    /* if not, output an error message and return. */
    failf("schedule filename not supplied");
    sched_free(sch);
    return NULL;
  }

  /* open the input file. */
  fh = fopen(fname, "r");
  if (!fh) {
    /* if unsuccessful, output an error message and return. */
    failf("failed to open schedule file '%s'", fname);
    sched_free(sch);
    return NULL;
  }

  /* attempt to read the first line of text from the schedule. */
  if (!fgets(buf, N_BUF, fh)) {
    /* if unsuccessful, output an error message and return. */
    failf("failed to read from '%s'", fname);
    sched_free(sch);
    fclose(fh);
    return NULL;
  }

  /* check for the line format of each dimensionality. */
  if (sscanf(buf, FMT_3D, &i1, &i2, &i3) == 3) {
    /* set three dimensions. */
    sch->d = 3;
  }
  else if (sscanf(buf, FMT_2D, &i1, &i2) == 2) {
    /* set two dimensions. */
    sch->d = 2;
  }
  else if (sscanf(buf, FMT_1D, &i1) == 1) {
    /* set one dimension. */
    sch->d = 1;
  }

  /* check for a sane dimensionality. */
  if (sch->d == 0) {
    /* if not sane, output an error message and return. */
    failf("failed to determine schedule dimensionality");
    sched_free(sch);
    fclose(fh);
    return NULL;
  }

  /* seek back to the beginning of the schedule file. */
  if (fseek(fh, SEEK_SET, 0)) {
    /* if unsuccessful, output an error message and return. */
    failf("failed to seek within the schedule file");
    sched_free(sch);
    fclose(fh);
    return NULL;
  }

  /* loop until the end of the schedule file is reached. */
  while (!feof(fh)) {
    /* read a new line of text from the schedule file. */
    if (fgets(buf, N_BUF, fh)) {
      /* initialize the indices. */
      i1 = i2 = i3 = 0;

      /* parse based on the dimensionality. */
      switch (sch->d) {
        /* one-dimensional. */
        case 1:
          dscan = sscanf(buf, FMT_1D, &i1);
          break;

        /* two-dimensional. */
        case 2:
          dscan = sscanf(buf, FMT_2D, &i1, &i2);
          break;

        /* three-dimensional. */
        case 3:
          dscan = sscanf(buf, FMT_3D, &i1, &i2, &i3);
          break;

        /* else. won't happen. */
        default:
          dscan = 0;
          break;
      }

      /* check for a successful parse. */
      if (dscan != sch->d) {
        /* if not, output an error message and return. */
        failf("failed to parse line %d of '%s'", sch->n + 1, fname);
        sched_free(sch);
        fclose(fh);
        return NULL;
      }

      /* reallocate the index array. */
      sch->n++;
      sch->idx = (int*) realloc(sch->idx, 3 * sch->n * sizeof(int));
      if (!sch->idx) {
        /* if unsuccessful, output an error message and return. */
        failf("failed to reallocate schedule array");
        sched_free(sch);
        fclose(fh);
        return NULL;
      }

      /* store the parsed indices. */
      sch->idx[3 * (sch->n - 1) + 0] = i1;
      sch->idx[3 * (sch->n - 1) + 1] = i2;
      sch->idx[3 * (sch->n - 1) + 2] = i3;

      /* update the maximum grid indices. */
      if (i1 > sch->n1) sch->n1 = i1;
      if (i2 > sch->n2) sch->n2 = i2;
      if (i3 > sch->n3) sch->n3 = i3;
    }
  }

  /* transform the maximum grid indices into powers of two. */
  sch->n1 = nextpow2(sch->n1);
  sch->n2 = nextpow2(sch->n2);
  sch->n3 = nextpow2(sch->n3);

  /* close the input file handle. */
  fclose(fh);

  /* return the allocated and prepared structure pointer. */
  return sch;
}

/* sched_pack(): pack the parsed indices in a schedule array into linear
 * indices based on the current dimensionality and sizes.
 *
 * arguments:
 *  @sch: pointer to the schedule structure to modify.
 *
 * returns:
 *  integer indicating whether (1) or not (0) packing succeeded.
 */
int sched_pack (sched *sch) {
  /* declare required variables:
   *  @i: packed value array index.
   *  @j: unpacked value array index.
   *  @i1, @i2, @i3: unpacked value.
   */
  int i, j, i1, i2, i3;

  /* loop over the packed index counts. */
  for (i = 0, j = 0; i < sch->n; i++, j += 3) {
    /* get the unpacked values from the array. */
    i1 = sch->idx[j];
    i2 = sch->idx[j + 1];
    i3 = sch->idx[j + 2];

    /* check that the first-dimension index is in bounds. */
    if (sch->d >= 1 && i1 >= sch->n1) {
      failf("grid x-index out of bounds (%d >= %d)", i1, sch->n1);
      return 0;
    }

    /* check that the second-dimension index is in bounds. */
    if (sch->d >= 2 && i2 >= sch->n2) {
      failf("grid y-index out of bounds (%d >= %d)", i2, sch->n2);
      return 0;
    }

    /* check that the third-dimension index is in bounds. */
    if (sch->d >= 3 && i3 >= sch->n3) {
      failf("grid z-index out of bounds (%d >= %d)", i3, sch->n3);
      return 0;
    }

    /* pack the indices together. this general statement works for
     * all dimensionalities because the sizes of unused dimensions
     * will be zero.
     */
    sch->idx[i] = i1 + sch->n1 * i2 + sch->n1 * sch->n2 * i3;
  }

  /* reallocate the schedule index array. */
  sch->idx = (int*) realloc(sch->idx, sch->n * sizeof(int));
  if (!sch->idx)
    return 0;

  /* return success. */
  return 1;
}

/* sched_kernel(): compute a set of convolution kernel coefficients for
 * use during reconstruction.
 *
 * arguments:
 *  @sch: pointer to the schedule structure to modify.
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

