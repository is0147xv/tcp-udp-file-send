all: tcpxmit.o tcprcv.o uclient.o userver.o
tcpxmit.o:
	gcc tcpxmit.c -o $@
tcprcv.o:
	gcc tcprcv.c -o $@
uclient.o:
	gcc uclient.c -o $@
userver.o:
	gcc userver.c -o $@
clear:
	rm *.o
