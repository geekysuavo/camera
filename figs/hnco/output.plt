set terminal pngcairo enhanced dashed dashlength 5 \
 font 'Free Sans Bold, 80' \
 size 3542, 3030 \
 linewidth 14 rounded
set output 'output.png'
set encoding utf8

n1 = 512
obs1 = 176.098999
sw1 = 3872.967041
off1 = 29066.607422
ppm1(x) = (((n1 - x) / n1) * sw1 + off1) / obs1

n2 = 256
obs2 = 70.959999
sw2 = 2600.104004
off2 = 7060.109375
ppm2(x) = (((n2 - x) / n2) * sw2 + off2) / obs2

set xlabel '^{13}C / ppm' font 'Free Sans, 120'
set ylabel '^{15}N / ppm' font 'Free Sans, 120'
set xrange [179 : 167] rev
set yrange [135 : 100] rev
unset colorbox
unset key

set palette defined (\
  0.0 '#222222', \
  1.0 '#bbbbbb' \
)

plot 'output.dat' u (ppm1($1)):(ppm2($2)):3 w l lt 1 lw 0.5 lc pal

