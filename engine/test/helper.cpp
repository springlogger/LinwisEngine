#include "TestHelpers.h"

#include <cmath>
#include <iostream>

namespace lw::test {

bool almostEqual(float left, float right, float epsilon) {
    return std::abs(left - right) <= epsilon;
}

void TestStats::require(const std::string& name, bool condition) {
    if (condition) {
        ++passed;
        std::cout << "[OK] " << name << '\n';
    } else {
        ++failed;
        std::cout << "[FAIL] " << name << '\n';
    }
}

int TestStats::exitCode() const {
    return failed == 0 ? 0 : 1;
}

} // namespace lw::test
