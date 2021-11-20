// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"
#include "core/concurrent/ring/ring.h"
#include "core/concurrent/ring/claim.h"
#include "core/concurrent/ring/processor.h"
#include "core/concurrent/scoped_task.h"

namespace coro {

template<class T>
Generator<T> pipeline(Generator<T> source) {
    namespace ring = core::mt::ring;
    
    const size_t buffer_size = 256;
    std::atomic<ring::sequence_t> final_idx{ring::FinalSequence};
    ring::Ring<T> data{buffer_size};
    ring::Processor<ring::SingleThreadClaimStrategy> producer{buffer_size};
    ring::Processor<ring::SingleThreadClaimStrategy> consumer{buffer_size};
    producer.add_write_barrier(consumer.cursor());
    consumer.add_read_barrier(producer.cursor());

    core::mt::scoped_task<void> producer_thread{[&]() {
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
