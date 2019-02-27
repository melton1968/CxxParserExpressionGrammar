// Copyright (C) 2019 by Mark Melton
//

#include "core/tool.h"
#include "peg/cst/examples/calc1.h"

int tool_main(int argc, const char *argv[])
{
    core::POpt opts;
    opts.process(argc, argv);

    for (auto str : opts.extra())
	cout << calculator::parse(str) << endl;

    return 0;
}

