
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
#ifndef __CAMERA_NMRPIPE_H__
#define __CAMERA_NMRPIPE_H__

/* PIPE_HDRSTR_SZ_*: lengths of strings embedded within
 * the headers of pipe-format files.
 */
#define PIPE_HDRSTR_SZ_LABEL      8
#define PIPE_HDRSTR_SZ_SRC       16
#define PIPE_HDRSTR_SZ_USER      16
#define PIPE_HDRSTR_SZ_OPER      32
#define PIPE_HDRSTR_SZ_COMMENT  160
#define PIPE_HDRSTR_SZ_TITLE     60

/* PIPE_QUAD_*: values that indicate the quadrature state
 * of a dimension within pipe-format files.
 */
#define PIPE_QUAD_COMPLEX  0
#define PIPE_QUAD_REAL     1
#define PIPE_QUAD_PSEUDO   2
#define PIPE_QUAD_SE       3
#define PIPE_QUAD_GE       4

/* nmrpipe_hdr: data structure for easily extracting information
 * from the header blocks of pipe-format files.
 */
struct nmrpipe_hdr {
  float magic;
  float format;
  float order;
  float pad0[6];
  float ndims;
  float obs_f3;
  float sw_f3;
  float orig_f3;
  float ftflag_f3;
  float plane_loc;
  float size_f3;
  char label_f2[PIPE_HDRSTR_SZ_LABEL];
  char label_f1[PIPE_HDRSTR_SZ_LABEL];
  char label_f3[PIPE_HDRSTR_SZ_LABEL];
  char label_f4[PIPE_HDRSTR_SZ_LABEL];
  float dimorder[4];
  float obs_f4;
  float sw_f4;
  float orig_f4;
  float ftflag_f4;
  float size_f4;
  float pad1[17];
  float apod_f3;
  float quad_f3;
  float pad2;
  float apod_f4;
  float quad_f4;
  float quad_f1;
  float quad_f2;
  float pipe;
  float units_f3;
  float units_f4;
  float ph0_f3;
  float ph1_f3;
  float ph0_f4;
  float ph1_f4;
  float aqsgn_f2;
  float partition;
  float car_f2;
  float car_f1;
  float car_f3;
  float car_f4;
  float user[5];
  float pipecount;
  float pad3;
  float first_plane;
  float last_plane;
  float center_f2;
  float center_f1;
  float center_f3;
  float center_f4;
  float pad4[12];
  float apod_f2;
  float ftsz_f2;
  float realsz;
  float ftsz_f1;
  float sz;
  float sw_f2;
  float orig_f2;
  float pad5[4];
  float quad;
  float pad6;
  float zf_f2;
  float ph0_f2;
  float ph1_f2;
  float lb_f2;
  float pad7[7];
  float obs_f2;
  float pad8[15];
  float mcflag;
  float pad9[16];
  float units_f2;
  float noise;
  float pad10[3];
  float temperature;
  float pad11[22];
  float rank;
  float pad12[18];
  float tau;
  float ftsz_f3;
  float ftsz_f4;
  float pad13[16];
  float obs_f1;
  float specnum;
  float ftflag_f2;
  float trans;
  float ftflag_f1;
  float pad14[6];
  float sw_f1;
  float pad15[4];
  float units_f1;
  float pad16[8];
  float lb_f1;
  float pad17;
  float ph0_f1;
  float ph1_f1;
  float max;
  float min;
  float orig_f1;
  float scale;
  float disp_max;
  float disp_min;
  float pad18[3];
  float phase2d;
  float x1_f2;
  float xn_f2;
  float x1_f1;
  float xn_f1;
  float x1_f3;
  float xn_f3;
  float x1_f4;
  float xn_f4;
  float pad19[18];
  float t_hour;
  float t_min;
  float t_sec;
  char src[PIPE_HDRSTR_SZ_SRC];
  char username[PIPE_HDRSTR_SZ_USER];
  float d_month;
  float d_day;
  float d_year;
  char title[PIPE_HDRSTR_SZ_TITLE];
  char comment[PIPE_HDRSTR_SZ_COMMENT];
  float pad20[7];
  float block_last;
  float block_cont;
  float block_base;
  float block_peak;
  float block_bmap;
  float block_hist;
  float block_1d;
  float pad21[20];
  float tdsz_f2;
  float tdsz_f1;
  float tdsz_f3;
  float tdsz_f4;
  float pad22[9];
  float virgin2d;
  float apodcode_f3;
  float apodq_f3[3];
  float c1_f3;
  float apodcode_f4;
  float apodq_f4[3];
  float c1_f4;
  float pad23[3];
  float apodcode_f2;
  float apodcode_f1;
  float apodq_f2[3];
  float c1_f2;
  float pad24;
  float apodq_f1[3];
  float c1_f1;
  float pad25[4];
  float apod_f1;
  float pad26[8];
  float zf_f1;
  float zf_f3;
  float zf_f4;
  float pad27[2];
  float file_count;
  float slice_count;
  float pad28[20];
  char oper[PIPE_HDRSTR_SZ_OPER];
  float pad29[3];
  float aqsgn_f1;
  float aqsgn_f3;
  float aqsgn_f4;
  float pad30[2];
  float offppm_f2;
  float offppm_f1;
  float offppm_f3;
  float offppm_f4;
  float pad_end[28];
};

/* nmrpipe: data structure that links together a pipe-format stream or
 * file with its file handle, header fields, and sampling schedule.
 */
typedef struct {
  /* @hdr: structure holding all header fields in the file or stream. */
  struct nmrpipe_hdr hdr;

  /* @sch: structure holding the sampling schedule and its inverse. */
  sched *sch;

  /* @fname: filename string of the pipe file, or NULL for pipe streams.
   * @fh: file handle of the open file or stream.
   */
  char *fname;
  FILE *fh;
}
nmrpipe;

/* function declarations: */

nmrpipe *pipesrc_open (char *fname, sched *sch);

nmrpipe *pipesink_open (char *fname, struct nmrpipe_hdr *hdr);

int pipesrc_nread (nmrpipe *P);

int pipesrc_read1 (nmrpipe *P, arr1 *x);

int pipesrc_read2 (nmrpipe *P, arr2 *x);

int pipesrc_read3 (nmrpipe *P, arr3 *x);

int pipesink_write1 (nmrpipe *P, arr1 *x);

int pipesink_write2 (nmrpipe *P, arr2 *x);

int pipesink_write3 (nmrpipe *P, arr3 *x);

void nmrpipe_close (nmrpipe *P);

#endif /* !__CAMERA_NMRPIPE_H__ */

