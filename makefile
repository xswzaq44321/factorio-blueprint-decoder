b2j: b2j.cpp base64.h
	g++ b2j.cpp -o b2j -lz
j2b: j2b.cpp base64.h
	g++ j2b.cpp -o j2b -lz
clean:
	rm *.exe
