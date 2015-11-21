
FIGS=output.png
DATA=output.dat

all: $(FIGS)

output.png: $(DATA)
	gnuplot output.plt

$(DATA): ../../tests/conv/conv-by-def.dat
	octave -qf output.m $^ $@

clean:
	rm -f $(FIGS) $(DATA)

again: clean all

