set terminal pngcairo enhanced dashed dashlength 5 \
 font 'Free Sans Bold, 80' \
 size 3543, 2530 \
 linewidth 14 rounded
set output 'output.png'
set encoding utf8

set xlabel 'Iteration Number' font 'Free Sans Bold, 100'
set xrange [1 : 250]

set ylabel 'Normalized Objective Value' font 'Free Sans Bold, 100'
set yrange [0.45 : 1.01]
set ytics 0.1

plot 'output.dat' \
    u 1:2 w l lt 1 lw 1.0 lc rgb '#222222' t 'δ = 1', \
 '' u 1:3 w l lt 2 lw 1.0 lc rgb '#222222' t 'δ = 10', \
 '' u 1:4 w l lt 3 lw 1.0 lc rgb '#222222' t 'δ = 25'

