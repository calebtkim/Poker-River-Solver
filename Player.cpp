#pragma once
#include "Player.h"

using std::vector;

Player::Player(vector<Hand>& range, int_fast32_t chip_count, vector<float>& bet_sizes, vector<float> raise_sizes) {
  this->range = range;
  this->chip_count = chip_count;
  this->bet_sizes = bet_sizes;
  this->raise_sizes = raise_sizes;
}