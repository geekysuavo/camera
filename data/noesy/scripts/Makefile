
SER_URL='https://www.dropbox.com/s/7yoyioer17uvms7/noesy.ser?dl=1'

SER=../ser
DAT=../data
FID=$(DAT)/fid/noe00010001.fid
NUS=$(DAT)/yzax/noe00010001.nus
FT1=$(DAT)/yzax/noe00010001.ft1
FT4=$(DAT)/yzax/noe00010001.ft4
NV=$(DAT)/noesy.nv

all: $(NV)

$(NV): $(FT4)
	./conv.sh

$(FT4): $(FT1)
	./proc234.sh

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

