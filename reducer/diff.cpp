#include <algorithm>
#include <random>
#include <climits>
#include <stdexcept>
#include <cassert>
#include "diff.h"

namespace diff {

std::ostream& operator << (std::ostream& os, operation_type op_type) {
    switch (op_type) {
    case OP_NONE: return os << "None";
    case OP_ENTER: return os << "Enter";
    case OP_INSERT: return os << "Insert";
    case OP_DELETE: return os << "Delete";
    default: assert(!"unexpected op_type");
    }
}

std::ostream& operator << (std::ostream& os, const operation& op) {
    return os << op.type << '\t' << op.context << "\t" << op.tag;
}

std::vector<operation> strdiff(std::string right, std::string wrong) {
    right = "^" + right + "$";
    wrong = "^" + wrong + "$";

    std::vector<std::vector<int>> dp(right.size() + 1, 
        std::vector<int>(wrong.size() + 1, INT_MAX / 2));
    std::vector<std::vector<operation_type>> dp_op(right.size() + 1,
        std::vector<operation_type>(wrong.size() + 1, OP_NONE));
    dp[0][0] = 0;

    std::mt19937 gen;
    for (size_t i = 1; i <= right.size(); i++) {
        for (size_t j = 1; j <= wrong.size(); j++) {
            int cost = INT_MAX / 2;
            std::vector<operation_type> ops;

            auto update = [&] (int new_cost, operation_type op) {
                if (new_cost < cost) {
                    ops = {op};
                    cost = new_cost;
                } else if (new_cost == cost) {
                    ops.push_back(op);
                }
            };

            if (right[i-1] == wrong[j-1]) update(dp[i-1][j-1], OP_ENTER);
            update(dp[i-1][j-1] + 1, OP_ENTER);
            update(dp[i-1][j] + 1, OP_DELETE);
            update(dp[i][j-1] + 1, OP_INSERT);
            
            dp[i][j] = cost;
            // for tie-breaking, we randomly pick an operation
            dp_op[i][j] = ops[gen() % ops.size()];
        }
    }

    std::vector<operation> ops;
    size_t ci = right.size(), cj = wrong.size();
    while (ci || cj) {
        operation op;
        op.type = dp_op[ci][cj];
        switch (op.type) {
        case OP_ENTER:
            ci--; cj--;
            if (ci == 0 || cj == 0) continue;
            op.tag = wrong[cj];
            break;
        case OP_INSERT:
            cj--;
            op.tag = wrong[cj];
            break;
        case OP_DELETE:
            ci--;
            break;
        default:
            assert(!"unexpected op_type");
        }
        if (ci <= 0 && ci >= right.size()) continue;
        op.context = {right[ci-1], right[ci]};
        if (op.type != OP_NONE) ops.push_back(std::move(op));
    }
    std::reverse(ops.begin(), ops.end());
    return ops;
}

}