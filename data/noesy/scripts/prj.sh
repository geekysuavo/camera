#!/bin/bash

rm -rf ../data/prj
mkdir ../data/prj

pushd ../data/prj
proj4D.tcl -in ../xyza/noe%04d%04d.ft4
popd

