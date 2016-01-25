#!/bin/bash

xyz2pipe -in ../data/yzx/hnco%04d.nus -x  | \
  ../../../bin/camera     \
    -sched ../nuslist     \
    -log ../data/hnco.log \
    -dims 2               \
    -xN 128               \
    -yN 256               \
    -iters 100            \
    -threads 8          | \
  pipe2xyz -out ../data/yzx/hnco%04d.ft1 -x -ov

times

