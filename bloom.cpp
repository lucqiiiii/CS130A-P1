#include "BloomFilter.h"
#include "HashSet.h"

#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>

void usage() {
  std::cerr << "USAGE: ./bloom [options] <file-to-store> <file-to-check>\n";
  std::cerr << "  -c      Disable coloration of the output.\n";
  std::cerr << "  -i <I>  Use method I to hash (reduce) integers.  Methods are:\n";
  std::cerr << "            division   - use the division method\n";
  std::cerr << "            reciprocal - use the multiplication method with fractions\n";
  std::cerr << "            squareroot - use the multiplication method with square roots\n";
  std::cerr << "  -k <K>  Use K hash functions in the Bloom filter.\n";
  std::cerr << "  -m <M>  Use M bits in the Bloom filter.\n";
  std::cerr << "  -n <N>  Insert only the first N lines of file-to-store.\n";
  std::cerr << "  -s <S>  Use method S to hash strings to integers.  Methods are:\n";
  std::cerr << "            jenkins - use Jenkins' one-at-a-time hashing\n";
  std::cerr << "            pearson - use 8-bit Pearson hashing\n";
  std::cerr << "  -t      Print the summary in tabular (CSV) format.\n";
  std::cerr << "  -v      Increase verbosity (may be repeated).\n";
}

int parseint(char opt, const char* arg) {
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
  const char* i = "squareroot";
  const char* s = "jenkins";

  bool c = true;
  int  k = 0;
  int  m = 0;
  int  n = 0;
  bool t = false;
  int  v = 0;
  int  opt;

  while((opt = getopt(argc, argv, "ci:k:m:n:s:tv")) != -1) {
    switch(opt) {
    case 'c':
      c = false;
      break;
    case 'i':
      i = optarg;
      break;
    case 'k':
      k = parseint('k', optarg);
      break;
    case 'm':
      m = parseint('m', optarg);
      break;
    case 'n':
      n = parseint('n', optarg);
      break;
    case 's':
      s = optarg;
      break;
    case 't':
      t = true;
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

 if(optind != argc - 2) {
    std::cerr << "Exactly two filenames are required.\n\n";
    usage();
    exit(1);
  }

  const std::string ADDL = "[" + color("INSERT", 34, c) + "] ";
  const std::string TPOS = "[" + color("T. POS", 32, c) + "] ";
  const std::string TNEG = "[" + color("T. NEG", 36, c) + "] ";
  const std::string FPOS = "[" + color("F. POS", 33, c) + "] ";
  const std::string FNEG = "[" + color("F. NEG", 31, c) + "] ";

  std::string line;
  BloomFilter filter(k, m, s, i);
  HashSet hashset;

  int nstores = 0;
  int nchecks = 0;
  int tpos = 0;
  int tneg = 0;
  int fpos = 0;
  int fneg = 0;

  // Insert lines from file-to-store
  std::ifstream store(argv[optind]);
  if(store.fail()) {
    std::cerr << "Unable to open file: " << argv[optind] << '\n';
    exit(1);
  }

  while(std::getline(store, line)) {
    if(v > 1) std::cout << ADDL << line << '\n';
    hashset.insert(line);
    filter.insert(line);

    nstores += 1;
    if(nstores == n) {
      break;
    }
  }

  if(v > 1) std::cout << '\n';
  store.close();

  // Look up lines from file-to-check
  std::ifstream check(argv[optind+1]);
  if(check.fail()) {
    std::cerr << "Unable to open file: " << argv[optind+1] << '\n';
    exit(1);
  }

  while(std::getline(check, line)) {
    bool sfound = hashset.lookup(line);
    bool ffound = filter.lookup(line);
    nchecks += 1;

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
  if(t) {
    std::cout << "Inserted " << nstores << " items.\n";
    std::cout << "Looked up " << nchecks << " items.\n";
    std::cout << " - True Positives:  " << color(std::to_string(tpos), 32, c) << '\n';
    std::cout << " - True Negatives:  " << color(std::to_string(tneg), 36, c) << '\n';
    std::cout << " - False Positives: " << color(std::to_string(fpos), 33, c) << '\n';
    std::cout << " - False Negatives: " << color(std::to_string(fneg), 31, c) << '\n';
  }
  else {
    std::cout << nstores << ',';
    std::cout << nchecks << ',';
    std::cout << tpos << ',';
    std::cout << tneg << ',';
    std::cout << fpos << ',';
    std::cout << fneg << '\n';
  }

  return 0;
}
