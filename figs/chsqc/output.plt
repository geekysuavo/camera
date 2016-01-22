set terminal pngcairo enhanced dashed dashlength 5 \
 font 'Free Sans Bold, 80' \
 size 7480, 2530 \
 linewidth 14 rounded
set output 'output.png'
set encoding utf8

n1 = 853
obs1 = 700.213013
sw1 = 1752.962402
off1 = -349.948181
ppm1(x) = (((n1 - x) / n1) * sw1 + off1) / obs1

n2 = 4096
obs2 = 176.070999
sw2 = 10570.825195
off2 = -1761.411865
ppm2(x) = (((n2 - x) / n2) * sw2 + off2) / obs2

set multiplot

set xlabel '^{1}H / ppm' font 'Free Sans, 120'
set ylabel '^{13}Cα / ppm' font 'Free Sans, 120'
set xrange [1.0 : 0.5] rev
set yrange [14 : 26] rev
unset colorbox
unset key

set palette defined (\
  0.0 '#111111', \
  1.0 '#999999' \
)

set size 0.5, 1.0
set origin 0.0, 0.0
set label 1 '(A)' at graph 0.93, 0.91 center font 'Free Sans Bold, 120'

plot 'output-ca.dat' u (ppm1($1)):(ppm2($2)):3 w l lt 1 lw 0.5 lc pal

unset label 1
set size 0.5, 1.0
set origin 0.5, 0.0
set label 2 '(B)' at graph 0.93, 0.91 center font 'Free Sans Bold, 120'

plot 'output-cacb.dat' u (ppm1($1)):(ppm2($2)):3 w l lt 1 lw 0.5 lc pal

unset multiplot

