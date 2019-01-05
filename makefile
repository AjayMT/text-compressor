
both: encoder decoder

encoder: encoder.cpp tree.cpp
	c++ -std=c++17 -o encoder encoder.cpp

decoder: decoder.cpp tree.cpp
	c++ -std=c++17 -o decoder decoder.cpp

.PHONY: clean
clean:
	rm -f ./encoder
	rm -f ./decoder
