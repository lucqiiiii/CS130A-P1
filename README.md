# Program One

In this assignment, you will investigate [Bloom filters][bf].  Bloom filters are
data structures that represent sets  and allow the user to query whether a given
item is a member of the set or not. They use hash functions and a clever storage
mechanism to  represent sets  using very little memory.  The cost of this is the
occasional false positive - a Bloom filter may report that an item is in the set
when it is not.

A Bloom filter consists of  _m_ bits  and _k_ hash functions  that hash items to
integers  in the range [0, _m_).  In a Bloom filter  representing the empty set,
all the bits are set to zero.  To add an item to the set,  run that item through
all the hash functions;  use the results as indices into the bits array, and set
those bits to one.  To test if an item is in the set,  run that item through all
the hash functions;  use the results to index into the bits and report true only
if all the referenced bits were ones.

You will implement  a Bloom filter  and a hash set (for use as a baseline).  You
will then experiment with changing the values of the Bloom filter parameters and
write a short report on your findings.


## Part One: The Code

This repo contains starter code that compares a Bloom filter against a true set.
It takes  two filenames  on the command line  as parameters.  Each line from the
first file is inserted. Each line from the second file is looked up.  A hash set
is used as a baseline to report each lookup as a true negative,  false positive,
etc.  The starter code will handle the testing logic, input, and output for you,
but you'll need to implement the data structures yourself:

- You'll find the `BloomFilter` class declared in `BloomFilter.h`;  implement it
  in `BloomFilter.cpp`.
- The `HashSet` class  declared in `HashSet.h` is the true set your Bloom filter
  will be compared against. Implement it in `HashSet.cpp`. Note that it's a set,
  not a map, and that you only need to implement insertion and lookup.  Use open
  address hashing with a hash function and probing scheme of your choice.
- The `HashFunctions.h` header file contains class declarations for a variety of
  hash functions.  Implement these in `HashFunctions.cpp`.


## Part Two: The Report

Use the program  you just completed  to observe the effects of  the Bloom filter
parameters  on the false positive rate.  For each parameter listed below,  graph
the false positive rate  of each hash function  over the range given.  Leave all
other parameters at their default values.  Explain your observations.

Use the same data that you used for the final question in Homework Two, that is,
the  `primaryName`  column  from the IMDb  `name.basics`  dataset,  available at
<https://datasets.imdbws.com/>.  Use the  entire name  as the key  (not just the
last eight bytes, as in the homework).

- The parameter _k_ controls the number of hash functions used.  Try setting _k_
  to values between TODO and TODO with the `-k` option.
- The parameter _m_ controls the number of bits in the Bloom filter. Try setting
  _m_ to values between TODO and TODO with the `-m` option.
- The parameter _n_  represents  the number of items  in the set.  Use the  `-n`
  option to control how many lines are inserted.  Try values of _n_ between TODO
  and TODO.


## Miscellaneous

- This is an individual assignment.  No group work!
- Your code must compile with no warnings (the `-Werror` flag enforces this).
- You may not use global variables or any data structures from other libraries.
- Do not edit  `bloom.cpp`  or any of the `*.h` files;  your code will be tested
  against the originals.
- The summary includes false negatives; this may be useful for debugging, but if
  your code is working properly, this number will always be zero.
- The  starter code  operates on entire lines,  but the IMDb data  contains more
  than just names on each line.  You'll need to preprocess your data set so that
  you only work with names;  the  `cut`  command  should be helpful here.  Don't
  forget to remove the header row!
- [False positive rates][fp]  should be given as percentages:  the percentage of
  all items not in the set that were incorrectly reported as in the set.


[bf]: https://en.wikipedia.org/wiki/Bloom_filter
[fp]: https://en.wikipedia.org/wiki/Evaluation_of_binary_classifiers
