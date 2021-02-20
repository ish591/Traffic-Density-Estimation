CFLAGS = -pthread -std=c++11 -Wall
OPENCV = `pkg-config opencv --cflags --libs`
LIBS = $(OPENCV)

SRC = ./src
BUILD = ./build

all:
	mkdir -p $(BUILD)
	g++ $(CFLAGS) $(LIBS) $(SRC)/main.cpp -o $(BUILD)/main.out

run:
	$(BUILD)/main.out

clean:
	rm -rf $(BUILD)
	rm -f *.o
	rm -f *.jpg