
FIGS=output.png
DATA=output-ca.dat output-cacb.dat

all: $(FIGS)

output.png: $(DATA)
	gnuplot output.plt

output-ca.dat: ../../data/chsqc-ca/data/hsqc.ft2
	octave -qf output.m $^ $@ 6e8 3e9
	sed -e 's,NaN,,g' -i $@

output-cacb.dat: ../../data/chsqc-cacb/data/hsqc.ft2
	octave -qf output.m $^ $@ 6e8 3e9
	sed -e 's,NaN,,g' -i $@

clean:
	rm -f $(FIGS) $(DATA)

again: clean all

