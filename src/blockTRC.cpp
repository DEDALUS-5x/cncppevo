/*
  ____  _            _    _____ ____   ____        _               
 | __ )| | ___   ___| | _|_   _|  _ \ / ___|   ___| | __ _ ___ ___ 
 |  _ \| |/ _ \ / __| |/ / | | | |_) | |      / __| |/ _` / __/ __|
 | |_) | | (_) | (__|   <  | | |  _ <| |___  | (__| | (_| \__ \__ \
 |____/|_|\___/ \___|_|\_\ |_| |_| \_\\____|  \___|_|\__,_|___/___/

IDEA:

Program class should be a list of *Block, so it can contain also pointers to BlockTRC instances. Is the effort worth?

program.cpp -> when adding a new BlockTRC through << operator, parse function of BlockTRC is called. During parsing, check the angle between the destination and the starting point. If angle > pi, then rise a flag (_shaping_required). Then after parsing and adding the block, so after << operator in Program, let's check the flag of the added block. If high, let's rise a flag in Program. This last flag then msut be checked and if high, add the arc "junction" block (this new block should be computed in Program class? TBD)
                                                                   
*/

#include <fmt/color.h>
#include <fmt/format.h>
#include <rang.hpp>
#include <sstream>
#include "blockTRC.hpp"

using namespace std;
using namespace cncpp;
using namespace fmt;
using namespace rang;

const map<BlockTRC::TRCType, string> BlockTRC::trc_types = {

  {TRCType::NONE, "None"},
  {TRCType::LEFT, "Left"},
  {TRCType::RIGHT, "Right"}
};

BlockTRC::BlockTRC(string line) : Block(line) , _trc(false) {}

BlockTRC::BlockTRC(string line, BlockTRC &prev) : Block(line, prev) {}

BlockTRC::~BlockTRC(){}

BlockTRC &BlockTRC::operator=(BlockTRC &b){

  Block::operator=(b);
  _trc = b.trc();
}



