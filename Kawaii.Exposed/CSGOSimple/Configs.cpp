#pragma once
#include "JSON.h"
#include "valve_sdk\csgostructs.hpp"
#include "options.hpp"
#include "Configs.h"
#include <unordered_map>
#include <array>
#include <algorithm>
#include <fstream>

class pstring : public std::string
{
public:
	pstring() : std::string() {}

	template<typename T>
	pstring(const T v) : std::string(v) {}

	template<typename T>
	pstring& operator<<(const T s)
	{
		std::stringstream stream;
		stream << *this;
		stream << s;
		*this = stream.str();
		return *this;
	}

	pstring& operator+(const unsigned int i)
	{
		std::stringstream stream;
		stream << *this;
		stream << i;
		*this = stream.str();
		return *this;
	}
};

namespace Configs
{
	void DumpClassIDs(const char* fileName)
	{
		auto stream = std::ofstream(fileName);

		stream << "--------------------[ KAWAII.EXPOSED CLASS IDs DUMP ]--------------------" << std::endl;
		stream << std::endl;
		stream << "enum ClassId" << std::endl;
		stream << "{" << std::endl;

		for (ClientClass* DumpClass = g_CHLClient->GetAllClasses(); DumpClass; DumpClass = DumpClass->m_pNext)
			stream << "\tClassId_" << DumpClass->m_pNetworkName << " = " << DumpClass->m_ClassID << "," << std::endl;

		stream << "}" << std::endl;
	}

	void GetVal(Json::Value &config, int* setting)
	{
		if (config.isNull())
			return;

		*setting = config.asInt();
	}

	void GetVal(Json::Value &config, bool* setting)
	{
		if (config.isNull())
			return;

		*setting = config.asBool();
	}

	void GetVal(Json::Value &config, float* setting)
	{
		if (config.isNull())
			return;

		*setting = config.asFloat();
	}

	void GetVal(Json::Value &config, Color* setting)
	{
		if (config.isNull())
			return;

		static int R, G, B, A;

		GetVal(config["R"], &R);
		GetVal(config["G"], &G);
		GetVal(config["B"], &B);
		GetVal(config["A"], &A);

		setting->SetColor(R, G, B, A);
	}

	void GetVal(Json::Value &config, char** setting)
	{
		if (config.isNull())
			return;

		*setting = strdup(config.asCString());
	}

	void GetVal(Json::Value &config, char* setting)
	{
		if (config.isNull())
			return;

		strcpy(setting, config.asCString());
	}

	void LoadColor(Json::Value &config, Color* color)
	{
		static int R, G, B, A;

		color->GetColor(R, G, B, A);

		config["R"] = R;
		config["G"] = G;
		config["B"] = B;
		config["A"] = A;
	}

	void SaveCFG(std::string fileName)
	{
		Json::Value settings;

		//
		// VISUALS
		//
		settings["Visuals"]["Player ESP"] = g_Options.esp_enabled;
		settings["Visuals"]["Enemies Only ESP"] = g_Options.esp_enemies_only;
		settings["Visuals"]["Flash Check"] = g_Options.esp_flash_check;
		settings["Visuals"]["Player Boxes"] = g_Options.esp_player_boxes;
		settings["Visuals"]["Player Names"] = g_Options.esp_player_names;
		settings["Visuals"]["Player Health"] = g_Options.esp_player_health;
		settings["Visuals"]["Player Armor"] = g_Options.esp_player_armour;
		settings["Visuals"]["Player Weapons"] = g_Options.esp_player_weapons;
		settings["Visuals"]["Player Weapons Text"] = g_Options.esp_player_weapons_type;
		settings["Visuals"]["Player Snaplines"] = g_Options.esp_player_snaplines;
		settings["Visuals"]["Crosshair"] = g_Options.esp_crosshair;
		settings["Visuals"]["Dropped Weapons ESP"] = g_Options.esp_dropped_weapons;
		settings["Visuals"]["Defuse Kit ESP"] = g_Options.esp_defuse_kit;
		settings["Visuals"]["Planted C4 ESP"] = g_Options.esp_planted_c4;
		settings["Visuals"]["Planted C4 Boxes"] = g_Options.esp_planted_c4_boxes;
		settings["Visuals"]["Planted C4 Text"] = g_Options.esp_planted_c4_text;
		settings["Visuals"]["Defuse Kit Boxes"] = g_Options.esp_defuse_kit_boxes;
		settings["Visuals"]["Defuse Kit Text"] = g_Options.esp_defuse_kit_text;
		settings["Visuals"]["Dropped Weapons Boxes"] = g_Options.esp_dropped_weapons_boxes;
		settings["Visuals"]["Dropped Weapons Text"] = g_Options.esp_dropped_weapons_text;
		settings["Visuals"]["No Visual Recoil"] = g_Options.visuals_no_aimpunch;
		settings["Visuals"]["Glow"] = g_Options.glow_enabled;
		settings["Visuals"]["Enemies Only Glow"] = g_Options.glow_enemies_only;
		settings["Visuals"]["Glow Style"] = g_Options.glow_style;
		settings["Visuals"]["Glow Players"] = g_Options.glow_players;
		settings["Visuals"]["Glow Chickens"] = g_Options.glow_chickens;
		settings["Visuals"]["Glow C4 Carrier"] = g_Options.glow_c4_carrier;
		settings["Visuals"]["Glow Planted C4"] = g_Options.glow_planted_c4;
		settings["Visuals"]["Glow Defuse Kits"] = g_Options.glow_defuse_kits;
		settings["Visuals"]["Glow Weapons"] = g_Options.glow_weapons;
		settings["Visuals"]["Chams Players"] = g_Options.chams_player_enabled;
		settings["Visuals"]["Chams Enemies Only"] = g_Options.chams_player_enemies_only;
		settings["Visuals"]["Chams Player Wireframe"] = g_Options.chams_player_wireframe;
		settings["Visuals"]["Chams Player XQZ"] = g_Options.chams_player_xqz;
		settings["Visuals"]["Chams Player Mode"] = g_Options.chams_player_mode;
		settings["Visuals"]["Chams Arms"] = g_Options.chams_arms_enabled;
		settings["Visuals"]["Chams Arms Wireframe"] = g_Options.chams_arms_wireframe;
		settings["Visuals"]["Chams Arms XQZ"] = g_Options.chams_arms_xqz;
		settings["Visuals"]["Chams Arms Mode"] = g_Options.chams_arms_mode;
		settings["Visuals"]["Night Mode"] = g_Options.nightmode;
		settings["Visuals"]["Disable Post Processing"] = g_Options.disable_post_processing;
		settings["Visuals"]["Grenade Prediction"] = g_Options.grenade_path_esp;
		settings["Visuals"]["No Hands"] = g_Options.misc_no_hands;
		settings["Visuals"]["Viewmodel FOV"] = g_Options.viewmodel_fov;
		settings["Visuals"]["Camera FOV"] = g_Options.fov;
		settings["Visuals"]["Ambient Light R"] = g_Options.mat_ambient_light_r;
		settings["Visuals"]["Ambient Light G"] = g_Options.mat_ambient_light_g;
		settings["Visuals"]["Ambient Light B"] = g_Options.mat_ambient_light_b;
		settings["Visuals"]["Paper"] = g_Options.paper;
		settings["Visuals"]["Minecraft"] = g_Options.lowrestextures;
		settings["Visuals"]["Sky"] = g_Options.sky;
		settings["Visuals"]["Spectator List"] = g_Options.spec_list;
		settings["Visuals"]["Spectator List Player Only"] = g_Options.spec_list_player_only;
		settings["Visuals"]["Watermarks"] = g_Options.watermarks;
		settings["Visuals"]["Translucent"] = g_Options.chams_translucent;
		settings["Visuals"]["Aimbot FOV Circle"] = g_Options.visuals_fov_circle;
		settings["Visuals"]["Nightmode"] = g_Options.nightmode;
		
		//
		// MISC
		//
		settings["Misc"]["Autoaccept"] = g_Options.autoaccept;
		settings["Misc"]["Third Person"] = g_Options.thirdperson;
		settings["Misc"]["Third Person Range"] = g_Options.thirdpersonrange;
		settings["Misc"]["Rank Reveal"] = g_Options.rank_reveal;
		settings["Misc"]["Bunnyhop Key"] = g_Options.misc_bhop_key;
		settings["Misc"]["Moonwalk"] = g_Options.memewalk;
		settings["Misc"]["Name"] = g_Options.name;
		settings["Misc"]["Clantag"] = g_Options.clantag;
		settings["Misc"]["Airstuck Key"] = g_Options.airstuck_key;
		settings["Misc"]["Autostrafer"] = g_Options.autostrafer;
		settings["Misc"]["Anti-Untrusted"] = g_Options.antiuntrusted;
		settings["Misc"]["Hitsound"] = g_Options.hitmarkers_sound;
		settings["Misc"]["Hitmarker"] = g_Options.hitmarkers;
		settings["Misc"]["No Flash"] = g_Options.noflash;


		//
		// SKINS
		//
		settings["Skins"]["Knife"] = g_Options.knife;
		settings["Skins"]["Knife Skin"] = g_Options.knife_skin;
		settings["Skins"]["Glove"] = g_Options.glove;
		settings["Skins"]["Glove Skin"] = g_Options.glove_skin;
		settings["Skins"]["AK47"] = g_Options.ak47;
		settings["Skins"]["GalilAR"] = g_Options.galil;
		settings["Skins"]["SSG08"] = g_Options.ssg08;
		settings["Skins"]["SG553"] = g_Options.sg553;
		settings["Skins"]["AWP"] = g_Options.awp;
		settings["Skins"]["G3SG1"] = g_Options.g3sg1;
		settings["Skins"]["Famas"] = g_Options.famas;
		settings["Skins"]["M4A4"] = g_Options.m4a4;
		settings["Skins"]["M4A1s"] = g_Options.m4a1s;
		settings["Skins"]["AUG"] = g_Options.aug;
		settings["Skins"]["SCAR20"] = g_Options.scar20;
		settings["Skins"]["Nova"] = g_Options.nova;
		settings["Skins"]["XM1014"] = g_Options.xm1014;
		settings["Skins"]["MAG7"] = g_Options.mag7;
		settings["Skins"]["SawedOff"] = g_Options.sawedoff;
		settings["Skins"]["M249"] = g_Options.m249;
		settings["Skins"]["Negev"] = g_Options.negev;
		settings["Skins"]["P2000"] = g_Options.p2000;
		settings["Skins"]["USPS"] = g_Options.usps;
		settings["Skins"]["Glock"] = g_Options.glock;
		settings["Skins"]["Dualies"] = g_Options.elites;
		settings["Skins"]["P250"] = g_Options.p250;
		settings["Skins"]["FiveSeven"] = g_Options.fiveseven;
		settings["Skins"]["Deagle"] = g_Options.deagle;
		settings["Skins"]["CZ7A"] = g_Options.cz75a;
		settings["Skins"]["Tec9"] = g_Options.tec9;
		settings["Skins"]["Revolver"] = g_Options.revolver;
		settings["Skins"]["MP9"] = g_Options.mp9;
		settings["Skins"]["MAC10"] = g_Options.mac10;
		settings["Skins"]["MP7"] = g_Options.mp7;
		settings["Skins"]["UMP45"] = g_Options.ump45;
		settings["Skins"]["P90"] = g_Options.p90;
		settings["Skins"]["Bizon"] = g_Options.bizon;
		settings["Skins"]["Player Model"] = g_Options.player_model;
		settings["Skins"]["Knife Model"] = g_Options.knife_model;

		//
		// AIMBOT / HVH
		//
		settings["Aimbot"]["Edge Distance"] = g_Options.antiaim_edge_dist;
		settings["Aimbot"]["Anti-Resolver"] = g_Options.antiaim_antiresolver;
		settings["Aimbot"]["Antiaim Pitch"] = g_Options.antiaim_pitch;
		settings["Aimbot"]["Antiaim Yaw"] = g_Options.antiaim_yaw;
		settings["Aimbot"]["Antiaim Fake Yaw"] = g_Options.antiaim_yaw_fake;
		settings["Aimbot"]["Third Person Angles"] = g_Options.antiaim_thirdperson_angle;
		settings["Aimbot"]["Spin Speed"] = g_Options.antiaim_spin_speed;
		settings["Aimbot"]["Tank Antiaim Key"] = g_Options.tankAntiaimKey;
		settings["Aimbot"]["Antiaim On Knife"] = g_Options.antiaim_knife;
		settings["Aimbot"]["RCS Amount"] = g_Options.rcs_amount;
		settings["Aimbot"]["Fake Lag Amount"] = g_Options.fakelag_amount;
		settings["Aimbot"]["Adaptive Fake Lag"] = g_Options.fakelag_adaptive;
		settings["Aimbot"]["Fake Walk Key"] = g_Options.fakewalk_key;
		settings["Aimbot"]["Resolver"] = g_Options.resolver;
		settings["Aimbot"]["Hitchance"] = g_Options.hitchance;

		settings["Aimbot"]["Aimbot FOV"] = g_Options.aimbot_AimbotFOV;
		settings["Aimbot"]["Aimbot Smooth"] = g_Options.aimbot_smoothness;
		settings["Aimbot"]["AimSpot"] = g_Options.legit_aimspot;
		settings["Aimbot"]["Aimbot Silent"] = g_Options.aimbot_silent;
		settings["Aimbot"]["Aimbot Key"] = g_Options.aimbot_key;
		//settings["Aimbot"]["Rage"] = g_Options.aimbot_rage;
		settings["Aimbot"]["No Recoil"] = g_Options.no_recoil;
		settings["Aimbot"]["Backtracking"] = g_Options.backtracking;
		settings["Aimbot"]["Backtracking Tracer"] = g_Options.backtracking_tracer;
		settings["Aimbot"]["Autoshoot"] = g_Options.autoshoot;
		settings["Aimbot"]["Visibility Check"] = g_Options.vischeck;


		settings["Aimbot"]["Pistol Aimbot FOV"] = g_Options.pistol_aimbot_AimbotFOV;
		settings["Aimbot"]["Pistol Aimbot Smooth"] = g_Options.pistol_aimbot_smoothness;
		settings["Aimbot"]["Pistol AimSpot"] = g_Options.pistol_legit_aimspot;
		settings["Aimbot"]["Pistol Aimbot Silent"] = g_Options.pistol_aimbot_silent;
		settings["Aimbot"]["Pistol Aimbot Key"] = g_Options.pistol_aimbot_key;
		settings["Aimbot"]["Pistol Backtracking"] = g_Options.pistol_backtracking;
		settings["Aimbot"]["Pistol Recoil"] = g_Options.pistol_rcs_amount;
		settings["Aimbot"]["Pistol Visibility Check"] = g_Options.pistol_vischeck;
		settings["Aimbot"]["Pistol Backtracking Ticks"] = g_Options.pistol_backtracking_ticks;
		settings["Aimbot"]["Pistol Standalone RCS"] = g_Options.pistol_standalone_rcs;

		settings["Aimbot"]["Rifle Aimbot FOV"] = g_Options.rifle_aimbot_AimbotFOV;
		settings["Aimbot"]["Rifle Aimbot Smooth"] = g_Options.rifle_aimbot_smoothness;
		settings["Aimbot"]["Rifle AimSpot"] = g_Options.rifle_legit_aimspot;
		settings["Aimbot"]["Rifle Aimbot Silent"] = g_Options.rifle_aimbot_silent;
		settings["Aimbot"]["Rifle Aimbot Key"] = g_Options.rifle_aimbot_key;
		settings["Aimbot"]["Rifle Backtracking"] = g_Options.rifle_backtracking;
		settings["Aimbot"]["Rifle Recoil"] = g_Options.rifle_rcs_amount;
		settings["Aimbot"]["Rifle Visibility Check"] = g_Options.rifle_vischeck;
		settings["Aimbot"]["Rifle Backtracking Ticks"] = g_Options.rifle_backtracking_ticks;
		settings["Aimbot"]["Rifle Standalone RCS"] = g_Options.rifle_standalone_rcs;

		settings["Aimbot"]["Sniper Aimbot FOV"] = g_Options.sniper_aimbot_AimbotFOV;
		settings["Aimbot"]["Sniper Aimbot Smooth"] = g_Options.sniper_aimbot_smoothness;
		settings["Aimbot"]["Sniper AimSpot"] = g_Options.sniper_legit_aimspot;
		settings["Aimbot"]["Sniper Aimbot Silent"] = g_Options.sniper_aimbot_silent;
		settings["Aimbot"]["Sniper Aimbot Key"] = g_Options.sniper_aimbot_key;
		settings["Aimbot"]["Sniper Backtracking"] = g_Options.sniper_backtracking;
		settings["Aimbot"]["Sniper Recoil"] = g_Options.sniper_rcs_amount;
		settings["Aimbot"]["Sniper Visibility Check"] = g_Options.sniper_vischeck;
		settings["Aimbot"]["Sniper Backtracking Ticks"] = g_Options.sniper_backtracking_ticks;
		settings["Aimbot"]["Sniper Standalone RCS"] = g_Options.sniper_standalone_rcs;

		settings["Aimbot"]["Shotgun Aimbot FOV"] = g_Options.shotgun_aimbot_AimbotFOV;
		settings["Aimbot"]["Shotgun Aimbot Smooth"] = g_Options.shotgun_aimbot_smoothness;
		settings["Aimbot"]["Shotgun AimSpot"] = g_Options.shotgun_legit_aimspot;
		settings["Aimbot"]["Shotgun Aimbot Silent"] = g_Options.shotgun_aimbot_silent;
		settings["Aimbot"]["Shotgun Aimbot Key"] = g_Options.shotgun_aimbot_key;
		settings["Aimbot"]["Shotgun Backtracking"] = g_Options.shotgun_backtracking;
		settings["Aimbot"]["Shotgun Recoil"] = g_Options.shotgun_rcs_amount;
		settings["Aimbot"]["Shotgun Visibility Check"] = g_Options.shotgun_vischeck;
		settings["Aimbot"]["Shotgun Backtracking Ticks"] = g_Options.shotgun_backtracking_ticks;
		settings["Aimbot"]["Shotgun Standalone RCS"] = g_Options.shotgun_standalone_rcs;

		settings["Aimbot"]["SMG Aimbot FOV"] = g_Options.smg_aimbot_AimbotFOV;
		settings["Aimbot"]["SMG Aimbot Smooth"] = g_Options.smg_aimbot_smoothness;
		settings["Aimbot"]["SMG AimSpot"] = g_Options.smg_legit_aimspot;
		settings["Aimbot"]["SMG Aimbot Silent"] = g_Options.smg_aimbot_silent;
		settings["Aimbot"]["SMG Aimbot Key"] = g_Options.smg_aimbot_key;
		settings["Aimbot"]["SMG Backtracking"] = g_Options.smg_backtracking;
		settings["Aimbot"]["SMG Recoil"] = g_Options.smg_rcs_amount;
		settings["Aimbot"]["SMG Visibility Check"] = g_Options.smg_vischeck;
		settings["Aimbot"]["SMG Standalone RCS"] = g_Options.smg_standalone_rcs;

		settings["Aimbot"]["Autowall"] = g_Options.autowall;
		settings["Aimbot"]["Autowall min damage"] = g_Options.autowall_min_damage;
		settings["Aimbot"]["Autoscope"] = g_Options.autoscope;
		//
		// TRIGGERBOT 
		//
		settings["Triggerbot"]["Active"] = g_Options.triggerbotactive;
		settings["Triggerbot"]["Trigger Key"] = g_Options.triggerbotkey;
		settings["Triggerbot"]["Trigger Spot"] = g_Options.triggerbot_spot;

		// 
		// COLORS
		//
		LoadColor(settings["Colors"]["Ally Visible Color"], &g_Options.color_esp_ally_visible);
		LoadColor(settings["Colors"]["Ally Occluded Color"], &g_Options.color_esp_ally_occluded);
		LoadColor(settings["Colors"]["Enemy Visible Color"], &g_Options.color_esp_enemy_visible);
		LoadColor(settings["Colors"]["Enemy Occluded Color"], &g_Options.color_esp_enemy_occluded);
		LoadColor(settings["Colors"]["Weapon ESP Color"], &g_Options.color_esp_weapons);
		LoadColor(settings["Colors"]["Defuse Kit ESP Color"], &g_Options.color_esp_defuse);
		LoadColor(settings["Colors"]["C4 ESP Color"], &g_Options.color_esp_c4);
		LoadColor(settings["Colors"]["Ally Glow Color"], &g_Options.color_glow_ally);
		LoadColor(settings["Colors"]["Enemy Glow Color"], &g_Options.color_glow_enemy);
		LoadColor(settings["Colors"]["Chicken Glow Color"], &g_Options.color_glow_chickens);
		LoadColor(settings["Colors"]["C4 Carrier Glow Color"], &g_Options.color_glow_c4_carrier);
		LoadColor(settings["Colors"]["Planted C4 Glow Color"], &g_Options.color_glow_planted_c4);
		LoadColor(settings["Colors"]["Defuse Kit Glow Color"], &g_Options.color_glow_defuse);
		LoadColor(settings["Colors"]["Weapons Glow Color"], &g_Options.color_glow_weapons);
		LoadColor(settings["Colors"]["Ally Chams Visible Color"], &g_Options.color_chams_player_ally_visible);
		LoadColor(settings["Colors"]["Ally Chams Occluded Color"], &g_Options.color_chams_player_ally_occluded);
		LoadColor(settings["Colors"]["Enemy Chams Visible Color"], &g_Options.color_chams_player_enemy_visible);
		LoadColor(settings["Colors"]["Enemy Chams Occluded Color"], &g_Options.color_chams_player_enemy_occluded);
		LoadColor(settings["Colors"]["Arm Chams Visible Color"], &g_Options.color_chams_arms_visible);
		LoadColor(settings["Colors"]["Arm Chams Occluded Color"], &g_Options.color_chams_arms_occluded);

		//
		// MENU
		//
		LoadColor(settings["Menu"]["Menu Color"], &g_Options.color_menu_main);
		LoadColor(settings["Menu"]["Menu Accents Color"], &g_Options.color_menu_body);
		LoadColor(settings["Menu"]["Menu Font Color"], &g_Options.color_menu_font);

		settings["Menu"]["Menu Fade Speed"] = g_Options.menu_fade_speed;

		//LoadColor(settings["Menu"]["Menu Hover Color"], &g_Options.menu_hover);
		//settings["Menu"]["Menu Roundness"] = g_Options.menu_roundness;

		Json::StyledWriter styledWriter;
		std::string strJson = styledWriter.write(settings);
		FILE* file = fopen(fileName.c_str(), "w+");
		if (file)
		{
			fwrite(strJson.c_str(), 1, strJson.length(), file);
			fclose(file);
		}
	}

	void LoadCFG(std::string fileName)
	{
		FILE* infile = fopen(fileName.c_str(), "r");

		if (!infile)
		{
			SaveCFG(fileName);
			return;
		}

		fseek(infile, 0, SEEK_END);
		long filesize = ftell(infile);
		char* buf = new char[filesize + 1];
		fseek(infile, 0, SEEK_SET);
		fread(buf, 1, filesize, infile);
		fclose(infile);

		buf[filesize] = '\0';
		std::stringstream ss;
		ss.str(buf);
		delete[] buf;

		Json::Value settings;
		ss >> settings;

		//
		// VISUALS
		//
		GetVal(settings["Visuals"]["Player ESP"], &g_Options.esp_enabled);
		GetVal(settings["Visuals"]["Enemies Only ESP"], &g_Options.esp_enemies_only);
		GetVal(settings["Visuals"]["Flash Check"], &g_Options.esp_flash_check);
		GetVal(settings["Visuals"]["Player Boxes"], &g_Options.esp_player_boxes);
		GetVal(settings["Visuals"]["Player Names"], &g_Options.esp_player_names);
		GetVal(settings["Visuals"]["Player Health"], &g_Options.esp_player_health);
		GetVal(settings["Visuals"]["Player Armor"], &g_Options.esp_player_armour);
		GetVal(settings["Visuals"]["Player Weapons"], &g_Options.esp_player_weapons);
		GetVal(settings["Visuals"]["Player Weapons Text"], &g_Options.esp_player_weapons_type);
		GetVal(settings["Visuals"]["Player Snaplines"], &g_Options.esp_player_snaplines);
		GetVal(settings["Visuals"]["Crosshair"], &g_Options.esp_crosshair);
		GetVal(settings["Visuals"]["Dropped Weapons ESP"], &g_Options.esp_dropped_weapons);
		GetVal(settings["Visuals"]["Defuse Kit ESP"], &g_Options.esp_defuse_kit);
		GetVal(settings["Visuals"]["Planted C4 ESP"], &g_Options.esp_planted_c4);
		GetVal(settings["Visuals"]["Planted C4 Boxes"], &g_Options.esp_planted_c4_boxes);
		GetVal(settings["Visuals"]["Planted C4 Text"], &g_Options.esp_planted_c4_text);
		GetVal(settings["Visuals"]["Defuse Kit Boxes"], &g_Options.esp_defuse_kit_boxes);
		GetVal(settings["Visuals"]["Defuse Kit Text"], &g_Options.esp_defuse_kit_text);
		GetVal(settings["Visuals"]["Dropped Weapons Boxes"], &g_Options.esp_dropped_weapons_boxes);
		GetVal(settings["Visuals"]["Dropped Weapons Text"], &g_Options.esp_dropped_weapons_text);
		GetVal(settings["Visuals"]["No Visual Recoil"], &g_Options.visuals_no_aimpunch);
		GetVal(settings["Visuals"]["Glow"], &g_Options.glow_enabled);
		GetVal(settings["Visuals"]["Enemies Only Glow"], &g_Options.glow_enemies_only);
		GetVal(settings["Visuals"]["Glow Style"], &g_Options.glow_style);
		GetVal(settings["Visuals"]["Glow Players"], &g_Options.glow_players);
		GetVal(settings["Visuals"]["Glow Chickens"], &g_Options.glow_chickens);
		GetVal(settings["Visuals"]["Glow C4 Carrier"], &g_Options.glow_c4_carrier);
		GetVal(settings["Visuals"]["Glow Planted C4"], &g_Options.glow_planted_c4);
		GetVal(settings["Visuals"]["Glow Defuse Kits"], &g_Options.glow_defuse_kits);
		GetVal(settings["Visuals"]["Glow Weapons"], &g_Options.glow_weapons);
		GetVal(settings["Visuals"]["Chams Players"], &g_Options.chams_player_enabled);
		GetVal(settings["Visuals"]["Chams Enemies Only"], &g_Options.chams_player_enemies_only);
		GetVal(settings["Visuals"]["Chams Player Wireframe"], &g_Options.chams_player_wireframe);
		GetVal(settings["Visuals"]["Chams Player XQZ"], &g_Options.chams_player_xqz);
		GetVal(settings["Visuals"]["Chams Player Mode"], &g_Options.chams_player_mode);
		GetVal(settings["Visuals"]["Chams Arms"], &g_Options.chams_arms_enabled);
		GetVal(settings["Visuals"]["Chams Arms Wireframe"], &g_Options.chams_arms_wireframe);
		GetVal(settings["Visuals"]["Chams Arms XQZ"], &g_Options.chams_arms_xqz);
		GetVal(settings["Visuals"]["Chams Arms Mode"], &g_Options.chams_arms_mode);
		GetVal(settings["Visuals"]["Night Mode"], &g_Options.nightmode);
		GetVal(settings["Visuals"]["Disable Post Processing"], &g_Options.disable_post_processing);
		GetVal(settings["Visuals"]["Grenade Prediction"], &g_Options.grenade_path_esp);
		GetVal(settings["Visuals"]["No Hands"], &g_Options.misc_no_hands);
		GetVal(settings["Visuals"]["Viewmodel FOV"], &g_Options.viewmodel_fov);
		GetVal(settings["Visuals"]["Camera FOV"], &g_Options.fov);
		GetVal(settings["Visuals"]["Ambient Light R"], &g_Options.mat_ambient_light_r);
		GetVal(settings["Visuals"]["Ambient Light G"], &g_Options.mat_ambient_light_g);
		GetVal(settings["Visuals"]["Ambient Light B"], &g_Options.mat_ambient_light_b);
		GetVal(settings["Visuals"]["Paper"], &g_Options.paper);
		GetVal(settings["Visuals"]["Minecraft"], &g_Options.lowrestextures);
		GetVal(settings["Visuals"]["Sky"], &g_Options.sky);
		GetVal(settings["Visuals"]["Spectator List"], &g_Options.spec_list);
		GetVal(settings["Visuals"]["Watermarks"], &g_Options.watermarks);
		GetVal(settings["Visuals"]["Spectator List Player Only"], &g_Options.spec_list_player_only);
		GetVal(settings["Visuals"]["Translucent"], &g_Options.chams_translucent);
		GetVal(settings["Visuals"]["No Flash"], &g_Options.noflash);
		GetVal(settings["Visuals"]["Aimbot FOV Circle"], &g_Options.visuals_fov_circle);
		//
		// MISC
		//
		GetVal(settings["Misc"]["Autoaccept"], &g_Options.autoaccept);
		GetVal(settings["Misc"]["Third Person"], &g_Options.thirdperson);
		GetVal(settings["Misc"]["Third Person Range"], &g_Options.thirdpersonrange);
		GetVal(settings["Misc"]["Rank Reveal"], &g_Options.rank_reveal);
		GetVal(settings["Misc"]["Bunnyhop Key"], &g_Options.misc_bhop_key);
		GetVal(settings["Misc"]["Moonwalk"], &g_Options.memewalk);
		GetVal(settings["Misc"]["Name"], &g_Options.name);
		GetVal(settings["Misc"]["Clantag"], &g_Options.clantag);
		GetVal(settings["Misc"]["Airstuck Key"], &g_Options.airstuck_key);
		GetVal(settings["Misc"]["Autostrafer"], &g_Options.autostrafer);
		GetVal(settings["Misc"]["Anti-Untrusted"], &g_Options.antiuntrusted);
		GetVal(settings["Misc"]["Hitsound"], &g_Options.hitmarkers_sound);
		GetVal(settings["Misc"]["Hitmarker"], &g_Options.hitmarkers);
		GetVal(settings["Aimbot"]["Autowall"], &g_Options.autowall);
		GetVal(settings["Aimbot"]["Autowall min damage"], &g_Options.autowall_min_damage);
		GetVal(settings["Aimbot"]["Autoscope"], &g_Options.autoscope);
		//
		// SKINS
		//
		GetVal(settings["Skins"]["Knife"], &g_Options.knife);
		GetVal(settings["Skins"]["Knife Skin"], &g_Options.knife_skin);
		GetVal(settings["Skins"]["Glove"], &g_Options.glove);
		GetVal(settings["Skins"]["Glove Skin"], &g_Options.glove_skin);
		GetVal(settings["Skins"]["AK47"], &g_Options.ak47);
		GetVal(settings["Skins"]["GalilAR"], &g_Options.galil);
		GetVal(settings["Skins"]["SSG08"], &g_Options.ssg08);
		GetVal(settings["Skins"]["SG553"], &g_Options.sg553);
		GetVal(settings["Skins"]["AWP"], &g_Options.awp);
		GetVal(settings["Skins"]["G3SG1"], &g_Options.g3sg1);
		GetVal(settings["Skins"]["Famas"], &g_Options.famas);
		GetVal(settings["Skins"]["M4A4"], &g_Options.m4a4);
		GetVal(settings["Skins"]["M4A1s"], &g_Options.m4a1s);
		GetVal(settings["Skins"]["AUG"], &g_Options.aug);
		GetVal(settings["Skins"]["SCAR20"], &g_Options.scar20);
		GetVal(settings["Skins"]["Nova"], &g_Options.nova);
		GetVal(settings["Skins"]["XM1014"], &g_Options.xm1014);
		GetVal(settings["Skins"]["MAG7"], &g_Options.mag7);
		GetVal(settings["Skins"]["SawedOff"], &g_Options.sawedoff);
		GetVal(settings["Skins"]["M249"], &g_Options.m249);
		GetVal(settings["Skins"]["Negev"], &g_Options.negev);
		GetVal(settings["Skins"]["P2000"], &g_Options.p2000);
		GetVal(settings["Skins"]["USPS"], &g_Options.usps);
		GetVal(settings["Skins"]["Glock"], &g_Options.glock);
		GetVal(settings["Skins"]["Dualies"], &g_Options.elites);
		GetVal(settings["Skins"]["P250"], &g_Options.p250);
		GetVal(settings["Skins"]["FiveSeven"], &g_Options.fiveseven);
		GetVal(settings["Skins"]["Deagle"], &g_Options.deagle);
		GetVal(settings["Skins"]["CZ7A"], &g_Options.cz75a);
		GetVal(settings["Skins"]["Tec9"], &g_Options.tec9);
		GetVal(settings["Skins"]["Revolver"], &g_Options.revolver);
		GetVal(settings["Skins"]["MP9"], &g_Options.mp9);
		GetVal(settings["Skins"]["MAC10"], &g_Options.mac10);
		GetVal(settings["Skins"]["MP7"], &g_Options.mp7);
		GetVal(settings["Skins"]["UMP45"], &g_Options.ump45);
		GetVal(settings["Skins"]["P90"], &g_Options.p90);
		GetVal(settings["Skins"]["Bizon"], &g_Options.bizon);
		//
		// AIMBOT / HVH
		//
		GetVal(settings["Aimbot"]["Edge Distance"], &g_Options.antiaim_edge_dist);
		GetVal(settings["Aimbot"]["Anti-Resolver"], &g_Options.antiaim_antiresolver);
		GetVal(settings["Aimbot"]["Antiaim Pitch"], &g_Options.antiaim_pitch);
		GetVal(settings["Aimbot"]["Antiaim Yaw"], &g_Options.antiaim_yaw);
		GetVal(settings["Aimbot"]["Antiaim Fake Yaw"], &g_Options.antiaim_yaw_fake);
		GetVal(settings["Aimbot"]["Third Person Angles"], &g_Options.antiaim_thirdperson_angle);
		GetVal(settings["Aimbot"]["Spin Speed"], &g_Options.antiaim_spin_speed);
		GetVal(settings["Aimbot"]["Tank Antiaim Key"], &g_Options.tankAntiaimKey);
		GetVal(settings["Aimbot"]["Antiaim On Knife"], &g_Options.antiaim_knife);
		GetVal(settings["Aimbot"]["RCS Amount"], &g_Options.rcs_amount);
		GetVal(settings["Aimbot"]["Fake Lag Amount"], &g_Options.fakelag_amount);
		GetVal(settings["Aimbot"]["Adaptive Fake Lag"], &g_Options.fakelag_adaptive);
		GetVal(settings["Aimbot"]["Fake Walk Key"], &g_Options.fakewalk_key);
		GetVal(settings["Aimbot"]["Resolver"], &g_Options.resolver);
		GetVal(settings["Aimbot"]["Aimbot FOV"], &g_Options.aimbot_AimbotFOV);
		GetVal(settings["Aimbot"]["Aimbot Silent"], &g_Options.aimbot_silent);
		GetVal(settings["Aimbot"]["AimSpot"], &g_Options.legit_aimspot);
		GetVal(settings["Aimbot"]["Aimbot Key"], &g_Options.aimbot_key);
		//GetVal(settings["Aimbot"]["Rage"], &g_Options.aimbot_rage);
		GetVal(settings["Aimbot"]["No Recoil"], &g_Options.no_recoil);
		GetVal(settings["Aimbot"]["Aimbot Smooth"], &g_Options.aimbot_smoothness);
		GetVal(settings["Aimbot"]["Backtracking"], &g_Options.backtracking);
		GetVal(settings["Aimbot"]["Backtracking Tracer"], &g_Options.backtracking_tracer);
		GetVal(settings["Aimbot"]["Autoshoot"], &g_Options.autoshoot);
		GetVal(settings["Aimbot"]["Visibility Check"], &g_Options.vischeck);
		GetVal(settings["Aimbot"]["Hitchance"], &g_Options.hitchance);


		GetVal(settings["Aimbot"]["Pistol Aimbot FOV"], &g_Options.pistol_aimbot_AimbotFOV);
		GetVal(settings["Aimbot"]["Pistol Aimbot Smooth"], &g_Options.pistol_aimbot_smoothness);
		GetVal(settings["Aimbot"]["Pistol AimSpot"], &g_Options.pistol_legit_aimspot);
		GetVal(settings["Aimbot"]["Pistol Aimbot Silent"], &g_Options.pistol_aimbot_silent);
		GetVal(settings["Aimbot"]["Pistol Aimbot Key"], &g_Options.pistol_aimbot_key);
		GetVal(settings["Aimbot"]["Pistol Backtracking"], &g_Options.pistol_backtracking);
		GetVal(settings["Aimbot"]["Pistol Recoil"], &g_Options.pistol_rcs_amount);
		GetVal(settings["Aimbot"]["Pistol Visibility Check"], &g_Options.pistol_vischeck);
		GetVal(settings["Aimbot"]["Pistol Backtracking Ticks"], &g_Options.pistol_backtracking_ticks);
		GetVal(settings["Aimbot"]["Pistol Standalone RCS"], &g_Options.pistol_standalone_rcs);

		GetVal(settings["Aimbot"]["Rifle Aimbot FOV"], &g_Options.rifle_aimbot_AimbotFOV);
		GetVal(settings["Aimbot"]["Rifle Aimbot Smooth"], &g_Options.rifle_aimbot_smoothness);
		GetVal(settings["Aimbot"]["Rifle AimSpot"], &g_Options.rifle_legit_aimspot);
		GetVal(settings["Aimbot"]["Rifle Aimbot Silent"], &g_Options.rifle_aimbot_silent);
		GetVal(settings["Aimbot"]["Rifle Aimbot Key"], &g_Options.rifle_aimbot_key);
		GetVal(settings["Aimbot"]["Rifle Backtracking"], &g_Options.rifle_backtracking);
		GetVal(settings["Aimbot"]["Rifle Recoil"], &g_Options.rifle_rcs_amount);
		GetVal(settings["Aimbot"]["Rifle Visibility Check"], &g_Options.rifle_vischeck);
		GetVal(settings["Aimbot"]["Rifle Backtracking Ticks"], &g_Options.rifle_backtracking_ticks);
		GetVal(settings["Aimbot"]["Rifle Standalone RCS"], &g_Options.rifle_standalone_rcs);

		GetVal(settings["Aimbot"]["Sniper Aimbot FOV"], &g_Options.sniper_aimbot_AimbotFOV);
		GetVal(settings["Aimbot"]["Sniper Aimbot Smooth"], &g_Options.sniper_aimbot_smoothness);
		GetVal(settings["Aimbot"]["Sniper AimSpot"], &g_Options.sniper_legit_aimspot);
		GetVal(settings["Aimbot"]["Sniper Aimbot Silent"], &g_Options.sniper_aimbot_silent);
		GetVal(settings["Aimbot"]["Sniper Aimbot Key"], &g_Options.sniper_aimbot_key);
		GetVal(settings["Aimbot"]["Sniper Backtracking"], &g_Options.sniper_backtracking);
		GetVal(settings["Aimbot"]["Sniper Recoil"], &g_Options.sniper_rcs_amount);
		GetVal(settings["Aimbot"]["Sniper Visibility Check"], &g_Options.sniper_vischeck);
		GetVal(settings["Aimbot"]["Sniper Backtracking Ticks"], &g_Options.sniper_backtracking_ticks);
		GetVal(settings["Aimbot"]["Sniper Standalone RCS"], &g_Options.sniper_standalone_rcs);

		GetVal(settings["Aimbot"]["Shotgun Aimbot FOV"], &g_Options.shotgun_aimbot_AimbotFOV);
		GetVal(settings["Aimbot"]["Shotgun Aimbot Smooth"], &g_Options.shotgun_aimbot_smoothness);
		GetVal(settings["Aimbot"]["Shotgun AimSpot"], &g_Options.shotgun_legit_aimspot);
		GetVal(settings["Aimbot"]["Shotgun Aimbot Silent"], &g_Options.shotgun_aimbot_silent);
		GetVal(settings["Aimbot"]["Shotgun Aimbot Key"], &g_Options.shotgun_aimbot_key);
		GetVal(settings["Aimbot"]["Shotgun Backtracking"], &g_Options.shotgun_backtracking);
		GetVal(settings["Aimbot"]["Shotgun Recoil"], &g_Options.shotgun_rcs_amount);
		GetVal(settings["Aimbot"]["Shotgun Visibility Check"], &g_Options.shotgun_vischeck);
		GetVal(settings["Aimbot"]["Shotgun Backtracking ticks"], &g_Options.shotgun_backtracking_ticks);
		GetVal(settings["Aimbot"]["Shotgun Standalone RCS"], &g_Options.shotgun_standalone_rcs);

		GetVal(settings["Aimbot"]["SMG Aimbot FOV"], &g_Options.smg_aimbot_AimbotFOV);
		GetVal(settings["Aimbot"]["SMG Aimbot Smooth"], &g_Options.smg_aimbot_smoothness);
		GetVal(settings["Aimbot"]["SMG AimSpot"], &g_Options.smg_legit_aimspot);
		GetVal(settings["Aimbot"]["SMG Aimbot Silent"], &g_Options.smg_aimbot_silent);
		GetVal(settings["Aimbot"]["SMG Aimbot Key"], &g_Options.smg_aimbot_key);
		GetVal(settings["Aimbot"]["SMG Backtracking"], &g_Options.smg_backtracking);
		GetVal(settings["Aimbot"]["SMG Recoil"], &g_Options.smg_rcs_amount);
		GetVal(settings["Aimbot"]["SMG Visibility Check"], &g_Options.smg_vischeck);
		GetVal(settings["Aimbot"]["SMG Backtracking ticks"], &g_Options.smg_backtracking_ticks);
		GetVal(settings["Aimbot"]["SMG Standalone RCS"], &g_Options.smg_standalone_rcs);
		//
		// TRIGGERBOT
		//

		GetVal(settings["Triggerbot"]["Active"], &g_Options.triggerbotactive);
		GetVal(settings["Triggerbot"]["Trigger key"], &g_Options.triggerbotkey);
		GetVal(settings["Triggerbot"]["Trigger key"], &g_Options.triggerbot_spot);

		//
		// COLORS
		//
		GetVal(settings["Colors"]["Ally Visible Color"], &g_Options.color_esp_ally_visible);
		GetVal(settings["Colors"]["Ally Occluded Color"], &g_Options.color_esp_ally_occluded);
		GetVal(settings["Colors"]["Enemy Visible Color"], &g_Options.color_esp_enemy_visible);
		GetVal(settings["Colors"]["Enemy Occluded Color"], &g_Options.color_esp_enemy_occluded);
		GetVal(settings["Colors"]["Weapon ESP Color"], &g_Options.color_esp_weapons);
		GetVal(settings["Colors"]["Defuse Kit ESP Color"], &g_Options.color_esp_defuse);
		GetVal(settings["Colors"]["C4 ESP Color"], &g_Options.color_esp_c4);
		GetVal(settings["Colors"]["Ally Glow Color"], &g_Options.color_glow_ally);
		GetVal(settings["Colors"]["Enemy Glow Color"], &g_Options.color_glow_enemy);
		GetVal(settings["Colors"]["Chicken Glow Color"], &g_Options.color_glow_chickens);
		GetVal(settings["Colors"]["C4 Carrier Glow Color"], &g_Options.color_glow_c4_carrier);
		GetVal(settings["Colors"]["Planted C4 Glow Color"], &g_Options.color_glow_planted_c4);
		GetVal(settings["Colors"]["Defuse Kit Glow Color"], &g_Options.color_glow_defuse);
		GetVal(settings["Colors"]["Weapons Glow Color"], &g_Options.color_glow_weapons);
		GetVal(settings["Colors"]["Ally Chams Visible Color"], &g_Options.color_chams_player_ally_visible);
		GetVal(settings["Colors"]["Ally Chams Occluded Color"], &g_Options.color_chams_player_ally_occluded);
		GetVal(settings["Colors"]["Enemy Chams Visible Color"], &g_Options.color_chams_player_enemy_visible);
		GetVal(settings["Colors"]["Enemy Chams Occluded Color"], &g_Options.color_chams_player_enemy_occluded);
		GetVal(settings["Colors"]["Arm Chams Visible Color"], &g_Options.color_chams_arms_visible);
		GetVal(settings["Colors"]["Arm Chams Occluded Color"], &g_Options.color_chams_arms_occluded);

		//
		// MENU
		//
		GetVal(settings["Menu"]["Menu Color"], &g_Options.color_menu_main);
		GetVal(settings["Menu"]["Menu Accents Color"], &g_Options.color_menu_body);
		GetVal(settings["Menu"]["Menu Font Color"], &g_Options.color_menu_font);
		//GetVal(settings["Menu"]["Menu Roundness"], &g_Options.menu_roundness);
		//GetVal(settings["Colors"]["Menu Hover"], &g_Options.menu_hover);

		GetVal(settings["Menu"]["Menu Fade Speed"], &g_Options.menu_fade_speed);

		// updating menu colors
		updatedColors = true;

		// update skins
		g_ClientState->ForceFullUpdate();
	}
}





































































































