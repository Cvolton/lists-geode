#pragma once

struct List {
	int m_id; //1
	std::string m_name; //2
	std::string m_desc; //3
	int m_version;
	int m_accountID;
	std::string m_userName;
	int m_downloads;
	int m_difficulty;
	int m_likes;
	int m_featuredIdx;
	std::string m_levels;
	int m_uploaded;
	int m_updated;

	List(std::unordered_map<int, std::string> parsedResponse);
	List() = default;

	std::vector<int> getLevelIDs() const;
	std::vector<int> getCompletedLevelIDs() const;
};