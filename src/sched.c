
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
  sch->n = 0;

  /* return the allocated and prepared structure pointer. */
  return sch;
}

/* sched_alloc1: allocate and populate a one-dimensional schedule
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
          fprintf(stderr, "Error: grid index out of bounds\n");
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

/* sched_alloc2: allocate and populate a two-dimensional schedule
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
          fprintf(stderr, "Error: grid index out of bounds\n");
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

/* sched_alloc3: allocate and populate a three-dimensional schedule
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
          fprintf(stderr, "Error: grid index out of bounds\n");
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

