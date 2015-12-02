#!/bin/bash

xyz2pipe -in ../data/yzax/noe%04d%04d.ft1 -x        | \
  nmrPipe -fn SP -off 0.5 -end 0.98 -pow 1 -c 0.5   | \
  nmrPipe -fn ZF -auto                              | \
  nmrPipe -fn FT -neg                               | \
  nmrPipe -fn PS -p0 0.0 -p1 0.0 -di                | \
  nmrPipe -fn TP                                    | \
  nmrPipe -fn SP -off 0.5 -end 0.98 -pow 1 -c 0.5   | \
  nmrPipe -fn ZF -auto                              | \
  nmrPipe -fn FT -alt -verb                         | \
  nmrPipe -fn PS -p0 0.0 -p1 0.0 -di                | \
  nmrPipe -fn TP                                    | \
  pipe2xyz -ov -out ../data/yzax/noe%04d%04d.ft3 -x

xyz2pipe -in ../data/yzax/noe%04d%04d.ft3 -z        | \
  nmrPipe -fn SP -off 0.5 -end 0.98 -pow 1 -c 0.5   | \
  nmrPipe -fn ZF -auto                              | \
  nmrPipe -fn FT -neg -verb                         | \
  nmrPipe -fn PS -p0 43.0 -p1 0.0 -di               | \
  pipe2xyz -ov -out ../data/yzax/noe%04d%04d.ft4 -z

xyz2pipe -in ../data/yzax/noe%04d%04d.ft4 -x -verb  | \
  pipe2xyz -ov -out ../data/xyza/noe%04d%04d.ft4 -a

