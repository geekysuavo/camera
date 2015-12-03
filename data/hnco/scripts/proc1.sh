#!/bin/bash

rm -rf ../data/yzx
mkdir ../data/yzx

# HN
xyz2pipe -in ../data/fid/hnco%04d.fid -x           | \
  nmrPipe -fn SOL                                  | \
  nmrPipe -fn SP -off 0.5 -end 0.98 -pow 2 -c 0.5  | \
  nmrPipe -fn ZF -auto                             | \
  nmrPipe -fn FT -verb                             | \
  nmrPipe -fn PS -p0 7.0 -p1 0.0 -di               | \
  nmrPipe -fn EXT -x1 10.0ppm -xn 5.5ppm -sw       | \
  pipe2xyz -ov -out ../data/yzx/hnco%04d.nus -z

