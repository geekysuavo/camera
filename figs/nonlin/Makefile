
FIGS=output.png
DATA=output-ints.dat output-corr.dat

all: $(FIGS)

output.png: $(DATA)
	gnuplot output.plt

output-ints.dat: ../../tests/nonlin/nonlin.dat
	cp $^ $@

output-corr.dat: ../../tests/nonlin/nonlin.corr
	cp $^ $@

clean:
	rm -f $(FIGS) $(DATA)

again: clean all

