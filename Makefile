CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -g

APP_TARGET  := chess
TEST_TARGET := tests_runner

APP_SRCS  := src/main.cpp src/board.cpp src/position.cpp src/game.cpp
APP_OBJS  := $(APP_SRCS:.cpp=.o)

TEST_SRCS := tests/test_main.cpp tests/test_board.cpp src/board.cpp src/position.cpp src/game.cpp
TEST_OBJS := $(TEST_SRCS:.cpp=.o)

.PHONY: all clean test

all: $(APP_TARGET)

$(APP_TARGET): $(APP_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

test: $(TEST_TARGET)
	./$(TEST_TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(APP_OBJS:.o=.d)
-include $(TEST_OBJS:.o=.d)

clean:
	rm -f $(APP_TARGET) $(TEST_TARGET) \
	      $(APP_OBJS) $(APP_OBJS:.o=.d) \
	      $(TEST_OBJS) $(TEST_OBJS:.o=.d)
