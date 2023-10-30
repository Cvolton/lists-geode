#pragma once

struct ListSearchObject {
	int m_type = 0;
	std::string m_str;
	int m_diff = 0;
	bool m_star = 0;
	int m_page = 0;

	std::string postRequest();
};