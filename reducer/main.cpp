#include <fstream>
#include <vector>
#include <thread>
#include "sched.h"

std::vector<std::string> task_list;

static void read_tasks() {
    std::fstream task_list_file("tasklist.txt");
    std::string file_name;
    while (task_list_file >> file_name) 
        task_list.push_back(std::move(file_name));
}

static void process() {
    std::vector<std::thread> readers, consumers;
    for (size_t i = 0; i < 40; i++) 
        readers.emplace_back(reader);
    for (size_t i = 0; i < 30; i++)
        consumers.emplace_back(worker);
    for (auto& th : readers) th.join();
    for (auto& th : consumers) th.join();
}

static void write_result() {
    std::ofstream of_op("ops.txt");
    std::ofstream of_digram("digram.txt");
    std::ofstream of_trigram("trigram.txt");
    for (auto& pr : op_counter.get_underlying_map()) {
        of_op << pr.first << '\t' << pr.second << std::endl;
    }
    for (auto& pr : digram_counter.get_underlying_map()) {
        of_digram << pr.first << '\t' << pr.second << std::endl;
    }
    for (auto& pr : trigram_counter.get_underlying_map()) {
        of_trigram << pr.first << '\t' << pr.second << std::endl;
    }
}

int main() {
    read_tasks();
    process();
    write_result();
    std::fprintf(stderr, "Done!\n");
}