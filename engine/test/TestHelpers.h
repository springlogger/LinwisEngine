#pragma once

#include <string>

namespace lw::test {

struct TestStats {
    int passed = 0;
    int failed = 0;

    void require(const std::string& name, bool condition);
    int exitCode() const;
};

bool almostEqual(float left, float right, float epsilon = 0.001f);

} // namespace lw::test
