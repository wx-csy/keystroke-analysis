#include <fstream>
#include <atomic>
#include <iterator>
#include "sched.h"

std::atomic<size_t> ticket;
std::atomic<size_t> progress;

static std::mutex mut_q;
static std::queue<std::string> files;
static std::condition_variable cv;

void reader() {
    size_t id;
    while ((id = ticket++) < task_list.size()) {
        std::fstream fs(task_list[id]);
        std::string buf{std::istreambuf_iterator<char>(fs), {}};
        std::unique_lock<std::mutex> lk(mut_q);
        files.push(std::move(buf));
        lk.unlock();
        cv.notify_one();
    }
    cv.notify_one();
}

void worker() {
    while (true) {
        std::unique_lock<std::mutex> lk(mut_q);
        cv.wait(lk, [] {return !files.empty() || progress >= task_list.size();});
        if (files.size()) {
            std::string file_content(std::move(files.front()));
            files.pop();
            lk.unlock();
            process_file(std::move(file_content));
        } else if (progress >= task_list.size()) {
            break;
        }
        if (progress++ % 100 == 0) {
            lk.lock();
            fprintf(stderr, "Progress: %zu / %zu\n", (size_t)progress, task_list.size());
        }
    }
    cv.notify_one();
}