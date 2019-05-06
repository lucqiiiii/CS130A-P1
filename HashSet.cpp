#include "HashSet.h"
#include "string"
#include "vector"
#include "IntegerHashes.h"
#include "StringHashes.h"

HashSet::HashSet(){
  this -> nitems = 0;
  this -> nslots = 101;
  slots = new std::string*[nslots];
  for(int i = 0; i < nslots; i++){
    slots[i] = NULL;
  }
  this -> strfn = new JenkinsHash();
  this -> intfn = new SquareRootHash(1,nslots);
}

HashSet::~HashSet(){
  for(int i = 0; i < nslots; i++){
    delete slots[i];
  }
  delete []slots;
  delete strfn;
  delete intfn;
}

void HashSet::insert(const std::string& value){
  uint64_t key = intfn -> hash(strfn -> hash(value));
  while(slots[key] != NULL){
    key = (key + 1) % nslots;
  }
  slots[key] = new std::string(value);
  nitems++;
  if(static_cast<double>(nitems)/nslots >= 0.7){
    rehash();
  }
}

bool HashSet::lookup(const std::string& value) const{
  uint64_t key = intfn -> hash(strfn -> hash(value));
  while(slots[key] != NULL){
    if(*(slots[key]) == value){
      return true;
    }
    key = (key + 1) % nslots;
  }
  return false;
}

void HashSet::rehash(){
  std::string** oldslots = slots;
  slots = new std::string*[nslots*2]();
  delete intfn;
  this -> intfn = new SquareRootHash(1,nslots*2);
  for(int i = 0; i < nslots; i++){
    if(oldslots[i] != NULL){
      uint64_t new_key = intfn -> hash(strfn -> hash(*(oldslots[i])));
      while(slots[new_key] != NULL){
        new_key = (new_key + 1) % (nslots*2);
      }
      if(slots[new_key] == NULL){
        slots[new_key] = oldslots[i];
      }
    }
  }
  delete []oldslots;
  nslots = 2 * nslots; 
} 
