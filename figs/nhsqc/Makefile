
FIGS=output.png
DATA=output-sigma.dat output-lambda.dat

all: $(FIGS)

output.png: $(DATA)
	gnuplot output.plt

output-sigma.dat: ../../data/nhsqc-ubq-sigma/data/hsqc.ft2
	octave -qf output.m $^ $@
	sed -e 's,NaN,,g' -i $@

output-lambda.dat: ../../data/nhsqc-ubq-lambda/data/hsqc.ft2
	octave -qf output.m $^ $@
	sed -e 's,NaN,,g' -i $@

clean:
	rm -f $(FIGS) $(DATA)

again: clean all

