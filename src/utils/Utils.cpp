#include "Utils.h"

#include <sstream>

std::vector<std::string> nikmon::stringUtils::split(const std::string& str, const char delimiter) {
    std::vector<std::string> arr;
    std::stringstream strStream(str);
    std::string segment;

    while (std::getline(strStream, segment, delimiter)) {
        arr.push_back(segment);
    }

    return arr;
}