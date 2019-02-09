// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "peg/sequence.h"
#include "peg/until.h"

namespace peg
{

template<typename P, // Initial delimiter.
	 typename Q, // Block text.
	 typename R> // Final delimiter.
using Block = Sequence<P, Until<R, Q>>;

}; // end peg
