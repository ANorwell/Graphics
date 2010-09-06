
EXE = shader
OUT_EXT = o
CC = g++
CFLAGS = -Wall
LFLAGS = -Wall -lglut -lGLEW

HEADER_FILES_ = $(shell find ./ -maxdepth 1 -regex .*\.h)
HEADER_FILES =  $(basename $(notdir $(HEADER_FILES_)))

CPP_FILES_ = $(shell find ./ -maxdepth 1 -regex .*\.cpp)
CPP_FILES =  $(basename $(notdir $(CPP_FILES_)))

OBJECTS = $(patsubst %, %.$(OUT_EXT), $(CPP_FILES))

%.$(OUT_EXT): %.cpp $(HEADER_FILES)
	$(CC) $(CFLAGS) -c %.cpp

all: $(OBJECTS)
	$(CC) $(LFLAGS) -o $(EXE) $(OBJECTS)

test:
	echo $(CPP_FILES)
	echo $(OBJECTS)
	echo $(HEADER_FILES)

clean:
	rm -f *.$(OUT_EXT)

bmptest:
	g++ -Wall -lglut -lGLEW -o bmptest test/bmptest.cpp Util.cpp Texture.cpp 
