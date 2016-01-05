#!/bin/bash

time                    \
../../../bin/camera     \
  -in ../data/hsqc.nus  \
  -out ../data/hsqc.ft1 \
  -log ../data/hsqc.log \
  -sched ../nuslist     \
  -dims 1               \
  -xN 2048              \
  -iters 400            \
  -lambda 0.01          \
  -threads 8

