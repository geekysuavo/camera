#!/bin/bash

rm -rf ../data/xyza
mkdir ../data/xyza

xyz2pipe -in ../data/fid/noe%04d%04d.fid -x -verb  | \
  nmrPipe -fn SP -off 0.5 -end 0.98 -pow 1 -c 0.5  | \
  nmrPipe -fn ZF -auto                             | \
  nmrPipe -fn FT                                   | \
  nmrPipe -fn PS -p0 29.0 -p1 0.0 -di              | \
  nmrPipe -fn EXT -x1 1.75ppm -xn 0.06ppm -sw      | \
  pipe2xyz -ov -out ../data/xyza/noe%04d%04d.nus -x

