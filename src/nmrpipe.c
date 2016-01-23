
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

/* pipesrc_open(): allocate a pipe structure pointer for reading
 * pipe-format nonuniform data from a file or a stream.
 *
 * arguments:
 *  @fname: input filename, or null to read from standard input.
 *  @sch: pointer to the schedule to use during reading.
 *
 * returns:
 *  pointer to a newly allocated and initialized pipe structure, or
 *  null on failure.
 */
nmrpipe *pipesrc_open (char *fname, sched *sch) {
  /* declare required variables:
   *  @n_bytes: number of bytes required to hold the pipe header.
   *  @bytes: array of bytes to hold the pipe header.
   *  @P: pointer to the pipe structure to allocate.
   */
  char *bytes;
  int n_bytes;
  nmrpipe *P;

  /* attempt to allocate a new structure pointer. */
  P = (nmrpipe*) malloc(sizeof(nmrpipe));
  if (!P)
    return NULL;

  /* initialize the structure fields. */
  P->sch = sch;
  P->fname = NULL;
  P->fh = stdin;

  /* check if an input filename was provided. */
  if (fname) {
    /* if so, attempt to open the file. */
    P->fname = fname;
    P->fh = fopen(fname, "r");
  }

  /* check that the input file handle is valid. */
  if (!P->fh) {
    /* if not, output an error message and return null. */
    failf("failed to open file '%s'", P->fname);
    nmrpipe_close(P);
    return NULL;
  }

  /* allocate an array to hold the raw pipe header data. */
  n_bytes = sizeof(struct nmrpipe_hdr);
  bytes = (char*) malloc(n_bytes * sizeof(char));
  if (!bytes) {
    /* if unsuccessful, return null. */
    nmrpipe_close(P);
    return NULL;
  }

  /* attempt to read the pipe header data from the file handle. */
  if (fread(bytes, sizeof(char), n_bytes, P->fh) != n_bytes) {
    /* if unsuccessful, output an error message and return null. */
    failf("failed to read pipe header");
    nmrpipe_close(P);
    return NULL;
  }

  /* copy the read bytes into the header structure. */
  memcpy(&P->hdr, bytes, n_bytes);
  free(bytes);

  /* return the allocated and prepared structure pointer. */
  return P;
}

/* pipesink_open(): allocate a pipe structure pointer for writing
 * pipe-format nonuniform data to a file or a stream.
 *
 * arguments:
 *  @fname: output filename, or null to write to standard output.
 *  @hdr: pointer to a pipe header structure that will define the output.
 *
 * returns:
 *  pointer to a newly allocated and initialized pipe structure, or
 *  null on failure.
 */
nmrpipe *pipesink_open (char *fname, struct nmrpipe_hdr *hdr) {
  /* declare required variables:
   *  @n_hdr: number of floats required to hold the pipe header.
   *  @P: pointer to the pipe structure to allocate.
   */
  int n_hdr;
  nmrpipe *P;

  /* attempt to allocate a new structure pointer. */
  P = (nmrpipe*) malloc(sizeof(nmrpipe));
  if (!P)
    return NULL;

  /* copy the contents of the supplied pipe header into the newly
   * allocated pipe structure.
   */
  memcpy(&P->hdr, hdr, sizeof(struct nmrpipe_hdr));

  /* initialize the structure fields. */
  P->sch = NULL;
  P->fname = NULL;
  P->fh = stdout;

  /* check if an output filename was provided. */
  if (fname) {
    /* if so, attempt to open the file. */
    P->fname = fname;
    P->fh = fopen(fname, "w");
  }

  /* check that the output file handle is valid. */
  if (!P->fh) {
    /* if not, output an error message and return null. */
    failf("failed to open file '%s'", P->fname);
    nmrpipe_close(P);
    return NULL;
  }

  /* attempt to write the pipe header data to the file handle. */
  n_hdr = sizeof(struct nmrpipe_hdr) / sizeof(float);
  if (fwrite(&P->hdr, sizeof(float), n_hdr, P->fh) != n_hdr) {
    /* if unsuccessul, output an error message and return null. */
    failf("failed to write pipe header");
    nmrpipe_close(P);
    return NULL;
  }

  /* return the allocated and prepared structure pointer. */
  return P;
}

/* pipesrc_nread(): compute the expected number of read() operations that
 * may be performed upon a pipe source without errors.
 *
 * arguments:
 *  @P: pointer to the pipe structure to read.
 *
 * returns:
 *  integer indicating the number of pipesrc_read[123]() calls that the
 *  pipe file or stream will successfully admit, or zero on failure.
 */
int pipesrc_nread (nmrpipe *P) {
  /* initialize the output value. */
  int n = 0;

  /* check if the f2 dimension has been cropped (via EXT) */
  if (P->hdr.x1_f2 && P->hdr.xn_f2) {
    /* it has. compute the cropped size. */
    n = P->hdr.xn_f2 - P->hdr.x1_f2 + 1;
  }
  else {
    /* it hasn't. return the complete size. */
    n = P->hdr.ftsz_f2 / 2;
  }

  /* return the determined read count. */
  return n;
}

/* pipesrc_read1(): read a nonuniform trace from a pipe source.
 *
 * arguments:
 *  @P: pointer to the pipe structure to read data from.
 *  @x: pointer to the array structure to store values in.
 *
 * returns:
 *  integer indicating whether (1) or not (0) the read was successful.
 */
int pipesrc_read1 (nmrpipe *P, arr1 *x) {
  /* declare required variables:
   *  @i: schedule index loop counter.
   *  @n_buf: number of floats to read.
   *  @n_read: actual number of floats read.
   *  @buf: buffer for holding raw float data.
   */
  int i, n_buf, n_read;
  float *buf;

  /* attempt to allocate a buffer for storing read data. */
  n_buf = 2 * P->sch->n;
  buf = (float*) malloc(n_buf * sizeof(float));
  if (!buf)
    return 0;

  /* attempt to read the expected number of floats from the file handle. */
  n_read = fread(buf, sizeof(float), n_buf, P->fh);

  /* check that the number of expected floats was read. */
  if (n_read != n_buf) {
    /* if not, return failure. */
    free(buf);
    return 0;
  }

  /* initialize the output array contents. */
  arr_zero1(x);

  /* loop over all schedule indices. */
  for (i = 0; i < P->sch->n; i++) {
    /* store the two coefficients into the output array. */
    x->x[P->sch->idx[i]] = buf[i] + buf[i + P->sch->n] * I;
  }

  /* free the read buffer and return success. */
  free(buf);
  return 1;
}

/* pipesrc_read2(): read a nonuniform plane from a pipe source.
 *
 * arguments:
 *  @P: pointer to the pipe structure to read data from.
 *  @x: pointer to the array structure to store values in.
 *
 * returns:
 *  integer indicating whether (1) or not (0) the read was successful.
 */
int pipesrc_read2 (nmrpipe *P, arr2 *x) {
  /* declare required variables:
   *  @i: schedule index loop counter.
   *  @n_buf: number of floats to read.
   *  @n_read: actual number of floats read.
   *  @buf: buffer for holding raw float data.
   */
  int i, n_buf, n_read;
  float *buf;

  /* attempt to allocate a buffer for storing read data. */
  n_buf = 4 * P->sch->n;
  buf = (float*) malloc(n_buf * sizeof(float));
  if (!buf)
    return 0;

  /* attempt to read the expected number of floats from the file handle. */
  n_read = fread(buf, sizeof(float), n_buf, P->fh);

  /* check that the number of expected floats was read. */
  if (n_read != n_buf) {
    /* if not, return failure. */
    free(buf);
    return 0;
  }

  /* initialize the output array contents. */
  arr_zero2(x);

  /* loop over all schedule indices. */
  for (i = 0; i < P->sch->n; i++) {
    /* store the four coefficients into the output array. */
    x->x[P->sch->idx[i]][0] = buf[4 * i];
    x->x[P->sch->idx[i]][1] = buf[4 * i + 1];
    x->x[P->sch->idx[i]][2] = buf[4 * i + 2];
    x->x[P->sch->idx[i]][3] = buf[4 * i + 3];
  }

  /* free the read buffer and return success. */
  free(buf);
  return 1;
}

/* pipesrc_read3(): read a nonuniform cube from a pipe source.
 *
 * arguments:
 *  @P: pointer to the pipe structure to read data from.
 *  @x: pointer to the array structure to store values in.
 *
 * returns:
 *  integer indicating whether (1) or not (0) the read was successful.
 */
int pipesrc_read3 (nmrpipe *P, arr3 *x) {
  /* declare required variables:
   *  @i: schedule index loop counter.
   *  @n_buf: number of floats to read.
   *  @n_read: actual number of floats read.
   *  @buf: buffer for holding raw float data.
   */
  int i, n_buf, n_read;
  float *buf;

  /* attempt to allocate a buffer for storing read data. */
  n_buf = 8 * P->sch->n;
  buf = (float*) malloc(n_buf * sizeof(float));
  if (!buf)
    return 0;

  /* attempt to read the expected number of floats from the file handle. */
  n_read = fread(buf, sizeof(float), n_buf, P->fh);

  /* check that the number of expected floats was read. */
  if (n_read != n_buf) {
    /* if not, return failure. */
    free(buf);
    return 0;
  }

  /* initialize the output array contents. */
  arr_zero3(x);

  /* loop over all schedule indices. */
  for (i = 0; i < P->sch->n; i++) {
    /* store the eight coefficients into the output array. */
    x->x[P->sch->idx[i]][0] = buf[8 * i];
    x->x[P->sch->idx[i]][1] = buf[8 * i + 1];
    x->x[P->sch->idx[i]][2] = buf[8 * i + 2];
    x->x[P->sch->idx[i]][3] = buf[8 * i + 3];
    x->x[P->sch->idx[i]][4] = buf[8 * i + 4];
    x->x[P->sch->idx[i]][5] = buf[8 * i + 5];
    x->x[P->sch->idx[i]][6] = buf[8 * i + 6];
    x->x[P->sch->idx[i]][7] = buf[8 * i + 7];
  }

  /* free the read buffer and return success. */
  free(buf);
  return 1;
}

/* pipesink_write1(): write a uniform trace to a pipe sink.
 *
 * arguments:
 *  @P: pointer to the pipe structure to write data to.
 *  @x: pointer to the array structure to pull values from.
 *
 * returns:
 *  integer indicating whether (1) or not (0) the write was successful.
 */
int pipesink_write1 (nmrpipe *P, arr1 *x) {
  /* declare required variables:
   *  @i: array index loop counter.
   *  @h: half the length of the hypercomplex array.
   *  @n_buf: number of floats to write.
   *  @n_write: actual number of floats written.
   *  @buf: buffer to hold data to be written.
   */
  int i, h, n_buf, n_write;
  float *buf;

  /* compute the relevant sizes for writing. */
  h = x->n / 2;
  n_buf = 2 * h;

  /* attempt to allocate a buffer to hold data for writing. */
  buf = (float*) calloc(n_buf, sizeof(float));
  if (!buf)
    return 0;

  /* loop over the hypercomplex array. */
  for (i = 0; i < h; i++) {
    /* store the two coefficients (1, u1) into the buffer. */
    buf[i] = creal(x->x[i]);
    buf[i + h] = cimag(x->x[i]);
  }

  /* attempt to write the output data to the file handle. */
  n_write = fwrite(buf, sizeof(float), n_buf, P->fh);
  free(buf);

  /* return whether the expected number of floats was written. */
  return (n_write == n_buf);
}

/* pipesink_write2(): write a uniform plane to a pipe sink.
 *
 * arguments:
 *  @P: pointer to the pipe structure to write data to.
 *  @x: pointer to the array structure to pull values from.
 *
 * returns:
 *  integer indicating whether (1) or not (0) the write was successful.
 */
int pipesink_write2 (nmrpipe *P, arr2 *x) {
  /* declare required variables:
   *  @i: buffer index counter.
   *  @i1, @i2: array index counters.
   *  @h1, @h2: half sizes of the array.
   *  @n_buf: number of floats to write.
   *  @n_write: actual number of floats written.
   *  @buf: buffer to hold data to be written.
   */
  int i, i1, i2, h1, h2, n_buf, n_write;
  float *buf;

  /* compute the relevant sizes for writing. */
  h1 = x->n1 / 2;
  h2 = x->n2 / 2;
  n_buf = 4 * h1 * h2;

  /* attempt to allocate a buffer to hold data for writing. */
  buf = (float*) malloc(n_buf * sizeof(float));
  if (!buf)
    return 0;

  /* loop over the second hypercomplex array dimension. */
  for (i2 = 0, i = 0; i2 < h2; i2++) {
    /* store a trace of the first (1) components. */
    for (i1 = 0; i1 < h1; i1++)
      buf[i++] = x->x[i1 + x->n1 * i2][0];

    /* store a trace of the second (u1) components. */
    for (i1 = 0; i1 < h1; i1++)
      buf[i++] = x->x[i1 + x->n1 * i2][1];

    /* store a trace of the third (u2) components. */
    for (i1 = 0; i1 < h1; i1++)
      buf[i++] = x->x[i1 + x->n1 * i2][2];

    /* store a trace of the fourth (u1u2) components. */
    for (i1 = 0; i1 < h1; i1++)
      buf[i++] = x->x[i1 + x->n1 * i2][3];
  }

  /* attempt to write the output data to the file handle. */
  n_write = fwrite(buf, sizeof(float), n_buf, P->fh);
  free(buf);

  /* return whether the expected number of floats was written. */
  return (n_write == n_buf);
}

/* pipesink_write3(): write a uniform cube to a pipe sink.
 *
 * arguments:
 *  @P: pointer to the pipe structure to write data to.
 *  @x: pointer to the array structure to pull values from.
 *
 * returns:
 *  integer indicating whether (1) or not (0) the write was successful.
 */
int pipesink_write3 (nmrpipe *P, arr3 *x) {
  /* declare required variables:
   *  @i: buffer index counter.
   *  @i1, @i2, @i3: array index counters.
   *  @h1, @h2, @h3: half sizes of the array.
   *  @n_buf: number of floats to write.
   *  @n_write: actual number of floats written.
   *  @buf: buffer to hold data to be written.
   */
  int i, i1, i2, i3, h1, h2, h3, n_buf, n_write;
  float *buf;

  /* compute the relevant sizes for writing. */
  h1 = x->n1 / 2;
  h2 = x->n2 / 2;
  h3 = x->n3 / 2;
  n_buf = 8 * h1 * h2 * h3;

  /* attempt to allocate a buffer to hold data for writing. */
  buf = (float*) malloc(n_buf * sizeof(float));
  if (!buf)
    return 0;

  /* loop over the third hypercomplex array dimension. */
  for (i3 = 0, i = 0; i3 < h3; i3++) {
    /* loop over the second hypercomplex array dimension. */
    for (i2 = 0; i2 < h2; i2++) {
      /* store a trace of the first (1) components. */
      for (i1 = 0; i1 < h1; i1++)
        buf[i++] = x->x[i1 + x->n1 * i2 + x->n1 * x->n2 * i3][0];

      /* store a trace of the second (u1) components. */
      for (i1 = 0; i1 < h1; i1++)
        buf[i++] = x->x[i1 + x->n1 * i2 + x->n1 * x->n2 * i3][1];

      /* store a trace of the third (u2) components. */
      for (i1 = 0; i1 < h1; i1++)
        buf[i++] = x->x[i1 + x->n1 * i2 + x->n1 * x->n2 * i3][2];

      /* store a trace of the fourth (u1u2) components. */
      for (i1 = 0; i1 < h1; i1++)
        buf[i++] = x->x[i1 + x->n1 * i2 + x->n1 * x->n2 * i3][3];
    }

    /* loop again over the second hypercomplex array dimension. */
    for (i2 = 0; i2 < h2; i2++) {
      /* store a trace of the fifth (u3) components. */
      for (i1 = 0; i1 < h1; i1++)
        buf[i++] = x->x[i1 + x->n1 * i2 + x->n1 * x->n2 * i3][4];

      /* store a trace of the sixth (u1u3) components. */
      for (i1 = 0; i1 < h1; i1++)
        buf[i++] = x->x[i1 + x->n1 * i2 + x->n1 * x->n2 * i3][5];

      /* store a trace of the seventh (u2u3) components. */
      for (i1 = 0; i1 < h1; i1++)
        buf[i++] = x->x[i1 + x->n1 * i2 + x->n1 * x->n2 * i3][6];

      /* store a trace of the eighth (u1u2u3) components. */
      for (i1 = 0; i1 < h1; i1++)
        buf[i++] = x->x[i1 + x->n1 * i2 + x->n1 * x->n2 * i3][7];
    }
  }

  /* attempt to write the output data to the file handle. */
  n_write = fwrite(buf, sizeof(float), n_buf, P->fh);
  free(buf);

  /* return whether the expected number of floats was written. */
  return (n_write == n_buf);
}

/* nmrpipe_close(): close a pipe-format file or stream and free
 * its associated heap-allocated memory.
 *
 * arguments:
 *  @P: pointer to the pipe structure to free.
 */
void nmrpipe_close (nmrpipe *P) {
  /* return if the structure pointer is null. */
  if (!P)
    return;

  /* check if the file handle is allocated. */
  if (P->fh) {
    /* if so, close and reset the file handle. */
    fclose(P->fh);
    P->fh = NULL;
  }

  /* free the structure pointer. */
  free(P);
  P = NULL;
}

