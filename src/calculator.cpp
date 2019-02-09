// Copyright (C) 2019 by Mark Melton
//

#include "core/tool.h"

int tool_main(int argc, const char *argv[])
{
    core::POpt opts;
    opts.process(argc, argv);

    return 0;
}

