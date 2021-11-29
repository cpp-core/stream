// Copyright 2021 by Mark Melton
//

#include <gtest/gtest.h>
#include <filesystem>
#include "coro/stream/stream.h"
#include "core/utility/fixed.h"
#include "core/file.h"

using namespace coro;
static const size_t NumberSamples = 5;

namespace fs = std::filesystem;

class Environment : public ::testing::Environment {
public:
    Environment()
	: dir_(fs::temp_directory_path())
	, generator_(str::alpha() * sampler<int>(0, 10) | group()) {
	if (not dir_.ends_with("/"))
	    dir_ += "/";
	dir_ += fmt::format("stream-test.{}/", getpid());
	fs::remove_all(dir_);
	fs::create_directories(dir_);
    }
    
    ~Environment() override { }
    
    void SetUp() override { }
    
    void TearDown() override {
	fs::remove_all(dir_);
    }

    core::File get_filename(const string& name) const {
	return fmt::format("{}/{}", dir_, name);
    }

    std::vector<std::string> get_sample() {
	return generator_.sample();
    }
    
    std::string dir_;
    Generator<std::vector<std::string>&&> generator_;
};

Environment *env{nullptr};

TEST(CoroStreamIo, PlainFile)
{
    for (auto i = 0; i < NumberSamples; ++i) {
	auto expected = env->get_sample();
	auto fn = env->get_filename("plain.dat");
	expected | write_lines(fn);
	auto actual = read_lines(fn) | collect<std::vector>();
	EXPECT_EQ(actual, expected);
    }
}
    
TEST(CoroStreamIo, ZstdFile)
{
    for (auto i = 0; i < NumberSamples; ++i) {
	auto expected = env->get_sample();
	auto fn = env->get_filename("plain.zst");
	expected | write_lines(fn);
	auto actual = read_lines(fn) | collect<std::vector>();
	EXPECT_EQ(actual, expected);
    }
}

struct ioable {
    void write(const char *data, size_t size) {
	if (size != 1 or data[0] != '\n')
	    vec.emplace_back(data, size);
    };
    std::vector<std::string> vec;
    size_t idx{0};
};

bool getline(ioable& w, std::string& line) {
    if (w.idx < w.vec.size()) {
	line = w.vec[w.idx];
	++w.idx;
	return true;
    }
    return false;
}

TEST(CoroStreamIo, WriteableReadable)
{
    ioable io;
    auto expected = env->get_sample();
    expected | write_lines(io);
    EXPECT_EQ(io.vec, expected);
    
    auto actual = read_lines(io) | collect<std::vector>();
    EXPECT_EQ(actual, expected);
}
    
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    env = new Environment;
    AddGlobalTestEnvironment(env);
    return RUN_ALL_TESTS();
}



