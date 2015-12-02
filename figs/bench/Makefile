
FIGS=output.png
DATA=camera.dat rnmrtk.dat output.dat

all: $(FIGS)

output.png: $(DATA)
	gnuplot output.plt

camera.dat: ../../data/nhsqc-ubq-sigma/data/hsqc.log
	awk -v a=388 -f camera.awk $^ | sort -n > $@

rnmrtk.dat: ../../data/nhsqc-ubq-sigma/data/bench.log
	a=$$(grep -n '^Chunk *696' $^ | cut -f 1 -d ':' | head -n 1) && \
	b=$$(grep -n '^Chunk *696' $^ | cut -f 1 -d ':' | tail -n 1) && \
	awk -v a=$${a} -v b=$${b} -f rnmrtk.awk $^ > $@

output.dat: camera.dat rnmrtk.dat
	octave -qf output.m $^ $@

clean:
	rm -f $(FIGS) $(DATA)

again: clean all

