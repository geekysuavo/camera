#!/bin/bash

if [ ! -d ../data ]; then
  mkdir ../data
fi

rm -rf ../data/fid
mkdir ../data/fid

bruk2pipe -in ../ser                                                        \
  -bad 0.0 -aswap -DMX -decim 2368 -dspfvs 20                               \
  -grpdly 67.9842224121094                                                  \
  -xN         2048  -yN             8  -zN         2614  -aN             1  \
  -xT         1024  -yT             8  -zT         2614  -aT             1  \
  -xMODE       DQD  -yMODE       Real  -zMODE      Real  -aMODE       Real  \
  -xSW    8445.946  -ySW    13192.612  -zSW    7002.801  -aSW    13192.612  \
  -xOBS    700.213  -yOBS     176.074  -zOBS    700.213  -aOBS     176.073  \
  -xCAR      4.700  -yCAR      34.999  -zCAR      4.700  -aCAR      34.999  \
  -xLAB         Ha  -yLAB          Ca  -zLAB         Hb  -aLAB          Cb  \
  -ndim          4  -aq2D      States                                     | \
  nmrPipe -fn MAC -macro ${NMRTXT}/ranceY.M -noRd -noWr                   | \
  pipe2xyz -x -out ../data/fid/noe%04d%04d.fid -verb -ov -to 0

