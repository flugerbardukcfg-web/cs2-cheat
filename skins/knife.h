#pragma once
#include "../../sdk/math/math.hpp"
#include <windows.h>
#include <map>
#include <vector>
#include <string>

struct knifeData_t {
	std::string name;
	std::string model_path;
	int def_index;
};

namespace knife_changer {
	void Run();

	inline std::vector < knifeData_t > knifes_data = {
		{ "M9 Bayonet", "weapons/models/knife/knife_m9/weapon_knife_m9.vmdl", 508 },
		{ "Classic", "weapons/models/knife/knife_css/weapon_knife_css.vmdl", 503 },
		{ "Bayonet", "weapons/models/knife/knife_bayonet/weapon_knife_bayonet.vmdl", 500 },
		{ "Flip", "weapons/models/knife/knife_flip/weapon_knife_flip.vmdl", 505 },
		{ "Gut", "weapons/models/knife/knife_gut/weapon_knife_gut.vmdl", 506 },
		{ "Karambit", "weapons/models/knife/knife_karambit/weapon_knife_karambit.vmdl", 507 },
		{ "Tactical", "weapons/models/knife/knife_tactical/weapon_knife_tactical.vmdl", 509 },
		{ "Falchion", "weapons/models/knife/knife_falchion/weapon_knife_falchion.vmdl", 512 },
		{ "Bowie", "weapons/models/knife/knife_bowie/weapon_knife_bowie.vmdl", 514 },
		{ "Butterfly", "weapons/models/knife/knife_butterfly/weapon_knife_butterfly.vmdl", 515 },
		{ "Push", "weapons/models/knife/knife_push/weapon_knife_push.vmdl", 516 },
		{ "Cord", "weapons/models/knife/knife_cord/weapon_knife_cord.vmdl", 517 },
		{ "Canis", "weapons/models/knife/knife_canis/weapon_knife_canis.vmdl", 518 },
		{ "Ursus", "weapons/models/knife/knife_ursus/weapon_knife_ursus.vmdl", 519 },
		{ "Navaja", "weapons/models/knife/knife_navaja/weapon_knife_navaja.vmdl", 520 },
		{ "Outdoor", "weapons/models/knife/knife_outdoor/weapon_knife_outdoor.vmdl", 521 },
		{ "Stiletto", "weapons/models/knife/knife_stiletto/weapon_knife_stiletto.vmdl", 522 },
		{ "Talon", "weapons/models/knife/knife_talon/weapon_knife_talon.vmdl", 523 },
		{ "Skeleton", "weapons/models/knife/knife_skeleton/weapon_knife_skeleton.vmdl", 525 },
		{ "Kukri", "weapons/models/knife/knife_kukri/weapon_knife_kukri.vmdl", 526 },
	};

	inline bool enable = false;
	inline int selected = 1;
}