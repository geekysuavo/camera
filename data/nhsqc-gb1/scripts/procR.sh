#!/bin/bash

time                    \
../../../bin/camera     \
  -in ../data/hsqc.nus  \
  -out ../data/hsqc.ft1 \
  -sched ../nuslist     \
  -dims 1               \
  -xN 512               \
  -iters 100            \
  -delta 10             \
  -lambda 0.7           \
  -threads 8

