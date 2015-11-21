
TARGETS= 1

PNG=$(addsuffix .png,$(addprefix final/figure-,$(TARGETS)))
TIF=$(addsuffix .tif,$(addprefix final/figure-,$(TARGETS)))

TIFOPTS=  -density 1000x1000
TIFOPTS+= -units PixelsPerInch
TIFOPTS+= -colorspace CMYK
TIFOPTS+= -compress LZW

.SUFFIXES: .png .tif

all: final $(PNG) $(TIF)

final:
	install -d $@

final/figure-1.png: conv-by-method/output.png
	cp $^ $@

%.png:
	$(MAKE) -C $(dir $@)

.png.tif:
	convert $(TIFOPTS) $^ $@

clean:
	rm -f $(PNG) $(TIF)

distclean: clean
	$(MAKE) -C conv-by-def clean
	$(MAKE) -C conv-by-lambda clean
	$(MAKE) -C conv-by-method clean
	$(MAKE) -C conv-by-steps clean
	$(MAKE) -C hnco clean
	$(MAKE) -C hsqc clean
	$(MAKE) -C nonlin clean
	$(MAKE) -C spec-by-def clean
	$(MAKE) -C split clean

again: clean all

