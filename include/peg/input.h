// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "core/common.h"

namespace peg
{

struct Input
{
    constexpr Input()
    {}
    
    Input(const string& str)
	: m_status(true)
	, m_begin(str.c_str())
	, m_end(m_begin + str.size())
	, m_mark(m_begin)
	, m_point(m_begin)
    { }

    explicit operator bool() const { return m_status; }

    bool status() const { return m_status; }
    const char *begin() const { return m_begin; }
    const char *end() const { return m_end; }
    const char *point() const { return m_point; }
    const char *mark() const { return m_mark; }
    bool eof() const { return m_point >= m_end; }
    bool bof() const { return m_point == m_begin; }
    
    char peek() const { return *m_point; }
    size_t source_size() const { return size_t(end() - begin()); }
    string_view source() const { return { begin(), source_size() }; }
    
    size_t match_size() const
    {
	if (m_status)
	    return m_point - m_mark;
	return 0;
    }
    
    string_view match() const
    {
	if (m_status)
	    return string_view{ m_mark, match_size() };
	return {};
    }

    void mark(const char *mark) { m_mark = mark; }
    
    Input success(size_t n = 0) const
    { return with_status(true, n); }

    Input failure() const
    { return with_status(false); }

    Input with_status(bool status, size_t advance = 0) const
    { return Input{ status, m_begin, m_end, m_mark, m_point + advance}; }

    friend bool operator==(const Input& a, const Input& b)
    {
	return a.m_status == b.m_status
	    and a.m_begin == b.m_begin
	    and a.m_end == b.m_end
	    and a.m_point == b.m_point
	    and a.m_mark == b.m_mark;
    }

    friend std::ostream& operator<<(std::ostream& os, const Input& input)
    { return os << input.status() << " " << size_t(input.mark()) << " " << size_t(input.point()); }

private:
    Input(bool status, const char *begin, const char *end, const char *mark, const char *point)
	: m_status(status)
	, m_begin(begin)
	, m_end(end)
	, m_mark(mark)
	, m_point(point)
    { }

    bool m_status{false};
    const char *m_begin{nullptr};
    const char *m_end{nullptr};
    const char *m_mark{nullptr};
    const char *m_point{nullptr};
};

}; // end peg
