INCLUDES = -Iinclude
LIBS = `pkg-config gtkmm-3.0 --cflags --libs`
FLAGS = -std=c++17 -Wall -fsanitize=address -MMD
DEFINES = -D DEBUG
COMPILER = g++ $(DEFINES) $(INCLUDES) $(FLAGS)
OBJECTS = main.o menu.o about.o settings.o game.o storage.o mct.o board.o

all: $(OBJECTS)
	$(COMPILER) -o sos $(OBJECTS) $(LIBS)
	rm *.d

mct.o: mct.cpp
	$(COMPILER) -O3 -c mct.cpp $(LIBS)

board.o: board.cpp
	$(COMPILER) -O3 -c board.cpp $(LIBS)

%.o: %.cpp
	$(COMPILER) -c $< $(LIBS)

clean:
	rm sos *.o *.d
