#ifndef RING_HPP
#define RING_HPP

#include "defines.hpp"
#include "block.hpp"
#include "machine.hpp"

namespace cncpp{

  class Ring : Object{

    public:
      Ring(Block *tail);
      ~Ring();

      void update_lookahead();
      void infill();

      string desc(bool colored = true) const override;

      void forward();
      void backward();

      Block *tail() const { return _tail;}
      Block *head() const { return _head;}

    private:

      Block *_tail = nullptr;
      Block *_head = nullptr;

      data_t n = 0;
      data_t dim = 4;

  };

}

#endif