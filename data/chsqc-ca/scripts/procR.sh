#!/bin/bash

time                    \
../../../bin/camera     \
  -in ../data/hsqc.nus  \
  -out ../data/hsqc.ft1 \
  -log ../data/hsqc.log \
  -sched ../nuslist     \
  -dims 1               \
  -xN 2048              \
  -xJ 36                \
  -iters 1000           \
  -delta 2              \
  -sigma 7e5            \
  -threads 8

