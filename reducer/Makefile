CXX = g++
CXXFLAGS += -Wall -O1 -fsanitize=undefined,address -ggdb -std=c++17
LD = g++
LDFLAGS = $(CXXFLAGS) -pthread

HEADERS = $(wildcard *.h)

.DEFAULT_GOAL = ops.txt

.PHONY: clean

%.o : %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: test.o diff.o
	$(LD) $(LDFLAGS) $^ -o $@
	./$@

process : diff.o job.o work.o main.o
	$(LD) $(LDFLAGS) $^ -o $@

ops.txt : tasklist.txt process
	./process

tasklist.txt :
	ls ../processed/*.txt > tasklist.txt

clean:
	rm -rf *.o test process tasklist.txt

.DELETE_ON_ERRORS: test