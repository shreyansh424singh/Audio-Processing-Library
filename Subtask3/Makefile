CC = g++

all: link

link: yourcode
	$(CC) -o yourcode.out yourcode -laudio -L. -Wl,-rpath=.
	rm yourcode

yourcode: audio yourcode.cpp
	$(CC) -c yourcode.cpp -o yourcode

audio: audio.cpp dnn_weights.h openblas.cpp
	$(CC) -o libaudio.so -fpic -shared audio.cpp -lblas
