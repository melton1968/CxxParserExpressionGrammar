// Copyright (C) 2019 by Mark Melton
//

#include <stack>
#include "core/tool.h"
#include "peg/peg.h"

using namespace peg;

struct Number : n::Integer {};
struct Expression;
struct ParenExpr : IfMust<c::OpenParen, Expression, c::CloseParen> {};
struct Atomic : Choice<Number, ParenExpr> {};
struct Infix : Choice<c::Plus, c::Minus, c::Star, c::Divide> {};
struct Expression : InfixList<Atomic, Infix, WhiteSpace> {};
struct Grammar : Must<Expression, EndOfLineOrFile> {};

using InfixOperators = std::stack<char>;
using Integer = uint64;

class Computation
{
public:
    using IntegerStack = std::stack<Integer>;
    using Data = std::stack<IntegerStack>;
    using InfixStack = std::stack<char>;

    Computation(bool trace = false)
	: m_trace(trace)
    { open(); };

    size_t depth() const { return m_data.size(); }
    size_t size() const { return m_data.top().size(); }

    void open()
    {
	m_data.emplace();
    }
    
    void close()
    {
	assert(size() == 1);
	auto n = pop();
	m_data.pop();
	push(n);
    }
    
    Integer result() const
    {
	assert(depth() == 1);
	assert(size() == 1);
	return m_data.top().top();
    }
    
    
    Integer pop() 
    {
	auto n = m_data.top().top();
	m_data.top().pop();
	return n;
    }

    void push(Integer n)
    {
	if (size() > 0)
	{
	    auto m = pop();
	    if (m_trace)
		cout << m << " " << m_infix.top() << " " << n << " = ";
	    n = reduce(m, n);
	    if (m_trace)
		cout << n << endl;
	}
	m_data.top().push(n);
    }

    void push_infix(char c) { m_infix.push(c); }
    
    char pop_infix()
    {
	auto c = m_infix.top();
	m_infix.pop();
	return c;
    }
    
    Integer reduce(Integer n, Integer m)
    {
	assert(m_infix.size() >= 1);
	auto op = pop_infix();
	
	switch (op)
	{
	case '+': return n + m;
	case '-': return n - m;
	case '*': return n * m;
	case '/': return n / m;
	default:
	    throw core::runtime_error("Unknown infix operator: %c", op);
	}
    }

private:
    bool m_trace;
    InfixStack m_infix;
    Data m_data;
};

template<class Rule>
struct Calculator : NullAction<Rule> {};

template<>
struct Calculator<Number> : NullAction<Number>
{
    static void success(const Input& input, Computation& c)
    {
	auto n = core::lexical_cast<Integer>(input.match());
	c.push(n);
    }
};

template<>
struct Calculator<Infix> : NullAction<Infix>
{
    static void success(const Input& input, Computation& c)
    {
	c.push_infix(input.match()[0]);
    }
};

template<>
struct Calculator<c::OpenParen> : NullAction<c::OpenParen>
{
    static void success(const Input&, Computation& c)
    { c.open(); }
};

template<>
struct Calculator<c::CloseParen> : NullAction<c::CloseParen>
{
    static void success(const Input&, Computation& c)
    { c.close(); }
};


int tool_main(int argc, const char *argv[])
{
    core::POpt opts;
    opts.add_flag("trace", 't', "Trace computation");
    opts.process(argc, argv);

    auto trace = opts.flag("trace");

    for (const auto& expr : opts.extra())
    {
	Computation c{ trace };
	auto r = parse<Grammar,Calculator>(expr, c);
	if (not r)
	    cout << "Failed to parse expression: " << expr << endl;
	else
	    cout << expr << " = " << c.result() << endl;
    }

    return 0;
}

