
SER_URL='https://www.dropbox.com/s/u5kvkij2g68isg8/hnco.ser?dl=1'

SER=../ser
DAT=../data
FID=$(DAT)/fid/hnco0001.fid
NUS=$(DAT)/yzx/hnco0001.nus
FT1=$(DAT)/yzx/hnco0001.ft1
FT3=$(DAT)/yzx/hnco0001.ft3
PRJ=$(DAT)/prj/15N.13C.dat
NV=$(DAT)/hnco.nv

all: $(NV) $(PRJ)

$(NV): $(FT3)
	./conv.sh

$(PRJ): $(FT3)
	./prj.sh

$(FT3): $(FT1)
	./proc23.sh

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

