
SER_URL='https://www.dropbox.com/s/0pvi8n8teces37d/chsqc-ca.ser?dl=1'

SER=../ser
DAT=../data
FID=$(DAT)/hsqc.fid
NUS=$(DAT)/hsqc.nus
FT1=$(DAT)/hsqc.ft1
FT2=$(DAT)/hsqc.ft2
NV=$(DAT)/hsqc.nv

all: $(NV)

$(NV): $(FT2)
	./conv.sh

$(FT2): $(FT1)
	./proc2.sh

$(FT1): $(NUS)
	./procR.sh

$(NUS): $(FID)
	./proc1.sh

$(FID): $(DAT) $(SER)
	./fid.sh

$(DAT):
	mkdir $@

$(SER):
	wget $(SER_URL) -O $@
	sha1sum -c $(addsuffix .sha1,$@)

clean:
	rm -rf $(DAT)

again: clean all

