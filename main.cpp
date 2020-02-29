#include <vector>
#include <iostream>

#include "enumerate.hpp"
#include "zip.hpp"

std::vector<int> function() {
    return std::vector{9, 10, 11, 12};
}

int main() {
    std::vector<int> v1{1, 2, 3, 4};
    std::vector<int> v2{5, 6, 7, 8};

    for (auto &[index, item] : enumerate(v1)) {
        std::cerr << "index " << index << ": " << item << '\n';
    }

    for (auto &[a, b, c] : zip(v1, v2, function())) {
        std::cerr << a << " " << b << " " << c << '\n';
    }
    std::cerr << std::endl;

    for (auto &[a, b, c] : zip(v1, v2, function())) {
        a += 1;
        b += 1;
        c += 1;
    }

    for (auto &[a, b, c] : zip(v1, v2, function())) {
        std::cerr << a << " " << b << " " << c << '\n';
    }

    
}
