#ifndef __DIFF_H__
#define __DIFF_H__

#include <string>
#include <functional>
#include <vector>
#include <utility>
#include <iostream>
#include <cstring>

namespace diff {

enum operation_type : int {
    OP_NONE,
    OP_INSERT,
    OP_DELETE,
    OP_REPLACE,
    OP_SWAP,
};

std::ostream& operator << (std::ostream& os, operation_type op_type);

struct operation {
    operation_type type = OP_NONE;
    std::string before;
    std::string after;

    bool operator == (const operation& rhs) const {
        return type == rhs.type && before == rhs.before && after == rhs.after;
    }
};

std::ostream& operator << (std::ostream& os, const operation& op);

std::vector<operation> strdiff(std::string right, std::string wrong);

}

namespace std {
    template <>
    class hash<diff::operation> {
        hash<string> hasher;
    public:
        size_t operator()(const diff::operation &op) const {
            return (hasher(op.before) + hasher(op.after)) ^ op.type;
        }
    };
}
#endif