// Copyright (C) 2019 by Mark Melton
//

#pragma once
#include "peg/peg.h"
#include "peg/string.h"
#include "peg/keyword.h"
#include "peg/clang/identifier.h"

// N1570
//
namespace peg::clang
{

#define PEG_CLANG_KEYWORD(S)					\
    namespace str {						\
    PEG_STR(S);							\
    };								\
    namespace k {						\
    using _ ## S = Keyword<str::_ ## S, IdentifierDigit>;	\
    };								\
    

PEG_CLANG_KEYWORD(auto);
PEG_CLANG_KEYWORD(break);
PEG_CLANG_KEYWORD(case);
PEG_CLANG_KEYWORD(char);
PEG_CLANG_KEYWORD(const);
PEG_CLANG_KEYWORD(continue);
PEG_CLANG_KEYWORD(default);
PEG_CLANG_KEYWORD(do);
PEG_CLANG_KEYWORD(double);
PEG_CLANG_KEYWORD(else);
PEG_CLANG_KEYWORD(enum);
PEG_CLANG_KEYWORD(extern);
PEG_CLANG_KEYWORD(float);
PEG_CLANG_KEYWORD(for);
PEG_CLANG_KEYWORD(goto);
PEG_CLANG_KEYWORD(if);
PEG_CLANG_KEYWORD(inline);
PEG_CLANG_KEYWORD(int);
PEG_CLANG_KEYWORD(long);
PEG_CLANG_KEYWORD(register);
PEG_CLANG_KEYWORD(restrict);
PEG_CLANG_KEYWORD(return);
PEG_CLANG_KEYWORD(short);
PEG_CLANG_KEYWORD(signed);
PEG_CLANG_KEYWORD(sizeof);
PEG_CLANG_KEYWORD(static);
PEG_CLANG_KEYWORD(struct);
PEG_CLANG_KEYWORD(switch);
PEG_CLANG_KEYWORD(typedef);
PEG_CLANG_KEYWORD(union);
PEG_CLANG_KEYWORD(unsigned);
PEG_CLANG_KEYWORD(void);
PEG_CLANG_KEYWORD(volatile);
PEG_CLANG_KEYWORD(while);
PEG_CLANG_KEYWORD(_Alignas);
PEG_CLANG_KEYWORD(_Alignof);
PEG_CLANG_KEYWORD(_Atomic);
PEG_CLANG_KEYWORD(_Bool);
PEG_CLANG_KEYWORD(_Complex);
PEG_CLANG_KEYWORD(_Generic);
PEG_CLANG_KEYWORD(_Imaginary);
PEG_CLANG_KEYWORD(_Noreturn);
PEG_CLANG_KEYWORD(_Static_assert);
PEG_CLANG_KEYWORD(_Thread_local);

// 6.4.1 Keywords
//
struct Keyword : Or<
    k::_auto,
    k::_break,
    k::_case,
    k::_char,
    k::_const,
    k::_continue,
    k::_default,
    k::_double,
    k::_do,
    k::_else,
    k::_enum,
    k::_extern,
    k::_float,
    k::_for,
    k::_goto,
    k::_if,
    k::_inline,
    k::_int,
    k::_long,
    k::_register,
    k::_restrict,
    k::_return,
    k::_short,
    k::_signed,
    k::_sizeof,
    k::_static,
    k::_struct,
    k::_switch,
    k::_typedef,
    k::_union,
    k::_unsigned,
    k::_void,
    k::_volatile,
    k::_while,
    k::__Alignas,
    k::__Alignof,
    k::__Atomic,
    k::__Bool,
    k::__Complex,
    k::__Generic,
    k::__Imaginary,
    k::__Noreturn,
    k::__Static_assert,
    k::__Thread_local>
{};

}; // end peg::clang
