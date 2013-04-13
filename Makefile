

CXX = g++-4.6

GTEST_DIR = ./gtest-1.6.0

COV_DIR = ./coverage

# Flags passed to the preprocessor.
CPPFLAGS += -I$(GTEST_DIR)/include -I$(GTEST_DIR) -I/usr/include/c++/4.6/x86_64-linux-gnu 

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread -fprofile-arcs -ftest-coverage -DDEBUG -lpthread -O3
CXXFLAGS_RELEASE = -O3 -lpthread

TEST_BIN = ./integral_test
DRIVER_BIN = ./integral_driver

all: driver
	

driver:
	$(CXX) $(CPPFLAGS) integral_driver.c integral.c -o $(DRIVER_BIN) $(CXXFLAGS_RELEASE)

# make tests - build and run all tests
tests: integral_tests.o integral.o $(GTEST_DIR)/gtest_main.a integral.o integral_driver.o
	@echo "Building tests...s"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $(TEST_BIN) integral_tests.o integral.o $(GTEST_DIR)/gtest_main.a
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $(DRIVER_BIN) integral_driver.o integral.o

# make memcheck - perfrom valgrind leakage checking
memcheck: tests
	@echo "Performing valgrind check..."
	valgrind --leak-check=full $(TEST_BIN)

# make covarage - extract line coverage information
coverage: tests
	#lcov -d . --zerocounters --compat split_src=on
	lcov -d . --zerocounters 
	$(TEST_BIN)
	@echo "Extracting line coverage information..."
	#lcov --capture  -d . -q -o coverage_results --compat split_crc
	lcov --capture  -d . -q -o coverage_results 
	@echo "Generating html representation..."
	rm -rf $(COV_DIR)
	mkdir $(COV_DIR)
	genhtml ./coverage_results -o $(COV_DIR)

# make help - get help
help:
	@grep "^# make" ./Makefile 

%.o: %.c
	$(CXX) $(CPPFLAGS) -c $^ -o $@

clean:
	rm -rf *.o coverage_results $(TEST_BIN) $(DRIVER_BIN)
	rm -rf $(COV_DIR) 
	rm -rf ./*.gcda ./*gcno

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

# House-keeping build targets.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

$(GTEST_DIR)/gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc -o $(GTEST_DIR)/gtest-all.o

$(GTEST_DIR)/gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc -o $(GTEST_DIR)/gtest_main.o

$(GTEST_DIR)/gtest.a : $(GTEST_DIR)/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

$(GTEST_DIR)/gtest_main.a : $(GTEST_DIR)/gtest-all.o $(GTEST_DIR)/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^
