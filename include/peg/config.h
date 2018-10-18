// Copyright (C) 2018 by Mark Melton
//

#pragma once

#include <cstdint>
#include <iostream>
#include <string>

namespace peg
{

using integer8 = std::int8_t;
using integer16 = std::int16_t;
using integer32 = std::int32_t;
using integer64 = std::int64_t;
using integer = integer32;

using natural8 = std::uint8_t;
using natural16 = std::uint16_t;
using natural32 = std::uint32_t;
using natural64 = std::uint64_t;
using natural = natural32;

using std::cerr;
using std::cout;
using std::endl;
using namespace std::literals;

};

