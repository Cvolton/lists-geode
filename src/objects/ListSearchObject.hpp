#pragma once

struct ListSearchObject {
	int m_type = 0;
	std::string m_str;
	std::string m_diff;
	bool m_star = 0;
	bool m_followed = 0;
	int m_page = 0;

	std::string postRequest();
};