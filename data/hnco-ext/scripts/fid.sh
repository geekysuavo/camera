#!/bin/bash

if [ ! -d ../data ]; then mkdir ../data; fi

rm -rf ../data/fid
mkdir ../data/fid

bruk2pipe -in ../ser \
  -bad 0.0 -aswap -DMX -decim 2376 -dspfvs 20 -grpdly 67.9872589111328  \
  -xN              4096  -yN                 4  -zN               296   \
  -xT              2048  -yT                 2  -zT               148   \
  -xMODE            DQD  -yMODE           Real  -zMODE           Real   \
  -xSW         8417.509  -ySW         2600.104  -zSW         3872.967   \
  -xOBS         700.213  -yOBS          70.960  -zOBS         176.099   \
  -xCAR           4.703  -yCAR         117.672  -zCAR         175.969   \
  -xLAB              HN  -yLAB             15N  -zLAB             13C   \
  -ndim               3  -aq2D          States                        | \
  nmrPipe -fn MAC -macro ${NMRTXT}/ranceY.M -noRd -noWr               | \
  pipe2xyz -x -out ../data/fid/hnco%04d.fid -verb -ov -to 0

