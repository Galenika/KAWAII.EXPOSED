
#include "Visuals.hpp"

#include "../options.hpp"
#include "../valve_sdk/csgostructs.hpp"
#include "../helpers/Math.hpp"
#include "../helpers/Utils.hpp"
#include <map>
#include "../SpoofedConvar.hpp"
#include <time.h>

bool lol = false;
bool lol1 = false;
bool lol2 = false;

static ConVar* cm49305ymv8smf3v4tv;
static SpoofedConvar* mat_showmiplevels_spoofed;

static ConVar* vn3rv2gn9gns9gmfv94rgn394;
static SpoofedConvar* mat_drawgray_spoofed;

static ConVar* anv89w34mwg894tv;
static SpoofedConvar* mat_showlowresimage_spoofed;

std::unordered_map<int, wchar_t> WeaponIcons = {
	{ weapon_deagle, '!' },
	{ weapon_elite, '"' },
	{ weapon_fiveseven, '#' },
	{ weapon_glock, '$' },
	{ weapon_ak47, '%' },
	{ weapon_aug, '&' },
	{ weapon_awp, '\'' },
	{ weapon_famas, '(' },
	{ weapon_g3sg1, ')' },
	{ weapon_galilar, '*' },
	{ weapon_m249, 'H' },
	{ weapon_m4a4, '+' },
	{ weapon_mac10, '-' },
	{ weapon_p90, ';' },
	{ weapon_ump45, '/' },
	{ weapon_xm1014, '0' },
	{ weapon_bizon, '1' },
	{ weapon_mag7, '2' },
	{ weapon_negev, '3' },
	{ weapon_sawedoff, '4' },
	{ weapon_tec9, '5' },
	{ weapon_taser, '6' },
	{ weapon_p2000, '7' },
	{ weapon_mp7, '8' },
	{ weapon_mp9, '9' },
	{ weapon_nova, ':' },
	{ weapon_p250, '.' },
	{ weapon_scar20, '<' },
	{ weapon_sg553, '=' },
	{ weapon_ssg08, '>' },
	{ weapon_knife_ct, '?' },
	{ weapon_flashbang, '@' },
	{ weapon_hegrenade, 'A' },
	{ weapon_smokegrenade, 'B' },
	{ weapon_molotov, 'C' },
	{ weapon_decoy, 'D' },
	{ weapon_incgrenade, 'E' },
	{ weapon_c4, 'F' },
	{ weapon_knife_t, 'G' },
	{ weapon_m4a1s, ',' },
	{ weapon_usp, 'I' },
	{ weapon_cz75, 'J' },
	{ weapon_revolver, 'K' },
	{ weapon_bayonet, 'L' },
	{ weapon_flip, 'M' },
	{ weapon_gut, 'N' },
	{ weapon_karambit, 'O' },
	{ weapon_m9bayonet, 'P' },
	{ weapon_huntsman, 'Q' },
	{ weapon_falchion, 'R' },
	{ weapon_bowie, 'S' },
	{ weapon_butterfly, 'T' },
	{ weapon_pushdagger, 'U' },
};

vgui::HFont esp_font;
vgui::HFont defuse_font;
vgui::HFont dropped_weapons_font;
vgui::HFont activeweapon_esp_font;

struct
{
    C_BasePlayer* pl;
    bool    is_enemy;
	bool is_visible;
    Color   clr;
    Vector  head;
    Vector  feet;
    RECT    bbox;
} esp_ctx;

RECT GetBBox(C_BaseEntity* ent)
{
    RECT rect{};
    auto collideable = ent->GetCollideable();

    if(!collideable)
        return rect;

    auto min = collideable->OBBMins();
    auto max = collideable->OBBMaxs();

    const matrix3x4_t& trans = ent->m_rgflCoordinateFrame();

    Vector points[] = {
        Vector(min.x, min.y, min.z),
        Vector(min.x, max.y, min.z),
        Vector(max.x, max.y, min.z),
        Vector(max.x, min.y, min.z),
        Vector(max.x, max.y, max.z),
        Vector(min.x, max.y, max.z),
        Vector(min.x, min.y, max.z),
        Vector(max.x, min.y, max.z)
    };

    Vector pointsTransformed[8];
    for(int i = 0; i < 8; i++) 
	{
        Math::VectorTransform(points[i], trans, pointsTransformed[i]);
    }

    Vector pos = ent->m_vecOrigin();
    Vector screen_points[8] = {};

    for(int i = 0; i < 8; i++) 
	{
        if(!Math::WorldToScreen(pointsTransformed[i], screen_points[i]))
            return rect;
    }

    auto left = screen_points[0].x;
    auto top = screen_points[0].y;
    auto right = screen_points[0].x;
    auto bottom = screen_points[0].y;

    for(int i = 1; i < 8; i++) {
        if(left > screen_points[i].x)
            left = screen_points[i].x;
        if(top < screen_points[i].y)
            top = screen_points[i].y;
        if(right < screen_points[i].x)
            right = screen_points[i].x;
        if(bottom > screen_points[i].y)
            bottom = screen_points[i].y;
    }
    return RECT{ (long)left, (long)top, (long)right, (long)bottom };
}
//--------------------------------------------------------------------------------
//void Visuals::Misc::DrawSkeleton(IClientEntity* pEntity)
//{
//	studiohdr_t* pStudioHdr = g_MdlInfo->GetStudiomodel(pEntity->GetModel());
//
//	if (!pStudioHdr)
//		return;
//
//	Vector vParent, vChild, sParent, sChild;
//
//	for (int j = 0; j < pStudioHdr->numbones; j++)
//	{
//		mstudiobone_t* pBone = pStudioHdr->GetBone(j);
//
//		if (pBone && (pBone->flags & BONE_USED_BY_HITBOX) && (pBone->parent != -1))
//		{
//			vChild = g_LocalPlayer->GetBonePos(j);
//			vParent = g_LocalPlayer->GetBonePos(pBone->parent);
//
//			if (Math::WorldToScreen(vParent, sParent) && Math::WorldToScreen(vChild, sChild))
//			{
//				g_VGuiSurface->DrawSetColor(255, 255, 255, 255);
//				g_VGuiSurface->DrawLine(sParent[0], sParent[1], sChild[0], sChild[1]);
//			}
//		}
//	}
//}
//--------------------------------------------------------------------------------
bool Visuals::CreateFonts()
{
    esp_font              = g_VGuiSurface->CreateFont_();
    defuse_font           = g_VGuiSurface->CreateFont_();
    dropped_weapons_font  = g_VGuiSurface->CreateFont_();
	activeweapon_esp_font = g_VGuiSurface->CreateFont_();

    g_VGuiSurface->SetFontGlyphSet(esp_font, "Segoe UI Light", 12, 700, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
	g_VGuiSurface->SetFontGlyphSet(activeweapon_esp_font, "csgo_icons", 8, 300, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
    g_VGuiSurface->SetFontGlyphSet(defuse_font, "Segoe UI Light", 12, 700, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
    g_VGuiSurface->SetFontGlyphSet(dropped_weapons_font, "Segoe UI Light", 12, 700, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);

    return true;
}
//--------------------------------------------------------------------------------
void Visuals::DestroyFonts()
{
    // Is there a way to destroy vgui fonts?
    // TODO: GetOffset out
}
//--------------------------------------------------------------------------------
bool Visuals::Player::Begin(C_BasePlayer* pl)
{
	esp_ctx.pl = pl;
	esp_ctx.is_enemy = g_LocalPlayer->m_iTeamNum() != pl->m_iTeamNum();
	esp_ctx.is_visible = g_LocalPlayer->CanSeePlayer(pl, HITBOX_CHEST);

	if (!esp_ctx.is_enemy && g_Options.esp_enemies_only)
		return false;

	esp_ctx.clr = esp_ctx.is_enemy ? (esp_ctx.is_visible ? g_Options.color_esp_enemy_visible : g_Options.color_esp_enemy_occluded) : (esp_ctx.is_visible ? g_Options.color_esp_ally_visible : g_Options.color_esp_ally_occluded);

	auto head = pl->GetHitboxPos(HITBOX_HEAD);
	auto origin = pl->m_vecOrigin();

	head.z += 15;

	if (!Math::WorldToScreen(head, esp_ctx.head) ||
		!Math::WorldToScreen(origin, esp_ctx.feet))
		return false;

	auto h = fabs(esp_ctx.head.y - esp_ctx.feet.y);
	auto w = h / 1.65f;

	esp_ctx.bbox.left = static_cast<long>(esp_ctx.feet.x - w * 0.5f);
	esp_ctx.bbox.right = static_cast<long>(esp_ctx.bbox.left + w);
	esp_ctx.bbox.bottom = static_cast<long>(esp_ctx.feet.y);
	esp_ctx.bbox.top = static_cast<long>(esp_ctx.head.y);

	return true;
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderBox()
{
    g_VGuiSurface->DrawSetColor(esp_ctx.clr);
    g_VGuiSurface->DrawOutlinedRect(esp_ctx.bbox.left, esp_ctx.bbox.top, esp_ctx.bbox.right, esp_ctx.bbox.bottom);
    g_VGuiSurface->DrawSetColor(Color::Outline);
    g_VGuiSurface->DrawOutlinedRect(esp_ctx.bbox.left - 1, esp_ctx.bbox.top - 1, esp_ctx.bbox.right + 1, esp_ctx.bbox.bottom + 1);
    g_VGuiSurface->DrawOutlinedRect(esp_ctx.bbox.left + 1, esp_ctx.bbox.top + 1, esp_ctx.bbox.right - 1, esp_ctx.bbox.bottom - 1);
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderName()
{
    wchar_t buf[128];
	int off = 4;

    player_info_t info = esp_ctx.pl->GetPlayerInfo();

    if(MultiByteToWideChar(CP_UTF8, 0, info.szName, -1, buf, 128) > 0) {
        int tw, th;
        g_VGuiSurface->GetTextSize(esp_font, buf, tw, th);

        g_VGuiSurface->DrawSetTextFont(esp_font);
        g_VGuiSurface->DrawSetTextColor(esp_ctx.clr);
		g_VGuiSurface->DrawSetTextPos(esp_ctx.bbox.right + off, esp_ctx.bbox.top);
        g_VGuiSurface->DrawPrintText(buf, wcslen(buf));
    }
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderHealth()
{
	auto  hp = esp_ctx.pl->m_iHealth();
	float box_h = (float)fabs(esp_ctx.bbox.bottom - esp_ctx.bbox.top);
	//float off = (box_h / 6.f) + 5;
	float off = 8;

	auto height = (((box_h * hp) / 100));

	int red = int(255 - (hp * 2.55f));
	int green = int(hp * 2.55f);

	int x = esp_ctx.bbox.left - off;
	int y = esp_ctx.bbox.top;
	int w = 4;
	int h = box_h;

	g_VGuiSurface->DrawSetColor(Color::Outline);
	g_VGuiSurface->DrawFilledRect(x, y, x + w, y + h);

	g_VGuiSurface->DrawSetColor(Color(red, green, 0, 255));
	g_VGuiSurface->DrawFilledRect(x + 1, y + 1, x + w - 1, y + height - 2);
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderArmour()
{
	auto  armour = esp_ctx.pl->m_ArmorValue();
	float box_h = (float)fabs(esp_ctx.bbox.bottom - esp_ctx.bbox.top);
	//float off = (box_h / 6.f) + 5;
	float off = 9;

	auto height = (((box_h * armour) / 100));

	int x = (esp_ctx.bbox.left - off) - 3;
	int y = esp_ctx.bbox.top;
	int w = 4;
	int h = box_h;

	g_VGuiSurface->DrawSetColor(Color::Outline);
	g_VGuiSurface->DrawFilledRect(x, y, x + w, y + h);

	g_VGuiSurface->DrawSetColor(Color(135, 180, 255, 255));
	g_VGuiSurface->DrawFilledRect(x + 1, y + 1, x + w - 1, y + height - 2);
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderWeapon()
{
	wchar_t buf[80];
	float off = 4;

	auto weapon = esp_ctx.pl->m_hActiveWeapon().Get();

	if (!weapon) return;

	if (g_Options.esp_player_weapons_type == ICONS)
	{
		wchar_t icon[2] = { (wchar_t)WeaponIcons[weapon->m_iItemDefinitionIndex()], 0 };

		// icon
		int tw, th;
		g_VGuiSurface->GetTextSize(activeweapon_esp_font, buf, tw, th);
		g_VGuiSurface->DrawSetTextFont(activeweapon_esp_font);
		g_VGuiSurface->DrawSetTextColor(Color(255, 255, 255, g_Options.color_esp_enemy_visible.a()));
		g_VGuiSurface->DrawSetTextPos(esp_ctx.bbox.right + off, esp_ctx.bbox.top + 11);
		g_VGuiSurface->DrawPrintText(icon, 1);
	} 
	else if (g_Options.esp_player_weapons_type == TEXT)
	{
		// words
		if (MultiByteToWideChar(CP_UTF8, 0, weapon->GetCSWeaponData()->szWeaponName + 7, -1, buf, 80) > 0)
		{
			int tw, th;
			g_VGuiSurface->GetTextSize(esp_font, buf, tw, th);

			g_VGuiSurface->DrawSetTextFont(esp_font);
			g_VGuiSurface->DrawSetTextColor(esp_ctx.clr);
			g_VGuiSurface->DrawSetTextPos(esp_ctx.bbox.right + off, esp_ctx.bbox.top + 8);
			g_VGuiSurface->DrawPrintText(buf, wcslen(buf));
		}
	}
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderSnapline()
{
    int screen_w, screen_h;
    g_EngineClient->GetScreenSize(screen_w, screen_h);

    g_VGuiSurface->DrawSetColor(esp_ctx.clr);
    g_VGuiSurface->DrawLine(screen_w / 2, screen_h, esp_ctx.feet.x, esp_ctx.feet.y);
}
//--------------------------------------------------------------------------------
void Visuals::Misc::RenderCrosshair()
{
	int w;
	int h;

    g_EngineClient->GetScreenSize(w, h);

    int cx = w / 2;
    int cy = h / 2;
	
	// Draws a sexy crosshair
	g_VGuiSurface->DrawSetColor(Color(0, 255, 0, 255));
	g_VGuiSurface->DrawLine(cx - 8, cy - 0, cx + 8, cy + 0);
	g_VGuiSurface->DrawLine(cx + 0, cy - 8, cx - 0, cy + 8);
	g_VGuiSurface->DrawSetColor(Color(255, 255, 255, 255));
	g_VGuiSurface->DrawLine(cx - 4, cy - 0, cx + 4, cy + 0);
	g_VGuiSurface->DrawLine(cx + 0, cy - 4, cx - 0, cy + 4);
}
//--------------------------------------------------------------------------------
void Visuals::Misc::RenderWeapon(C_BaseCombatWeapon* ent)
{
	wchar_t buf[80];
	auto clean_item_name = [](const char* name) -> const char* {
		if (name[0] == 'C')
			name++;

		auto start = strstr(name, "Weapon");
		if (start != nullptr)
			name = start + 6;

		return name;
	};

	// We don't want to Render weapons that are being held
	if (ent->m_hOwnerEntity().IsValid())
		return;

	auto bbox = GetBBox(ent);

	if (bbox.right == 0 || bbox.bottom == 0)
		return;

	if (g_Options.esp_dropped_weapons_boxes)
	{
		// Drawing the box
		g_VGuiSurface->DrawSetColor(g_Options.color_esp_weapons);
		g_VGuiSurface->DrawOutlinedRect(bbox.left, bbox.bottom, bbox.right, bbox.top);
		g_VGuiSurface->DrawSetColor(Color::Outline);
		g_VGuiSurface->DrawOutlinedRect(bbox.left - 1, bbox.bottom - 1, bbox.right + 1, bbox.top + 1);
		g_VGuiSurface->DrawOutlinedRect(bbox.left + 1, bbox.bottom + 1, bbox.right - 1, bbox.top - 1);
	}

	if (g_Options.esp_dropped_weapons_text)
	{
		auto name = clean_item_name(ent->GetClientClass()->m_pNetworkName);

		if (g_Options.esp_player_weapons_type == 0)
		{
			wchar_t icon[2] = { (wchar_t)WeaponIcons[ent->m_iItemDefinitionIndex()], 0 };

			// icon
			int w = bbox.right - bbox.left;
			int tw, th;
			g_VGuiSurface->GetTextSize(activeweapon_esp_font, buf, tw, th);
			g_VGuiSurface->DrawSetTextFont(activeweapon_esp_font);
			g_VGuiSurface->DrawSetTextColor(Color(255, 255, 255, g_Options.color_esp_weapons.a()));
			g_VGuiSurface->DrawSetTextPos(bbox.right + 4, bbox.bottom + 1);
			g_VGuiSurface->DrawPrintText(icon, 1);
		}
		else if (g_Options.esp_player_weapons_type == 1)
		{
			if (MultiByteToWideChar(CP_UTF8, 0, name, -1, buf, 80) > 0)
			{
				int w = bbox.right - bbox.left;
				int tw, th;
				g_VGuiSurface->GetTextSize(esp_font, buf, tw, th);

				g_VGuiSurface->DrawSetTextFont(esp_font);
				g_VGuiSurface->DrawSetTextColor(g_Options.color_esp_weapons);
				g_VGuiSurface->DrawSetTextPos((bbox.left + w * 0.5f) - tw * 0.5f, bbox.bottom + 1);
				g_VGuiSurface->DrawPrintText(buf, wcslen(buf));
			}
		}
	}
}
//--------------------------------------------------------------------------------
void Visuals::Misc::RenderDefuseKit(C_BaseEntity* ent)
{
	if (ent->m_hOwnerEntity().IsValid())
		return;

	auto bbox = GetBBox(ent);

	if (bbox.right == 0 || bbox.bottom == 0)
		return;

	if (g_Options.esp_defuse_kit_boxes)
	{
		// Drawing the box
		g_VGuiSurface->DrawSetColor(g_Options.color_esp_defuse);
		g_VGuiSurface->DrawOutlinedRect(bbox.left, bbox.bottom, bbox.right, bbox.top);
		g_VGuiSurface->DrawSetColor(Color::Outline);
		g_VGuiSurface->DrawOutlinedRect(bbox.left - 1, bbox.bottom - 1, bbox.right + 1, bbox.top + 1);
		g_VGuiSurface->DrawOutlinedRect(bbox.left + 1, bbox.bottom + 1, bbox.right - 1, bbox.top - 1);
	}

	if (g_Options.esp_defuse_kit_text)
	{
		wchar_t* buf = L"X";

		int w = bbox.right - bbox.left;
		int tw, th;
		g_VGuiSurface->GetTextSize(activeweapon_esp_font, buf, tw, th);
		g_VGuiSurface->DrawSetTextFont(activeweapon_esp_font);
		g_VGuiSurface->DrawSetTextColor(Color::White);
		g_VGuiSurface->DrawSetTextPos(bbox.right + 4, bbox.bottom + 1);
		g_VGuiSurface->DrawPrintText(buf, wcslen(buf));
	}
}
//--------------------------------------------------------------------------------
void Visuals::Misc::RenderPlantedC4(C_BaseEntity* ent)
{
	auto bbox = GetBBox(ent);

	if (bbox.right == 0 || bbox.bottom == 0)
		return;

	if (g_Options.esp_planted_c4_boxes)
	{
		// Drawing the box
		g_VGuiSurface->DrawSetColor(g_Options.color_esp_weapons);
		g_VGuiSurface->DrawOutlinedRect(bbox.left, bbox.bottom, bbox.right, bbox.top);
		g_VGuiSurface->DrawSetColor(Color::Outline);
		g_VGuiSurface->DrawOutlinedRect(bbox.left - 1, bbox.bottom - 1, bbox.right + 1, bbox.top + 1);
		g_VGuiSurface->DrawOutlinedRect(bbox.left + 1, bbox.bottom + 1, bbox.right - 1, bbox.top - 1);
	}

	if (g_Options.esp_planted_c4_text)
	{
		wchar_t* buf = L"F";

		int w = bbox.right - bbox.left;
		int tw, th;
		g_VGuiSurface->GetTextSize(activeweapon_esp_font, buf, tw, th);
		g_VGuiSurface->DrawSetTextFont(activeweapon_esp_font);
		g_VGuiSurface->DrawSetTextColor(Color::White);
		g_VGuiSurface->DrawSetTextPos(bbox.right + 4, bbox.bottom + 1);
		g_VGuiSurface->DrawPrintText(buf, wcslen(buf));
	}
}

int chatSpamTick = 0;
std::vector<std::string> messageList;
std::vector<std::string> cspam_kawaii = {
	"kawaii.exposed - premium cheating software, get kawaii.exposed!",
	"kawaii.exposed - cheating is for cool kids, get kawaii.exposed!",
	"kawaii.exposed - same price as an esea sub, lol, get kawaii.exposed!",
	"kawaii.exposed - would u like some sauce with ur pasta?",
	"kawaii.exposed - no sir, i've never heard of ayyware b4",
	"kawaii.exposed - you pay for that baim?",
	"kawaii.exposed - you probably don't know the difference between a float and an int",
	"kawaii.exposed - does it come with junkcode"
};
std::vector<std::string> cspam_Edgy = { "Why don't you try playing fair for once in your life", "The scars you can't see are the hardest to heal", "Being forgotten by the ones you love really hurts", "It's like your screaming, but no one can hear", "If only we could have heard each other's voices", "I came to see her, To save her. I wanted her to be alive", "All we did was hurt each other, I'm sorry", "I can't hear you, but your words still hurt", "Did I change or did you just stop loving me", "You hurt me, are you happy now?", "You told me you wouldn't leave me, here I am alone", "I'm still here, where are you?", "I like to think I actually mean something to you", "I fought for someone who would not fight for me", "I had a crush on you until you crushed me", "You said you loved me but you never showed it", "I wish that I could stop because it hurts so much.", "It hurts to say goodbye to someone you wanna spend your life with", "You always ignored me unless you needed me", "If you give up on me, I'm going to give up on me too", "Have you ever loved someone so much it hurt", "This is not a love story", "Pretty when you cry", "I'm afraid to forget the pain", "I love you, but you're not mine", "I've been thinking too much", "I'm tired of being lonely", "I think I fell in love again", "You can't wake up, this is not a dream", "What if we lost our minds", "I'm not okay, but it's okay", "Darkness is beautiful", "I'm different, fuck your opinion", "Don't say maybe if you want to say no", "The worst day of loving someone is the day you lose them", "You left in peace, left me in pieces", "Kiss my sadness away", "You don't understand. I tried. I really tried.", "I love the things you hate about yourself", "No more love yous", "I miss your voice", "Sadness comes out at night", "I'm not so strong anymore", "Love me", "fuck this shit", "I love you forever", "I wanna be yours", "I'm sorry I wasn't what you wanted", "Maybe I just wanna be yours", "What if I can't forget you", "You have a decision to make", "Why is everything I do, wrong", "Overthinking kills your happiness", "The best thing I can do, is wait for you", "People change and often, to what they said they wouldn't be", "I miss your stupid face", "I still like you, I'm just tired of trying", "Tears come from the heart and not the brain", "Nothing is more sad than the death of an illusion", "Sadness flies away on the wings of time", "A sad soul can kill quicker than cancer", "To have felt too much is to feel nothing", "It doesn't hurt to feel sad from time to time", "I hate that I'm still hoping", "If you love me, let me know", "You promised me", "Don't play with my feelings", "He's going to break you", "My heart finally said enough is enough", "Someday you might miss me, atleast I hope so", "I know you have a family now", "At the time it was so unclear", "Its my fault for believing you", "Nothings really gonna change", "I tried to reach out to you, but in vain", "I think I'm lost just like you used to be", "You're the reason I don't believe in love", "Don't fall in love", "You're the pain I wont give up", "It hurts, but it's ok.. I'm used to it", "I cannot eat, I cannot drink, I cannot love", "Our love is my regret", "I still repeat the things you said to me in my head", "I broke my own heart loving you", "For you, I was a chapter. For me, you were the book", "I feel so damn worthless", "Please tell me i'm not as forgettable as your silence says", "I fought for someone who would not fight for me", "You will always have a piece of my heart", "I lied because I don't want you to feel this pain", "I wish you know how much it hurt me when you left", "I wanted to scream but all I could do was whisper that I'm fine", "I have poured my heart out... And now I am empty", "The person who tries to keep everyone together is the loneliest", "In reality, nobody really gives a fuck", "Yes, I still love you, I still care, but no I don't want you back", "I think a part of me will always be waiting for you", "I love you, and its killing me", "You didn't love her, you just didn't want to be alone", "You don't destroy the person that you love", "I still pretend that you'll love me some day", "I still waste my wishes on you", "Everyone I could ever want, nothing I could ever have", "The worst feeling is not knowing whether you should wait", "I still love you. But you don't care", "I've been having a lot of bad days lately", "I really do miss what we almost had", "I wish that I had never met you", "It sucks doesn't it. Feeling like you aren't good enough", "I just wish I could lose these feelings as fast as I lost you.", "Grief is not as heavy as guilt, but it takes more away from you.", "Iéˆ¥æª’l be okay is that what you want to say", "When you cry so much it makes you realize breathing is hard.", "It's sad when someone you know becomes someone you knew.", "I did everything right for someone that does everything wrong.", "Love is the person you think about during the sad songs.", "Sometimes it takes losing something to realize what youéˆ¥æªe had.", "You only wanted me when you needed me", "I didn't choose to love you. It just happened.", "I'm tired of fighting", "You had me at a point where I would have left everything for you", "Once upon a time I was falling inlove, now I'm only falling apart", "I just want to feel important to someone", "My stomach drops when I think of anyone else having you", "Tears are the words the heart can't say", "Leave him alone. That's what he wants", "I left because you never asked me to stay", "Have you ever been so sad it physically hurts", "I looked at him as a friend until I realized I love him", "Wanting someone that doesn't want you back is so awful", "I knew he didn't love me, but I adored him anyway", "Just because I let you go, doesn't mean I wanted to", "Don't say you miss me when you don't even try to talk to me", "And then I think maybe I was designed to be alone", "I'm breaking inside", "Love brought us together, bad timing tore us apart", "I miss how me and you used to be", "Never let an old flame burn you twice", "You can never love people as much as you can miss them", "Broke your heart on your birthday", "Never run back to whatever broke you", "You not wanting me was the beginning of me wanting myself", "Sometimes we create our own heartbreaks through expectation", "If you don't let your past die, your past won't let you live", "I bet you'll never remember what I will never forget", "You made my mind up for me when you started to ignore me", "Yes, I've changed. Pain does that to people", "Love doesn't hurt; loving the wrong person does", "The story goes on without you", "Thanks to you, I don't even recognize myself anymore", "If he's dumb enough to walk away, be smart enough to let him go", "You don't deserve my tears", "The sad thing is that I actually thought you'd be different", "I can pretend I'm not dying inside, but just so you know... I am", "I'm too broken to hold on, but too in love to let go", "Apologies don't fix broken hearts", "Truth is, you're the reason I don't believe in love anymore", "You're an expert at sorry and keeping the lines blurry", "Be all in or get out. There is no halfway", "People don't change; they reveal who they really are", "If I cut you off, chances are, you handed me the scissors", "Now we're back to the way we started, as strangers", "Don't fall in love. Fall off a bridge. It hurts less", "It hurts when you make time for everyone else but me", "Someday, someone is going to thank you for letting me go", "It takes a second to say hello and a lifetime to say goodbye", "If someone won't fight for you, they're not right for you", "Have a nice life. I'm done trying to be in it", "Don't take my love for granted because it won't last forever", "You know a girl is hurt when she ignores you", "Silence is the loudest cry", "Don't say you miss me when it's your fault I'm gone", "The worst part of it all wasn't losing him; it was losing me", "The opposite of love is not hate, it's indifference", "I hate being second to those I put first", "It hurts to let go, but sometimes it hurts more to hold on", "The hottest love has the coldest end", "Of all the lies I've heard, 'I love you' was my favorite", "As soon as forever is through, I'll be over you", "Cry as I may, these tears won't wash away", "The best way to mend a broken heart is time and girlfriends", "He taught me how to love, but not how to stop", "Tears are words the heart can't express", "The hardest thing to do is waking up without you", "True love is when you shed a tear and still want him", "Wherever you are, you will always be in my heart", "In my dreams you're mine forever", "Loving you was my favorite mistake", "You can't buy love, but you can pay heavily for it", "I hate you and everything we once were", "True love will never fade, unless it was a lie", "If you love me so much, why are you walking away", "The memories are worth the pain", "While I was holding on, all you did was let go", "Every time we talk, I fall a little harder", "When you left, I lost a part of me", "A million words won't bring you back", "It's a thing called heartbreak and it hurts like hell", "Where there is love, there is pain", "Love is like heaven, but it can hurt like hell", "Why did I fall for you when you keep falling for her", "All I know is that I'm lost without you", "I miss you more than you'll ever know", "How could an angel break my heart", "Everything...affects everything", "My room is so quiet and empty it hurts", "I went down to the river...", "People rarely bring flowers to a suicide", "After all, suicide is contagious", "It's better to burn out than to fade away", "he sun stopped shining for me is all", "Every suicide is a solution to a problem", "Nine men in ten are would-be suicides", "Suicide does not mean there was no killer", "Suicide is not a blot on anyoneéˆ¥æªš name; it is a tragedy", "Drunkenness is temporary suicide", "No one ever lacks a good reason for suicide", "Civilizations die from suicide, not by murder", "Suicide leaves everyone feeling guilty", "People do not die from suicide, they die from sadness", "Fear of heights is fear of a desire to jump", "He killed himself for wanting to live" };
void Visuals::Misc::ChatSpam() {
	switch (g_Options.chatspam) {
	case chatspam_none:
		break;
	case KAWAII_SPAM:
		messageList = cspam_kawaii;
		break;
	case TUMBLR_SPAM:
		messageList = cspam_Edgy;
			break;
		}
		srand(time(0) + rand());
		int selection = int(static_cast<int>(messageList.size()) * rand() / (RAND_MAX + 1.0));
		chatSpamTick++;
		if (chatSpamTick >= 100) {
			g_EngineClient->ExecuteClientCmd(("say " + messageList.at(selection)).c_str());
			chatSpamTick = 0;
		}
	}

//bool PrecacheModel(const char* szModelName)
//{
//	INetworkStringTable* m_pModelPrecacheTable = -ClientStringTableContainer>FindTable("modelprecache");
//
//	if (m_pModelPrecacheTable)
//	{
//		Interfaces::ModelInfo()->GetModelIndex(szModelName);
//		const char *ptr = szModelName;
//		int idx = m_pModelPrecacheTable->AddString(false, szModelName);
//		if (idx == INVALID_STRING_INDEX)
//			return false;
//	}
//	return true;
//}
//

//--------------------------------------------------------------------------------
void Visuals::Misc::RenderDrugs()
{
	if (meme)
	{
		if (!lol)
		{
			cm49305ymv8smf3v4tv = g_CVar->FindVar("mat_showmiplevels");
			mat_showmiplevels_spoofed = new SpoofedConvar(cm49305ymv8smf3v4tv);
			lol = true;
		}

		if (g_Options.drugs && mat_showmiplevels_spoofed->GetInt() == 0)
			mat_showmiplevels_spoofed->SetInt(1);
		else if (!g_Options.drugs && mat_showmiplevels_spoofed->GetInt() == 1)
			mat_showmiplevels_spoofed->SetInt(0);
	}
}
//--------------------------------------------------------------------------------
void Visuals::Misc::RenderPaper()
{
	if (meme)
	{
		if (!lol1)
		{
			vn3rv2gn9gns9gmfv94rgn394 = g_CVar->FindVar("mat_drawgray");
			mat_drawgray_spoofed = new SpoofedConvar(vn3rv2gn9gns9gmfv94rgn394);
			lol1 = true;
		}

		if (g_Options.paper && mat_drawgray_spoofed->GetInt() == 0)
			mat_drawgray_spoofed->SetInt(1);
		else if (!g_Options.paper && mat_drawgray_spoofed->GetInt() == 1)
			mat_drawgray_spoofed->SetInt(0);
	}
}
//--------------------------------------------------------------------------------
void Visuals::Misc::RenderLowResTextures()
{
	if (meme)
	{
		if (!lol2)
		{
			anv89w34mwg894tv = g_CVar->FindVar("mat_showlowresimage");
			mat_showlowresimage_spoofed = new SpoofedConvar(anv89w34mwg894tv);
			lol2 = true;
		}

		if (g_Options.lowrestextures && mat_showlowresimage_spoofed->GetInt() == 0)
			mat_showlowresimage_spoofed->SetInt(1);
		else if (!g_Options.lowrestextures && mat_showlowresimage_spoofed->GetInt() == 1)
			mat_showlowresimage_spoofed->SetInt(0);
	}
}
//--------------------------------------------------------------------------------


//
//auto flash = g_MatSystem->FindMaterial(mdl->szName, TEXTURE_GROUP_CLIENT_EFFECTS);
//auto flashwhite = g_MatSystem->FindMaterial(mdl->szName, TEXTURE_GROUP_CLIENT_EFFECTS);
//
//if (!flash || !flashwhite)
//return;
//
//if (g_Options.noflash)
//{
//	flash->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
//	flashwhite->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
//	g_MdlRender->ForcedMaterialOverride(flash);
//	g_MdlRender->ForcedMaterialOverride(flashwhite);
//}



//--------------------------------------------------------------------------------


































































































