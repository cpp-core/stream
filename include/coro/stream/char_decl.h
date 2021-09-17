// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"

namespace costr::chr {

Gen<char> uniform();
Gen<char> lowercase();
Gen<char> uppercase();
Gen<char> alpha();
Gen<char> alphanum();
Gen<char> ident();

}; // costr
