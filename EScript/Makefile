CXX      = g++
CXXFLAGS = -Wall -fPIC -std=c++17 -I./
LDFLAGS  = -shared
TARGET = lib/libescript.so
DEPS = dep.d
-include obj.d


all: $(TARGET)

clean: 
	rm -f src/*.o

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) -o $(TARGET)

-include $(DEPS)
