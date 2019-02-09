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
    
    const char *begin() const { return m_begin; }
    const char *end() const { return m_end; }
    const char *loc() const { return m_loc; }
    const char *mark() const { return m_mark; }
    
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

private:
    bool m_valid;
    const char *m_begin;
    const char *m_end;
    const char *m_loc;
    const char *m_mark;
};

}; // end peg
