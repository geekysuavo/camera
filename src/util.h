
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

/* ensure once-only inclusion. */
#ifndef __CAMERA_UTIL_H__
#define __CAMERA_UTIL_H__

/* failf(): preprocessor macro function to print an error message to
 * standard error.
 */
#define failf(...)  failf_fn(__FILE__, __LINE__, __VA_ARGS__)

/* function declarations: */

int nextpow2 (int n);

char *strdup (char *s);

void usagef (void);

void failf_fn (const char *f, const unsigned int l,
               const char *format, ...);

#endif /* !__CAMERA_UTIL_H__ */

