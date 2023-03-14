#include "Utils.h"

#include <Poco/File.h>

std::string TestsUtils::makeCopy(const std::string& filePath) {
    Poco::File file(filePath);
    auto copyFileName = filePath + "_copy";
    file.copyTo(copyFileName);

    return copyFileName;
}