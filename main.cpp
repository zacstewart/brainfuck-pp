#include <iostream>
#include <fstream>
#include <sys/stat.h>

using namespace std;

class Brainfuck {
  public:
    char data[30000];
    char *d;
    const char *p;

    Brainfuck(const char prog[]) {
      d = data;
      p = prog;
    }

    void pincr() {
      d++;
    }

    void pdecr() {
      d--;
    }

    void bincr() {
      (*d)++;
    }

    void bdecr() {
      (*d)--;
    }

    void puts() {
      std::cout << *d;
    }

    void gets() {
      std::cin >> *d;
    }

    void bropen() {
      int bal = 1;
      if (*d == '\0') {
        do {
          p++;
          if      (*p == '[') bal++;
          else if (*p == ']') bal--;
        } while ( bal != 0 );
      }
    }

    void brclose() {
      int bal = 0;
      do {
        if      (*p == '[') bal++;
        else if (*p == ']') bal--;
        p--;
      } while ( bal != 0 );
    }

    void evaluate() {
      while (*p) {
        switch (*p) {
          case '>':
            pincr();
            break;
          case '<':
            pdecr();
            break;
          case '+':
            bincr();
            break;
          case '-':
            bdecr();
            break;
          case '.':
            puts();
            break;
          case ',':
            gets();
            break;
          case '[':
            bropen();
            break;
          case ']':
            brclose();
            break;
        }
        p++;
      }
    }
};

int main(int argc, const char* argv[]) {
  std::ifstream file;
  int length;
  char* buffer;
  struct stat stats;

  if( stat (argv[1], &stats) == 0 ) {
    file.open(argv[1]);
    file.seekg(0, std::ios::end);
    length = file.tellg();
    file.seekg(0, std::ios::beg);
    buffer = new char[length];
    file.read(buffer, length);
    file.close();
  }
  else {
    buffer = (char*) argv[1];
  }
  
  Brainfuck interpreter = Brainfuck(buffer);
  interpreter.evaluate();
}
