#!/bin/bash

bruk2pipe -in ../ser                            \
  -bad 0.0 -aswap -DMX -decim 2376 -dspfvs 20   \
  -grpdly 67.9872589111328                      \
  -xN              4096  -yN               104  \
  -xT              2048  -yT                52  \
  -xMODE            DQD  -yMODE  Echo-AntiEcho  \
  -xSW         8417.508  -ySW         2837.684  \
  -xOBS         700.213  -yOBS          70.960  \
  -xCAR           4.703  -yCAR         119.985  \
  -xLAB              HN  -yLAB             15N  \
  -ndim               2  -aq2D          States  \
  -out ../data/hsqc.fid -verb -ov

