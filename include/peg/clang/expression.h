// Copyright (C) 2019 by Mark Melton
//

#pragma once
#include "peg/peg.h"
#include "peg/clang/constant.h"
#include "peg/clang/keyword.h"
#include "peg/clang/identifier.h"
#include "peg/clang/string.h"

namespace peg::clang
{

// 6.5.1 Primary expression
//
struct Expression;
struct GenericSelection;

struct PrimaryExpression : Or<
    Identifier,
    Constant,
    StringLiteral,
    Seq<c::OpenParen, Expression, c::CloseParen>,
    GenericSelection>
{};

// 6.5.1.1 Generic selection
//
struct TypeName;
struct AssignmentExpression;

struct GenericAssociation : Or<
    Seq<TypeName, c::Colon, AssignmentExpression>,
    Seq<k::_default, c::Colon, AssignmentExpression>>
{};

struct GenericAssocList : Or<
    GenericAssociation,
    Seq<GenericAssocList, c::Comma, GenericAssociation>>
{};

struct GenericSelection : Seq<
    k::__Generic,
    c::OpenParen,
    AssignmentExpression,
    c::Comma,
    GenericAssocList,
    c::CloseParen>
{};

// 6.5.2 Postfix operators
//
struct InitializerList;

struct ArgumentExpressionList : Or<
    AssignmentExpression,
    Seq<ArgumentExpressionList, c::Comma, AssignmentExpression>>
{};

struct PostfixExpression : Or<
    PrimaryExpression,
    Seq<PostfixExpression, c::OpenBracket, Expression, c::CloseBracket>,
    Seq<PostfixExpression, c::OpenParen, Maybe<ArgumentExpressionList>, c::CloseParen>,
    Seq<PostfixExpression, c::Period, Identifier>,
    Seq<PostfixExpression, s::RightArrow, Identifier>,
    Seq<PostfixExpression, s::PlusPlus>,
    Seq<PostfixExpression, s::MinusMinus>,
    Seq<c::OpenParen, TypeName, c::CloseParen, c::OpenCurly, InitializerList, Maybe<c::Comma>,
	c::CloseCurly>>
{};

// 6.5.3 Unary operators
//
struct CastExpression;

struct UnaryOperator : Or<
    c::BitAnd,
    c::Multiply,
    c::Plus,
    c::Minus,
    c::Tilde,
    c::Bang>
{};

struct UnaryExpression : Or<
    PostfixExpression,
    Seq<s::PlusPlus, UnaryExpression>,
    Seq<s::MinusMinus, UnaryExpression>,
    Seq<UnaryOperator, CastExpression>,
    Seq<k::_sizeof, UnaryExpression>,
    Seq<k::_sizeof, c::OpenParen, TypeName, c::CloseParen>,
    Seq<k::__Alignof, c::OpenParen, TypeName, c::CloseParen>>
{};

// 6.5.4 Cast operators
//
struct CastExpression : Or<
    UnaryExpression,
    Seq<c::OpenParen, TypeName, c::CloseParen, CastExpression>>
{};

// 6.5.5 Multiplicative operators
//
struct MultiplicativeExpression : Or<
    CastExpression,
    Seq<MultiplicativeExpression, c::Multiply, CastExpression>,
    Seq<MultiplicativeExpression, c::Divide, CastExpression>,
    Seq<MultiplicativeExpression, c::Percent, CastExpression>>
{};

// 6.5.6 Additive operators
//
struct AdditiveExpression : Or<
    MultiplicativeExpression,
    Seq<AdditiveExpression, c::Plus, MultiplicativeExpression>,
    Seq<AdditiveExpression, c::Minus, MultiplicativeExpression>>
{};

// 6.5.7 Bitwise shift operators
//
struct ShiftExpression : Or<
    AdditiveExpression,
    Seq<ShiftExpression, s::LeftShift, AdditiveExpression>,
    Seq<ShiftExpression, s::RightShift, AdditiveExpression>>
{};

// 6.5.8 Relational operators
//
struct RelationalExpression : Or<
    ShiftExpression,
    Seq<RelationalExpression, c::LessThan, ShiftExpression>,
    Seq<RelationalExpression, c::GreaterThan, ShiftExpression>,    
    Seq<RelationalExpression, s::LessEqual, ShiftExpression>,
    Seq<RelationalExpression, s::GreaterEqual, ShiftExpression>>
{};

// 6.5.9 Equality operators
//
struct EqualityExpression : Or<
    RelationalExpression,
    Seq<EqualityExpression, s::EqualEqual, RelationalExpression>,
    Seq<EqualityExpression, s::NotEqual, RelationalExpression>>
{};

// 6.5.10 Bitwise and operator
//
struct AndExpression : Or<
    EqualityExpression,
    Seq<AndExpression, c::BitAnd, EqualityExpression>>
{};

// 6.5.11 Bitwise exclusive or operator
//
struct ExclusiveOrExpression : Or<
    AndExpression,
    Seq<ExclusiveOrExpression, c::Caret, AndExpression>>
{};

// 6.5.12 Bitwise inclusive or operator
//
struct InclusiveOrExpression : Or<
    ExclusiveOrExpression,
    Seq<InclusiveOrExpression, c::BitOr, ExclusiveOrExpression>>
{};

// 6.5.13 Logical And operator
//
struct LogicalAndExpression : Or<
    InclusiveOrExpression,
    Seq<LogicalAndExpression, s::LogicalAnd, InclusiveOrExpression>>
{};

// 6.5.14 Logical Or operator
//
struct LogicalOrExpression : Or<
    LogicalAndExpression,
    Seq<LogicalOrExpression, s::LogicalAnd, LogicalAndExpression>>
{};

// 6.5.15 Conditional operator
//
struct ConditionalExpression : Or<
    LogicalOrExpression,
    Seq<LogicalOrExpression, c::QuestionMark, Expression, c::Colon, ConditionalExpression>>
{};

// 6.5.16 Assignment operators
//
struct AssignmentOperator : Or<
    c::Equal,
    s::MultiplyEqual,
    s::DivideEqual,
    s::PercentEqual,
    s::PlusEqual,
    s::MinusEqual,
    s::LeftShiftEqual,
    s::RightShiftEqual,
    s::BitAndEqual,
    s::BitOrEqual>
{};

struct AssignmentExpression : Or<
    ConditionalExpression,
    Seq<UnaryExpression, AssignmentOperator, AssignmentExpression>>
{};

// 6.5.17 Comma operator
//
struct Expression : Or<
    AssignmentExpression,
    Seq<Expression, c::Comma, AssignmentExpression>>
{};

// 6.6 Constant expressions
//
struct ConstantExpression : ConditionalExpression {};

// 6.7.1 Storage class specifiers
//
struct StorageClassSpecifier : Or<
    k::_typedef,
    k::_extern,
    k::_static,
    k::__Thread_local,
    k::_auto,
    k::_register>
{};

// 6.7.2.1 Structure and union specifiers.
//
struct Declarator;
struct TypeSpecifier;
struct TypeQualifier;
struct StaticAssertDeclaration;

struct StructDeclarator : Or<
    Declarator,
    Seq<Maybe<Declarator>, c::Colon, ConstantExpression>>
{};
    
struct StructDeclaratorList : Or<
    StructDeclarator,
    Seq<StructDeclaratorList, c::Comma, StructDeclarator>>
{};

struct SpecifierQualifierList : Or<
    Seq<TypeSpecifier, Maybe<SpecifierQualifierList>>,
    Seq<TypeQualifier, Maybe<SpecifierQualifierList>>>
{};

struct StructDeclaration : Or<
    Seq<SpecifierQualifierList, Maybe<StructDeclaratorList>, c::SemiColon>,
    StaticAssertDeclaration>
{};

struct StructOrUnion : Or<k::_struct, k::_union> {};
struct StructDeclarationList : OneOrMore<StructDeclaration> {};

struct StructOrUnionSpecifier : Or<
    Seq<StructOrUnion, Maybe<Identifier>, c::OpenCurly, StructDeclarationList, c::CloseCurly>,
    Seq<StructOrUnion, Identifier>>
{};

// 6.7.2.2 Enumeration specifiers
//
struct Enumerator : Seq<EnumerationConstant, Maybe<c::Equal, ConstantExpression>> {};

struct EnumeratorList : Or<
    Enumerator,
    Seq<EnumeratorList, c::Comma, Enumerator>>
{};

struct EnumSpecifier : Or<
    Seq<k::_enum, Maybe<Identifier>, c::OpenCurly, EnumeratorList, Maybe<c::Comma>,
	c::CloseCurly>,
    Seq<k::_enum, Identifier>>
{};

// 6.7.2.4 Atomic type specifier
//
struct AtomicTypeSpecifier : Seq<k::__Atomic, c::OpenParen, TypeName, c::CloseParen> {};

// 6.7.2 Type specifiers
//
struct TypedefName;

struct TypeSpecifier : Or<
    k::_void,
    k::_char,
    k::_short,
    k::_int,
    k::_long,
    k::_float,
    k::_double,
    k::_signed,
    k::_unsigned,
    k::__Bool,
    k::__Complex,
    AtomicTypeSpecifier,
    StructOrUnionSpecifier,
    EnumSpecifier,
    TypedefName>
{};

// 6.7.3 Type qualifier
//
struct TypeQualifier : Or<
    k::_const,
    k::_restrict,
    k::_volatile,
    k::__Atomic>
{};

// 6.7.4 Function specifier
//
struct FunctionSpecifier : Or<k::_inline, k::__Noreturn> {};

// 6.7.5 Alignment specifier
//
struct AlignmentSpecifier : Or<
    Seq<k::__Alignas, c::OpenParen, TypeName, c::CloseParen>,
    Seq<k::__Alignas, c::OpenParen, ConstantExpression, c::CloseParen>>
{};



// 6.7.6 Declarators
//
struct AbstractDeclarator;
struct DeclarationSpecifiers;
struct DirectDeclarator;
struct IdentifierList;
struct ParameterDeclaration;
struct Pointer;
struct TypeQualifierList;

struct Declarator : Seq<Maybe<Pointer>, DirectDeclarator> {};

struct DirectDeclarator : Or<
    Identifier,
    Seq<c::OpenParen, Declarator, c::CloseParen>,
    Seq<DirectDeclarator, c::OpenBracket, Maybe<TypeQualifierList>, Maybe<AssignmentExpression>,
	c::CloseBracket>,
    Seq<DirectDeclarator, c::OpenBracket, k::_static, Maybe<TypeQualifierList>,
	AssignmentExpression, c::CloseBracket>,
    Seq<DirectDeclarator, c::OpenBracket, TypeQualifierList, k::_static,
	AssignmentExpression, c::CloseBracket>,
    Seq<DirectDeclarator, c::OpenBracket, Maybe<TypeQualifierList>, c::Star, c::CloseBracket>,
    Seq<DirectDeclarator, c::OpenParen, TypeQualifierList, c::CloseParen>,
    Seq<DirectDeclarator, c::OpenParen, Maybe<IdentifierList>, c::CloseParen>>
{};

struct Pointer : OneOrMore<c::Star, Maybe<TypeQualifierList>> {};
struct TypeQualifierList : OneOrMore<TypeQualifier> {};

struct ParameterTypeList : Or<
    ParameterTypeList,
    Seq<ParameterTypeList, c::Comma, s::Elipsis>>
{};

struct ParameterList : Or<
    ParameterDeclaration,
    Seq<ParameterList, c::Comma, ParameterDeclaration>>
{};

struct ParameterDeclaration : Or<
    Seq<DeclarationSpecifiers, Declarator>,
    Seq<DeclarationSpecifiers, Maybe<AbstractDeclarator>>>
{};

struct IdentifierList : Or<
    Identifier,
    Seq<IdentifierList, c::Comma, Identifier>>
{};

// 6.7.7 Type names
//
struct DirectAbstractDeclarator : Or<
    Seq<c::OpenParen, AbstractDeclarator, c::CloseParen>,
    Seq<Maybe<DirectAbstractDeclarator>, c::OpenBracket, Maybe<TypeQualifierList>,
	Maybe<AssignmentExpression>, c::CloseParen>,
    Seq<Maybe<DirectAbstractDeclarator>, c::OpenBracket, k::_static, Maybe<TypeQualifierList>,
	AssignmentExpression, c::CloseBracket>,
    Seq<Maybe<DirectAbstractDeclarator>, c::OpenBracket, TypeQualifierList,
	k::_static, AssignmentExpression, c::CloseBracket>,
    Seq<Maybe<DirectAbstractDeclarator>, c::OpenBracket, c::Star, c::CloseBracket>,
    Seq<Maybe<DirectAbstractDeclarator>, c::OpenParen, Maybe<ParameterTypeList>, c::CloseParen>>
{};
	
struct AbstractDeclarator : Or<
    Pointer,
    Seq<Maybe<Pointer>, DirectAbstractDeclarator>>
{};

struct TypeName : Seq<SpecifierQualifierList, Maybe<AbstractDeclarator>> {};

// 6.7.8 Type definitions
//
struct TypedefName : Identifier {};

// 6.7.9 Initialization
//
struct Initializer;

struct Designator : Or<
    Seq<c::OpenBracket, ConstantExpression, c::CloseBracket>,
    Seq<c::Period, Identifier>>
{};

struct DesignatorList : OneOrMore<Designator> {};
struct Designation : Seq<DesignatorList, c::Equal> {};

struct Initializer;
struct InitializerList;

struct InitializerList : Or<
    Seq<Maybe<Designation>, Initializer>,
    Seq<InitializerList, c::Comma, Maybe<Designation>, Initializer>>
{};

struct Initializer : Or<
    AssignmentExpression,
    Seq<c::OpenCurly, InitializerList, Maybe<c::Comma>, c::CloseCurly>>
{};
	

// 6.7.10 Static assertions
//
struct StaticAssertDeclaration : Seq<
    k::__Static_assert,
    c::OpenParen,
    ConstantExpression,
    c::Comma,
    StringLiteral,
    c::CloseParen,
    c::SemiColon>
{};

// 6.7 Declarations
//
struct InitDeclarator : Seq<Declarator, Maybe<c::Equal, Initializer>> {};

struct InitDeclaratorList : Or<
    InitDeclarator,
    Seq<InitDeclaratorList, c::Comma, InitDeclarator>>
{};

struct DeclarationSpecifiers : Or<
    Seq<StorageClassSpecifier, Maybe<DeclarationSpecifiers>>,
    Seq<TypeSpecifier, Maybe<DeclarationSpecifiers>>,
    Seq<TypeQualifier, Maybe<DeclarationSpecifiers>>,
    Seq<FunctionSpecifier, Maybe<DeclarationSpecifiers>>,
    Seq<AlignmentSpecifier, Maybe<DeclarationSpecifiers>>>
{};

struct Declaration : Or<
    Seq<DeclarationSpecifiers, /*Maybe<InitDeclarationList>,*/ c::SemiColon>,
    StaticAssertDeclaration>
{};
	
}; // end ns peg::clang
