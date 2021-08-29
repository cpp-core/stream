// Copyright (C) 2021 by Mark Melton
//

#include "coro/yield.h"
#include "core/match.h"

namespace cot
{

std::ostream& operator<<(std::ostream& os, const Yield::Code& yield) {
    core::match
	(yield,
	 [&](const Yield::Exception&) { os << "exception"; },
	 [&](const Yield::Finished&) { os << "finished"; },
	 [&](const Yield::Resume&) { os << "resume"; },
	 [&](const Yield::ResumeAfter& v) { os << "resue-after-duration " << v.duration; },
	 [&](const Yield::ResumeAt& v) { os << "resume-at-time " << v.tp; },
	 [&](const Yield::ResumeOnSocket&) { os << "resume-on-socket"; },
	 [&](const Yield::Shutdown&) { os << "shutdown"; },
	 [&](const Yield::Suspend&) { os << "suspend"; },
	 [&](const Yield::Terminate&) { os << "terminate"; }
	 );
    return os;
}

}; // cot

