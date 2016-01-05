#!/bin/bash

time                    \
../../../bin/camera     \
  -in ../data/hsqc.nus  \
  -out ../data/hsqc.ft1 \
  -log ../data/hsqc.log \
  -sched ../nuslist     \
  -dims 1               \
  -xN 1024              \
  -iters 200            \
  -delta 2              \
  -sigma 1e5            \
  -threads 8

