#include "ListSearchObject.hpp"

#include <fmt/format.h>
#include <Geode/Geode.hpp>
#include "include/sha1.hpp"

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

	auto GM = GameManager::sharedState();
	auto AM = GJAccountManager::sharedState();
	SHA1 gjp2;
	gjp2.update(fmt::format("{}mI29fmAnxgTs", std::string(AM->m_password)));

	return fmt::format("gameVersion=22&binaryVersion=40&gdw=0&uuid={}&udid={}&accountID={}&gjp2={}&type={}&str={}&diff={}&star={}&page={}&secret=Wmfd2893gb7{}", GM->m_playerUserID.value(), std::string(GM->m_playerUDID), AM->m_accountID, gjp2.final(), m_type, m_str, m_diff, m_star ? 1 : 0, m_page, typeSpecific.str());
}