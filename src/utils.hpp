#pragma once
#include <string>

inline std::string getDifficultyIcon(int diff){
    if(diff == -2) return getDifficultyIcon(6);
    if(diff == -1) return "difficulty_00_btn_001.png";
    if(diff == 0 || diff == -3) return "difficulty_auto_btn_001.png";
    if(diff <= 10) return fmt::format("difficulty_{:02}_btn_001.png", diff);
    return getDifficultyIcon(0);
}

inline std::string getDemonDifficultyIcon(int diff){
    if(diff <= 5) return getDifficultyIcon(diff);
    if(diff <= 10) return fmt::format("difficulty_{:02}_btn2_001.png", diff);
    return getDemonDifficultyIcon(0);
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