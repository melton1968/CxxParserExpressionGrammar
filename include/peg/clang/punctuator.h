// Copyright (C) 2019 by Mark Melton
//

#pragma once
#include "peg/peg.h"

namespace peg
{

// 6.4.6 Punctuators
//
struct Punctuators : Or<
    c::OpenBracket,
    c::CloseBracket,
    c::OpenParen,
    c::CloseParen,
    c::OpenCurly,
    c::CloseCurly,
    c::Period,
    s::RightArrow,
    s::PlusPlus,
    s::MinusMinus,
    c::BitAnd,
    c::Multiply,
    c::Plus,
    c::Minus,
    c::Tilde,
    c::Bang,
    c::ForwardSlash,
    c::Percent,
    s::ShiftLeft,
    s::ShiftRight,
    c::LessThan,
    c::GreaterThan,
    s::LessEqual,
    s::GreaterEqual,
    s::EqualEqual,
    s::NotEqual,
    c::Caret,
    c::BitOr,
    s::LogicalAnd,
    s::LogicalOr,
    c::QuestionMark,
    c::Colon,
    c::SemiColon,
    s::Elipsis,
    c::Equal,
    s::MultiplyEqual,
    s::DivideEqual,
    s::PlusEqual,
    s::MinusEqual,
    s::LeftShiftEqual,
    s::RightShiftEqual,
    s::BitAndEqual,
    s::BitOrEqual,
    c::Comma,
    c::Hash,
    s::DoubleHash,
    s::OpenBracketSpecial,
    s::CloseBracketSpecial,
    s::OpenCurlySpecial,
    s::CloseCurlySpecial,
    s::HashSpecial,
    s::DoubleHashSpecial>
{};


}; // end ns peg
