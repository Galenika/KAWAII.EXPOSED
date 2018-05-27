#include "Interfaces.h"
#include "Hooks.hpp"
#include "Menu.hpp"
#include "Options.hpp"
#include "helpers/InputSys.hpp"
#include "helpers/Utils.hpp"
#include "features/bhop.hpp"
#include "features/Chams.hpp"
#include "features/Visuals.hpp"
#include "features/glow.hpp"
#include "features/memewalk.h"
#include "features/nightmode.h"
#include "features/skinchanger.h"
#include "features/grenadeprediction.h"
#include "features/thirdperson.h"
#include "features\visuals.hpp"
#include "Aimbot.h"
#include "AntiAims.h"
#include "SpectatorList.h"
#include "FakeLag.h"
#include "AutoStrafer.h"
#include "Circlestrafer.h"
#include "hitmarkers.h"
#include "triggerbot.h"
#include "backtracking.h"
#include "clantag.h"
#include "helpers/math.hpp"
#include "Resolver.h"
#include "aimbot2.h"

#include "SpoofedConvar.hpp"
#include "fonts\IconsFontAwesome.h"
#include "IMDLCache.h"


IVEngineClient*       g_EngineClient = nullptr;
IBaseClientDLL*       g_CHLClient = nullptr;
IClientEntityList*    g_EntityList = nullptr;
CGlobalVarsBase*      g_GlobalVars = nullptr;
IEngineTrace*         g_EngineTrace = nullptr;
ICvar*                g_CVar = nullptr;
IPanel*               g_VGuiPanel = nullptr;
IClientMode*          g_ClientMode = nullptr;
IVDebugOverlay*       g_DebugOverlay = nullptr;
ISurface*             g_VGuiSurface = nullptr;
CInput*               g_Input = nullptr;
IVModelInfoClient*    g_MdlInfo = nullptr;
IVModelRender*        g_MdlRender = nullptr;
IVRenderView*         g_RenderView = nullptr;
IMaterialSystem*      g_MatSystem = nullptr;
IGameEventManager2*   g_GameEvents = nullptr;
IMoveHelper*          g_MoveHelper = nullptr;
IMDLCache*            g_MdlCache = nullptr;
IPrediction*          g_Prediction = nullptr;
CGameMovement*        g_GameMovement = nullptr;
IEngineSound*         g_EngineSound = nullptr;
CGlowObjectManager*   g_GlowObjManager = nullptr;
IViewRender*          g_ViewRender = nullptr;
IDirect3DDevice9*     g_D3DDevice9 = nullptr;
CClientState*         g_ClientState = nullptr;
IPhysicsSurfaceProps* g_PhysSurface = nullptr;
C_LocalPlayer         g_LocalPlayer;







































































































