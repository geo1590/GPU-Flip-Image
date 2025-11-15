# Define the compiler and flags
NVCC = /usr/local/cuda/bin/nvcc
CXX = g++
CXXFLAGS = -std=c++11 -I/usr/local/cuda/include -Iinclude -I/content/myFlip/cuda-samples/Common/UtilNPP -I/content/myFlip/cuda-samples/Common -I/usr/include/opencv4
LDFLAGS = -L/usr/local/cuda/lib64 -lcudart -lnppc -lnppial -lnppicc -lnppidei -lnppif -lnppig -lnppim -lnppist -lnppisu -lnppitc -lfreeimage -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs

# Define directories
SRC_DIR = src
BIN_DIR = bin
DATA_DIR = data
DATA_OUT_DIR = out_data
LIB_DIR = lib

# Define source files and target executable
SRC = $(SRC_DIR)/imageFlipNPP.cpp
INCLUDE = $(SRC_DIR)/imageFlipNPP.h
TARGET = $(BIN_DIR)/imageFlipNPP

# Define the default rule
all: $(TARGET)

# Rule for building the target executable
$(TARGET): $(SRC) $(INCLUDE)
	mkdir -p $(BIN_DIR)
	$(NVCC) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS) -std=c++17 -Xcompiler -std=c++17 

# Rule for running the application
run: $(TARGET)
	./$(TARGET) > output.txt

# Clean up
clean:
	rm -rf $(BIN_DIR)/*
	rm -rf $(DATA_OUT_DIR)/*
	rm -f output.txt > /dev/null 2>&1

# Installation rule (not much to install, but here for completeness)
install:
	@echo "No installation required."

# Help command
help:
	@echo "Available make commands:"
	@echo "  make        - Build the project."
	@echo "  make run    - Run the project."
	@echo "  make clean  - Clean up the build files."
	@echo "  make install- Install the project (if applicable)."
	@echo "  make help   - Display this help message."
