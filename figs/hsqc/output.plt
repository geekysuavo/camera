set terminal pngcairo enhanced dashed dashlength 5 \
 font 'Free Sans Bold, 80' \
 size 7480, 2530 \
 linewidth 14 rounded
set output 'output.png'
set encoding utf8

n1 = 1228
obs1 = 700.213013
sw1 = 2523.608398
off1 = 4341.180176
ppm1(x) = (((n1 - x) / n1) * sw1 + off1) / obs1

n2 = 2048
obs2 = 70.959999
sw2 = 2837.684082
off2 = 7096.679199
ppm2(x) = (((n2 - x) / n2) * sw2 + off2) / obs2

set multiplot

set xlabel '^{1}H / ppm' font 'Free Sans, 120'
set ylabel '^{15}N / ppm' font 'Free Sans, 120'
set xrange [8.95 : 8.0] rev
set yrange [127.5 : 118] rev
unset colorbox
unset key

set palette defined (\
  0.0 '#222222', \
  1.0 '#bbbbbb' \
)

set size 0.5, 1.0
set origin 0.0, 0.0
set label 1 '(A)' at graph 0.93, 0.91 center font 'Free Sans Bold, 120'

plot 'output-sigma.dat' u (ppm1($1)):(ppm2($2)):3 w l lt 1 lw 0.5 lc pal

unset label 1
set size 0.5, 1.0
set origin 0.5, 0.0
set label 2 '(B)' at graph 0.93, 0.91 center font 'Free Sans Bold, 120'

plot 'output-lambda.dat' u (ppm1($1)):(ppm2($2)):3 w l lt 1 lw 0.5 lc pal

unset label 2
unset xlabel
unset ylabel
unset xtics
unset ytics

set xrange [9.8 : 6.5] rev
set yrange [135 : 100] rev

set size 0.18, 0.4
set origin 0.315, 0.165

plot \
 '-' u 1:2:3 w filledc lw 0 lc rgb 'white', \
 '-' u 1:2:3 w filledc lt 2 lw 0.1 lc rgb '#cccccc', \
 'output-sigma.dat' u (ppm1($1)):(ppm2($2)):3 w l lt 1 lw 0.5 lc pal
6.5 100 135
9.8 100 135
end
8.00 118 127.5
8.95 118 127.5
end

unset multiplot

