#!/bin/bash

if [ ! -d ../data ]; then mkdir ../data; fi

bruk2pipe -in ../ser -DMX -swap -decim 24 -dspfvs 12 \
  -xN            2048   -yN                   80     \
  -xT            1024   -yT                   40     \
  -xMODE          DQD   -yMODE     Echo-AntiEcho     \
  -xSW       7002.801   -ySW            1824.568     \
  -xOBS       500.130   -yOBS             50.678     \
  -xCAR         4.697   -yCAR            118.000     \
  -xLAB            1H   -yLAB                15N     \
  -ndim             2   -aq2D             States     \
  -out ../data/hsqc.fid -ov -verb

