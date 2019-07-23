CXX = g++
TARGET = blackjack-simulator
CXXFLAGS = -Wall -O3 -std=c++11
SRCS = card.cc config.cc dealer.cc hand.cc player.cc rule.cc simulation.cc main.cc
OBJS := $(SRCS:.cc=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

run: $(TARGET)
	./$(TARGET) -g 10000

clean:
	rm -f $(TARGET) $(OBJS)
