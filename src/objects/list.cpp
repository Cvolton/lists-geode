#include "list.hpp"

#include <Geode/Geode.hpp>
using namespace geode::prelude;

List::List(std::unordered_map<int, std::string> parsedResponse) {
	//1:25:2:Best ViPriN Levels:3:OkQ=:5:1:49:1093804:50:AutoNick:10:3:7:7:14:1:19::51:1938283,1193581,1285881:28:1695502837:29:0
	m_id = std::stoi(parsedResponse[1]);
	m_name = parsedResponse[2];
	m_desc = parsedResponse[3];
	m_version = std::stoi(parsedResponse[5]);
	m_accountID = std::stoi(parsedResponse[49]);
	m_userName = parsedResponse[50];
	m_downloads = std::stoi(parsedResponse[10]);
	m_difficulty = std::stoi(parsedResponse[7]);
	m_likes = std::stoi(parsedResponse[14]);
	m_featuredIdx = std::stoi(parsedResponse[19] == "" ? "0" : parsedResponse[19]);
	m_levels = parsedResponse[51];
	m_uploaded = std::stoi(parsedResponse[28]);
	m_updated = std::stoi(parsedResponse[29]);
	m_55 = std::stoi(parsedResponse[55]);
	m_56 = std::stoi(parsedResponse[56]);
}

std::vector<int> List::getLevelIDs() const {
	std::vector<int> vector;

    std::stringstream responseStream(m_levels);
    std::string currentKey;

    while(getline(responseStream, currentKey, ',')){
        vector.push_back(std::stoi(currentKey));
    }

    return vector;
}

GJGameLevel* getLevelFromSaved(int levelID) {
    return static_cast<GJGameLevel*>(GameLevelManager::sharedState()->m_onlineLevels->objectForKey(std::to_string(levelID)));
}

std::vector<int> List::getCompletedLevelIDs() const {
	std::vector<int> vector = getLevelIDs();
	std::vector<int> vectorCopy;
	for(auto id : vector) {
		auto level = getLevelFromSaved(id);
		if(level && level->m_normalPercent == 100) vectorCopy.push_back(id);
	}
	return vectorCopy;
}

std::string List::getDescription() const {
	return LevelTools::base64DecodeString(m_desc);
}