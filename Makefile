GXX = g++
FLAGS = -Wall -Werror -O
LIBS = -lstdc++fs
SRC = *.cc

ImportsCleaner:
	$(GXX) $(FLAGS) $(SRC) $(LIBS)

clean:
	rm *.out

run:
	./a.out
