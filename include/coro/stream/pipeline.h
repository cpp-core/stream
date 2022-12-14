// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"
#include "core/cc/ring/ring.h"
#include "core/cc/ring/claim.h"
#include "core/cc/ring/processor.h"
#include "core/cc/scoped_task.h"

namespace coro {

template<Stream S>
Generator<stream_value_t<S>&&> pipeline(S source) {
    namespace ring = core::cc::ring;
    
    const size_t buffer_size = 256;
    std::atomic<ring::sequence_t> final_idx{ring::FinalSequence};
    ring::Ring<stream_value_t<S>> data{buffer_size};
    ring::Processor<ring::SingleThreadClaimStrategy> producer{buffer_size};
    ring::Processor<ring::SingleThreadClaimStrategy> consumer{buffer_size};
    producer.add_write_barrier(consumer.cursor());
    consumer.add_read_barrier(producer.cursor());

    core::cc::scoped_task<void> producer_thread{[&]() {
	while (source.next()) {
	    auto idx = producer.claim();
	    data[idx] = source();
	    producer.publish(idx);
	}
	final_idx.store(producer.cursor()->get());
    }};

    while (consumer.cursor()->get() <= final_idx.load()) {
	auto [begin, end] = consumer.claim_all();
	for (auto idx = begin; idx < end; ++idx)
	    co_yield data[idx];
	consumer.publish({begin, end});
    }

    co_return;
}

}; // coro
