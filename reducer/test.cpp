#include <cassert>
#include <vector>
#include <iostream>
#include "diff.h"

int main() {
    using namespace diff;
    typedef std::vector<operation> vec;
    assert((strdiff("abcd", "abdd") == vec{operation{OP_REPLACE, "bcd", "bdd"}}));
    assert((strdiff("abcd", "abd") == vec{operation{OP_DELETE, "bcd", "bd"}}));
    assert((strdiff("abcd", "abcde") == vec{operation{OP_INSERT, "d$", "de$"}}));
    assert((strdiff("abcd", "abdc") == vec{operation{OP_SWAP, "cd", "dc"}}));
    assert((strdiff("abcd", "abef") == 
        vec{operation{OP_REPLACE, "bcd", "bed"}, operation{OP_REPLACE, "cd$", "cf$"}}));
    printf("All tests passed!\n");
    return 0;
}