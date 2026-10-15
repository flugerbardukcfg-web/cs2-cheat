#pragma once
#include "../../sdk/interfaces/interfaces.hpp"

namespace chams
{
	inline bool bChams;
	inline bool bChams_team;
	inline bool bChams_weapon;
	inline bool bChams_hands;
	inline bool bChams_ignoreZ;
	inline bool bChams_bloom;
	bool init();
	CMaterial2* create_visible(const char* name);
	CMaterial2* create_invisible_for_black(const char* name);
	CMaterial2* create_visible_for_black(const char* name);
	CMaterial2* create_invisible(const char* name);
	CMaterial2* create_bloom(const char* name);
	inline CMaterial2* invisibleforblackC{};
	inline CMaterial2* visibleforblackC{};
	inline CMaterial2* visibleC{};
	inline CMaterial2* invisibleC{};
	inline CMaterial2* bloomC{};
	inline bool chams_init = false;
	inline bool player = false;
	inline bool weapon = false;
	inline bool hands = false;
	inline bool invisible = false;
	inline bool bloom = false;
	inline int bloom_value = 50;
	inline int visible_value = 50;
	inline int invisible_value = 50;
	inline int weapon_value = 50;
	inline int hands_value = 50;
}
