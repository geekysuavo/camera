#!/bin/bash

time                    \
../../../bin/camera     \
  -in ../data/hsqc.nus  \
  -out ../data/hsqc.ft1 \
  -log ../data/hsqc.log \
  -sched ../nuslist     \
  -dims 1               \
  -xN 512               \
  -iters 100            \
  -sigma 100            \
  -threads 8

