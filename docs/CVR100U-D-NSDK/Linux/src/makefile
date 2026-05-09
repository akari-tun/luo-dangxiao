libs = -L../lib -l100UD
libs += -L../lib -lusb-1.0
libs += -L../lib -lwlt

CC= gcc
RM=rm -f

lib100UDTest:LibTestMain.c
	$(CC) -o ../bin/lib100UDTest LibTestMain.c -ldl

test:lib100UDTest
	LD_LIBRARY_PATH=../lib ../bin/lib100UDTest

clean:
	$(RM) zp.bmp fp.dat LibTestMainPhoto.bmp wz.txt programLog.txt ../bin/lib100UDTest
