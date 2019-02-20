// Copyright (C) 2019 by Mark Melton
//

#include <typeindex>
#include <stack>
#include <fmt/format.h>
#include "core/tool.h"
#include "core/demangle.h"
#include "core/mp/mp.h"
#include "peg/peg.h"

std::string replace_all(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}


using namespace peg;

struct MyControl
{
    template<class Parser, template<class> class... Actions, class... States>
    static Input match(Input input, States&&... states)
    {
	(Actions<Parser>::start(input, states...) , ...);
	
	auto r = Parser::template match<MyControl, Actions...>(input, states...);
	r.mark(input.point());

	bool valid = (Actions<Parser>::validate(r, states...) and ...);

	auto final_r = valid ? r : input.failure();
	if (final_r) (Actions<Parser>::success(r, states...) , ...);
	else (Actions<Parser>::failure(final_r, states...), ...);
	
	return final_r;
    }

    template<class T>
    static void raise()
    {
	throw std::runtime_error("Control::raise");
    }
};

template<class Parser>
struct NullAction
{
    template<class... States> static void start(Input input, States&&... states) {}
    template<class... States> static void success(Input input, States&&... states) {}
    template<class... States> static void failure(Input input, States&&... states) {}
    template<class... States> static bool validate(Input input, States&&... states) { return true;}
};

template<class Parser>
struct DebugAction : ::NullAction<Parser>
{
    using Type = DebugAction<Parser>;
    template<class... States>
    static void apply(const Input& input, States&&...)
    {
	auto view = core::type_name<Type>();
	cout << string(view) << ": ";
	cout << input.match() << endl;
    }
};

namespace parse
{

struct Node
{
    using Ptr = std::unique_ptr<Node>;
    using Children = std::vector<Ptr>;
    string_view content;
    Children children;
};

struct Tree
{
    std::stack<Node::Ptr> nodes;
    size_t tree_level{0};
    size_t parse_level{0};
};

string walk(Node::Ptr& node)
{
    string r;
    if (node->children.size() == 0)
    {
	cout << node->content << endl;
	r = string(node->content) + " "s;
    }
    else
    {
	r += "(";
	for (auto& c : node->children)
	    r += walk(c);
	r += ")";
    }
    return r;
}

}; // end ns parse

template<class Parser>
struct ParseTreeAction : ::NullAction<Parser>
{
    static void start(const Input& input, parse::Tree& pt)
    {
	++pt.parse_level;
    }
    
    static void failure(const Input& input, parse::Tree& pt)
    {
	--pt.parse_level;
    }
    
    static void success(const Input& input, parse::Tree& pt)
    {
	while (++pt.tree_level <= pt.parse_level)
	    pt.nodes.push(std::make_unique<parse::Node>());
	
	--pt.parse_level;
	--pt.tree_level;
	--pt.tree_level;

	assert(pt.nodes.size() > 1);
	auto n = std::move(pt.nodes.top());
	pt.nodes.pop();
	n->content = input.match();
	pt.nodes.top()->children.emplace_back(std::move(n));
    }
};

struct Expr : Recurse<
    Or<
	Seq<Expr, c::Plus, c::a>,
	c::a
	>
    >
{};

template<class T>
struct get_children
{ using type = std::tuple<>; };

template<class T>
requires requires(T) { typename T::Children; }
struct get_children<T>
{ using type = typename T::Children; };

template<class T>
using get_children_t = typename get_children<T>::type;

template<class T, class Exclude = std::tuple<>>
struct print_type
{
    using Head = core::mp::head_t<T>;
    using Tail = core::mp::tail_t<T>;
    using NewExclude = core::mp::append_t<Exclude, std::tuple<Head>>;
    using Children = get_children_t<Head>;
    
    static void apply()
    {
	auto str = core::type_name<Head>();
	str = replace_all(str, "peg::", "");
	str = replace_all(str, "detail::", "");
	str = replace_all(str, "character::", "");
	cout << "P= " << str << endl;
	if constexpr (not core::mp::contains_v<Head, Exclude> and core::mp::size_v<Children> > 0)
			 print_type<Children, NewExclude>::apply();

	using TailSize = typename core::mp::size<Tail>::type;
	if constexpr (TailSize::value > 0)
			 print_type<Tail, NewExclude>::apply();
    }
};

int tool_main(int argc, const char *argv[])
{
    core::POpt opts;
    opts.process(argc, argv);

    print_type<std::tuple<Expr>>::apply();
    cout << endl;

    string str = "a+a+a+a";
    Input input{str};
    parse::Tree parse_tree;
    parse_tree.nodes.push(std::make_unique<parse::Node>());
    auto r = MyControl::template match<Expr, ParseTreeAction>(input, parse_tree);
    cout << "match: " << r.match() << endl;

    auto root = std::move(parse_tree.nodes.top());
    parse_tree.nodes.pop();
    assert(parse_tree.nodes.empty());

    // cout << parse::walk(root) << endl;
    
    cout << root->content << endl;
    // for (auto& c : root->children)
    // {
    // 	cout << c->content << endl;
    // 	for (auto& gc : c->children)
    // 	{
    // 	    cout << "    " << gc->content << endl;
    // 	    for (auto &ggc : gc->children)
    // 	    {
    // 		cout << "        " << ggc->content << endl;
    // 		for (auto &gggc : ggc->children)
    // 		    cout << "            " << gggc->content << endl;
    // 	    }
    // 	}
    // }


    return 0;
}

