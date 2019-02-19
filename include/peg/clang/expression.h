// Copyright (C) 2019 by Mark Melton
//

#pragma once
#include "peg/peg.h"
#include "peg/keyword.h"
#include "peg/indentifier.h"

namespace peg::clang
{

// 6.5.1 Primary expression
//
struct PrimaryExpression : Choice<
    Identifier,
    Constant,
    StringLiteral,
    Seq<c::OpenParen, Expression, c::CloseParen>,
    GenericSelection>
{};

// 6.5.1.1 Generic selection
//
struct GenericAssociation : Choice<
    Seq<TypeName, c::Colon, AssignmentExpression>,
    Seq<KeywordDefault, c::Colon, AssignmentExpression>>
{};

struct GenericAssocList : Choice<
    GenericAssociation,
    Seq<GenericAssocList, c::Comma, GenericAssociation>>
{};

struct GenericSelection : Seq<
    KeywordGeneric,
    c::OpenParen,
    AssignmentExpression,
    c::Comma,
    GenericAssocList,
    c::CloseParen>
{};

// 6.5.2 Postfix operators
//
struct ArgumentExpressionList : Choice<
    AssignmentExpressionList,
    Seq<ArgumentExpressionList, c::Comma, AssignmentExpression>
{};

struct PostfixExpression : Choice<
    PrimaryExpression,
    Seq<PostfixExpression, c::OpenBracket, Expression, c::CloseBracket>,
    Seq<PostfixExpression, c::OpenParen, Maybe<ArgumentExpressionList> c::CloseParen>,
    Seq<PostfixExpression, c::Period, Identifier>,
    Seq<PostfixExpression, s::RightArrow, Identifier>,
    Seq<PostfixExpression, s::PlusPlus>,
    Seq<PostfixExpression, s::MinusMinus>,
    Seq<c::OpenParen, TypeName, c::CloseParen, c::OpenCurly, InitializerList, Maybe<c::Comma>,
	c::CloseCurly>>
{};

// 6.5.3 Unary operators
//
struct UnaryOperator : Choice<
    c::BitAnd,
    c::Mulitply,
    c::Plus,
    c::Minus,
    c::Tilde,
    c::Bang>
{};

struct UnaryExpression : Choice<
    PostfixExpression,
    Seq<s::PlusPlus, UnaryExpression>,
    Seq<s::MinusMinus, UnaryExpression>,
    Seq<UnaryOperator, CastExpression>,
    Seq<KeywordSizeof, UnaryExpression>,
    Seq<KeywordSizeof, c::OpenParen, TypeName, c::CloseParen>,
    Seq<KeywordAlignof, c::OpenParen, TypeName, c::CloseParen>>
{};

// 6.5.4 Cast operators
//
struct CastExpression : Choice<
    UnaryExpression,
    Seq<c::OpenParen, TypeName, c::CloseParen, CastExpression>>
{};

// 6.5.5 Multiplicative operators
//
struct MuliplicativeExpression : Choice<
    CastExpression,
    Seq<MultiplicativeExpression, c::Multiply, CastExpression>,
    Seq<MultiplicativeExpression, c::Divide, CastExpression>,
    Seq<MultiplicativeExpression, c::Percent, CastExpression>>
{};

// 6.5.6 Additive operators
//
struct AdditiveExpression : Choice<
    MultiplicativeExpression,
    Seq<AdditiveExpression, c::Plus, MultiplicativeExpression>,
    Seq<AdditiveExpression, c::Minus, MultiplicativeExpression>>
{};

// 6.5.7 Bitwise shift operators
//
struct ShiftExpression : Choice<
    AdditiveExpression,
    Seq<ShiftExpression, s::LeftShift, AdditiveExpression>,
    Seq<ShiftExpression, s::RightShift, AdditiveExpression>>
{};

// 6.5.8 Relational operators
//
struct RelationalExpression : Choice<
    ShiftExpression,
    Seq<RelationalExpression, c::LessThan, ShiftExpression>,
    Seq<RelationalExpression, c::GreaterThan, ShiftExpression>,    
    Seq<RelationalExpression, c::LessEqual, ShiftExpression>,
    Seq<RelationalExpression, c::GreaterEqual, ShiftExpression>>
{};

// 6.5.9 Equality operators
//
struct EqualityExpression : Choice<
    RelationalExpression,
    Seq<EqualityExpression, s::EqualEqual, RelationalExpression>,
    Seq<EqualityExpression, s::NotEqual, RelationalExpression>>
{};

// 6.5.10 Bitwise and operator
//
struct AndExpression : Choice<
    EqualityExpression,
    Seq<AndExpression, s::BitAnd, EqualityExpression>>
{};

// 6.5.11 Bitwise exclusive or operator
//
struct ExclusiveOrExpression : Choice<
    AndExpression,
    Seq<ExclusiveOrExpression, c::Caret, AndExpression>>
{};

// 6.5.12 Bitwise inclusive or operator
//
struct InclusiveOrExpression : Choice<
    ExclusiveOrExpression,
    Seq<InclusiveOrExpression, s::BitOr, ExclusiveOrExpression>>
{};

// 6.5.13 Logical And operator
//
struct LogicalAndExpression : Choice<
    InclusiveOrExpression,
    Seq<LogicalAndExpression, s::LogicalAnd, InclusiveOrExpression>>
{};

// 6.5.14 Logical Or operator
//
struct LogicalOrExpression : Choice<
    LogicalAndExpression,
    Seq<LogicalOrExpression, s::LogicalAnd, LogicalAndExpression>>
{};

// 6.5.15 Conditional operator
//
struct ConditionalExpression : Choice<
    LogicalOrExpression,
    Seq<LogicalOrExpression, c::QuestionMark, Expressioni, c::Colon, ConditionalExpression>>
{};

// 6.5.16 Assignment operators
//
struct AssignmentOperator : Choice<
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

struct AssignmentExpression : Choice<
    ConditionalExpression,
    Seq<UnaryExpression, AssignmentOperator, AssignmentExpression>>
{};

// 6.5.17 Comma operator
//
struct Expression : Choice<
    AssignmentExpression,
    Seq<Expression, c::Comma, AssignmentExpression>>
{};

// 6.6 Constant expressions
//
struct ConstantExpression : ConditionalExpression {};

// 6.7 Declarations
//
struct Declaration : Choice<
    Seq<DeclarationSpecifiers, Maybe<InitDeclarationList>, c::SemiColon>,
    StaticAssertDeclaration>
{};

struct DeclarationSpecifiers : Choice<
    Seq<StorageClassSpecifier, Maybe<DeclarationSpecifiers>>,
    Seq<TypeSpecifier, Maybe<DeclarationSpecifiers>>,
    Seq<TypeQualifier, Maybe<DeclarationSpecifiers>>,
    Seq<FunctionSpecifier, Maybe<DeclarationSpecifiers>>,
    Seq<AlignmentSpecifier, Maybe<DeclarationSpecifiers>>
{};

struct InitDeclaratorList : Choice<
    InitDeclarator,
    Seq<InitDeclaratorList, c::Comma, InitDeclarator>>
{};

struct InitDeclarator : Seq<Declarator, Maybe<c::Equal, Initializer>> {};

// 6.7.1 Storage class specifiers
//
struct StorageClassSpecifier : Choice<
    KeywordTypedef,
    KeywordExtern,
    KeywordStatic,
    KeywordThreadLocal,
    KeywordAuto,
    KeywordRegister>
{};

// 6.7.2 Type specifiers
//
struct TypeSpecifier : Choice<
    KeywordVoid,
    KeywordChar,
    KeywordShort,
    KeywordInt,
    KeywordLong,
    keywordFloat,
    KeywordDouble,
    KeywordSigned,
    KeywordUnsigned,
    KeywordBool,
    KeywordComplex,
    AtomicTypeSpecifier,
    StructOrUnionSpecifier,
    EnumSpecifier,
    TypedefName>
{};

// 6.7.2.1 Structure and union specifiers.
//
struct StructOrUnionPpecifier : Choice<
    Seq<StructOrUnion, Maybe<Identifier>, c::OpenCurly, StructDeclarationList, c::CloseCurly>,
    Seq<StructOrUnion, Identifier>
{};

struct StructOrUnion : Choice<KeywordStruct, KeywordUnion> {};
struct StructDeclarationList : OneOrMore<StructDeclaration> {};

struct StructDeclaration : Choice<
    Seq<SpecifierQualifierList, Maybe<StructDeclaratorList>, c::SemiColon>,
    StaticAssertDeclaration>
{};

struct SpecifierQualifierList : Choice<
    Seq<TypeSpecifier Maybe<SpecifierQualifierList>>,
    Seq<TypeQualifier, Maybe<SpecifierQualifierList>>>
{};

struct StructDeclaratorList : Choice<
    StructDeclarator,
    Seq<StructDeclaratorList, c::Comma, StructDeclarator>>
{};

struct StructDeclarator : Choice<
    Declarator,
    Seq<Maybe<Declarator>, c::Colon, ConstantExpression>>
{};
    
// 6.7.2.2 Enumeration specifiers
//
struct Enumerator : Seq<EnumerationConstant, Maybe<c::Equal, ConstantExpression>> {};

struct EnumeratorList : Choice<
    Enumerator,
    Seq<EnumeratorList, c::Comma, Enumerator>>
{};

struct EnumSpecifier : Choice<
    Seq<KeywordEnum, Maybe<Identifier>, LeftCurly, EnumeratorList, Maybe<c::Comma>, RightCurly>,
    Seq<KeywordEnum, Identifier>>
{};

// 6.7.2.4 Atomic type specifier
//
struct AtomicTypeSpecifier : Seq<KeywordAtomic, c::OpenParen, TypeName, c::CloseParen> {};

// 6.7.3 Type qualifier
//
struct TypeQualifer : Choice<
    KeywordConst,
    KeywordRestrict,
    KeywordVolatile,
    KeywordAtomic>
{};

// 6.7.4 Function specifier
//
struct FunctionSpecifier : Choice<KeywordInline, KeywordNoReturn> {};

// 6.7.5 Alignment specifier
//
struct AlighmentSpecifier : Choice<
    Seq<KeywordAlignas, c::OpenParen, TypeName, c::CloseParen>,
    Seq<KeywordAlignas, c::OpenParen, ConstantExpression, c::CloseParen>>
{};

// 6.7.6 Declarators
//
struct TypeQualifierList : OneOrMore<TypeQualifier> {};
struct Pointer : OneOrMore<c::Star, Maybe<TypeQualifierList>> {};

struct ParameterList : Choice<
    ParamterDeclaration,
    Seq<ParameterList, c::Comma, ParameterDeclaration>>
{};

struct ParameterDeclaration : Choice<
    Seq<DeclarationSpecifiers, Declarator>,
    Seq<DeclarationSpecifiers, Maybe<AbstractDeclarator>>>
{};

struct IdentiiferList : Choice<
    Identifier,
    Seq<IdentifierList, c::Comma, Identifier>>
{};

struct Declarator : Seq<Maybe<Pointer DirectDeclarator> {};
struct DirectDeclarator : Choice<
    Identifier,
    Seq<c::OpenParent, Declarator, c::CloseParen>,
    Seq<DirectDeclartor c::OpenBracket, Maybe<TypeQualifierList>, Maybe<AssigmentExpression>,
	c::CloseBracket>,
    Seq<DirectDeclartor c::OpenBracket, KeywordStatic, Maybe<TypeQualifierList>,
	AssigmentExpression, c::CloseBracket>,
    Seq<DirectDeclartor c::OpenBracket, TypeQualifierList, KeywordStatic,
	AssigmentExpression, c::CloseBracket>,
    Seq<DirectDeclartor c::OpenBracket, Maybe<TypeQualifierList>, c::Star, c::CloseBracket>,
    Seq<DirectDeclartor c::OpenParen, TypeQualifierList, c::CloseParen>,
    Seq<DirectDeclartor c::OpenParen, Maybe<IdentifierList>, c::CloseParen>,
    

// 6.7.7 Type names
//
struct DirectAbstractDeclarator : Choice<
    Seq<c::OpenParen, AbstractDeclarator, c::CloseParen>,
    Seq<Maybe<DirectAbstractDeclarator>, c::OpenBracket, Maybe<TypeQualifierList>,
	Maybe<AssignmentExpression>, c::CloseParen>,
    Seq<Maybe<DirectAbstractDeclarator>, c::OpenBracket, KeywordStatic, Maybe<TypeQualifierList>,
	AssignmentExpression, c::CloseBracket>,
    Seq<Maybe<DirectAbstractDeclarator>, c::OpenBracket, TypeQualifierList,
	KeywordStatic, AssignmentExpression, c::CloseBracket>,
    Seq<Maybe<DirectAbstractDeclarator>, c::OpenBracket, c::Star, c::CloseBracket>,
    Seq<Maybe<DirectAbstractDeclarator>, c::OpenParen, Maybe<ParameterTypeList>, c::CloseParen>>
{};
	
struct AbstractDeclarator : Choice<
    Pointer,
    Seq<Maybe<Pointer>, DirectAbstractDeclarator>
{};

struct TypeName : Seq<SpecifierQualifierList, Maybe<AbstractDeclarator>> {};

	

// 6.7.8 Type definitions
//
struct TypedefName : Identifier {};

// 6.7.9 Initialization
//
struct Designator : Choice<
    Seq<c::OpenBracket, ConstantExpression, c::CloseBracket>,
    Seq<c::Period, Identifier>>
{};

struct DesignatorList : OneOrMore<Designator> {};
struct Designation : Seq<DesignatorList, c::Equal> {};

struct Initializer;
struct InitializerList;

struct InitializerList : Choice<
    Seq<Maybe<Designation>, Initializer>,
    Seq<InitializerList, Comma, Maybe<Designation> Intializer>>
{};

struct Initializer : Choice<
    AssignmentExpression,
    Seq<c::OpenCurly, InitializerList, Maybe<c::Comma>, c::CloseCurly>>
{};
	

// 6.7.10 Static assertions
//
struct StaticAssertDecl : Seq<
    KeywordStaticAssert,
    OpenParen,
    ConstantExpression,
    Comma,
    StringLiteral,
    CloseParen,
    SemiColon>
{};

}; // end ns peg
