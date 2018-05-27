#include "Chams.hpp"
#include <fstream>

#include "../valve_sdk/csgostructs.hpp"
#include "../Options.hpp"
#include "../Hooks.hpp"
#include "../helpers/InputSys.hpp"


Chams::Chams()
{
	// glow
	std::ofstream("csgo\\materials\\glow.vmt") << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/achievements/glow"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
	// vapor
	std::ofstream("csgo\\materials\\vapor.vmt") << R"#("UnlitGeneric"
{
  "$basetexture" "models/inventory_items/dogtags/dogtags_lightray"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
	// glow
	std::ofstream("csgo\\materials\\glow_ignorez.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/achievements/glow"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
	// vapor
	std::ofstream("csgo\\materials\\vapor_ignorez.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "models/inventory_items/dogtags/dogtags_lightray"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";



	// others
	std::ofstream("csgo\\materials\\regular.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
	std::ofstream("csgo\\materials\\ignorez.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
	std::ofstream("csgo\\materials\\flat.vmt") << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
	std::ofstream("csgo\\materials\\flat_ignorez.vmt") << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";

	// ya
	materialGlow = g_MatSystem->FindMaterial("glow", TEXTURE_GROUP_MODEL);
	materialVapor = g_MatSystem->FindMaterial("vapor", TEXTURE_GROUP_MODEL);
	materialGlowIgnoreZ = g_MatSystem->FindMaterial("glow_ignorez", TEXTURE_GROUP_MODEL);
	materialVaporIgnoreZ = g_MatSystem->FindMaterial("vapor_ignorez", TEXTURE_GROUP_MODEL);

	materialRegular = g_MatSystem->FindMaterial("regular", TEXTURE_GROUP_MODEL);
	materialRegularIgnoreZ = g_MatSystem->FindMaterial("ignorez", TEXTURE_GROUP_MODEL);
	materialFlatIgnoreZ = g_MatSystem->FindMaterial("flat_ignorez", TEXTURE_GROUP_MODEL);
	materialFlat = g_MatSystem->FindMaterial("flat", TEXTURE_GROUP_MODEL);
}

Chams::~Chams()
{
	// ya
	remove("csgo\\materials\\glow.vmt");
	remove("csgo\\materials\\vapor.vmt");
	remove("csgo\\materials\\glow_ignorez.vmt");
	remove("csgo\\materials\\vapor_ignorez.vmt");

	remove("csgo\\materials\\regular.vmt");
	remove("csgo\\materials\\ignorez.vmt");
	remove("csgo\\materials\\flat.vmt");
	remove("csgo\\materials\\flat_ignorez.vmt");
}


void Chams::OverrideMaterial(bool ignoreZ, bool flat, bool wireframe, bool glass, const Color& rgba, bool glow, bool vapor)
{
	IMaterial* material = nullptr;

	//material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);

	if (flat)
	{
		if (ignoreZ)
			material = materialFlatIgnoreZ;
		else
			material = materialFlat;
	}
	else
	{
		if (glow)
		{
			if (ignoreZ)
				material = materialGlowIgnoreZ;
			else
				material = materialGlow;
		}
		else
		{
			if (vapor)
			{
				if (ignoreZ)
					material = materialVaporIgnoreZ;
				else
					material = materialVapor;
			}
			else
			{
				if (ignoreZ)
					material = materialRegularIgnoreZ;
				else
					material = materialRegular;
			}
		}
	}

	if (glass)
	{
		material = materialFlat;
		material->AlphaModulate(0.35f);
	}
	else
	{
		material->AlphaModulate(rgba.a() / 255.0f);
	}

	material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, wireframe);
	material->ColorModulate(
		rgba.r() / 255.0f,
		rgba.g() / 255.0f,
		rgba.b() / 255.0f);

	g_MdlRender->ForcedMaterialOverride(material);
}


void Chams::OnDrawModelExecute(
	IMatRenderContext* ctx,
	const DrawModelState_t &state,
	const ModelRenderInfo_t &info,
	matrix3x4_t *matrix)
{
	static auto fnDME = Hooks::mdlrender_hook.get_original<Hooks::DrawModelExecute>(index::DrawModelExecute);

	const auto mdl = info.pModel;

	bool is_arm = strstr(mdl->szName, "arms") != nullptr;
	bool is_player = strstr(mdl->szName, "models/player") != nullptr;
	bool is_sleeve = strstr(mdl->szName, "sleeve") != nullptr;
	//bool is_weapon = strstr(mdl->szName, "weapons/v_") != nullptr;

	if (is_player && g_Options.chams_player_enabled)
	{
		// 
		// Draw player Chams.
		// 
		auto ent = C_BasePlayer::GetPlayerByIndex(info.entity_index);

		if (ent && g_LocalPlayer && ent->IsAlive())
		{
			const auto enemy = ent->m_iTeamNum() != g_LocalPlayer->m_iTeamNum();

			if (ent == g_LocalPlayer && g_Options.chams_localplayer)
			{
				auto clr_front = enemy ? g_Options.color_chams_player_enemy_visible : g_Options.color_chams_player_ally_visible;
				auto clr_back = enemy ? g_Options.color_chams_player_enemy_occluded : g_Options.color_chams_player_ally_occluded;

				if (g_Options.chams_player_mode == CHAM_TYPE_MATERIAL)
				{
					OverrideMaterial(
						g_Options.chams_player_xqz,
						false,
						g_Options.chams_player_wireframe,
						false,
						clr_front,
						false,
						false);
					fnDME(g_MdlRender, ctx, state, info, matrix);
					OverrideMaterial(
						false,
						false,
						g_Options.chams_player_wireframe,
						false,
						clr_back,
						false,
						false);
				}
				else if (g_Options.chams_player_mode == CHAM_TYPE_FLAT)
				{
					OverrideMaterial(
						g_Options.chams_player_xqz,
						true,
						g_Options.chams_player_wireframe,
						false,
						clr_back,
						false,
						false);
					fnDME(g_MdlRender, ctx, state, info, matrix);
					OverrideMaterial(
						false,
						true,
						g_Options.chams_player_wireframe,
						false,
						clr_front,
						false,
						false);
				}
				else if (g_Options.chams_player_mode == CHAM_TYPE_VAPOR)
				{
					OverrideMaterial(
						g_Options.chams_player_xqz,
						false,
						g_Options.chams_player_wireframe,
						false,
						clr_back,
						false,
						true);
					fnDME(g_MdlRender, ctx, state, info, matrix);
					OverrideMaterial(
						false,
						false,
						g_Options.chams_player_wireframe,
						false,
						clr_front,
						false,
						true);
				}

				QAngle OrigAng;
				OrigAng = g_LocalPlayer->m_angEyeAngles();
				g_LocalPlayer->SetAngle2(ChamFakeAngle);

				clr_front = Color(255, 255, 255, 100);
				clr_back = Color(255, 255, 255, 100);

				if (g_Options.chams_player_mode == CHAM_TYPE_MATERIAL)
				{
					OverrideMaterial(
						g_Options.chams_player_xqz,
						false,
						g_Options.chams_player_wireframe,
						false,
						clr_front,
						false,
						false);
					fnDME(g_MdlRender, ctx, state, info, matrix);
					OverrideMaterial(
						false,
						false,
						g_Options.chams_player_wireframe,
						false,
						clr_back,
						false,
						false);
				}
				else if (g_Options.chams_player_mode == CHAM_TYPE_FLAT)
				{
					OverrideMaterial(
						g_Options.chams_player_xqz,
						true,
						g_Options.chams_player_wireframe,
						false,
						clr_back,
						false,
						false);
					fnDME(g_MdlRender, ctx, state, info, matrix);
					OverrideMaterial(
						false,
						true,
						g_Options.chams_player_wireframe,
						false,
						clr_front,
						false,
						false);
				}
				else if (g_Options.chams_player_mode == CHAM_TYPE_VAPOR)
				{
					OverrideMaterial(
						g_Options.chams_player_xqz,
						false,
						g_Options.chams_player_wireframe,
						false,
						clr_back,
						false,
						true);
					fnDME(g_MdlRender, ctx, state, info, matrix);
					OverrideMaterial(
						false,
						false,
						g_Options.chams_player_wireframe,
						false,
						clr_front,
						false,
						true);
				}

				g_LocalPlayer->DrawModel(1, 255);
				g_MdlRender->ForcedMaterialOverride(nullptr);
				g_LocalPlayer->SetAngle2(OrigAng);
			}

			if (!enemy && g_Options.chams_player_enemies_only)
				return;

			const auto clr_front = enemy ? g_Options.color_chams_player_enemy_visible : g_Options.color_chams_player_ally_visible;
			const auto clr_back = enemy ? g_Options.color_chams_player_enemy_occluded : g_Options.color_chams_player_ally_occluded;

			if (g_Options.chams_player_mode == CHAM_TYPE_MATERIAL)
			{
				OverrideMaterial(
					g_Options.chams_player_xqz,
					false,
					g_Options.chams_player_wireframe,
					false,
					clr_front,
					false,
					false);
				fnDME(g_MdlRender, ctx, state, info, matrix);
				OverrideMaterial(
					false,
					false,
					g_Options.chams_player_wireframe,
					false,
					clr_back,
					false,
					false);
			}
			else if (g_Options.chams_player_mode == CHAM_TYPE_FLAT)
			{
				OverrideMaterial(
					g_Options.chams_player_xqz,
					true,
					g_Options.chams_player_wireframe,
					false,
					clr_back,
					false,
					false);
				fnDME(g_MdlRender, ctx, state, info, matrix);
				OverrideMaterial(
					false,
					true,
					g_Options.chams_player_wireframe,
					false,
					clr_front,
					false,
					false);
			}
			else if (g_Options.chams_player_mode == CHAM_TYPE_VAPOR)
			{
				OverrideMaterial(
					g_Options.chams_player_xqz,
					false,
					g_Options.chams_player_wireframe,
					false,
					clr_back,
					false,
					true);
				fnDME(g_MdlRender, ctx, state, info, matrix);
				OverrideMaterial(
					false,
					false,
					g_Options.chams_player_wireframe,
					false,
					clr_front,
					false,
					true);
			}
		}
	}
	/*else if (is_sleeve && g_Options.chams_arms_enabled)
	{
		auto material = g_MatSystem->FindMaterial(mdl->szName, TEXTURE_GROUP_MODEL);
		if (!material)
			return;
		// 
		// Remove sleeves when drawing Chams.
		// 
		material->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
		g_MdlRender->ForcedMaterialOverride(material);
	}*/
	else if (is_arm || is_sleeve)
	{
		auto material = g_MatSystem->FindMaterial(mdl->szName, TEXTURE_GROUP_MODEL);

		if (!material)
			return;

		if (g_Options.misc_no_hands)
		{
			// 
			// No hands.
			// 
			material->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			g_MdlRender->ForcedMaterialOverride(material);
		}
		else if (g_Options.chams_arms_enabled)
		{
			if (g_Options.chams_arms_mode == CHAM_TYPE_MATERIAL)
			{
				OverrideMaterial(
					g_Options.chams_arms_xqz,
					false,
					g_Options.chams_arms_wireframe,
					false,
					g_Options.color_chams_arms_occluded,
					false,
					false);
				fnDME(g_MdlRender, ctx, state, info, matrix);
				OverrideMaterial(
					false,
					false,
					g_Options.chams_arms_wireframe,
					false,
					g_Options.color_chams_arms_visible,
					false,
					false);
			}
			else if (g_Options.chams_arms_mode == CHAM_TYPE_FLAT)
			{
				OverrideMaterial(
					g_Options.chams_arms_xqz,
					true,
					g_Options.chams_arms_wireframe,
					false,
					g_Options.color_chams_arms_occluded,
					false,
					false);
				fnDME(g_MdlRender, ctx, state, info, matrix);
				OverrideMaterial(
					false,
					true,
					g_Options.chams_arms_wireframe,
					false,
					g_Options.color_chams_arms_visible ,
					false,
					false);
			}
			else if (g_Options.chams_arms_mode == CHAM_TYPE_VAPOR)
			{
				OverrideMaterial(
					g_Options.chams_arms_xqz,
					false,
					g_Options.chams_arms_wireframe,
					false,
					g_Options.color_chams_arms_occluded,
					false,
					true);
				fnDME(g_MdlRender, ctx, state, info, matrix);
				OverrideMaterial(
					false,
					false,
					g_Options.chams_arms_wireframe,
					false,
					g_Options.color_chams_arms_visible,
					false,
					true);
			}
		}
	}
}


































































































