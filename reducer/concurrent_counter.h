#ifndef __CONCURRENT_COUNTER_H__
#define __CONCURRENT_COUNTER_H__

#include <unordered_map>
#include <atomic>
#include <mutex>
#include <shared_mutex>

template <
    class Key,
    class T, 
    class Hash = std::hash<Key>, 
    class KeyEqual = std::equal_to<Key>
> class concurrent_counter {
    mutable std::shared_mutex mut;
    std::unordered_map<Key, std::atomic<T>, Hash, KeyEqual> counter;

    // caller of this function must owns the mut with shared ownership
    std::atomic<T>& get_value_ref(const Key& key) {
        typename decltype(counter)::iterator it;
        while ((it = counter.find(key)) == counter.end()) {
            mut.unlock_shared();
            mut.lock();
            counter.try_emplace(key, 0);
            mut.unlock();
            mut.lock_shared();
        }
        return it->second;
    }

public :
    T increment(const Key& key) {
        std::shared_lock<std::shared_mutex> lock(mut);
        return ++get_value_ref(key);
    }

    T operator[] (const Key& key) {
        std::shared_lock<std::shared_mutex> lock(mut);
        return get_value_ref(key);
    }

    // not thread-safe
    auto& get_underlying_map() {
        return counter;
    }
};

#endif