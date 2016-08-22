all:
	g++ utils.cpp hashclass.cpp compressor.cpp example.cpp  

clean:
	rm -rf *.out *.o
