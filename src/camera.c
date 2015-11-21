
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

/* CAMERA_HELP_MSG: a short string literal to display to standard error
 * if the user runs the program without arguments or with the '-help'
 * argument.
 */
#define CAMERA_HELP_MSG "\
 CAMERA: Convex Accelerated Maximum Entropy Reconstruction Algorithm\n\
 Copyright (C) 2015  Bradley Worley  <geekysuavo@gmail.com>\n\
 Released under the GNU General Public License (ver. 3.0)\n\
\n\
 Usage:\n\
   camera [OPTIONS]\n\
\n\
 Options:\n\
   -in       Input filename.                                [stdin]\n\
   -out      Output filename.                              [stdout]\n\
   -sched    Sampling schedule filename.                  [nuslist]\n\
   -dims     Number of dimensions to reconstruct.               [1]\n\
   -xN       Final length of the x-dimension.                   [0]\n\
   -yN       Final length of the y-dimension.                   [0]\n\
   -zN       Final length of the z-dimension.                   [0]\n\
   -iters    Number of iterations to perform.                 [250]\n\
   -threads  Number of parallel threads to execute.             [1]\n\
   -delta    Regularization functional background value.      [1.0]\n\
   -sigma    Constant-aim mode data noise estimate.           [1.0]\n\
   -lambda   Constant-lambda mode Lagrange multiplier.        [off]\n\
   -log      Optional filename for logging output.            [off]\n\
   -help     Print this help message.\n\
\n\
 Citation:\n\
   B. Worley, J. Magn. Reson., 2015, In preparation.\n\n"

/* predeclare strdup() for use by main() */
char *strdup (char *s);

/* main(): application entry point.
 *
 * arguments:
 *  @argc: number of command line arguments.
 *  @argv: command line argument string array.
 *
 * returns:
 *  integer indicating whether execution terminated without error (0)
 *  or not (!0).
 */
int main (int argc, char **argv) {
  /* declare required string variables:
   *  @fname_in: input filename string, or NULL if reading from stdin.
   *  @fname_out: output filename string, or NULL if writing to stdout.
   *  @fname_log: output log filename string, or NULL if not logging.
   *  @fname_sched: required sampling schedule filename string.
   */
  char *fname_in, *fname_out, *fname_log, *fname_sched;

  /* declare required integer variables:
   *  @dims: number of dimensions to reconstruct.
   *  @nx: grid size along the first reconstruction dimension.
   *  @ny: grid size along the second reconstruction dimension.
   *  @nz: grid size along the third reconstruction dimension.
   *  @iters: number of reconstruction iterations to perform.
   *  @threads: number of parallel reconstructions to perform.
   *  @i: general-purpose loop counter.
   *  @ret: reconstruction return status.
   */
  int dims, nx, ny, nz, iters, threads, i;
  int ret = 0;

  /* declare required floating-point variables:
   *  @delta: regularization functional normalizing constant.
   *  @sigma: reconstruction-to-data noise estimate.
   *  @lambda: constant Lagrange multiplier.
   */
  hx0 delta, sigma, lambda;

  /* initialize the filename strings. */
  fname_in = NULL;
  fname_out = NULL;
  fname_log = NULL;
  fname_sched = strdup("nuslist");

  /* initialize the integer variables. */
  nx = 0;
  ny = 0;
  nz = 0;
  dims = 1;
  iters = 250;
  threads = 1;

  /* initialize the float variables. */
  delta = 1.0f;
  sigma = 1.0f;
  lambda = 0.0f;

  /* loop over all command line arguments. */
  i = 1;
  while (i < argc) {
    /* check for a recognized argument flag. */
    if (strcmp(argv[i], "-in") == 0) {
      /* check that the flag is followed by another argument. */
      if (i + 1 >= argc) {
        /* if not, output an error message and return failure. */
        fprintf(stderr, "Error: 'in' argument requires a value\n");
        return 1;
      }

      /* duplicate the input filename string. */
      fname_in = strdup(argv[++i]);
    }
    else if (strcmp(argv[i], "-out") == 0) {
      /* check that the flag is followed by another argument. */
      if (i + 1 >= argc) {
        /* if not, output an error message and return failure. */
        fprintf(stderr, "Error: 'out' argument requires a value\n");
        return 1;
      }

      /* duplicate the output filename string. */
      fname_out = strdup(argv[++i]);
    }
    else if (strcmp(argv[i], "-log") == 0) {
      /* check that the flag is followed by another argument. */
      if (i + 1 >= argc) {
        /* if not, output an error message and return failure. */
        fprintf(stderr, "Error: 'log' argument requires a value\n");
        return 1;
      }

      /* duplicate the log filename string. */
      fname_log = strdup(argv[++i]);
    }
    else if (strcmp(argv[i], "-sched") == 0) {
      /* check that the flag is followed by another argument. */
      if (i + 1 >= argc) {
        /* if not, output an error message and return failure. */
        fprintf(stderr, "Error: 'sched' argument requires a value\n");
        return 1;
      }

      /* duplicate the schedule filename string after
       * freeing the previously set value.
       */
      free(fname_sched);
      fname_sched = strdup(argv[++i]);
    }
    else if (strcmp(argv[i], "-dims") == 0) {
      /* attempt to parse the dimension count. */
      if (i + 1 >= argc || sscanf(argv[++i], "%d", &dims) != 1) {
        /* if unsuccessful, output an error message and return failure. */
        fprintf(stderr, "Error: 'dims' argument requires a value\n");
        return 1;
      }
    }
    else if (strcmp(argv[i], "-xN") == 0) {
      /* attempt to parse the first-dimension size. */
      if (i + 1 >= argc || sscanf(argv[++i], "%d", &nx) != 1) {
        /* if unsuccessful, output an error message and return failure. */
        fprintf(stderr, "Error: 'xN' argument requires a value\n");
        return 1;
      }
    }
    else if (strcmp(argv[i], "-yN") == 0) {
      /* attempt to parse the second-dimension size. */
      if (i + 1 >= argc || sscanf(argv[++i], "%d", &ny) != 1) {
        /* if unsuccessful, output an error message and return failure. */
        fprintf(stderr, "Error: 'yN' argument requires a value\n");
        return 1;
      }
    }
    else if (strcmp(argv[i], "-zN") == 0) {
      /* attempt to parse the third-dimension size. */
      if (i + 1 >= argc || sscanf(argv[++i], "%d", &nz) != 1) {
        /* if unsuccessful, output an error message and return failure. */
        fprintf(stderr, "Error: 'zN' argument requires a value\n");
        return 1;
      }
    }
    else if (strcmp(argv[i], "-iters") == 0) {
      /* attempt to parse the iteration count. */
      if (i + 1 >= argc || sscanf(argv[++i], "%d", &iters) != 1) {
        /* if unsuccessful, output an error message and return failure. */
        fprintf(stderr, "Error: 'iters' argument requires a value\n");
        return 1;
      }
    }
    else if (strcmp(argv[i], "-threads") == 0) {
      /* attempt to parse the thread count. */
      if (i + 1 >= argc || sscanf(argv[++i], "%d", &threads) != 1) {
        /* if unsuccessful, output an error message and return failure. */
        fprintf(stderr, "Error: 'threads' argument requires a value\n");
        return 1;
      }
    }
    else if (strcmp(argv[i], "-delta") == 0) {
      /* attempt to parse the regularization background value. */
      if (i + 1 >= argc || sscanf(argv[++i], "%f", &delta) != 1) {
        /* if unsuccessful, output an error message and return failure. */
        fprintf(stderr, "Error: 'delta' argument requires a value\n");
        return 1;
      }
    }
    else if (strcmp(argv[i], "-sigma") == 0) {
      /* attempt to parse the reconstruction noise estimate. */
      if (i + 1 >= argc || sscanf(argv[++i], "%f", &sigma) != 1) {
        /* if unsuccessful, output an error message and return failure. */
        fprintf(stderr, "Error: 'sigma' argument requires a value\n");
        return 1;
      }
    }
    else if (strcmp(argv[i], "-lambda") == 0) {
      /* attempt to parse the reconstruction Lagrange multiplier. */
      if (i + 1 >= argc || sscanf(argv[++i], "%f", &lambda) != 1) {
        /* if unsuccessful, output an error message and return failure. */
        fprintf(stderr, "Error: 'lambda' argument requires a value\n");
        return 1;
      }
    }
    else if (strcmp(argv[i], "-help") == 0) {
      /* output the usage message to standard error, because the '-help'
       * flag has been specified.
       */
      fprintf(stderr, CAMERA_HELP_MSG);
      return 0;
    }
    else {
      /* output an error message and return failure, because this argument
       * is not recognized by the program.
       */
      fprintf(stderr, "Error: undefined argument '%s'\n", argv[i]);
      return 1;
    }

    /* increment the argument index. */
    i++;
  }

  /* check if zero arguments were supplied. */
  if (argc == 1) {
    /* if so, print the usage message to standard error. */
    fprintf(stderr, CAMERA_HELP_MSG);
    return 0;
  }

  /* check that the dimension count is in bounds. */
  if (dims < 1 || dims > 3) {
    /* if not, output an error message and return failure. */
    fprintf(stderr, "Error: unsupported dimension count %d\n", dims);
    return 1;
  }

  /* check that the iteration count is in bounds. */
  if (iters < 1) {
    /* if not, output an error message and return failure. */
    fprintf(stderr, "Error: unsupported iteration count %d\n", iters);
    return 1;
  }

  /* check that the thread count is in bounds. */
  if (threads < 1 || threads > 8192) {
    /* if not, output an error message and return failure. */
    fprintf(stderr, "Error: unsupported thread count %d\n", threads);
    return 1;
  }

  /* check that the background is in bounds. */
  if (delta <= 0.0f) {
    /* if not, output an error message and return failure. */
    fprintf(stderr, "Error: unsupported background %.3f\n", delta);
    return 1;
  }

  /* check that the noise estimate is in bounds. */
  if (sigma <= 0.0f) {
    /* if not, output an error message and return failure. */
    fprintf(stderr, "Error: unsupported noise estimate %.3f\n", sigma);
    return 1;
  }

  /* check that the Lagrange multiplier is in bounds. */
  if (lambda < 0.0f) {
    /* if not, output an error message and return failure. */
    fprintf(stderr, "Error: unsupported Lagrange multiplier %.3f\n", lambda);
    return 1;
  }

  /* set the number of threads to be used during reconstruction. */
  omp_set_num_threads(threads);

  /* execute the dimensionality-appropriate reconstruction function. */
  switch (dims) {
    /* one-dimensional reconstruction. */
    case 1:
      ret = task_run1(fname_in, fname_out, fname_log, fname_sched,
                      iters, threads, delta, sigma, lambda, nx);
      break;

    /* two-dimensional reconstruction. */
    case 2:
      ret = task_run2(fname_in, fname_out, fname_log, fname_sched,
                      iters, threads, delta, sigma, lambda, nx, ny);
      break;

    /* three-dimensional reconstruction. */
    case 3:
      ret = task_run3(fname_in, fname_out, fname_log, fname_sched,
                      iters, threads, delta, sigma, lambda, nx, ny, nz);
      break;
  }

  /* check if the reconstruction succeeded. */
  if (!ret) {
    /* if not, output an error message and return failure. */
    fprintf(stderr, "Error: failed to reconstruct data\n");
    return 1;
  }

  /* free the input filename string, if necessary. */
  if (fname_in)
    free(fname_in);

  /* free the output filename string, if necessary. */
  if (fname_out)
    free(fname_out);

  /* free the log filename string, if necessary. */
  if (fname_log)
    free(fname_log);

  /* free the schedule filename string. */
  free(fname_sched);

  /* return success. */
  return 0;
}

/* strdup(): duplicate the contents of a string in heap memory.
 *
 * arguments:
 *  @s: pointer to a string to duplicate.
 *
 * returns:
 *  pointer to a copy of the string @s, allocated on the heap.
 */
char *strdup (char *s) {
  /* declare required variables:
   *  @sdup: pointer to the duplicated string.
   */
  char *sdup;

  /* allocate a new character pointer of sufficient length. */
  sdup = (char*) malloc((strlen(s) + 2) * sizeof(char));
  if (!sdup)
    return NULL;

  /* copy the contents of the input string into the new pointer. */
  strcpy(sdup, s);

  /* return the newly allocated and prepared character pointer. */
  return sdup;
}

