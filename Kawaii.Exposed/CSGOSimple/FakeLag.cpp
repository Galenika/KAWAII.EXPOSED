//#include "FakeLag.h"
//#include "valve_sdk\csgostructs.hpp"
//#include "Options.hpp"
//#include <algorithm>
//
//int m_heldTicks = 0;
//int ticks = 0;
//void on_create_move(C_BasePlayer* local, CUserCmd* cmd, bool& bsendpacket)
//{
//	if (local)
//	{
//		if (local->IsAlive())
//		{
//			if (g_Options.fakelag_amount > 0)
//			{
//				if (!(cmd->buttons & IN_ATTACK))
//				{
//					if ((cmd->buttons & IN_ATTACK))
//					{
//						bsendpacket = true;
//						return;
//					}
//					if (ticks >= 15)
//					{
//						bsendpacket = true;
//						ticks = 0;
//					}
//					else
//					{
//						bsendpacket = ticks < 15 - g_Options.fakelag_amount;
//					}
//					ticks++;
//				}
//			}
//
//			if (g_Options.fakelag_amount)
//			{
//				float tick_interval_sqr = pow(g_GlobalVars->interval_per_tick, 2);
//				float vel_2d_sqr = local->m_vecVelocity().LengthSqr();
//
//				int unclamped_Ticks = std::min< int >(15, static_cast<int>(std::ceilf(sqrt(4096.0f / (vel_2d_sqr * tick_interval_sqr)))));
//				int clamped_Ticks = ((std::max)(0, (std::min)(unclamped_Ticks, 14)) ? unclamped_Ticks : 14);
//
//				if (m_heldTicks < clamped_Ticks)
//				{
//					bsendpacket = false;
//					m_heldTicks++;
//				}
//				else
//				{
//					bsendpacket = true;
//					m_heldTicks = 0;
//				}
//			}
//		}
//	}
//}





































































































