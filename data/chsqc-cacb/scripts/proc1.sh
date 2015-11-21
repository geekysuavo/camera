#!/bin/bash

nmrPipe -in ../data/hsqc.fid                        | \
  nmrPipe -fn SOL                                   | \
  nmrPipe -fn SP -off 0.45 -end 0.98 -pow 2         | \
  nmrPipe -fn ZF -auto                              | \
  nmrPipe -fn FT                                    | \
  nmrPipe -fn PS -p0 122.0 -p1 -100.0 -di           | \
  nmrPipe -fn EXT -x1 2.0ppm -xn -0.5ppm -sw -verb  | \
  nmrPipe -fn TP                                      \
  -verb -ov -out ../data/hsqc.nus

#
