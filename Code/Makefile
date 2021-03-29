CFLAGS = -pthread -std=c++11 -Wall
OPENCV = `pkg-config opencv --cflags --libs`
LIBS = $(OPENCV)

SRC = ./Source_code
BUILD = ./build

all:
	mkdir -p $(BUILD)
	mkdir -p ../Analysis/results
	echo "Building.."
	g++ $(CFLAGS) $(LIBS) $(SRC)/main.cpp -o $(BUILD)/main.out
	echo "Generated $(BUILD)/main.out \nEnter 'make run' to execute."

graph:
	python3 ../Analysis/plotter.py

download: 
	if [ -f "./Assets/trafficvideo.mp4" ]; then echo "Already downloaded!"; else curl https://www.cse.iitd.ac.in/~rijurekha/cop290_2021/trafficvideo.mp4 -o assets/trafficvideo.mp4; fi

run:
	mkdir -p ../Analysis/results
	if [ -f "./build/main.out" ]; then echo "Running $(BUILD)/main.out"; $(BUILD)/main.out; else echo "Build not found. Run 'make' first."; fi

clean:
	rm -rf $(BUILD)
	rm -rf ../Analysis/results
	echo "Cleaned."

help: 
	echo "Commands:"
	echo "'make'         : builds the executable."
	echo "'make run'     : runs the executable. Does not create the executable."
	echo "'make clean'   : removes the build files."
	echo "'make graph'   : plot the graph for results/out.txt and save it in the results directory."
	echo "'make download': downloads assets and stores them in the appropriate directory."

model:
	rm -rf model_result
	mkdir model_result
	cp results/* model_result/

.DEFAULT: 
	@echo "No such command found. Run 'make help' for more info."

.SILENT: all run clean help download graph model