#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>

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

void usage() {
  cout << "Compile a string or file of Brainfuck code."  << endl << endl;
  cout << "Usage: brainfuck -h | -e <string> | filename" << endl << endl;

  cout << "    <filename>   Compile and run a source file."                << endl;
  cout << "    -e <string>  Directly evaluate a string as Brainfuck code." << endl;
  cout << "    -h           Print this usage."                             << endl << endl;
}

int main(int argc, char **argv) {
  std::ifstream file;
  int length;
  int opt;
  char* filename;
  char* buffer;
  struct stat stats;

  opterr = 0;
  while ((opt = getopt (argc, argv, "he:")) != -1) {
    switch (opt) {
      case 'h':
        usage();
        exit(0);

      case 'e':
        buffer = (char*) optarg;
        break;

      case '?':
        if (optopt == 'e')
          fprintf (stderr, "Option '-%c' requires an argument.\n", optopt);
        else
          fprintf (stderr, "Unknown option '-%c'.\n", optopt);
        exit(1);

      default:
        exit(2);
      }
  }

  if(! buffer) {
    // not run with -e, lets look for a file
    filename = (char*) argv[optind];

    if( stat (filename, &stats) == 0 ) {
      file.open(filename);
      file.seekg(0, std::ios::end);
      length = file.tellg();
      file.seekg(0, std::ios::beg);
      buffer = new char[length];
      file.read(buffer, length);
      file.close();
    }
    else {
      fprintf (stderr, "Cannot open file %s.\n", filename);
      exit(2);
    }
  }

  Brainfuck interpreter = Brainfuck(buffer);
  interpreter.evaluate();
}
