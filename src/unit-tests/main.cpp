#include "gtest/gtest.h"

#include "Config.h"

int main(int argc, char *argv[]) {
    if (Config::getInstance()->getResourcesPath().empty()) {
        std::cout
                << "You need to set \"" << Config::pathToResourcesVariableName << "\" environment variable "
                << "with path to unit-tests resources folder." << std::endl
                << "For example: \"export " << Config::pathToResourcesVariableName << "=/path/to/resources/\"" << std::endl;
        return 1;
    }

    std::cout << "Path to unit-tests resources folder: " << Config::getInstance()->getResourcesPath() << std::endl;

    testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}