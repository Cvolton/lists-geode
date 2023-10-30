#include "ListSearchObject.hpp"

#include <fmt/format.h>
#include <Geode/Geode.hpp>

using namespace geode::prelude;

std::string ListSearchObject::postRequest() {
	std::ostringstream typeSpecific;

	if(m_type == 12) {
		typeSpecific << "&followed=";
		bool useComma = false;

		auto dict = CCDictionaryExt<std::string, CCString>(GameLevelManager::sharedState()->m_followedCreators);
		for(auto [creator, yes] : dict) {
			typeSpecific << (useComma ? "," : "") << creator;
			useComma = true;
		}
	}

	return fmt::format("gameVersion=22&binaryVersion=40&gdw=0&type={}&str={}&diff={}&star={}&page={}&secret=Wmfd2893gb7{}", m_type, m_str, m_diff, m_star ? 1 : 0, m_page, typeSpecific.str());
}