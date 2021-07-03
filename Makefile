DBGFLAGS = -g3 #-DZOBRIST_SIZE="(1<<12)"
OPTFLAGS = -Ofast -DNDEBUG -flto -fwhole-program -march=native -m64 -march=native
PROFFLAGS = $(OPTFLAGS) -pg
CXXFLAGS = -std=c++20 -I. -Wall -Wextra -Wno-unused -Wno-parentheses
# clang:
#CXX = clang++-13
#CXXFLAGS += -Wno-unused-parameter -Wno-range-loop-construct -Wno-unknown-attributes -Wno-ignored-optimization-argument
# CXXFLAGS += -fopt-info
HPPFILES = $(wildcard *.hpp) m42.h m42v2.h m42_proxy.h
SOURCES = m42.cpp m42v2.cpp

CORES = $(shell getconf _NPROCESSORS_ONLN)
all :; @$(MAKE) _all -j$(CORES)
_all : bench bench_prof bench_dbg test_m42

bench: bench.cpp $(SOURCES) $(HPPFILES) Makefile
	$(CXX) $(OPTFLAGS) $(CXXFLAGS) bench.cpp $(SOURCES) $(LDFLAGS) -o $@

bench_prof: bench.cpp $(SOURCES) $(HPPFILES) Makefile
	$(CXX) $(PROFFLAGS) $(CXXFLAGS) bench.cpp $(SOURCES) $(LDFLAGS) -o $@

bench_dbg: bench.cpp $(SOURCES) $(HPPFILES) Makefile
	$(CXX) $(DBGFLAGS) $(CXXFLAGS) bench.cpp $(SOURCES) $(LDFLAGS) -o $@

test_m42 : test_m42.cpp m42.h
	$(CXX) $(OPTFLAGS) $(CXXFLAGS) test_m42.cpp $(LDFLAGS) -o $@

clean:
	rm -vf *.o
	rm -vf bench bench_prof bench_dbg test_m42
