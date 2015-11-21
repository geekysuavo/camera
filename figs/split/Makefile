
FIGS=output.png
DATA=output.dat

all: $(FIGS)

output.png: $(DATA)
	gnuplot output.plt

$(DATA): ../../tests/split/split.dat
	cp $^ $@

clean:
	rm -f $(FIGS) $(DATA)

again: clean all

