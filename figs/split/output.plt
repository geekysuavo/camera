set terminal pngcairo enhanced dashed dashlength 5 \
 font 'Free Sans Bold, 80' \
 size 7480, 2530 \
 linewidth 14 rounded
set output 'output.png'
set encoding utf8

set multiplot layout 1, 2

unset key
unset ytics
set border 1
set xtics format '' nomirror

set xlabel '100 Samples' font 'Free Sans Bold, 100'
set xrange [1080 : 1180]
set yrange [-50 : 750]

g(y,n) = y + 200 * n

set label 1 'IST-S'  at graph 0.01, 0.65 left font 'Free Sans Bold, 120'
set label 2 'CAMERA' at graph 0.01, 0.40 left font 'Free Sans Bold, 120'
set label 3 'CAMERA' at graph 0.01, 0.12 left font 'Free Sans Bold, 120'
set label 4 'σ = 1×10^{-6}' at graph 0.99, 0.42 right font 'Free Sans, 120'
set label 5 'σ = 0.8'       at graph 0.99, 0.12 right font 'Free Sans, 120'

plot 'output.dat' \
    u 1:(g($3,2)) w l lt 1 lw 1.0 lc rgb '#222222', \
 '' u 1:(g($4,1)) w l lt 1 lw 1.0 lc rgb '#222222', \
 '' u 1:(g($5,0)) w l lt 1 lw 1.0 lc rgb '#222222'

set xlabel '300 Samples' font 'Free Sans Bold, 100'

unset label 1
unset label 2
unset label 3

plot 'output.dat' \
    u 1:(g($7,2)) w l lt 1 lw 1.0 lc rgb '#222222', \
 '' u 1:(g($8,1)) w l lt 1 lw 1.0 lc rgb '#222222', \
 '' u 1:(g($9,0)) w l lt 1 lw 1.0 lc rgb '#222222'

unset multiplot

