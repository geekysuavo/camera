#!/bin/bash

# 15N, 13C
xyz2pipe -in ../data/yzx/hnco%04d.ft1 -x           | \
  nmrPipe -fn ZF -zf 2                             | \
  nmrPipe -fn FT -neg                              | \
  nmrPipe -fn PS -p0 -18.5 -p1 36.0 -di            | \
  nmrPipe -fn TP                                   | \
  nmrPipe -fn ZF -auto                             | \
  nmrPipe -fn FT -alt -verb                        | \
  nmrPipe -fn PS -p0 7.0 -p1 0.0 -di               | \
  pipe2xyz -ov -out ../data/yzx/hnco%04d.ft3 -x

