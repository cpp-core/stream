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
    struct ResumeAt { chron::TimeInNanos tp; };
    struct ResumeAfter { chron::InNanos duration; };
    struct ResumeOnSocket {};
    struct Shutdown {};
    struct Suspend {};
    struct Terminate {};
    using Code = std::variant<Exception,
			      Finished,
			      Resume,
			      ResumeAfter,
			      ResumeAt,
			      ResumeOnSocket,
			      Shutdown,
			      Suspend,
			      Terminate>;

    using CoReturnTypes = std::tuple<Exception, Finished, Shutdown, Terminate>;
    using CoYieldTypes = std::tuple<ResumeAt, ResumeAfter, Resume, ResumeOnSocket, Suspend>;
};

std::ostream& operator<<(std::ostream& os, const Yield::Code& yield);

}; // cot
