#include "SpectatorList.h"
#include "valve_sdk\csgostructs.hpp"
#include "imgui\imgui.h"
#include "options.hpp"

namespace specList
{
	int specs = 0;
	int modes = 0;
	std::string spect = "";
	std::string mode = "";

	int lastspecs = 0;
	int lastmodes = 0;
	std::string lastspect = "";
	std::string lastmode = "";

	bool sync = false;

	void doSpectatorList()
	{
		if (g_EngineClient->IsInGame() && g_EngineClient->IsConnected())
		{
			int localIndex = g_EngineClient->GetLocalPlayer();
			auto pLocalEntity = C_BasePlayer::GetPlayerByIndex(localIndex);

			player_info_t pLocalInfo;
			g_EngineClient->GetPlayerInfo(pLocalEntity->EntIndex(), &pLocalInfo);

			if (pLocalEntity)
			{
				// Reset spectator lists
				specs = 0;
				modes = 0;
				spect = "";
				mode = "";

				// Get spectators for all the people
				for (int i = 0; i < g_EngineClient->GetMaxClients(); i++)
				{
					auto pBaseEntity = C_BasePlayer::GetPlayerByIndex(i);

					if (!pBaseEntity)
						continue;
					if (pBaseEntity->m_iHealth() > 0)
						continue;
					if (pBaseEntity == pLocalEntity)
						continue;
					if (pBaseEntity->IsDormant())
						continue;

					player_info_t pInfo;
					g_EngineClient->GetPlayerInfo(pBaseEntity->EntIndex(), &pInfo);
					if (pInfo.ishltv)
						continue;

					player_info_t pInfoTarget;
					g_EngineClient->GetPlayerInfo(pBaseEntity->m_hObserverTarget().GetEntryIndex(), &pInfoTarget);

					// Only show people who spectate the local player
					if (g_Options.spec_list_player_only)
					{
						// Return if the target it not the local player
						if (pLocalInfo.steamID64 == pInfoTarget.steamID64)
						{
							spect += pInfo.szName;
							spect += "\n";
							specs++;
						}
					}
					else
					{
						spect += pInfo.szName;
						spect += "\n";
						specs++;

						mode += pInfoTarget.szName;

						mode += "\n";
						modes++;
					}
				}
			}
		}
	}
}


































































































