CXX = g++
BINDIR = ./bin
TARGET = ./bin/blackjack-simulator
CXXFLAGS = -Wall -O3 -std=c++11
SRCS = $(wildcard *.cc)
OBJDIR = ./obj
OBJS := $(addprefix $(OBJDIR)/, $(SRCS:.cc=.o))

$(TARGET): $(OBJS)
	mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

$(OBJDIR)/%.o: %.cc
	mkdir -p $(OBJDIR)
	@[ -d $(OBJDIR) ]
	$(CXX) $(CXXFLAGS) -o $@ -c $<

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BINDIR) $(OBJDIR)
