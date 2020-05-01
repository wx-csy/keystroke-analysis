#include <cassert>
#include <vector>
#include <iostream>
#include "diff.h"

int main() {
    using namespace diff;
    std::string a, b; 
    std::cin >> a >> b;
    for (auto op : strdiff(a, b)) 
        std::cout << op << std::endl;
    return 0;
}