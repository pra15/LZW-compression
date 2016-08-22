all:
	g++ -o compressor utils.cpp hashclass.cpp compressor.cpp example.cpp  

clean:
	rm -rf *.out *.o
