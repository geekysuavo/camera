set terminal pngcairo enhanced dashed dashlength 5 \
 font 'Free Sans Bold, 80' \
 size 7480, 2530 \
 linewidth 14 rounded
set output 'output.png'
set encoding utf8

set multiplot layout 1, 8

unset key
unset ytics
set border 1
set lmargin 1
set rmargin 1
set xtics format '' nomirror
set xrange [1835 - 25 : 1835 + 25]
set yrange [-3e7 : 5.8e8]

set xlabel '0 Hz' font 'Free Sans Bold, 100'
set label 1 '(A)' at graph 0.25, 0.9 center font 'Free Sans Bold, 120'
plot 'output.dat' u 1:3 w l lt 1 lw 1.0 lc rgb '#222222'

set xlabel '21 Hz' font 'Free Sans Bold, 100'
set label 1 '(B)' at graph 0.25, 0.9 center font 'Free Sans Bold, 120'
plot 'output.dat' u 1:8 w l lt 1 lw 1.0 lc rgb '#222222'

set xlabel '26 Hz' font 'Free Sans Bold, 100'
set label 1 '(C)' at graph 0.25, 0.9 center font 'Free Sans Bold, 120'
plot 'output.dat' u 1:9 w l lt 1 lw 1.0 lc rgb '#222222'

set xlabel '31 Hz' font 'Free Sans Bold, 100'
set label 1 '(D)' at graph 0.25, 0.9 center font 'Free Sans Bold, 120'
plot 'output.dat' u 1:10 w l lt 1 lw 1.0 lc rgb '#222222'

set xlabel '36 Hz' font 'Free Sans Bold, 100'
set label 1 '(E)' at graph 0.25, 0.9 center font 'Free Sans Bold, 120'
plot 'output.dat' u 1:11 w l lt 1 lw 1.0 lc rgb '#222222'

set xlabel '41 Hz' font 'Free Sans Bold, 100'
set label 1 '(F)' at graph 0.25, 0.9 center font 'Free Sans Bold, 120'
plot 'output.dat' u 1:12 w l lt 1 lw 1.0 lc rgb '#222222'

set xlabel '46 Hz' font 'Free Sans Bold, 100'
set label 1 '(G)' at graph 0.25, 0.9 center font 'Free Sans Bold, 120'
plot 'output.dat' u 1:13 w l lt 1 lw 1.0 lc rgb '#222222'

set xlabel '51 Hz' font 'Free Sans Bold, 100'
set label 1 '(H)' at graph 0.25, 0.9 center font 'Free Sans Bold, 120'
plot 'output.dat' u 1:14 w l lt 1 lw 1.0 lc rgb '#222222'

unset multiplot

