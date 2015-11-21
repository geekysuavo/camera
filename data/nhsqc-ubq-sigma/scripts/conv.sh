#!/bin/bash

nmrPipe -in ../data/hsqc.ft2 | \
  pipe2xyz -nv -x -out ../data/hsqc.nv -verb -ov

