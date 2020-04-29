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
    OP_ENTER,
    OP_INSERT,
    OP_DELETE,
};

std::ostream& operator << (std::ostream& os, operation_type op_type);

struct operation {
    operation_type type = OP_NONE;
    std::string context;
    std::string tag;

    bool operator == (const operation& rhs) const {
        return type == rhs.type && context == rhs.context && tag == rhs.tag;
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
            return (hasher(op.context) + hasher(op.tag)) ^ op.type;
        }
    };
}
#endif