#include <algorithm>
#include <climits>
#include <stdexcept>
#include <cassert>
#include "diff.h"

namespace diff {

std::ostream& operator << (std::ostream& os, operation_type op_type) {
    switch (op_type) {
    case OP_NONE: return os << "None";
    case OP_INSERT: return os << "Insert";
    case OP_DELETE: return os << "Delete";
    case OP_REPLACE: return os << "Replace";
    case OP_SWAP: return os << "Swap";
    default: assert(!"unexpected op_type");
    }
}

std::ostream& operator << (std::ostream& os, const operation& op) {
    return os << op.type << ' ' << op.before << " -> " << op.after;
}

static constexpr int MAXCHAR = 4096;

static bool update(int& x, int y) {
    if (y < x) {
        x = y;
        return true;
    } else {
        return false;
    }
}

std::vector<operation> strdiff(std::string right, std::string wrong) {
    if (right == wrong) return {};

    right = "^" + right + "$";
    wrong = "^" + wrong + "$";

    std::vector<std::vector<int>> dp(right.size() + 1, 
        std::vector<int>(wrong.size() + 1, INT_MAX / 2));
    std::vector<std::vector<operation_type>> dp_op(right.size() + 1,
        std::vector<operation_type>(wrong.size() + 1, OP_NONE));
    dp[0][0] = 0;

    for (size_t i = 1; i <= right.size(); i++) {
        for (size_t j = 1; j <= wrong.size(); j++) {
            int cost = INT_MAX / 2;
            operation_type op = OP_NONE;

            if (right[i-1] == wrong[j-1] && update(cost, dp[i-1][j-1])) op = OP_NONE;
            if (update(cost, dp[i-1][j-1] + 1)) op = OP_REPLACE;
            if (update(cost, dp[i-1][j] + 1)) op = OP_DELETE;
            if (update(cost, dp[i][j-1] + 1)) op = OP_INSERT;
            if (i > 1 && j > 1 && 
                right[i-1] == wrong[j-2] && right[i-2] == wrong[j-1] &&
                update(cost, dp[i-2][j-2] + 1)) op = OP_SWAP;
            
            dp[i][j] = cost;
            dp_op[i][j] = op;
        }
    }

    std::vector<operation> ops;
    int ci = right.size(), cj = wrong.size();
    while (ci || cj) {
        operation op;
        op.type = dp_op[ci][cj];
        switch (op.type) {
        case OP_NONE:
            ci--; cj--;
            break;
        case OP_REPLACE:
            ci--; cj--;
            op.before.resize(3);
            op.before[0] = right[ci-1]; op.before[1] = right[ci]; op.before[2] = right[ci+1];
            op.after.resize(3);
            op.after[0] = right[ci-1]; op.after[1] = wrong[cj]; op.after[2] = right[ci+1];
            break;
        case OP_INSERT:
            cj--;
            op.before.resize(2);
            op.before[0] = right[ci-1]; op.before[1] = right[ci];
            op.after.resize(3);
            op.after[0] = right[ci-1]; op.after[1] = wrong[cj]; op.after[2] = right[ci];
            break;
        case OP_DELETE:
            ci--;
            op.before.resize(3);
            op.before[0] = right[ci-1]; op.before[1] = right[ci]; op.before[2] = right[ci+1];
            op.after.resize(2);
            op.after[0] = right[ci-1]; op.after[1] = right[ci+1];
            break;
        case OP_SWAP:
            ci -= 2; cj -= 2;
            op.before.resize(2);
            op.before[0] = right[ci]; op.before[1] = right[ci+1];
            op.after.resize(2);
            op.after[0] = wrong[cj]; op.after[1] = wrong[cj+1];
            break;
        default:
            assert(!"unexpected op_type");
        }
        if (op.type != OP_NONE) ops.push_back(std::move(op));
    }
    std::reverse(ops.begin(), ops.end());
    return ops;
}

}