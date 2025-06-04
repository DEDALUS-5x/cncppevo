/*

  ____  _            _      _____ ____   ____        _               
 | __ )| | ___   ___| | __ |_   _|  _ \ / ___|   ___| | __ _ ___ ___ 
 |  _ \| |/ _ \ / __| |/ /   | | | |_) | |      / __| |/ _` / __/ __|
 | |_) | | (_) | (__|   <    | | |  _ <| |___  | (__| | (_| \__ \__ \
 |____/|_|\___/ \___|_|\_\   |_| |_| \_\\____|  \___|_|\__,_|___/___/

  It represents an extension of the block class. It implements the Tool Radius Compensation feature.

 */

#ifndef BLOCK_TRC_HPP
#define BLOCK_TRC_HPP

#include "defines.hpp"
#include "block.hpp"

using namespace std;

namespace cncpp{

  class BlockTRC : Block{

    public:

      enum class TRCType{
        NONE = 0,
        LEFT = 1,
        RIGHT = 2
      };

      static const map<TRCType, string> trc_types;

      /*
        _     _  __                      _      
       | |   (_)/ _| ___  ___ _   _  ___| | ___ 
       | |   | | |_ / _ \/ __| | | |/ __| |/ _ \
       | |___| |  _|  __/ (__| |_| | (__| |  __/
       |_____|_|_|  \___|\___|\__, |\___|_|\___|
                              |___/             
      */
      BlockTRC(string line);
      BlockTRC(string line, BlockTRC &prev);
      ~BlockTRC();

      /*
           _                                        
          / \   ___ ___ ___  ___ ___  ___  _ __ ___ 
         / _ \ / __/ __/ _ \/ __/ __|/ _ \| '__/ __|
        / ___ \ (_| (_|  __/\__ \__ \ (_) | |  \__ \
       /_/   \_\___\___\___||___/___/\___/|_|  |___/
                                                    
      */
      
      bool trc() const { return _trc; }

      BlockTRC &operator=(BlockTRC &b);

    private:

      bool _trc = false;
      bool _shaping_required = false;


  };
}


#endif
