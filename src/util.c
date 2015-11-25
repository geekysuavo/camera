
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

