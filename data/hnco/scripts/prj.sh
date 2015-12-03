#!/bin/bash

rm -rf ../data/prj
mkdir ../data/prj

pushd ../data/prj
proj3D.tcl -in ../yzx/hnco%04d.ft3
popd

