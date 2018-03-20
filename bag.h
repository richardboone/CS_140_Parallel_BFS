#ifndef BAG_H
#define BAG_H

#include "cilk/cilk.h"
//#include "cilk/reducer.h"

using namespace std;

class Bag {

 private:
  int count;
  int bagsize;
  int *data;
  void resize();

 public:
  Bag();
  Bag(int size);
  Bag(Bag& copy);
  ~Bag();
  void insert(int n);
  void remove(int n);
  int getElement(int index);
  int size();
  //void merge(Bag* b);
  void clear();
  bool inbag(int n);
  int howmany(int n);
  Bag add(Bag);
  bool isEmpty();
  Bag& operator=(Bag);
  Bag operator+(Bag);

  //friend class Bag_reducer;
  //friend class cilk::monoid_base<Bag >;
};

/*
class bag_reducer {
 public:
  struct Monoid: cilk::monoid_base<Bag > {
    static void reduce(Bag* left, Bag* right) {
      left->merge(right);
    }
  };

 private:
  cilk::reducer<Monoid> imp;
};
*/

#endif
