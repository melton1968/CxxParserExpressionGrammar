// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "peg/peg.h"

// N1570
//
namespace peg::clang
{

// 6.4.1 Keywords
//
struct Keyword : Or<
    String<'a', 'u', 't', 'o'>,
    String<'b', 'r', 'e', 'a', 'k'>,
    String<'c', 'a', 's', 'e'>,
    String<'c', 'h', 'a', 'r'>,
    String<'c', 'o', 'n', 's', 't'>,
    String<'c', 'o', 'n', 't', 'i', 'n', 'u', 'e'>,
    String<'d', 'e', 'f', 'a', 'u', 'l', 't'>,
    String<'d', 'o'>,
    String<'d', 'o', 'u', 'b', 'l', 'e'>,
    String<'e', 'l', 's', 'e'>,
    String<'e', 'n', 'u', 'm'>,
    String<'e', 'x', 't', 'e', 'r', 'n'>,
    String<'f', 'l', 'o', 'a', 't'>,
    String<'f', 'o', 'r'>,
    String<'g', 'o', 't', 'o'>,
    String<'i', 'f'>,
    String<'i', 'n', 'l', 'i', 'n', 'e'>,
    String<'i', 'n', 't'>,
    String<'l', 'o', 'n', 'g'>,
    String<'r', 'e', 'g', 'i', 's', 't', 'e', 'r'>,
    String<'r', 'e', 's', 't', 'r', 'i', 'c', 't'>,
    String<'r', 'e', 't', 'u', 'r', 'n'>,
    String<'s', 'h', 'o', 'r', 't'>,
    String<'s', 'i', 'g', 'n', 'e', 'd'>,
    String<'s', 'i', 'z', 'e', 'o', 'f'>,
    String<'s', 't', 'a', 't', 'i', 'c'>,
    String<'s', 't', 'r', 'u', 'c', 't'>,
    String<'s', 'w', 'i', 't', 'c', 'h'>,
    String<'t', 't', 'p', 'e', 'd', 'e', 'f'>,
    String<'u', 'n', 'i', 'o', 'n'>,
    String<'u', 'n', 's', 'i', 'g', 'n', 'e', 'd'>,
    String<'v', 'o', 'i', 'd'>,
    String<'v', 'o', 'l', 'a', 't', 'i', 'l', 'e'>,
    String<'w', 'h', 'i', 'l', 'e'>,
    String<'_', 'A', 'l', 'i', 'g', 'n', 'a', 's'>,
    String<'_', 'A', 'l', 'i', 'g', 'n', 'o', 'f'>,
    String<'_', 'A', 't', 'o', 'm', 'i', 'c'>,
    String<'_', 'B', 'o', 'o', 'l'>,
    String<'_', 'C', 'o', 'm', 'p', 'l', 'e', 'x'>,
    String<'_', 'G', 'e', 'n', 'e', 'r', 'i', 'c'>,
    String<'_', 'I', 'm', 'a', 'g', 'i', 'n', 'a', 'r', 'y'>,
    String<'_', 'N', 'o', 'r', 'e', 't', 'u', 'r', 'n'>,
    String<'_', 'S', 't', 'a', 't', 'i', 'c', '_', 'a', 's', 's', 'e', 'r', 't'>,
    String<'_', 'T', 'h', 'r', 'e', 'a', 'd', '_', 'l', 'o', 'c', 'a', 'l'>>
{};

}; // end peg::clang
