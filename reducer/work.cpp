#include <sstream>
#include <string>
#include <unordered_map>
#include "sched.h"

static void process_case(std::string right, std::string wrong) {
    for (size_t i = 0; i + 2 <= right.size(); i++) 
        digram_counter.increment(right.substr(i, 2));
    for (size_t i = 0; i + 3 <= right.size(); i++)
        trigram_counter.increment(right.substr(i, 3));
    for (const auto& op : diff::strdiff(right, wrong))
        op_counter.increment(op);
}

void process_file(std::string str) {
    std::stringstream ss(str);
    std::string line;
    while (std::getline(ss, line)) {
        std::stringstream lss(line);
        std::string sid, input, user;
        std::getline(lss, sid, '\t');
        std::getline(lss, input, '\t');
        std::getline(lss, user, '\t');
        section_counter.increment(input);
        process_case(input, user);
    }
}

