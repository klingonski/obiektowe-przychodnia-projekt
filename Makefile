CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic

przychodnia: src/*.cpp src/*.h
	$(CXX) $(CXXFLAGS) src/*.cpp -o przychodnia

run: przychodnia
	./przychodnia

clean:
	rm -f przychodnia
