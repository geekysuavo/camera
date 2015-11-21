#!/bin/bash

bruk2pipe -in ../ser                            \
  -bad 0.0 -aswap -DMX -decim 2376 -dspfvs 20   \
  -grpdly 67.9872589111328                      \
  -xN              4096  -yN               202  \
  -xT              2048  -yT               101  \
  -xMODE            DQD  -yMODE  Echo-AntiEcho  \
  -xSW         8417.508  -ySW        10570.825  \
  -xOBS         700.213  -yOBS         176.071  \
  -xCAR           4.695  -yCAR          20.000  \
  -xLAB              1H  -yLAB             13C  \
  -ndim               2  -aq2D          States  \
  -out ../data/hsqc.fid -verb -ov

