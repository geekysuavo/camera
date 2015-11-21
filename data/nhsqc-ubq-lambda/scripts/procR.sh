#!/bin/bash

time                    \
../../../bin/camera     \
  -in ../data/hsqc.nus  \
  -out ../data/hsqc.ft1 \
  -sched ../nuslist     \
  -dims 1               \
  -xN 1024              \
  -iters 500            \
  -delta 2              \
  -lambda 500           \
  -threads 8

