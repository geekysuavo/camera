#!/bin/bash

# 13C
nmrPipe -in ../data/hsqc.ft1            | \
  nmrPipe -fn ZF -auto                  | \
  nmrPipe -fn FT                        | \
  nmrPipe -fn PS -p0 90.0 -p1 0.0 -di   | \
  nmrPipe -fn POLY -ord 0 -auto         | \
  nmrPipe -fn TP                        | \
  nmrPipe -fn POLY -ord 0 -auto           \
  -verb -ov -out ../data/hsqc.ft2

