#ifndef HASHSET_H
#define HASHSET_H

#include "HashFunctions.h"
#include <string>

class HashSet {
  int           nitems; // The number of items in the set
  int           nslots; // The number of slots in the table
  HashFunction* hashfn; // The hash function to use for inserts
  HashFunction* stepfn; // The hash function to use for probing (if you do double hashing)
  std::string** slots;  // The slots themselves

  void rehash();

public:
  HashSet();
  ~HashSet();

  void insert(const std::string& value);
  bool lookup(const std::string& value) const;
};

#endif
