all: brainfuck

brainfuck: main.cpp
	g++ main.cpp -o brainfuck

clean:
	rm -f brainfuck
