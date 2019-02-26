// Copyright (C) 2019 by Mark Melton
//

#pragma once
#include <typeindex>
#include "core/common.h"
#include "core/demangle.h"

namespace peg::cst
{

struct Node
{
    static constexpr bool IsNode = true;
    using Self = Node;
    using Ptr = std::unique_ptr<Self>;
    using Children = std::vector<Ptr>;

    template<class T, class... Args>
    static Ptr make(Args&&... args)
    { return std::make_unique<T>(args...); }

    Node() = default;
    Node(string_view content)
	: m_content(content)
    { }

    Node(const Node&) = delete;
    Node(const Node&&) = delete;
    
    virtual ~Node() = default;
    virtual Ptr make_unique() const { return std::make_unique<Self>(); }
    virtual string type_name() const { return core::type_name<Self>(); }

    const auto& tid() const { return typeid(*this); }

    string_view content() const { return m_content; }
    void set_content(string_view content) { m_content = content; }

    const Ptr& child(size_t i) const
    { return m_children[i]; }

    const Children& children() const
    { return m_children; }

    Children& children()
    { return m_children; }

    void emplace_child(Ptr child)
    { m_children.emplace_back(std::move(child)); }

    Ptr move_child(size_t i)
    { return std::move(m_children[i]); }

    Ptr clone() const
    {
	auto n = make_unique();
	n->set_content(m_content);
	
	for (auto& c : m_children)
	{
	    auto new_child = c->clone();
	    n->emplace_child(std::move(new_child));
	}
	return n;
    }

    void print(std::ostream& os, size_t level = 0) const
    {
	os << "   ";
	for (size_t i = 0; i < level; ++i)
	    os << "|   ";
	os << "'" << content() << "'";
	os << "    " << core::type_name(*this);
	os << endl;
	
	for (const auto& n : children())
	    n->print(os, level+1);
    }

    friend std::ostream& operator<<(std::ostream& os, const Ptr& n)
    {
	n->print(os);
	return os;
    }

    friend bool operator==(const Ptr& a, const Ptr& b)
    {
	if (a->content() != b->content())
	    return false;

	if (a->children().size() != b->children().size())
	    return false;

	for (size_t i = 0; i < a->children().size(); ++i)
	    if (a->child(i) != b->child(i))
		return false;
	return true;
    }

    friend bool operator!=(const Ptr& a, const Ptr& b)
    { return not (a == b); }

private:
    string_view m_content;
    Children m_children;
};

namespace detail
{

template<class Base, class Derived>
struct Prototype : public Base
{
    using Proto = Prototype<Base,Derived>;

    template<class... Args>
    Prototype(Args&&... args)
	: Base(args...)
    { }
    
    virtual ~Prototype() = default;
    
    virtual std::unique_ptr<Base> make_unique() const override
    { return std::make_unique<Derived>(); }
    
    virtual string type_name() const override { return core::type_name<Derived>(); }
};

}; // end ns detail

template<class Derived>
using ProtoNode = detail::Prototype<Node, Derived>;

template<class Node>
concept bool IsNode = Node::IsNode == true;

}; // end ns peg::cst
