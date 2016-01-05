
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
  /* declare required variables:
   *  @T: task data structure pointer.
   */
  task *T;

  /* initialize a reconstruction task. */
  T = task_alloc(argc, argv);

  /* check if task initialization was successful. */
  if (!T) {
    /* if not, output an error message and return failure. */
    failf("unable to initialize a reconstruction task");
    return 1;
  }

  /* execute the reconstruction task. */
  if (!task_run(T)) {
    /* if unsuccessful, output an error message and return failure. */
    failf("reconstruction task execution failed");
    task_free(T);
    return 1;
  }

  /* free the reconstruction task structure. */
  task_free(T);

  /* return success. */
  return 0;
}

