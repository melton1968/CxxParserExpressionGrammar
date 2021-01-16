// Copyright (C) 2019, 2021 by Mark Melton
//

#include <stack>
#include <sstream>
#include "core/tool.h"
#include "core/demangle.h"

struct Input
{
    Input(string_view str)
	: m_begin(str.begin())
	, m_end(str.begin() + str.size())
	, m_mark(m_begin)
	, m_point(m_begin)
    { }

    const char *begin() const { return m_begin; }
    const char *end() const { return m_end; }
    const char *mark() const { return m_mark; }
    const char *point() const { return m_point; }

    char peek() const { return *m_point; }
    bool eof() const { return m_point == m_end; }
    bool bof() const { return m_point == m_begin; }

    size_t match_size() const { return m_point - m_mark; }
    string_view match() const { return { m_mark, match_size() }; }

    void advance(index_t distance) { m_point += distance; }
    void reset_mark() { m_mark = m_point; }
    void set_mark(const char *mark) { m_mark = mark; }

private:
    const char *m_begin{nullptr};
    const char *m_end{nullptr};
    const char *m_mark{nullptr};
    const char *m_point{nullptr};
};

template<class Derived>
struct NodeMixin
{
    using Self = NodeMixin<Derived>;
    using Mixin = NodeMixin<Derived>;
    using Ptr = std::unique_ptr<Derived>;
    using Children = std::vector<Ptr>;

    NodeMixin() = default;
    virtual ~NodeMixin() = default;
    
    NodeMixin(string_view content)
	: m_content(content)
    { }

    NodeMixin(const Self&) = delete;
    NodeMixin(const Self&&) = delete;

    template<class U, class... Args>
    static Ptr make(Args&&... args)
    { return std::make_unique<U>(args...); }

    string_view content() const { return m_content; }
    void set_content(string_view content) { m_content = content; }

    const Ptr& child(size_t i) const { return m_children[i]; }
    Ptr move_child(size_t i) { return std::move(m_children[i]); }
    void emplace_child(Ptr child) { m_children.emplace_back(std::move(child)); }

    size_t number_children() const { return m_children.size(); }
    void discard_children() { return m_children.clear(); }
    const Children& children() const { return m_children; }
    
    void print(std::ostream& os, size_t level = 0) const
    {
	os << "    ";
	for (size_t i = 0; i < level; ++i)
	    os << "|   ";
	os << "'" << content() << "'";
	os << "    " << core::type_name(*this);
	os << endl;

	for (const auto& n : children())
	    n->print(os, level+1);
    }

    string str() const
    {
	std::stringstream ss;
	print(ss);
	return ss.str();
    }
    
private:
    string_view m_content;
    Children m_children;
};

template<class T>
struct ContextContainer
{
    using Ptr = typename T::Ptr;
    
    const Ptr& top() const { return m_output.back(); }
	
    template<class U, class... Args>
    void push(Args&&... args)
    {
	auto node = T::template make<U>(args...);
	m_output.push_back(std::move(node));
    }

    template<class U, class... Args>
    void push_child(Args&&... args)
    {
	ExpectGE(count(), 1);
	
	auto node = T::template make<U>(args...);
	m_output.back()->emplace_child(std::move(node));
    }

    template<class U, class... Args>
    void push_parent(Args&&... args)
    {
	ExpectGE(count(), 1);

	auto parent = T::template make<U>(args...);
	parent->emplace_child(std::move(m_output.back()));
	std::swap(parent, m_output.back());
    }

    void absorb_child()
    {
	ExpectGE(count(), 2);
	
	auto child = std::move(m_output.back());
	m_output.pop_back();
	m_output.back()->emplace_child(std::move(child));
    }
    
    void swap()
    {
	ExpectGE(count(), 2);
	std::swap(m_output[count() - 1], m_output[count() - 2]);
    }

    void pop()
    {
	ExpectGE(count(), 1);
	m_output.pop_back();
    }

    size_t count() const { return m_output.size(); }

    void print(std::ostream& os, size_t level = 0) const
    {
	for (const auto& n : m_output)
	    n->print(os);
    }

    string str() const
    {
	std::stringstream ss;
	print(ss);
	return ss.str();
    }
    
private:
    std::vector<typename T::Ptr> m_output;
};

struct ActionTag
{ static constexpr bool IsAction = true; };

template<class T>
concept ContextAction = T::IsAction == true;

template<class... Ts>
concept ContextActions = (ContextAction<Ts> and ...);

struct ParserTag
{ static constexpr bool IsParser = true; };

template<class T>
concept ParserCombinator = T::IsParser == true;

template<class... Ts>
concept ParserCombinators = (ParserCombinator<Ts> and ...);

template<class N>
struct Push : ActionTag
{
    template<class Context>
    static void apply(const Input& input, Context& context)
    { context.template push<N>(input.match()); }
};

template<class N>
struct PushChild : ActionTag
{
    template<class Context>
    static void apply(const Input& input, Context& context)
    { context.template push_child<N>(input.match()); }
};

template<class N>
struct PushParent : ActionTag
{
    template<class Context>
    static void apply(const Input& input, Context& context)
    { context.template push_parent<N>(input.match()); }
};

struct AbsorbChild : ActionTag
{
    template<class Context>
    static void apply(const Input& input, Context& context)
    { context.absorb_child(); }
};

struct Pop : ActionTag
{
    template<class Context>
    static void apply(const Input& input, Context& context)
    { context.pop(); }
};

template<class N>
struct Swap : ActionTag
{
    template<class Context>
    static void apply(Input& input, Context& context)
    { context.swap(); }
};

struct Mark : ParserTag
{
    template<class Control, class Context>
    static bool match(Input& input, Context&)
    {
	input.reset_mark();
	return true;
    }
};

struct End : ParserTag
{
    template<class Control, class Context>
    static bool match(Input& input, Context&)
    { return input.eof(); }
};

template<char C>
struct Char : ParserTag
{
    template<class Control, class Context>
    static bool match(Input& input, Context&)
    {
	bool r = input.peek() == C;
	if (r) input.advance(1);
	return r;
    }
};

struct Digits : ParserTag
{
    template<class Control, class Context>
    static bool match(Input& input, Context& context)
    {
	auto m = input.point();
	auto p = m;
	while (p != input.end() and *p >= '0' and *p <= '9')
	{
	    input.advance(1);
	    ++p;
	}
	return p > m;
    }
};

template<class Parser, ContextActions... Actions>
struct Link : ParserTag
{
    template<class Control, class Context>
    static bool match(Input& input, Context& context)
    {
	if (not Parser::template match<Control>(input, context))
	    return false;
	(Actions::apply(input, context), ...);
	return true;
    }
};


template<char C, ContextActions... Actions>
struct LinkC : Link<Char<C>, Actions...> {};

template<class Parser, class... Parsers>
struct Sequence
{
    template<class Control, class Context>
    static bool match(Input& input, Context& context)
    {
	if (not Control::template match<Parser>(input, context))
	    return false;
	return (Control::template match<Parsers>(input, context) and ...);
    }
};

template<class... Ps>
struct Choice : ParserTag
{
    template<class Control, class Context>
    static bool match(Input& input, Context& context)
    { return (Control::template match<Ps>(input, context) or ...); }
};

template<class P>
struct ZeroOrMore : ParserTag
{
    template<class Control, class Context>
    static bool match(Input& input, Context& context)
    {
	while (Control::template match<P>(input, context));
	return true;
    }
};

struct BasicControl
{
    using Self = BasicControl;
    
    template<class Parser, class Context>
    static bool match(Input& input, Context& context)
    {
	while (std::isspace(input.peek()))
	    input.advance(1);

	input.reset_mark();
	auto old_point = input.point();
	auto r = Parser::template match<Self>(input, context);
	
	if (input.point() > old_point and not r)
	{
	    cout << "error: " << string(input.begin(), input.end()) << endl;
	    cout << "error: " << string(old_point - input.begin(), ' ') << "^" << endl;
	    exit(0);
	}

	return r;
    }
};

struct BasicNode : NodeMixin<BasicNode>
{
    using Mixin::Mixin;
    virtual index_t eval() const = 0;
};

struct Num : BasicNode
{
    using BasicNode::BasicNode;
    virtual index_t eval() const override { return core::lexical_cast<int>(content()); }
};

struct Add : BasicNode
{
    using BasicNode::BasicNode;
    virtual index_t eval() const override { return child(0)->eval() + child(1)->eval(); }
};

struct Sub : BasicNode
{
    using BasicNode::BasicNode;
    virtual index_t eval() const override { return child(0)->eval() - child(1)->eval(); }
};

struct Mul : BasicNode
{
    using BasicNode::BasicNode;
    virtual index_t eval() const override { return child(0)->eval() * child(1)->eval(); }
};

struct Div : BasicNode
{
    using BasicNode::BasicNode;
    virtual index_t eval() const override { return child(0)->eval() / child(1)->eval(); }
};


struct Term;
struct Factor;
struct Expression;

struct AddAction : LinkC<'+', PushParent<Add>> {};
struct SubAction : LinkC<'-', PushParent<Sub>> {};
struct MulAction : LinkC<'*', PushParent<Mul>> {};
struct DivAction : LinkC<'/', PushParent<Div>> {};

struct TermAction : Link<Term, AbsorbChild> {};
struct FactorAction : Link<Factor, AbsorbChild> {};

struct AddSuffix : Sequence<AddAction, TermAction> {};
struct SubSuffix : Sequence<SubAction, TermAction> {};
struct TermSuffix : Choice<AddSuffix, SubSuffix> {};

struct MulSuffix : Sequence<MulAction, Mark, FactorAction> {};
struct DivSuffix : Sequence<DivAction, Mark, FactorAction> {};
struct FactorSuffix : Choice<MulSuffix, DivSuffix> {};

struct Number : Sequence<Link<Digits, Push<Num>>> {};
struct Group : Sequence<Char<'('>, Expression, Char<')'>> {};

struct Factor : Choice<Number, Group> {};
struct Term : Sequence<Factor, ZeroOrMore<FactorSuffix>> {};
struct Expression : Sequence<Term, ZeroOrMore<TermSuffix>> {};

struct Grammar : Sequence<Expression, End> {};

int tool_main(int argc, const char *argv[])
{
    auto opts = ArgParse
	(
	 argValues<'*',vector,string>("", "", 1)
	 );
    opts.parse(argc, argv);

    for (auto str : opts.get<'*'>())
    {
    	Input input{str};
    	ContextContainer<BasicNode> context;
    	auto r = BasicControl::template match<Grammar>(input, context);
    	cout << r << " " << context.top()->eval() << endl;
    	cout << context.str() << endl;
    }

    return 0;
}

