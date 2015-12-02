#!/bin/bash

if [ ! -d ../data ]; then
  install -d ../data
fi

awk '{print($1+1)}' ../nuslist > ../nuslist.scd
n=$(wc -l < ../nuslist.scd)
n2=$(expr ${n} \* 2)

section -c 8192 2048

cp ../ser ../ser.dat
echo 'Format little-endian int-32'    > ../ser.par
echo 'DOM    t1            t2'       >> ../ser.par
echo "N      ${n} C        2048 C"   >> ../ser.par
echo 'SW     2837.68       8417.51'  >> ../ser.par
echo 'SF     70.9600       700.2130' >> ../ser.par
echo 'PPM    119.985       4.703'    >> ../ser.par
echo 'QUAD   STATES-TPPI   STATES'   >> ../ser.par
echo "LAYOUT T1:${n2}      T2:4096"  >> ../ser.par
rnmrtk LOAD ../ser.dat
rnmrtk SEEPAR
rm -f ../ser.par ../ser.dat

rnmrtk << EOF
dim t2
shrink 1980 69
seepar
EOF

rnmrtk << EOF
dim t2
sefix1 t1
sstdc 16 20 0.00
bc 12.5
sinebell square shift 90.0 use 1980
zerofill 4096
fft 0.50
phase 29.0 0.0
shrink 2048 2048
realpart
reverse
seepar
EOF

rnmrtk << EOF
dim t1
zerofill 1024
EOF
MSAOPTS="t1 500 ../nuslist.scd 2.00 10.00 0.00 0.0 0.0 0.00 2 SINGLE"
msa ${MSAOPTS} > ../data/bench.log

rm -f hsqc.sec
rnmrtk << EOF
dim f1
realpart
reverse
EOF

rm -f ../data/bench.ft2
rnmrtk << EOF
putnmrpipe ../data/bench.ft2
EOF

section -d
rm -f ../nuslist.scd

echo
times

