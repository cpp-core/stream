// Copyright (C) 2021 by Mark Melton
//

#pragma once
#include <variant>
#include "core/chrono.h"
#include "core/mp/list.h"

namespace cot
{

struct Yield {
    struct Exception { std::exception_ptr eptr; };
    struct Finished {};
    struct Resume {};
    struct ResumeAfter { chron::InNanos duration; };
    struct ResumeAfterReal { chron::InNanos duration; };
    struct ResumeAt { chron::TimeInNanos tp; };
    struct Shutdown {};
    struct Suspend {};
    struct Terminate {};
    using Code = std::variant<Exception,
			      Finished,
			      Resume,
			      ResumeAfter,
			      ResumeAfterReal,
			      ResumeAt,
			      Shutdown,
			      Suspend,
			      Terminate>;

    using CoReturnTypes = std::tuple<Exception, Finished, Shutdown, Terminate>;
    using CoYieldTypes = std::tuple<ResumeAfter, ResumeAfterReal, ResumeAt, Resume, Suspend>;
};

std::ostream& operator<<(std::ostream& os, const Yield::Code& yield);

}; // cot
