set terminal pngcairo enhanced dashed dashlength 5 \
 font 'Free Sans Bold, 80' \
 size 7480, 2530 \
 linewidth 14 rounded
set output 'output.png'
set encoding utf8

set multiplot layout 1, 2

set key right bottom

set xlabel 'Input Amplitude' font 'Free Sans Bold, 100'
set xrange [100 : 4200]

set ylabel 'Output Amplitude' font 'Free Sans Bold, 100'
set yrange [0 : 4200]

set label 1 '(A)' at graph 0.1, 0.9 center font 'Free Sans Bold, 120'

plot 'output-ints.dat' \
    u 2:3 w lp pt 4 ps 5 lt 3 lw 1.0 lc rgb '#222222' t 'σ = 0.001', \
 '' u 2:7 w lp pt 6 ps 6 lt 3 lw 1.0 lc rgb '#222222' t 'σ = 0.7', \
 '' u 2:9 w lp pt 8 ps 6 lt 3 lw 1.0 lc rgb '#222222' t 'σ = 1.4', \
 '' u 2:11 w lp pt 12 ps 7 lt 3 lw 1.0 lc rgb '#222222' t 'σ = 2.1'

set xlabel 'Estimated Noise' font 'Free Sans Bold, 100'
set xrange [0 : 5]

set ylabel 'Intensity Correlation' font 'Free Sans Bold, 100'
set ytics 0.02 format '%0.2f'
set yrange [0.85 : 1.001]

unset label 1
set label 2 '(B)' at graph 0.9, 0.9 center font 'Free Sans Bold, 120'

plot 'output-corr.dat' \
    u 1:2 w lp pt 5 ps 4 lt 3 lw 1.0 lc rgb '#222222' not

unset multiplot

