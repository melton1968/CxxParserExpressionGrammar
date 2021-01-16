// Copyright (C) 2019, 2021 by Mark Melton
//

#include "core/tool.h"
#include "core/demangle.h"


// #define K(str)							\
//     struct K_ ## str						\
//     {								\
// 	static constexpr std::array<char,5> str = { #str };	\
// 	static constexpr char get(size_t i) { return str[i]; }	\
// 	static constexpr size_t size() { return str.size(); }	\
//     };								\
    
    
// K(abcd);


template<class S>
struct matcher
{
    static bool match(string_view s)
    {
	for (size_t i = 0; i < S::size(); ++i)
	    if (S::get(i) != s[i])
		return false;
	return true;
    }
};

int tool_main(int argc, const char *argv[])
{
    // matcher<K_abcd> m;
    // cout << m.match("abc") << endl;
    // cout << m.match("abcd") << endl;
    return 0;
}

