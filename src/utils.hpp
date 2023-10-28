#pragma once
#include <string>

inline std::string getDifficultyIcon(int diff){
    if(diff == -1) return "difficulty_00_btn_001.png";
    if(diff == 0) return "difficulty_auto_btn_001.png";
    if(diff <= 10) return fmt::format("difficulty_{:02}_btn_001.png", diff);
    return "difficulty_01_btn_001.png";
}

inline std::string fixColorCrashes(std::string input) {
    int tags = 0;

    std::string openingTag = "<c";
    for(std::string::size_type pos = 0; (pos = input.find(openingTag, pos)) != std::string::npos; tags++) {
        pos += openingTag.length();
    }

    std::string closingTag = "</c>";
    for(std::string::size_type pos = 0; (pos = input.find(closingTag, pos)) != std::string::npos; tags--) {
        pos += closingTag.length();
    }

    for(int i = 0; i < tags; i++) {
        input.append("  </c>");
    }

    return input;
}

inline std::string fixNullByteCrash(std::string input) {
    for(auto& character : input) {
        if(character == '\0') character = ' ';
    }
    return input;
}

inline std::string timeToString(time_t input) {
    char dateString[255];
    strftime(dateString, sizeof(dateString), "%F %H:%M", localtime(&input));
    return dateString;
}