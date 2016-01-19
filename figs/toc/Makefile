
PNG=output.png
SVG=output.svg

PNGOPTS=-z -w 1500 -h 600

all: $(PNG)

$(PNG): $(SVG)
	inkscape $(PNGOPTS) -f $^ -e $@

clean:
	rm -f $(PNG)

again: clean all

