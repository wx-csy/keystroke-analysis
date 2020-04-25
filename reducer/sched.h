#ifndef __SCHED_H__
#define __SCHED_H__

#include <queue>
#include <string>
#include <vector>
#include <mutex>
#include <condition_variable>
#include "diff.h"
#include "concurrent_counter.h"

extern std::vector<std::string> task_list;

extern concurrent_counter<diff::operation, size_t> op_counter;
extern concurrent_counter<std::string, size_t> digram_counter;
extern concurrent_counter<std::string, size_t> trigram_counter;
extern concurrent_counter<std::string, size_t> section_counter;

void process_file(std::string string);
void reader();
void worker();

#endif