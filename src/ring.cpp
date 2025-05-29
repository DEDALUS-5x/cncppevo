#include "ring.hpp"
#include <rang.hpp>
#include <fmt/core.h>
#include <fstream>
#include <sstream>

using namespace cncpp;
using namespace std;
using namespace rang;
using namespace fmt;

Ring::Ring(Block *tail) : _tail(tail), _head(nullptr) {

  cerr << style::italic << format("Creating ring starting from: {:}", _tail -> desc()) << style::reset << endl;
}

Ring::~Ring(){
  cerr << style::italic << "Destroying ring " << style::reset << endl;
}

void Ring::update_lookahead(){
  
}