// Copyright (C) 2021 by Mark Melton
//

#include "coro/strand/yield.h"
#include "core/utility/match.h"
#include "core/chrono/duration.h"

namespace coros
{

std::ostream& operator<<(std::ostream& os, const Yield::Code& yield) {
    core::match
	(yield,
	 [&](const Yield::Exception&) { os << "exception"; },
	 [&](const Yield::Finished&) { os << "finished"; },
	 [&](const Yield::Resume&) { os << "resume"; },
	 [&](const Yield::ResumeAfter& v) { os << "resue-after-duration " << v.duration; },
	 [&](const Yield::ResumeAfterReal& v) { os << "resue-after-real-duration " << v.duration;},
	 [&](const Yield::ResumeAt& v) { os << "resume-at-time " << v.tp; },
	 [&](const Yield::Shutdown&) { os << "shutdown"; },
	 [&](const Yield::Suspend&) { os << "suspend"; },
	 [&](const Yield::Terminate&) { os << "terminate"; }
	 );
    return os;
}

}; // coros

