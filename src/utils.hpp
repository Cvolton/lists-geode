#pragma once
#include <string>

inline std::string getDifficultyIcon(int diff){
    if(diff == -1) return "difficulty_00_btn_001.png";
    if(diff == 0) return "difficulty_auto_btn_001.png";
    if(diff <= 10) return fmt::format("difficulty_{:02}_btn_001.png", diff);
    return "difficulty_01_btn_001.png";
}