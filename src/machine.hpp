// running the program and load it into the machine. A program is the full list of blocks

/*
  __  __            _     _                   _               
 |  \/  | __ _  ___| |__ (_)_ __   ___    ___| | __ _ ___ ___ 
 | |\/| |/ _` |/ __| '_ \| | '_ \ / _ \  / __| |/ _` / __/ __|
 | |  | | (_| | (__| | | | | | | |  __/ | (__| | (_| \__ \__ \
 |_|  |_|\__,_|\___|_| |_|_|_| |_|\___|  \___|_|\__,_|___/___/
                                                              
*/

#ifndef MACHINE_HPP
#define MACHINE_HPP

#include "defines.hpp"
#include "point.hpp"

using namespace std;

namespace cncpp{

  class Machine /*: Object*/{

    public:

      data_t A() const { return _A;}
      Point zero() const { return _zero;}

      /**
       * 
       * @brief quantize time -> starting from t0 and having a delta t, it returns the number of sample to the next tick after the the delta t. In a nutshell, it's a rounding up to the next multiple of tq
       * @param t time duration of the trapezoid
       * @param dq refernce to the elapsed time (modified in the body)
       * 
       */
      data_t quantize(data_t t, data_t &dq) const;

      /*
      --- ACCESSORS ---
      */
      data_t tq() const { return _tq;}
      data_t error() const { return _error;}

    private:  

      data_t _A = 5.0;
      Point _zero = Point(0, 0, 0);
      data_t _tq = 0.005;                 // sampling time -> tick
      data_t _error = 0.0;

  };
}

#endif // MACHINE_HPP
