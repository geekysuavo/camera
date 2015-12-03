#!/bin/bash

xyz2pipe -in ../data/xyza/noe%04d%04d.ft4 -x | \
  pipe2xyz -nv -out ../data/noe.nv -verb -ov

