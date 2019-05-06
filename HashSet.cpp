#include "HashSet.h"
#include "string"
#include "vector"
#include "IntegerHashes.h"
#include "StringHashes.h"

HashSet::HashSet(){
  this -> nitems = 0;
  this -> nslots = 101;
  slots = new std::string*[nslots];
  this -> strfn = new JenkinsHash();
  this -> intfn = new SquareRootHash(1,nslots);
}

HashSet::~HashSet(){
  for(int i = 0; i < nslots; i++){
    delete slots[i];
  }
  delete slots;
  delete strfn;
  delete intfn;
}

void HashSet::insert(const std::string& value){
  uint64_t key = intfn -> hash(strfn -> hash(value));
  while(slots[key] != NULL && *(slots[key]) != value){
    key++;
  }
  if(slots[key] == NULL){
    slots[key] = &value;
    nitems++;
  }
  else if(*(slots[key]) == value){
    return;
  }
  if(nitems == nslots){
    rehash();
  }
}

bool HashSet::lookup(const std::string& value) const{
  uint64_t key = intfn -> hash(strfn -> hash(value));
  while(slots[key] != NULL){
    if(*(slots[key]) == value){
      return true;
    }
    key++;
  }
  return false;
}

void HashSet::rehash(){
  std::string** oldslots = slots;
  slots = new std::string*[nslots*2];
  for(int i = 0; i < nslots; i++){
    *(slots[i]) = *(oldslots[i]);
    delete oldslots[i];
  }
  delete oldslots;
  nslots = 2 * nslots; 
} 
