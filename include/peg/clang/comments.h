// Copyright (C) 2019 by Mark Melton
//

#pragma once
#include "peg/peg.h"

namespace peg::clang
{

// 6.4.9 Comments
//
struct LineComment : Seq<String<'/','/'>, Until<c::Newline, AnyCharacter, false>> {};
struct PairedComment : Seq<String<'/','*'>, Until<String<'*','/'>, AnyCharacter>> {};
struct Comment : Or<LineComment, PairedComment> {};

}; // end ns peg
