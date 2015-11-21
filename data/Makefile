
all:
	$(MAKE) -C nhsqc-gb1/scripts
	$(MAKE) -C nhsqc-ubq-sigma/scripts
	$(MAKE) -C nhsqc-ubq-lambda/scripts
	$(MAKE) -C hnco/scripts
	$(MAKE) -C noesy/scripts

clean:
	$(MAKE) -C nhsqc-gb1/scripts $@
	$(MAKE) -C nhsqc-ubq-sigma/scripts $@
	$(MAKE) -C nhsqc-ubq-lambda/scripts $@
	$(MAKE) -C hnco/scripts $@
	$(MAKE) -C noesy/scripts $@

distclean: clean
	rm -f */ser

again: clean all

