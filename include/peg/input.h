// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "core/common.h"

namespace peg
{

struct Input
{
    Input(const string& str)
	: m_valid(true)
	, m_begin(str.c_str())
	, m_end(m_begin + str.size())
	, m_loc(m_begin)
	, m_mark(m_begin)
    { }

    Input(bool valid, const char *begin, const char *end, const char *loc, const char *mark)
	: m_valid(valid)
	, m_begin(begin)
	, m_end(end)
	, m_loc(loc)
	, m_mark(mark)
    { }

    explicit operator bool() const { return m_valid; }

    char peek() const { return *m_loc; }

    bool status() const { return m_valid; }
    const char *begin() const { return m_begin; }
    const char *end() const { return m_end; }
    const char *loc() const { return m_loc; }
    const char *mark() const { return m_mark; }
    string_view view() const { return { begin(), size_t(end() - begin()) }; }
    
    bool eof() const { return m_loc >= m_end; }
    bool bof() const { return m_loc == m_begin; }

    void mark(const char *mark) { m_mark = mark; }
    
    Input success(size_t n = 0) const
    {
	return Input{ true, m_begin, m_end, m_loc + n, m_mark };
    }

    Input failure() const
    {
	return Input{ false, m_begin, m_end, m_loc, m_mark };
    }

    Input with_status(bool success) const
    {
	return Input{ success, m_begin, m_end, m_loc, m_mark };
    }

    string match() const
    {
	if (m_valid)
	    return string(m_mark, m_loc);
	return string();
    }

    size_t match_size() const
    {
	if (m_valid)
	    return m_loc - m_mark;
	return 0;
    }
    
    string_view match_view() const
    {
	if (m_valid)
	    return string_view{ m_mark, match_size() };
	return {};
    }

    size_t size() const
    {
	if (m_valid)
	    return m_loc - m_mark;
	return 0;
    }

    friend bool operator==(const Input& a, const Input& b)
    {
	return a.m_valid == b.m_valid
	    and a.m_begin == b.m_begin
	    and a.m_end == b.m_end
	    and a.m_loc == b.m_loc
	    and a.m_mark == b.m_mark;
    }

private:
    bool m_valid;
    const char *m_begin;
    const char *m_end;
    const char *m_loc;
    const char *m_mark;
};

}; // end peg
