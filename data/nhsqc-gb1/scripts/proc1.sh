#!/bin/bash

nmrPipe -in ../data/hsqc.fid                       | \
  nmrPipe  -fn SOL                                 | \
  nmrPipe  -fn SP -off 0.3 -end 0.98 -pow 2 -c 0.5 | \
  nmrPipe  -fn ZF -auto                            | \
  nmrPipe  -fn FT                                  | \
  nmrPipe  -fn PS -p0 45.8 -p1 0.0 -di             | \
  nmrPipe  -fn EXT -left -sw -verb                 | \
  nmrPipe  -fn TP                                    \
  -verb -ov -out ../data/hsqc.nus

