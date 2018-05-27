#include "thirdperson.h"

#include "../valve_sdk/csgostructs.hpp"
#include "../options.hpp"
#include "../SpoofedConvar.hpp"


namespace thirdperson
{
	static bool bInThirdPerson = false;

	void FrameStageNotify()
	{
		if (!g_EngineClient->IsTakingScreenshot())
		{
			if (meme && g_EngineClient->IsInGame() && g_LocalPlayer)
			{
				static auto cam_idealdist = g_CVar->FindVar("cam_idealdist");
				static auto cam_ideallag = g_CVar->FindVar("cam_ideallag");

				cam_ideallag->SetValue(0);

				cam_idealdist->SetValue(g_Options.thirdpersonrange);

				if (g_LocalPlayer->IsAlive() && g_Options.thirdperson)
				{
					if (!bInThirdPerson)
					{
						g_EngineClient->ExecuteClientCmd("thirdperson");
						bInThirdPerson = true;
					}
				}
				else if (!g_LocalPlayer->IsAlive() && g_Options.thirdperson)
				{
					if (bInThirdPerson)
					{
						g_EngineClient->ExecuteClientCmd("firstperson");
						bInThirdPerson = false;
					}
				}
				else if (!g_Options.thirdperson)
				{
					if (bInThirdPerson)
					{
						g_EngineClient->ExecuteClientCmd("firstperson");
						bInThirdPerson = false;
					}
				}
			}
		}
		else if (g_EngineClient->IsTakingScreenshot())
		{
			if (bInThirdPerson)
			{
				g_EngineClient->ExecuteClientCmd("firstperson");
				bInThirdPerson = false;
			}
		}
	}
}


































































































