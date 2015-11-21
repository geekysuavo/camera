#!/bin/bash

xyz2pipe -in ../data/yzx/hnco%04d.nus -x  | \
  ../../../bin/camera   \
    -sched ../nuslist   \
    -dims 2             \
    -xN 128             \
    -yN 256             \
    -iters 100          \
    -delta 1            \
    -sigma 1            \
    -threads 8        | \
  pipe2xyz -out ../data/yzx/hnco%04d.ft1 -x -ov

times

