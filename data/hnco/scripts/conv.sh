#!/bin/bash

xyz2pipe -in ../data/yzx/hnco%04d.ft3 -z | \
  pipe2xyz -nv -out ../data/hnco.nv -verb -ov

