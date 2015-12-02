set terminal pngcairo enhanced dashed dashlength 5 \
 font 'Free Sans Bold, 80' \
 size 3543, 2530 \
 linewidth 14 rounded
set output 'output.png'
set encoding utf8

set xlabel 'Transform Count' font 'Free Sans Bold, 100'
set xrange [1 : 1e3]
set xtics 200

set ylabel 'Normalized Objective Value' font 'Free Sans Bold, 100'
set yrange [-0.08 : 1.01]
set ytics 0.1

plot 'output.dat' \
    u 2:3 w l lt 1 lw 1.2 lc rgb '#222222' t 'CAMERA', \
 '' u 4:5 w l lt 2 lw 1.0 lc rgb '#222222' t 'RNMRTK'

