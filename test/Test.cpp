#include "TestHelpers.h"

#include <iostream>

namespace lw::test {
void runVector3Tests(TestStats& stats);
void runCameraTests(TestStats& stats);
}

int main() {
    lw::test::TestStats stats;

    lw::test::runVector3Tests(stats);
    lw::test::runCameraTests(stats);

    std::cout << "\nPassed: " << stats.passed << "\nFailed: " << stats.failed << '\n';
    return stats.exitCode();
}
