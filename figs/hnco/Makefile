
FIGS=output.png
DATA=output.dat

all: $(FIGS)

output.png: $(DATA)
	gnuplot output.plt

$(DATA):
	proj3D.tcl -in '../../data/hnco-ext/data/yzx/hnco%04d.ft3'
	octave -qf output.m 15N.13C.dat $@
	sed -e 's,NaN,,g' -i $@
	rm -f 15N.13C.dat
	rm -f 15N.HN.dat
	rm -f 13C.HN.dat

clean:
	rm -f $(FIGS) $(DATA)

again: clean all

