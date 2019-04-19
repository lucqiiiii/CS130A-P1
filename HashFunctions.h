#ifndef HASHFUNCTIONS_H
#define HASHFUNCTIONS_H

#include <cstdint>
#include <string>

class HashFunction {
  int i; // The index of this hash function
  int m; // The range of the output

public:
  HashFunction(int i, int m);
  virtual uint64_t hash(const std::string& value) const = 0;
};

class AsciiSumHash: public HashFunction {
public:
  AsciiSumHash(int i, int m);
  uint64_t hash(const std::string& value) const;
};

class DivisionHash: public HashFunction {
public:
  DivisionHash(int i, int m);
  uint64_t hash(const std::string& value) const;
};

class MultiplicationHash: public HashFunction {
  double b;
public:
  MultiplicationHash(int i, int m, double b);
  uint64_t hash(const std::string& value) const;
};

#endif
