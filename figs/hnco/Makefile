
FIGS=output.png
DATA=output.dat

all: $(FIGS)

output.png: $(DATA)
	gnuplot output.plt

$(DATA): ../../data/hnco-ext/data/prj/15N.13C.dat
	octave -qf output.m $^ $@
	sed -e 's,NaN,,g' -i $@

clean:
	rm -f $(FIGS) $(DATA)

again: clean all

