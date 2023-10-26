#include "ListSearchObject.hpp"

#include <fmt/format.h>

std::string ListSearchObject::postRequest() {
	return fmt::format("gameVersion=22&binaryVersion=40&gdw=0&type={}&str={}&diff={}&star={}&page={}&secret=Wmfd2893gb7", m_type, m_str, m_diff, m_star, m_page);
}