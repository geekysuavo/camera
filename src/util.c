
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

/* USAGE_MSG: a short string literal to display to standard error if
 * the user runs the program without arguments or with the
 * '-help' argument.
 */
#define USAGE_MSG "\
 CAMERA: Convex Accelerated Maximum Entropy Reconstruction Algorithm\n\
 Copyright (C) 2015-2016  Bradley Worley  <geekysuavo@gmail.com>\n\
 Released under the GNU General Public License (ver. 3.0)\n\
\n\
 Usage:\n\
   camera [OPTIONS]\n\
\n\
 Options:\n\
   -in       Input filename.                                [stdin]\n\
   -out      Output filename.                              [stdout]\n\
   -sched    Sampling schedule filename.                  [nuslist]\n\
   -dims     Number of dimensions to reconstruct.            [auto]\n\
   -xN       Final complex length of the x-dimension.        [auto]\n\
   -yN       Final complex length of the y-dimension.        [auto]\n\
   -zN       Final complex length of the z-dimension.        [auto]\n\
   -xJ       Deconvolution J-coupling of the x-dimension.       [0]\n\
   -yJ       Deconvolution J-coupling of the y-dimension.       [0]\n\
   -zJ       Deconvolution J-coupling of the z-dimension.       [0]\n\
   -xW       Deconvolution linewidth of the x-dimension.        [0]\n\
   -yW       Deconvolution linewidth of the y-dimension.        [0]\n\
   -zW       Deconvolution linewidth of the z-dimension.        [0]\n\
   -iters    Number of iterations to perform.                 [250]\n\
   -threads  Number of parallel threads to execute.             [1]\n\
   -accel    Initial line search acceleration factor.         [500]\n\
   -delta    Regularization functional background value.      [1.0]\n\
   -sigma    Constant-aim mode data noise estimate.           [1.0]\n\
   -lambda   Constant-lambda mode Lagrange multiplier.        [off]\n\
   -log      Optional filename for logging output.            [off]\n\
   -help     Print this help message.\n\
\n\
 Citation:\n\
   B. Worley, J. Magn. Reson., 2016, 265: 90-98.\n\n"

/* nextpow2(): compute the smallest power of two that is not less
 * than a given value.
 *
 * arguments:
 *  @n: the input value.
 *
 * returns:
 *  the computed power of two, or zero if the input value is zero.
 */
int nextpow2 (int n) {
  /* declare required variables:
   *  @n2: the output power of two.
   */
  int n2;

  /* check for a zero input. */
  if (n == 0)
    return 0;

  /* loop until the value is identified. */
  for (n2 = 1; n2 < n;)
    n2 <<= 1;

  /* return the computed value. */
  return n2;
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

/* usagef(): print a usage statement to standard error.
 */
void usagef (void) {
  /* output the usage statement. */
  fprintf(stderr, USAGE_MSG);
}

/* failf_fn(): target of the preprocessor macro failf(), and the function
 * that does all the dirty work of printing error messages to the terminal.
 *
 * arguments:
 *  @f: source code filename from where the message was printed.
 *  @l: source code line number at which the message was printed.
 *  @format: printf-style message formatting string.
 *  @...: arguments corresponding to flags in @format.
 */
void failf_fn (const char *f, const unsigned int l,
               const char *format, ...) {
  /* declare required variables:
   *  @vl: variable arguments list structure.
   */
  va_list vl;

  /* print the error message heading. */
  fprintf(stderr, "Error (%s:%u): ", f, l);

  /* print the error message body. */
  va_start(vl, format);
  vfprintf(stderr, format, vl);
  va_end(vl);

  /* print the error message footing. */
  fprintf(stderr, "\n");
  fflush(stderr);
}

