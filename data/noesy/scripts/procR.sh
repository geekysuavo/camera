#!/bin/bash

xyz2pipe -in ../data/yzax/noe%04d%04d.nus -x  | \
  ../../../bin/camera  \
    -sched ../nuslist  \
    -dims 3            \
    -xN 64             \
    -yN 64             \
    -zN 64             \
    -iters 200         \
    -delta 1           \
    -sigma 10          \
    -threads 8       | \
  pipe2xyz -out ../data/yzax/noe%04d%04d.ft1 -x -verb -ov

times

