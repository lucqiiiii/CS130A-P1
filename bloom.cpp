#include "BloomFilter.h"
#include "HashSet.h"

#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>

void usage() {
  std::cerr << "USAGE: ./bloom [options] <file-to-store> <file-to-check>\n";
  std::cerr << "  -c      Disable coloration of the output.\n";
  std::cerr << "  -k <K>  Use K hash functions in the Bloom filter.\n";
  std::cerr << "  -m <M>  Use M bits in the Bloom filter.\n";
  std::cerr << "  -n <N>  Insert only the first N lines of file-to-store.\n";
  std::cerr << "  -v      Increase verbosity (may be repeated).\n";
}

int positive(char opt, const char* arg) {
  try {
    int result = std::stoi(arg);
    if(result > 0) return result;
  }
  catch(std::exception&) {
    // Fall through.
  }

  std::cerr << "Option -" << opt << " requires a positive integer argument.\n\n";
  usage();
  exit(1);
}

std::string color(std::string text, int code, bool c) {
  if(c) return "\e" + std::to_string(code) + "m" + text + "\e0m";
  return text;
}

int main(int argc, char** argv) {
  bool c = true;
  int  k = 0;
  int  m = 0;
  int  n = 0;
  int  v = 0;
  int  opt;

  while((opt = getopt(argc, argv, "ck:m:n:v")) != -1) {
    switch(opt) {
    case 'c':
      c = false;
      break;
    case 'k':
      k = positive('k', optarg);
      break;
    case 'm':
      m = positive('m', optarg);
      break;
    case 'n':
      n = positive('n', optarg);
      break;
    case 'v':
      v += 1;
      break;
    case '?':
      std::cerr << "Unknown option -" << optopt << "\n\n";
      usage();
      exit(1);
    default:
      std::cerr << "Something has gone very wrong...\n\n";
      exit(1);
    }
  }

  // TODO: Check positional arguments.

  const std::string ADDL = "[" + color("INSERT", 34, c) + "] ";
  const std::string TPOS = "[" + color("T. POS", 32, c) + "] ";
  const std::string TNEG = "[" + color("T. NEG", 36, c) + "] ";
  const std::string FPOS = "[" + color("F. POS", 33, c) + "] ";
  const std::string FNEG = "[" + color("F. NEG", 31, c) + "] ";

  int count = 0;
  std::string line;
  BloomFilter filter(k, m);
  HashSet hashset;

  int tpos = 0;
  int tneg = 0;
  int fpos = 0;
  int fneg = 0;

  // Insert lines from file-to-store
  std::ifstream store(argv[1]);
  if(store.fail()) {
    std::cerr << "Unable to open file: " << argv[1] << '\n';
    exit(1);
  }

  while(std::getline(store, line)) {
    if(v > 1) std::cout << ADDL << line << '\n';
    hashset.insert(line);
    filter.insert(line);

    count += 1;
    if(count == n) {
      break;
    }
  }

  if(v > 1) std::cout << '\n';
  std::cout << "Inserted " << count << " items.\n\n";
  store.close();

  // Look up lines from file-to-check
  std::ifstream check(argv[2]);
  if(check.fail()) {
    std::cerr << "Unable to open file: " << argv[2] << '\n';
    exit(1);
  }

  while(std::getline(check, line)) {
    bool sfound = hashset.lookup(line);
    bool ffound = filter.lookup(line);

    if(sfound) {
      if(ffound) {
        if(v > 0) std::cout << TPOS << line << '\n';
        tpos += 1;
      }
      else {
        if(v > 0) std::cout << FNEG << line << '\n';
        fneg += 1;
      }
    }
    else {
      if(ffound) {
        if(v > 0) std::cout << FPOS << line << '\n';
        fpos += 1;
      }
      else {
        if(v > 0) std::cout << TNEG << line << '\n';
        tneg += 1;
      }
    }
  }

  if(v > 0) std::cout << '\n';
  check.close();

  // Summarize the results
  std::cout << "True Positives:  " << color(std::to_string(tpos), 32, c) << '\n';
  std::cout << "True Negatives:  " << color(std::to_string(tneg), 36, c) << '\n';
  std::cout << "False Positives: " << color(std::to_string(fpos), 33, c) << '\n';
  std::cout << "False Negatives: " << color(std::to_string(fneg), 31, c) << '\n';
  return 0;
}
