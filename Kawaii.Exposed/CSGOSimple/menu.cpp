#include "Menu.hpp"
#define NOMINMAX
#include <Windows.h>
#include <chrono>
#include <stdio.h>
#include <ctype.h>
#include <cctype>

#include "valve_sdk/csgostructs.hpp"
#include "helpers/InputSys.hpp"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_dx9.h"
#include "Options.hpp"
#include "valve_sdk\csgostructs.hpp"
#include "fonts/IconsFontAwesome.h"
#include "helpers\utils.hpp"
#include "Configs.h"
#include "SpectatorList.h"

static ConVar* cl_mouseenable = nullptr;

ImVec4 ColorToImVec4(Color* clr)
{
	int r, g, b, a;
	clr->GetColor(r, g, b, a);
	return ImVec4((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, (float)a / 255.0f);
}

namespace ImGuiEx
{
	inline bool ColorEdit4(const char* label, Color* v, bool show_alpha = true)
	{
		auto clr = ImVec4{
			v->r() / 255.0f,
			v->g() / 255.0f,
			v->b() / 255.0f,
			v->a() / 255.0f
		};

		if (ImGui::ColorEdit4(label, &clr.x, show_alpha)) {
			v->SetColor(clr.x, clr.y, clr.z, clr.w);
			return true;
		}
		return false;
	}
	inline bool ColorEdit3(const char* label, Color* v)
	{
		return ColorEdit4(label, v, false);
	}
}


namespace ImGuiCustom
{
	inline bool CheckBoxFont(const char* name_, bool* pB_, const char* pOn_ = ICON_FA_TOGGLE_ON, const char* pOff_ = ICON_FA_TOGGLE_OFF)
	{
		if (*pB_) ImGui::Text(pOn_);
		else ImGui::Text(pOff_);
		bool bHover = false;
		bHover = bHover || ImGui::IsItemHovered();
		ImGui::SameLine();
		ImGui::Text(name_);
		bHover = bHover || ImGui::IsItemHovered();
		if (bHover && ImGui::IsMouseClicked(0))
		{
			*pB_ = !*pB_;
			return true;
		}
		return false;
	}

	inline bool Checkbox(const char* name_, bool* pB_)
	{
		return CheckBoxFont(name_, pB_, ICON_FA_TOGGLE_ON, ICON_FA_TOGGLE_OFF);
	}

	inline bool MenuItemCheckbox(const char* name_, bool* pB_)
	{
		bool retval = ImGui::MenuItem(name_);
		ImGui::SameLine();
		if (*pB_) ImGui::Text(ICON_FA_TOGGLE_ON);
		else ImGui::Text(ICON_FA_TOGGLE_OFF);
		if (retval) *pB_ = !*pB_;
		return retval;
	}

	bool shouldListen = false;
	int keyOutput = 0;

	std::string ToUpper(std::string str)
	{
		std::transform(str.begin(), str.end(), str.begin(), (int(*)(int))std::toupper);
		return str;
	}

	void KeyBind(int* key)
	{
		const char* text = keyNames[*key];

		if (shouldListen && keyOutput == *key) text = "PRESS A KEY";
		else text = ToUpper(std::string(text)).c_str();

		if (ImGui::Button(text))
		{
			shouldListen = true;
			keyOutput = *key;
		}

		if (shouldListen)
		{
			for (int i = 0; i < 124; i++)
				*key = InputSys::Get().IsKeyDown(i);
		}
	}
}

void Menu::Initialize()
{
	_visible = false;
	cl_mouseenable = g_CVar->FindVar("cl_mouseenable");
	ImGui_ImplDX9_Init(InputSys::Get().GetMainWindow(), g_D3DDevice9);
	CreateStyle();
}

void Menu::Shutdown()
{
	ImGui_ImplDX9_Shutdown();
	cl_mouseenable->SetValue(true);
}

void Menu::OnDeviceLost()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
}

void Menu::OnDeviceReset()
{
	ImGui_ImplDX9_CreateDeviceObjects();
}

void setColors()
{

}

enum MenuTabs_t
{
	TAB_AIMBOT,
	TAB_VISUALS,
	TAB_MISC,
	TAB_SKINS,
	TAB_CONFIG
};

enum AimbotTabs_t
{
	AIMBOTTAB_HVH,
	AIMBOTTAB_AIMBOT,
	AIMBOTTAB_TRIGGERBOT,
	EXTRA_TAB
};

enum Visual_Tabs
{
	Visuals_ESP,
	Visuals_Glow,
	Visuals_Chams,
	Visuals_Misc
};

enum Legitbot_Tabs
{
	Legit_Pistol,
	Legit_Rifle,
	Legit_Sniper,
	Legit_Shotgun,
	Legit_SMG
};

enum misc_tabs
{
	misc_colors,
	misc_configs
};

enum othermisc_tabs
{
	misc_1,
	misc_2
};

enum misc_colors
{
	esp_colors,
	glow_colors,
	chams_colors,
	menu_colors
};

enum SkinChanger_Tabs
{
	Skins_Pistol,
	Skins_Rifle,
	Skins_Sniper,
	Skins_Heavy,
	Skins_Shotgun,
	Skins_SMG,
	Skins_Knife,
	Skins_Models,
};

static char* knife_names[] =
{
	"Default Knife",
	"Bayonet",
	"Flip Knife",
	"Gut Knife",
	"Karambit",
	"M9 Bayonet",
	"Huntsman",
	"Falchion",
	"Bowie",
	"Butterfly",
	"Shadow Daggers"
};

static char* glove_names[] =
{
	"Default Gloves",
	"Bloodhound Gloves",
	"Sport Gloves",
	"Slick Gloves",
	"Leather Wrap Gloves",
	"Moto Gloves",
	"Specialist Gloves"
};

static char* player_models[] =
{
	"No Model",
	"Reina Kousaka", // Search this for the player models
	"Neptune", // Called Mirai_Nikki:
	"Octodad", // Called "Banana Joe"
};

static char* knife_models[] =
{
	"No Model",
	"Minecraft Pickaxe",
	"SAO Sword"				//Search up minecraft_pickaxe to find knife models
};

static char* GlowStyles[] =
{
	"Outline",
	"Full Bloom",
	"Aura",
	"Colored",
	"Colored Pulsating"
};

static char* clantagstuff[] = 
{
	"DISABLED",
	"NO CLANTAG",
	"KAWAII.EXPOSED STATIC",
	"KAWAII.EXPOSED ANIMATED",
	"SKEET"
};

static char* chatspamthings[] = {
	"DISABLED",
	"KAWAII.EXPOSED",
	"EDGY",
};

static char* skythings[] = {
	"DISABLED",
	"VIETNAM",
	"VERTIGO",
	"SKY CSGO NIGHT02",
	"SKY_CSGO_NIGHT02B"
};

static char* keybinds[] = {
	"DISABLED",
	"MOUSE 1",
	"MOUSE 2",
	"MOUSE 3",
	"MOUSE 4",
	"MOUSE 5",
	"SHIFT",
};


static int curSkinsConfigTab = 0;
static int curVisualsTab = 0;
static int curMiscTab = 0;
static int curColorTab = 0;
static int curMiscOtherTab = 0;
static int curWeaponConfigTab = 0;



void skinchangerupdate()
{
	static bool bbbbbbbb = false;
	if (!bbbbbbbb)
	{
		g_ClientState->ForceFullUpdate();
		bbbbbbbb = true;
	}
	else if (bbbbbbbb) bbbbbbbb = false;
}


void Menu::Render()
{
	if (!_visible)
	{
		if (g_Options.spec_list)
		{
			ImGui_ImplDX9_NewFrame();

			ImGui::GetIO().MouseDrawCursor = _visible;

			if (g_Options.spec_list_player_only)
			{
				// Spectators list
				if (ImGui::Begin("Spectator List", &g_Options.spec_list, ImVec2(150, 400), 0.4F, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders))
				{
					if (specList::specs > 0)
						specList::spect += "\n";

					specList::lastspecs = specList::specs;
					specList::lastmodes = specList::modes;
					specList::lastspect = specList::spect;
					specList::lastmode = specList::mode;

					ImGui::Text(specList::lastspect.c_str());

					ImGui::End();
				}
			}
			else
			{
				// Spectators list
				if (ImGui::Begin("Spectator List", &g_Options.spec_list, ImVec2(0, 0), 0.4F, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders))
				{
					if (specList::specs > 0)
						specList::spect += "\n";
					if (specList::modes > 0)
						specList::mode += "\n";

					specList::lastspecs = specList::specs;
					specList::lastmodes = specList::modes;
					specList::lastspect = specList::spect;
					specList::lastmode = specList::mode;

					ImGui::Columns(2);
					ImGui::Separator();

					ImGui::Text("Spectator");
					ImGui::NextColumn();

					ImGui::Text("Target");
					ImGui::NextColumn();
					ImGui::Separator();

					ImGui::Text(specList::lastspect.c_str());
					ImGui::NextColumn();

					ImGui::Text(specList::lastmode.c_str());
					ImGui::Columns(1);

					ImGui::End();
				}
			}

			ImGui::Render();
		}
		return;
	}

	auto& style = ImGui::GetStyle();
	setColors();
	ImGui::GetIO().MouseDrawCursor = _visible;
	ImGui_ImplDX9_NewFrame();

	static float menuFadeTimer = 0.0f;
	if (_visible)
	{
		if (menuFadeTimer <= 1.0f - (g_Options.menu_fade_speed / 100.0f)) menuFadeTimer += (g_Options.menu_fade_speed / 100.0f);
		style.Alpha = menuFadeTimer;
		goto RenderMenu;
	}
	else if (menuFadeTimer > (g_Options.menu_fade_speed / 100.0f) && !_visible)
	{
		menuFadeTimer -= (g_Options.menu_fade_speed / 100.0f);
		style.Alpha = menuFadeTimer;
		goto RenderMenu;
	}
	else goto SkipMenuRender;
	ImGui::NewFrame();
RenderMenu:
	ImGui::Begin("KAWAII.EXPOSED", &_visible, ImVec2(300, 375), 1.00f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
	{
		ImGui::Text("                                                      Kawaii.Exposed");
		ImGui::Separator();

		if (ImGui::Button(ICON_FA_CROSSHAIRS, ImVec2((630.0f - 24.0f) / 5.0f, 25))) g_Options.curTab = TAB_AIMBOT;
		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_EYE, ImVec2((630.0f - 24.0f) / 5.0f, 25))) g_Options.curTab = TAB_VISUALS;
		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_SLIDERS, ImVec2((630.0f - 24.0f) / 5.0f, 25))) g_Options.curTab = TAB_MISC;
		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_PAINT_BRUSH, ImVec2((630.0f - 24.0f) / 5.0f, 25))) g_Options.curTab = TAB_SKINS;
		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_WRENCH, ImVec2((630.0f - 24.0f) / 5.0f, 25))) g_Options.curTab = TAB_CONFIG;

		ImGui::Separator();

		switch (g_Options.curTab)
		{
		case TAB_AIMBOT:
			//if (ImGui::Button("RAGE", ImVec2(((630.0f - 12.0f) / 3.0f) - 1, 25))) g_Options.curTab_aimbot = AIMBOTTAB_HVH;
			ImGui::SameLine();
			if (ImGui::Button("LEGIT AIMBOT", ImVec2(((630.0f - 12.0f) / 3.0f) - 2, 25))) g_Options.curTab_aimbot = AIMBOTTAB_AIMBOT;
			ImGui::SameLine();
			if (ImGui::Button("TRIGGERBOT", ImVec2(((630.0f - 12.0f) / 3.0f) - 1, 25))) g_Options.curTab_aimbot = AIMBOTTAB_TRIGGERBOT;
			ImGui::SameLine();
			if (ImGui::Button("EXTRA", ImVec2(((630.0f - 12.0f) / 3.0f) - 1, 25))) g_Options.curTab_aimbot = EXTRA_TAB;

			ImGui::Separator();

			switch (g_Options.curTab_aimbot)
			{
			case AIMBOTTAB_HVH:
				//ImGuiCustom::Checkbox("ACTIVE", &g_Options.aimbot_rage);
				//ImGuiCustom::Checkbox("SILENT", &g_Options.aimbot_silent);
				//ImGuiCustom::Checkbox("RESOLVER", &g_Options.resolver);
				//ImGuiCustom::Checkbox("ANTIAIM ON KNIFE", &g_Options.antiaim_knife);
				//	ImGuiCustom::Checkbox("ADAPTIVE FAKE LAG", &g_Options.fakelag_adaptive);
				//ImGuiCustom::Checkbox("AUTO WALL", &g_Options.autowall);
				//ImGuiCustom::Checkbox("AUTO SHOOT", &g_Options.autoshoot);
				//ImGuiCustom::Checkbox("AUTO SCOPE", &g_Options.autoscope);
				//ImGuiCustom::Checkbox("NO RECOIL", &g_Options.no_recoil);
				//ImGui::PushItemWidth(300);
				//ImGui::Combo("PITCH", &g_Options.antiaim_pitch, AntiaimPitchNames, ARRAYSIZE(AntiaimPitchNames));
				//ImGui::Combo("REAL YAW", &g_Options.antiaim_yaw, AntiaimYawNames, ARRAYSIZE(AntiaimYawNames));
				//ImGui::Combo("FAKE YAW", &g_Options.antiaim_yaw_fake, AntiaimYawNames, ARRAYSIZE(AntiaimYawNames));
				//ImGui::Combo("THIRDPERSON ANGLE", &g_Options.antiaim_thirdperson_angle, AntiaimThirdpersonAngle, ARRAYSIZE(AntiaimThirdpersonAngle));
				//ImGui::PopItemWidth();
				//ImGui::SliderFloat("ANTIAIM EDGE DISTANCE", &g_Options.antiaim_edge_dist, 0.0f, 100.0f);
				//		ImGui::SliderInt("FAKE LAG", &g_Options.fakelag_amount, 0, 20);
				//ImGui::SliderFloat("AUTOWALL MIN DAMAGE", &g_Options.autowall_min_damage, 1.0f, 100.0f);
				//ImGui::SliderFloat("MIN HITCHANCE", &g_Options.hitchance, 0.0f, 100.0f);
				//ImGui::Combo("HITSCAN AMOUNT", &g_Options.hitscan_amount, HitscanNames, ARRAYSIZE(HitscanNames));
				break;
			case AIMBOTTAB_AIMBOT:
				if (ImGui::Button("PISTOLS", ImVec2((630.0f - 24.0f) / 5.0f, 25))) curWeaponConfigTab = Legit_Pistol;
				ImGui::SameLine();
				if (ImGui::Button("RIFLES", ImVec2((630.0f - 24.0f) / 5.0f, 25))) curWeaponConfigTab = Legit_Rifle;
				ImGui::SameLine();
				if (ImGui::Button("SNIPERS", ImVec2((630.0f - 24.0f) / 5.0f, 25))) curWeaponConfigTab = Legit_Sniper;
				ImGui::SameLine();
				if (ImGui::Button("SHOTGUNS", ImVec2((630.0f - 24.0f) / 5.0f, 25))) curWeaponConfigTab = Legit_Shotgun;
				ImGui::SameLine();
				if (ImGui::Button("SMGS", ImVec2((630.0f - 24.0f) / 5.0f, 25))) curWeaponConfigTab = Legit_SMG;
				ImGui::Separator();
				switch (curWeaponConfigTab)
				{
				case Legit_Pistol:
					ImGui::Text("PISTOLS");
					ImGui::Separator();
					ImGui::Combo("AIMKEY", &g_Options.pistol_aimbot_key, keyNames, ARRAYSIZE(keyNames));
					ImGuiCustom::Checkbox("VISCHECK", &g_Options.pistol_vischeck);
					ImGuiCustom::Checkbox("SILENT", &g_Options.pistol_aimbot_silent);
					ImGuiCustom::Checkbox("BACKTRACKING", &g_Options.pistol_backtracking);
					ImGui::PushItemWidth(150);
					ImGui::Combo("HITBOX", &g_Options.pistol_legit_aimspot, AimSpots, ARRAYSIZE(AimSpots));
					ImGui::PopItemWidth();
					ImGui::SliderFloat("FOV", &g_Options.pistol_aimbot_AimbotFOV, 0.0f, 180.0f);
					ImGui::SliderFloat("SMOOTHNESS", &g_Options.pistol_aimbot_smoothness, 0.0f, 1.0f);
					ImGui::SliderFloat("RCS AMOUNT", &g_Options.pistol_rcs_amount, 0.0f, 2.0f);
					ImGui::SliderInt("BACKTRACKING TICKS", &g_Options.pistol_backtracking_ticks, 0.0f, 20.0f);
					ImGuiCustom::Checkbox("STANDALONE RCS", &g_Options.pistol_standalone_rcs);
					break;
				case Legit_Rifle:
					ImGui::Text("ASSAULT RIFLES");
					ImGui::Separator();
					ImGui::Combo("AIMKEY", &g_Options.rifle_aimbot_key, keyNames, ARRAYSIZE(keyNames));
					ImGuiCustom::Checkbox("VISCHECK", &g_Options.rifle_vischeck);
					ImGuiCustom::Checkbox("SILENT", &g_Options.rifle_aimbot_silent);
					ImGuiCustom::Checkbox("BACKTRACKING", &g_Options.rifle_backtracking);
					ImGui::PushItemWidth(150);
					ImGui::Combo("HITBOX", &g_Options.rifle_legit_aimspot, AimSpots, ARRAYSIZE(AimSpots));
					ImGui::PopItemWidth();
					ImGui::SliderFloat("FOV", &g_Options.rifle_aimbot_AimbotFOV, 0.0f, 180.0f);
					ImGui::SliderFloat("SMOOTHNESS", &g_Options.rifle_aimbot_smoothness, 0.0f, 1.0f);
					ImGui::SliderFloat("RCS AMOUNT", &g_Options.rifle_rcs_amount, 0.0f, 2.0f);
					ImGui::SliderInt("BACKTRACKING TICKS", &g_Options.rifle_backtracking_ticks, 0.0f, 20.0f);
					ImGuiCustom::Checkbox("STANDALONE RCS", &g_Options.rifle_standalone_rcs);
					break;
				case Legit_Sniper:
					ImGui::Text("SNIPER RIFLES");
					ImGui::Separator();
					ImGui::Combo("AIMKEY", &g_Options.sniper_aimbot_key, keyNames, ARRAYSIZE(keyNames));
					ImGuiCustom::Checkbox("VISCHECK", &g_Options.sniper_vischeck);
					ImGuiCustom::Checkbox("SILENT", &g_Options.sniper_aimbot_silent);
					ImGuiCustom::Checkbox("BACKTRACKING", &g_Options.sniper_backtracking);
					ImGui::PushItemWidth(150);
					ImGui::Combo("HITBOX", &g_Options.sniper_legit_aimspot, AimSpots, ARRAYSIZE(AimSpots));
					ImGui::PopItemWidth();
					ImGui::SliderFloat("FOV", &g_Options.sniper_aimbot_AimbotFOV, 0.0f, 180.0f);
					ImGui::SliderFloat("SMOOTHNESS", &g_Options.sniper_aimbot_smoothness, 0.0f, 1.0f);
					ImGui::SliderFloat("RCS AMOUNT", &g_Options.sniper_rcs_amount, 0.0f, 2.0f);
					ImGui::SliderInt("BACKTRACKING TICKS", &g_Options.sniper_backtracking_ticks, 0.0f, 20.0f);
					ImGuiCustom::Checkbox("STANDALONE RCS", &g_Options.sniper_standalone_rcs);
					break;
				case Legit_Shotgun:
					ImGui::Text("SHOTGUNS");
					ImGui::Separator();
					ImGui::Combo("AIMKEY", &g_Options.shotgun_aimbot_key, keyNames, ARRAYSIZE(keyNames));
					ImGuiCustom::Checkbox("VISCHECK", &g_Options.shotgun_vischeck);
					ImGuiCustom::Checkbox("SILENT", &g_Options.shotgun_aimbot_silent);
					ImGuiCustom::Checkbox("BACKTRACKING", &g_Options.shotgun_backtracking);
					ImGui::PushItemWidth(150);
					ImGui::Combo("HITBOX", &g_Options.shotgun_legit_aimspot, AimSpots, ARRAYSIZE(AimSpots));
					ImGui::PopItemWidth();
					ImGui::SliderFloat("FOV", &g_Options.shotgun_aimbot_AimbotFOV, 0.0f, 180.0f);
					ImGui::SliderFloat("SMOOTHNESS", &g_Options.shotgun_aimbot_smoothness, 0.0f, 1.0f);
					ImGui::SliderFloat("RCS AMOUNT", &g_Options.shotgun_rcs_amount, 0.0f, 2.0f);
					ImGui::SliderInt("BACKTRACKING TICKS", &g_Options.shotgun_backtracking_ticks, 0.0f, 20.0f);
					ImGuiCustom::Checkbox("STANDALONE RCS", &g_Options.shotgun_standalone_rcs);
					break;
				case Legit_SMG:
					ImGui::Text("SMGS");
					ImGui::Separator();
					ImGui::Combo("AIMKEY", &g_Options.smg_aimbot_key, keyNames, ARRAYSIZE(keyNames));
					ImGuiCustom::Checkbox("VISCHECK", &g_Options.smg_vischeck);
					ImGuiCustom::Checkbox("SILENT", &g_Options.smg_aimbot_silent);
					ImGuiCustom::Checkbox("BACKTRACKING", &g_Options.smg_backtracking);
					ImGui::PushItemWidth(150);
					ImGui::Combo("HITBOX", &g_Options.smg_legit_aimspot, AimSpots, ARRAYSIZE(AimSpots));
					ImGui::PopItemWidth();
					ImGui::SliderFloat("FOV", &g_Options.smg_aimbot_AimbotFOV, 0.0f, 180.0f);
					ImGui::SliderFloat("SMOOTHNESS", &g_Options.smg_aimbot_smoothness, 0.0f, 1.0f);
					ImGui::SliderFloat("RCS AMOUNT", &g_Options.smg_rcs_amount, 0.0f, 2.0f);
					ImGui::SliderInt("BACKTRACKING TICKS", &g_Options.smg_backtracking_ticks, 0.0f, 20.0f);
					ImGuiCustom::Checkbox("STANDALONE RCS", &g_Options.smg_standalone_rcs);
					break;
				}
				break;
			case AIMBOTTAB_TRIGGERBOT:
				ImGui::Text("TRIGGERBOT");
				ImGui::Separator();
				ImGuiCustom::Checkbox("ACTIVE", &g_Options.triggerbotactive);
				ImGui::Combo("TRIGGERKEY", &g_Options.triggerbotkey, keyNames, ARRAYSIZE(keyNames));
				ImGui::Combo("TRIGGERSPOT", &g_Options.triggerbot_spot, TriggerSpots, ARRAYSIZE(TriggerSpots));
				break;
			}
			break;


		case TAB_VISUALS:

			if (ImGui::Button("ESP", ImVec2(((630.0f - 24.0f) / 4.0f) + 1, 25))) curVisualsTab = Visuals_ESP;
			ImGui::SameLine();
			if (ImGui::Button("GLOW", ImVec2(((630.0f - 24.0f) / 4.0f) + 1, 25))) curVisualsTab = Visuals_Glow;
			ImGui::SameLine();
			if (ImGui::Button("CHAMS", ImVec2(((630.0f - 24.0f) / 4.0f) + 1, 25))) curVisualsTab = Visuals_Chams;
			ImGui::SameLine();
			if (ImGui::Button("MISC", ImVec2(((630.0f - 24.0f) / 4.0f) + 1, 25))) curVisualsTab = Visuals_Misc;
			ImGui::Separator();

			switch (curVisualsTab)
			{
			case Visuals_ESP:
				ImGuiCustom::Checkbox("ENABLED", &g_Options.esp_enabled);
				ImGuiCustom::Checkbox("ENEMIES ONLY", &g_Options.esp_enemies_only);
				ImGuiCustom::Checkbox("BOXES", &g_Options.esp_player_boxes);
			//	ImGuiCustom::Checkbox("SKELETON", &g_Options.skeleton_shit);
				ImGuiCustom::Checkbox("NAMES", &g_Options.esp_player_names);
				ImGuiCustom::Checkbox("HEALTH", &g_Options.esp_player_health);
				ImGuiCustom::Checkbox("ARMOR", &g_Options.esp_player_armour);
				ImGuiCustom::Checkbox("WEAPON", &g_Options.esp_player_weapons);
				ImGuiCustom::Checkbox("SNAPLINES", &g_Options.esp_player_snaplines);
				ImGuiCustom::Checkbox("DEFUSE KIT", &g_Options.esp_defuse_kit);
				ImGuiCustom::Checkbox("PLANTED C4", &g_Options.esp_planted_c4);
				break;
			case Visuals_Glow:
				ImGuiCustom::Checkbox("ENABLED", &g_Options.glow_enabled);
				ImGuiCustom::Checkbox("ENEMIES ONLY", &g_Options.glow_enemies_only);
				ImGuiCustom::Checkbox("PLAYERS", &g_Options.glow_players);
				ImGuiCustom::Checkbox("CHICKENS", &g_Options.glow_chickens);
				ImGuiCustom::Checkbox("C4 CARRIER", &g_Options.glow_c4_carrier);
				ImGuiCustom::Checkbox("PLANTED C4", &g_Options.glow_planted_c4);
				ImGuiCustom::Checkbox("DEFUSE KITS", &g_Options.glow_defuse_kits);
				ImGuiCustom::Checkbox("WEAPONS", &g_Options.glow_weapons);

				ImGui::NewLine();

				ImGui::Text("GLOW STYLE");
				ImGui::Combo("##Glow Style", &g_Options.glow_style, GlowStyles, ARRAYSIZE(GlowStyles));
				break;
			case Visuals_Chams:
				ImGui::Text("PLAYER");
				ImGui::Separator();
				ImGuiCustom::Checkbox("ENABLED", &g_Options.chams_player_enabled);
				ImGuiCustom::Checkbox("ENEMIES ONLY", &g_Options.chams_player_enemies_only);
				ImGuiCustom::Checkbox("LOCAL PLAYER", &g_Options.chams_localplayer);
				ImGuiCustom::Checkbox("WIREFRAME", &g_Options.chams_player_wireframe);
				ImGuiCustom::Checkbox("XQZ", &g_Options.chams_player_xqz);

				ImGui::NewLine();

				ImGui::Text("PLAYER CHAMS MODE");
				ImGui::Combo("##Player Chams Mode", &g_Options.chams_player_mode, "Material\0Flat\0Vapor\0");

				ImGui::NewLine();
				ImGui::Text("ARMS");
				ImGui::Separator();
				ImGuiCustom::Checkbox("ENABLED", &g_Options.chams_arms_enabled);
				ImGuiCustom::Checkbox("WIREFRAME", &g_Options.chams_arms_wireframe);
				ImGuiCustom::Checkbox("XQZ", &g_Options.chams_arms_xqz);

				ImGui::NewLine();

				ImGui::Text("ARM CHAMS MODE");
				ImGui::Combo("##Arm Chams Mode", &g_Options.chams_arms_mode, "Material\0Flat\0Vapor\0");
				break;
			case Visuals_Misc:
				ImGuiCustom::Checkbox("BACKTRACK TRACER", &g_Options.backtracking_tracer);
				ImGuiCustom::Checkbox("GRENADE PREDICTION", &g_Options.grenade_path_esp);
				ImGuiCustom::Checkbox("LSD", &g_Options.drugs);
				ImGuiCustom::Checkbox("SNOW", &g_Options.paper);
				ImGuiCustom::Checkbox("MINECRAFT", &g_Options.lowrestextures);
				ImGuiCustom::Checkbox("DROPPED WEAPONS", &g_Options.esp_dropped_weapons);
				ImGuiCustom::Checkbox("PLANTED C4 Boxes", &g_Options.esp_planted_c4_boxes);
				ImGuiCustom::Checkbox("PLANTED C4 Text", &g_Options.esp_planted_c4_text);
				ImGuiCustom::Checkbox("DEFUSE KIT BOXES", &g_Options.esp_defuse_kit_boxes);
				ImGuiCustom::Checkbox("DEFUSE KIT TEXT", &g_Options.esp_defuse_kit_text);
				ImGuiCustom::Checkbox("DROPPED WEAPON BOXES", &g_Options.esp_dropped_weapons_boxes);
				ImGuiCustom::Checkbox("DROPPED WEAPON TEXT", &g_Options.esp_dropped_weapons_text);
				ImGuiCustom::Checkbox("CROSSHAIR", &g_Options.esp_crosshair);
				break;
			}
			break;

		case TAB_MISC:
			if (ImGui::Button("MISC 1", ImVec2(((630.0f - 24.0f) / 2.0f) + 6, 25))) curMiscOtherTab = misc_1;
			ImGui::SameLine();
			if (ImGui::Button("MISC 2", ImVec2(((630.0f - 24.0f) / 2.0f) + 6, 25))) curMiscOtherTab = misc_2;
			ImGui::Separator();

			switch (curMiscOtherTab) {

			case misc_1:
				ImGuiCustom::Checkbox("BUNNYHOP", &g_Options.misc_bhop_key);
				ImGuiCustom::Checkbox("NO VISUAL RECOIL", &g_Options.visuals_no_aimpunch);
				ImGuiCustom::Checkbox("NO FLASH", &g_Options.noflash);
				ImGuiCustom::Checkbox("THIRD-PERSON", &g_Options.thirdperson);
				ImGuiCustom::Checkbox("SPECTATOR LIST", &g_Options.spec_list);
				ImGuiCustom::Checkbox("LOCAL SPECTATOR LIST", &g_Options.spec_list_player_only);
				ImGuiCustom::Checkbox("WATERMARKS", &g_Options.watermarks);
				ImGui::NewLine();
				ImGui::SliderInt("LOCAL PLAYER FOV FOV", &g_Options.fov, 0.0f, 100.0f);
				ImGui::SliderInt("VIEWMODEL FOV FOV", &g_Options.viewmodel_fov, 0.0f, 100.0f);
				ImGui::SliderFloat("MENU FADE SPEED", &g_Options.menu_fade_speed, 0.50f, 5.0f);
				ImGui::SliderInt("THIRD PERSON FOV FOV", &g_Options.thirdpersonrange, 30, 200);
				ImGui::NewLine();
				ImGui::Combo("CLANTAG CHANGER", &g_Options.clantag, clantagstuff, ARRAYSIZE(clantagstuff));
				ImGui::Combo("CHAT SPAM", &g_Options.chatspam, chatspamthings, ARRAYSIZE(chatspamthings));
				ImGui::Combo("SKY CHANGER", &g_Options.sky, skythings, ARRAYSIZE(skythings));
				break;
			case misc_2:
				ImGuiCustom::Checkbox("AUTO ACCEPT", &g_Options.autoaccept);
		//		ImGuiCustom::Checkbox("LEGIT AA", &g_Options.legit_antiaim);
				ImGuiCustom::Checkbox("MOONWALK", &g_Options.memewalk);
				ImGuiCustom::Checkbox("RANK REVEAL", &g_Options.rank_reveal);
				ImGuiCustom::Checkbox("HITMARKERS", &g_Options.hitmarkers);
				ImGuiCustom::Checkbox("NO HANDS", &g_Options.misc_no_hands);
				ImGuiCustom::Checkbox("DISABLE POST PROC", &g_Options.disable_post_processing);
				ImGuiCustom::Checkbox("NIGHTMODE", &g_Options.nightmode);
				ImGui::NewLine();
				ImGui::Combo("Hitmarker Sound", &g_Options.hitmarkers_sound, HitSounds, ARRAYSIZE(HitSounds));
				ImGui::Combo("Airstuck Key", &g_Options.airstuck_key, keyNames, ARRAYSIZE(keyNames));
				ImGui::SliderInt("FakeLag", &g_Options.fakelag_amount, 0, 15);

				break;
			}
			break;
		case TAB_SKINS:
			if (ImGui::Button("APPLY", ImVec2(((630.0f - 24.0f) / 1.0f) + 16.0f, 25))) g_ClientState->ForceFullUpdate();
			if (ImGui::Button("SNIPERS", ImVec2(((630.0f - 24.0f) / 5.0f), 25))) curSkinsConfigTab = Skins_Sniper;
			ImGui::SameLine();
			if (ImGui::Button("RIFLES", ImVec2(((630.0f - 24.0f) / 5.0f), 25))) curSkinsConfigTab = Skins_Rifle;
			ImGui::SameLine();
			if (ImGui::Button("PISTOLS", ImVec2(((630.0f - 24.0f) / 5.0f), 25))) curSkinsConfigTab = Skins_Pistol;
			ImGui::SameLine();
			if (ImGui::Button("HEAVY", ImVec2(((630.0f - 24.0f) / 5.0f), 25))) curSkinsConfigTab = Skins_Heavy;
			ImGui::SameLine();
			if (ImGui::Button("SHOTGUNS", ImVec2(((630.0f - 24.0f) / 5.0f), 25))) curSkinsConfigTab = Skins_Shotgun;
			ImGui::Separator();
			if (ImGui::Button("SMGS", ImVec2(((630.0f - 24.0f) / 3.0f) + 3.0f, 25))) curSkinsConfigTab = Skins_SMG;
			ImGui::SameLine();
			if (ImGui::Button("KNIVES", ImVec2(((630.0f - 24.0f) / 3.0f) + 3.0f, 25))) curSkinsConfigTab = Skins_Knife;
			ImGui::SameLine();
			if (ImGui::Button("MODELS", ImVec2(((630.0f - 24.0f) / 3.0f) + 3.0f, 25))) curSkinsConfigTab = Skins_Models;
			ImGui::Separator();

			switch (curSkinsConfigTab)
			{
			case Skins_Sniper:
				ImGui::Text("SNIPER RIFLES");
				ImGui::Separator();
				ImGui::Combo("AWP", &g_Options.awp, awpSkinNames, ARRAYSIZE(awpSkinNames));
				ImGui::Combo("SSG08", &g_Options.ssg08, ssg08SkinNames, ARRAYSIZE(ssg08SkinNames));
				ImGui::Combo("G3SG1", &g_Options.g3sg1, g3sg1SkinNames, ARRAYSIZE(g3sg1SkinNames));
				ImGui::Combo("SCAR-20", &g_Options.scar20, scar20Names, ARRAYSIZE(scar20Names));
				break;
			case Skins_Rifle:
				ImGui::Text("ASSAULT RIFLES");
				ImGui::Separator();
				ImGui::Combo("AK-47", &g_Options.ak47, ak47SkinNames, ARRAYSIZE(ak47SkinNames));
				ImGui::Combo("GALIL", &g_Options.galil, galilSkinNames, ARRAYSIZE(galilSkinNames));
				ImGui::Combo("M4A4", &g_Options.m4a4, m4a4Skinnames, ARRAYSIZE(m4a4Skinnames));
				ImGui::Combo("M4A1s", &g_Options.m4a1s, m4a1Skinnames, ARRAYSIZE(m4a1Skinnames));
				ImGui::Combo("FAMAS", &g_Options.famas, famasSkinNames, ARRAYSIZE(famasSkinNames));
				ImGui::Combo("SG553", &g_Options.sg553, sg553SkinNames, ARRAYSIZE(sg553SkinNames));
				ImGui::Combo("AUG", &g_Options.aug, augSkinNames, ARRAYSIZE(augSkinNames));
				break;
			case Skins_Pistol:
				ImGui::Text("PISTOLS");
				ImGui::Separator();
				ImGui::Combo("DESERT EAGLE", &g_Options.deagle, deagleSkinNames, ARRAYSIZE(deagleSkinNames));
				ImGui::Combo("TEC-9", &g_Options.tec9, tec9SkinNames, ARRAYSIZE(tec9SkinNames));
				ImGui::Combo("FIVE-SEVEN", &g_Options.fiveseven, fivesevenSkinNames, ARRAYSIZE(fivesevenSkinNames));
				ImGui::Combo("CZ75", &g_Options.cz75a, cz75SkinNames, ARRAYSIZE(cz75SkinNames));
				ImGui::Combo("GLOCK", &g_Options.glock, glockSkinNames, ARRAYSIZE(glockSkinNames));
				ImGui::Combo("USP-S", &g_Options.usps, uspSkinNames, ARRAYSIZE(uspSkinNames));
				ImGui::Combo("P2000", &g_Options.p2000, p2000SkinNames, ARRAYSIZE(uspSkinNames));
				ImGui::Combo("DUAL BERRETAS", &g_Options.elites, dualiesSkinNames, ARRAYSIZE(dualiesSkinNames));
				ImGui::Combo("P250", &g_Options.p250, p250SkinNames, ARRAYSIZE(p250SkinNames));
				ImGui::Combo("R8 REVOLVER", &g_Options.revolver, revolverSkinNames, ARRAYSIZE(revolverSkinNames));
				break;
			case Skins_Heavy:
				ImGui::Text("HEAVY");
				ImGui::Separator();
				ImGui::Combo("NEGEV", &g_Options.negev, negevNames, ARRAYSIZE(negevNames));
				ImGui::Combo("M249", &g_Options.m249, m249Names, ARRAYSIZE(m249Names));
				break;
			case Skins_Shotgun:
				ImGui::Text("SHOTGUNS");
				ImGui::Separator();
				ImGui::Combo("NOVA", &g_Options.nova, novaNames, ARRAYSIZE(novaNames));
				ImGui::Combo("SAWED-OFF", &g_Options.sawedoff, sawedoffSkinNames, ARRAYSIZE(sawedoffSkinNames));
				ImGui::Combo("XM1014", &g_Options.xm1014, xm1014SkinNames, ARRAYSIZE(xm1014SkinNames));
				ImGui::Combo("MAG-7", &g_Options.mag7, mag7SkinNames, ARRAYSIZE(mag7SkinNames));
				break;
			case Skins_SMG:
				ImGui::Text("SMGS");
				ImGui::Separator();
				ImGui::Combo("MAC10", &g_Options.mac10, mac10SkinNames, ARRAYSIZE(mac10SkinNames));
				ImGui::Combo("MP9", &g_Options.mp9, mp9SkinNames, ARRAYSIZE(mp9SkinNames));
				ImGui::Combo("MP7", &g_Options.mp7, mp7SkinNames, ARRAYSIZE(mp7SkinNames));
				ImGui::Combo("UMP45", &g_Options.ump45, umpSkinNames, ARRAYSIZE(umpSkinNames));
				ImGui::Combo("P90", &g_Options.p90, p90SkinNames, ARRAYSIZE(p90SkinNames));
				ImGui::Combo("PP-BIZON", &g_Options.bizon, bizonSkinNames, ARRAYSIZE(bizonSkinNames));
				break;
			case Skins_Knife:
				ImGui::Text("KNIVES");
				ImGui::Separator();
				ImGui::Combo("KNIFE", &g_Options.knife, knife_names, ARRAYSIZE(knife_names));
				ImGui::Combo("SKIN", &g_Options.knife_skin, knife_skins, ARRAYSIZE(knife_skins));
				break;
			case Skins_Models:
				ImGui::Text("MODELS");
				ImGui::Separator();
				ImGui::Combo("PLAYER MODEL", &g_Options.player_model, player_models, ARRAYSIZE(player_models));
				ImGui::Combo("KNIFE MODEL", &g_Options.knife_model, knife_models, ARRAYSIZE(knife_models));
				break;
			}

			break;
		case TAB_CONFIG:
			if (ImGui::Button("COLORS", ImVec2(((630.0f - 24.0f) / 2.0f) + 6, 25))) curMiscTab = misc_colors;
			ImGui::SameLine();
			if (ImGui::Button("CONFIGS", ImVec2(((630.0f - 24.0f) / 2.0f) + 6, 25))) curMiscTab = misc_configs;
			ImGui::Separator();

			switch (curMiscTab) {

			case misc_colors:
				if (ImGui::Button("ESP", ImVec2(((630.0f - 24.0f) / 4.0f) + 1, 25))) curColorTab = esp_colors;
				ImGui::SameLine();
				if (ImGui::Button("GLOW", ImVec2(((630.0f - 24.0f) / 4.0f) + 1, 25))) curColorTab = glow_colors;
				ImGui::SameLine();
				if (ImGui::Button("CHAMS", ImVec2(((630.0f - 24.0f) / 4.0f) + 1, 25))) curColorTab = chams_colors;
				ImGui::SameLine();
				if (ImGui::Button("MENU", ImVec2(((630.0f - 24.0f) / 4.0f) + 1, 25))) curColorTab = menu_colors;
				ImGui::Separator();
				switch (curColorTab) {
				case esp_colors:
					ImGui::PushItemWidth(500);
					ImGuiEx::ColorEdit3("Team (Visible)", &g_Options.color_esp_ally_visible);
					ImGuiEx::ColorEdit3("Team (Hidden)", &g_Options.color_esp_ally_occluded);
					ImGuiEx::ColorEdit3("Enemy (Visible)", &g_Options.color_esp_enemy_visible);
					ImGuiEx::ColorEdit3("Enemy (Hidden)", &g_Options.color_esp_enemy_occluded);
					ImGuiEx::ColorEdit3("Weapons", &g_Options.color_esp_weapons);
					ImGuiEx::ColorEdit3("Defuse Kit", &g_Options.color_esp_defuse);
					ImGuiEx::ColorEdit3("Planted C4", &g_Options.color_esp_c4);
					ImGui::PopItemWidth();
					break;
				case glow_colors:
					ImGui::PushItemWidth(500);
					ImGuiEx::ColorEdit3("Ally", &g_Options.color_glow_ally);
					ImGuiEx::ColorEdit3("Enemy", &g_Options.color_glow_enemy);
					ImGuiEx::ColorEdit3("Chickens", &g_Options.color_glow_chickens);
					ImGuiEx::ColorEdit3("C4 Carrier", &g_Options.color_glow_c4_carrier);
					ImGuiEx::ColorEdit3("Planted C4", &g_Options.color_glow_planted_c4);
					ImGuiEx::ColorEdit3("Defuse Kits", &g_Options.color_glow_defuse);
					ImGuiEx::ColorEdit3("Weapons", &g_Options.color_glow_weapons);
					ImGui::PopItemWidth();
					break;
				case chams_colors:
					ImGui::PushItemWidth(500);
					ImGuiEx::ColorEdit3("Ally (Visible)", &g_Options.color_chams_player_ally_visible);
					ImGuiEx::ColorEdit3("Ally (Hidden)", &g_Options.color_chams_player_ally_occluded);
					ImGuiEx::ColorEdit3("Enemy (Visible)", &g_Options.color_chams_player_enemy_occluded);
					ImGuiEx::ColorEdit3("Enemy (Hidden)", &g_Options.color_chams_player_enemy_visible);
					// Arms
					ImGuiEx::ColorEdit3("Arms (Visible)", &g_Options.color_chams_arms_visible);
					ImGuiEx::ColorEdit3("Arms (Hidden)", &g_Options.color_chams_arms_occluded);
					ImGui::PopItemWidth();
					break;
				case menu_colors:
					//if (ImGuiEx::ColorEdit4("Main", &g_Options.menu_color))
					//	updatedColors = true;

					//if (ImGuiEx::ColorEdit4("Accent", &g_Options.menu_accents_color))
					//	updatedColors = true;

					//if (ImGuiEx::ColorEdit4("Text", &g_Options.menu_text_color))
					//	updatedColors = true;

					//if (ImGuiEx::ColorEdit4("Outline", &g_Options.menu_outline_color))
					//	updatedColors = true;

					//if (ImGuiEx::ColorEdit4("Hover", &g_Options.menu_hover))
					//	updatedColors = true;
					break;
				}
				break;
			case misc_configs:
				if (ImGui::Button("SAVE LEGIT CONFIG")) Configs::SaveCFG("legit.weebcfg");
				if (ImGui::Button("LOAD LEGIT CONFIG")) Configs::LoadCFG("legit.weebcfg");
				ImGui::Separator();
				if (ImGui::Button("SAVE RAGE CONFIG")) Configs::SaveCFG("rage.weebcfg");
				if (ImGui::Button("LOAD RAGE CONFIG")) Configs::LoadCFG("rage.weebcfg");
				ImGui::Separator();
				if (ImGui::Button("SAVE CUSTOM 1")) Configs::SaveCFG("custom1.weebcfg");
				if (ImGui::Button("LOAD CUSTOM 1")) Configs::LoadCFG("custom1.weebcfg");
				ImGui::Separator();
				if (ImGui::Button("SAVE CUSTOM 2")) Configs::SaveCFG("custom2.weebcfg");
				if (ImGui::Button("LOAD CUSTOM 2")) Configs::LoadCFG("custom2.weebcfg");
				break;
			}
		}
	}

	ImGui::End();

	ImGui::Render();

SkipMenuRender: {}
}


void Menu::Show()
{
	_visible = true;
	cl_mouseenable->SetValue(false);
}

void Menu::Hide()
{
	_visible = false;
	cl_mouseenable->SetValue(true);
}

void Menu::Toggle()
{
	_visible = !_visible;
	cl_mouseenable->SetValue(!_visible);
}

void Menu::CreateStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImGuiIO& io = ImGui::GetIO();

	ImFontConfig config;
	//ImFontConfig config_weapon;
	config.MergeMode = true;
	io.Fonts->AddFontDefault();
	static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFont* iconfont = io.Fonts->AddFontFromMemoryCompressedTTF(IconFont_compressed_data, IconFont_compressed_size, 20.f, &config, icon_ranges);
	//ImFont* weaponsfont = io.Fonts->AddFontFromMemoryCompressedTTF(weapon_font_compressed_data, weapon_font_compressed_size, 16.f, &config_weapon, io.Fonts->GetGlyphRangesDefault());
	ImFont* font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf", 18.0f, &config, io.Fonts->GetGlyphRangesDefault());
	iconfont->DisplayOffset.y = 4.3;

	style.WindowMinSize = ImVec2(630, 375);
	style.FramePadding = ImVec2(1, 1);
	style.ItemSpacing = ImVec2(4, 4);
	style.ItemInnerSpacing = ImVec2(6, 3);
	style.Alpha = 1.0f;
	style.WindowRounding = 0.00f;
	style.FrameRounding = 1.25f;
	style.IndentSpacing = 6.0f;
	style.ItemInnerSpacing = ImVec2(1, 1);
	style.ColumnsMinSpacing = 50.0f;
	style.GrabMinSize = 5.00f;
	style.GrabRounding = 100.0f;
	style.ScrollbarSize = 12.0f;
	style.ScrollbarRounding = 16.0f;
	style.AntiAliasedLines = true;
	style.AntiAliasedShapes = true;
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

	Color buttonactive = Color(134, 61, 255, 162);
	Color buttonhovered = Color(134, 61, 255, 162);

	style.Colors[ImGuiCol_Text] = ImVec4(0.725f, 0.725f, 0.725f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.725f, 0.725f, 0.725f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.188f, 0.188f, 0.188f, 1.f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.188f, 0.188f, 0.188f, 1.f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.188f, 0.188f, 0.188f, 1.f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.05f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.05f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.228f, 0.228f, 0.228f, 1.f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.502f, 0.796f, 0.769f, 0.10f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.502f, 0.796f, 0.769f, 0.10f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.216f, 0.278f, 0.31f, 1.f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.216f, 0.278f, 0.31f, 1.f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.216f, 0.278f, 0.31f, 1.f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.188f, 0.188f, 0.188f, 1.f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.188f, 0.188f, 0.188f, 1.f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.725f, 0.725f, 0.725f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.502f, 0.796f, 0.769f, 0.10f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.502f, 0.796f, 0.769f, 0.10f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.228f, 0.228f, 0.228f, 1.f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.216f, 0.278f, 0.31f, 1.f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.725f, 0.725f, 0.725f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.725f, 0.725f, 0.725f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.228f, 0.228f, 0.228f, 1.f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.502f, 0.796f, 0.769f, 0.10f);
	style.Colors[ImGuiCol_ButtonActive] = ColorToImVec4(&buttonactive);
	style.Colors[ImGuiCol_Header] = ImVec4(0.228f, 0.228f, 0.228f, 1.f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.502f, 0.796f, 0.769f, 0.10f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.502f, 0.796f, 0.769f, 0.10f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.502f, 0.796f, 0.769f, 0.10f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.228f, 0.228f, 0.228f, 1.f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.502f, 0.796f, 0.769f, 0.10f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.502f, 0.796f, 0.769f, 0.10f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.82f, 0.82f, 0.82f, 0.00f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.82f, 0.82f, 0.82f, 0.00f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.502f, 0.796f, 0.769f, 0.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.07f);
}





































































































