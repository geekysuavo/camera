set terminal pngcairo enhanced dashed dashlength 5 \
 font 'Free Sans Bold, 80' \
 size 3543, 6000 \
 linewidth 14 rounded
set output 'output.png'
set encoding utf8

unset key

set xlabel 'Normalized Frequency' font 'Free Sans Bold, 100'
set xrange [-0.5 : 0.5]
set xtics 0.25

set yrange [-0.1 : 3.3]
unset ylabel
unset ytics

f(x) = (x - 1024) / 2048
g(y,n) = y + 1.1 * n

set label 'δ = 1' at screen 0.15, 0.115 center font 'Free Sans Bold, 100'
set label 'δ = 10' at screen 0.15, 0.41 center font 'Free Sans Bold, 100'
set label 'δ = 25' at screen 0.15, 0.71 center font 'Free Sans Bold, 100'

plot 'output.dat' \
    u (f($1)):(g($3,0)) w l lt 1 lw 1.0 lc rgb '#222222', \
 '' u (f($1)):(g($4,1)) w l lt 1 lw 1.0 lc rgb '#222222', \
 '' u (f($1)):(g($5,2)) w l lt 1 lw 1.0 lc rgb '#222222'

