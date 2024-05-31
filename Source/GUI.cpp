#include "stdafx.h"

typedef bool(__cdecl* fpIsDLCPresent)();
typedef bool(__cdecl* fpSetName)();
typedef bool(__cdecl* fpTriggerScriptEvent)(int, void*, int, int);
typedef bool(__cdecl* fpSetLobbyWeather)(int, int, int, __int64);
typedef bool(__cdecl* fpIncrementStatHook)(__int64 a1, __int64 a2, float a3);
typedef void(__cdecl* fpAddOwnedExplosion)(Ped ped, float x, float y, float z, int explosionType, float damageScale, BOOL isAudible, BOOL isInvisible, float cameraShake);
typedef bool(__cdecl* fpAddTextCompSubstrPlayerName)(char* text);
typedef bool(__cdecl* fpEndTextCmdDisplayText)(float x, float y);
typedef bool(__cdecl* fpBeginTextCmdDisplayText)(char* text);
typedef bool(__cdecl* fpSetLobbyTime)(int, int, int);
typedef bool(__cdecl* fpGetPlayerPed)(Player player);
typedef bool(__cdecl* fpSendMessage2)(char* message, int* networkHandle);
typedef uint32_t* (*__cdecl        fpFileRegister)(int*, const char*, bool, const char*, bool);
typedef BOOL(_cdecl* fpGetEventData)(int eventGroup, int eventIndex, uint64_t* argStruct, int argStructSize);
class Hook
{
private:
	static BOOL InitializeHooks();
	static void FindPatterns();
	static void FailPatterns(const char* name);

public:
	static fpFileRegister            m_fileregister;
	static std::vector<LPVOID>		m_hooks;
	static uint64_t* m_frameCount;
	static fpIsDLCPresent			is_DLC_present;
	static fpSetName			    SetName;
	static fpTriggerScriptEvent	    trigger_script_event;
	static fpSetLobbyWeather	    set_lobby_weather;
	static fpGetEventData	        get_event_data;
	static fpIncrementStatHook	    increment_stat_hook;
	static fpAddOwnedExplosion	    add_owned_explosion;
	static fpSetLobbyTime	        set_lobby_time;
	static fpSendMessage2	        send_message2;
	static fpGetPlayerPed	        get_player_ped;
	static fpAddTextCompSubstrPlayerName	    add_text_comp_substr_playername;
	static fpEndTextCmdDisplayText	    end_text_cmd_display_text;
	static fpBeginTextCmdDisplayText	    begin_text_cmd_display_text;
	static void Start(HMODULE hmoduleDLL);
	static void Cleanup();
	static eGameState GetGameState();
	static BlipList* GetBlipList();
	static uint64_t getWorldPtr();
	static void onTickInit();
	static bool HookNatives();
	static __int64** getGlobalPtr();
	static void defuseEvent(RockstarEvent e, bool toggle);

	static __int64* getGlobalPatern(int index);

	// Native function handler type
	typedef void(__cdecl* NativeHandler)(scrNativeCallContext* context);
	struct NativeRegistrationNew
	{
		uint64_t nextRegistration1;
		uint64_t nextRegistration2;
		Hook::NativeHandler handlers[7];
		uint32_t numEntries1;
		uint32_t numEntries2;
		uint64_t hashes;

		inline NativeRegistrationNew* getNextRegistration()
		{
			uintptr_t result;
			auto v5 = reinterpret_cast<uintptr_t>(&nextRegistration1);
			auto v12 = 2i64;
			auto v13 = v5 ^ nextRegistration2;
			auto v14 = (char*)&result - v5;
			do
			{
				*(DWORD*)&v14[v5] = v13 ^ *(DWORD*)v5;
				v5 += 4i64;
				--v12;
			} while (v12);

			return reinterpret_cast<NativeRegistrationNew*>(result);
		}

		inline uint32_t getNumEntries()
		{
			return ((uintptr_t)&numEntries1) ^ numEntries1 ^ numEntries2;
		}

		inline uint64_t getHash(uint32_t index)
		{

			auto naddr = 16 * index + reinterpret_cast<uintptr_t>(&nextRegistration1) + 0x54;
			auto v8 = 2i64;
			uint64_t nResult;
			auto v11 = (char*)&nResult - naddr;
			auto v10 = naddr ^ *(DWORD*)(naddr + 8);
			do
			{
				*(DWORD*)&v11[naddr] = v10 ^ *(DWORD*)(naddr);
				naddr += 4i64;
				--v8;
			} while (v8);

			return nResult;
		}
	};
	static NativeHandler GetNativeHandler(uint64_t origHash);
};

void Menu::Drawing::Text(const char * text, RGBAF rgbaf, VECTOR2 position, VECTOR2_2 size, bool center)
{
	UI::SET_TEXT_CENTRE(center);
	UI::SET_TEXT_COLOUR(rgbaf.r, rgbaf.g, rgbaf.b, rgbaf.a);
	UI::SET_TEXT_FONT(rgbaf.f);
	UI::SET_TEXT_SCALE(size.w, size.h);
	//UI::SET_TEXT_DROPSHADOW(1, 0, 0, 0, 0);
	//UI::SET_TEXT_EDGE(1, 0, 0, 0, 0); // Let's make the text a bit nice fuck that black ugly outline 
	//UI::SET_TEXT_OUTLINE();
	UI::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME((char*)text);
	UI::END_TEXT_COMMAND_DISPLAY_TEXT(position.x, position.y);
}

void Menu::Drawing::Spriter(std::string Streamedtexture, std::string textureName, float x, float y, float width, float height, float rotation, int r, int g, int b, int a)
{
	if (!GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED((char*)Streamedtexture.c_str()))
	{
		GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT((char*)Streamedtexture.c_str(), false);
	}
	else
	{
		GRAPHICS::DRAW_SPRITE((char*)Streamedtexture.c_str(), (char*)textureName.c_str(), x, y, width, height, rotation, r, g, b, a);
	}
}

void Menu::Drawing::Rect(RGBA rgba, VECTOR2 position, VECTOR2_2 size)
{
	GRAPHICS::DRAW_RECT(position.x, position.y, size.w, size.h, rgba.r, rgba.g, rgba.b, rgba.a);
}

float gGlareDir;
float conv360(float base, float min, float max)
{
	float fVar0;
	if (min == max) return min;
	fVar0 = max - min;
	base -= SYSTEM::ROUND(base - min / fVar0) * fVar0;
	if (base < min) base += fVar0;
	return base;
}

//Tick Boxes Position
float Menu::Settings::TickBoxLevel = 0.114f;
float Menu::Settings::TickBoxThicnessLevel = 0.015f;
float Menu::Settings::TickBoxLevel2 = 0.114f;
float Menu::Settings::TickBoxHeightLevel = 0.025f;

//Header Subtitle Pos 
float Menu::Settings::SubtitleXPos = 0.00f;
float Menu::Settings::SubtitleYPos = 0.1450f;
float Menu::Settings::HeaderXPos = 0.17f;
float Menu::Settings::HeaderYPos = 0.0775f;
float Menu::Settings::TextPosX = 0.104f;
float Menu::Settings::TextPosY = 0.120f;
float Menu::Settings::TextWidness = 0.025f;
float Menu::Settings::TextThicness = 0.020f;
float Menu::Settings::RectThicness = 0.210f;
float Menu::Settings::Rectwideness = 0.04f; 

//End Arrows Okay i see

float Menu::Settings::ArrowPosX = 0.140f;
float Menu::Settings::ArrowPosY = 0.020f;
float Menu::Settings::ArrowWidness = 0.035f;
float Menu::Settings::ArrowThicness = 180;

//TextThicness
float Menu::Settings::menuX = 0.17f;
bool Menu::Settings::selectPressed = false;
bool Menu::Settings::leftPressed = false;
bool Menu::Settings::rightPressed = false;
bool firstopen = true;
int Menu::Settings::maxVisOptions = 16;
int Menu::Settings::currentOption = 0;
int Menu::Settings::optionCount = 0;
int Menu::Settings::alpha = 255;
SubMenus Menu::Settings::currentMenu;
int Menu::Settings::menuLevel = 0;
int Menu::Settings::optionsArray[1000]; 
SubMenus Menu::Settings::menusArray[1000];

RGBAF Menu::Settings::SubtitleText{ 255, 255, 255, 255,255 };
RGBA Menu::Settings::SubtitleRect{ 255, 255, 255, 255 };
RGBAF Menu::Settings::count{ 255, 255, 255, 255, 6 };
RGBAF Menu::Settings::titleText{ 230, 230, 230, 255, 7 };
RGBA Menu::Settings::titleRect{ 0, 0, 0, 255 };
RGBAF Menu::Settings::optionText{ 230, 230, 230, 255, 6 };
RGBAF Menu::Settings::breakText{ 255, 255, 255, 255, 1 };
RGBA Menu::Settings::optionRect{ 0, 0, 0, 255 };
RGBA Menu::Settings::scroller{ 229,223,94, 255 };
RGBAF Menu::Settings::integre{ 230, 230, 230, 255, 2 };
RGBA Menu::Settings::line{ 0, 0, 0, 255 };
RGBA Menu::Settings::primary{ 0, 0, 255 };
RGBA Menu::Settings::secondary{ 0, 0, 0 };

//globo
float GlareY = 0.4954f; //Direita
float Glarewidth = 1.0240f; //Largura do brilho
float Glareheight = 0.0445f; //Altura do brilho

static fpFileRegister file_register =
(fpFileRegister)(
	Memory::pattern(
		"48 89 5C 24 ? 48 89 6C 24 ? 48 89 7C 24 ? 41 54 41 56 41 57 48 83 EC 50 48 8B EA 4C 8B FA 48 8B D9 4D 85 C9").count(
			1).get(0).get<decltype(file_register)>());


bool FileExists(const std::string& fileName) {
	struct stat buffer;
	return (stat(fileName.c_str(), &buffer) == 0);
}

void Menu::Drawing::YTD() {
	std::string path = "C://Nano/Textures/";
	std::string name = "Nano.ytd";
	const std::string fullPath = path + name;

	int textureID;
	if (FileExists(fullPath))
		file_register(&textureID, fullPath.c_str(), true, name.c_str(), false);
}


//Fuck this old shitty way 
void YTDNotification()
{
	std::string path = "C://Nano/";
	std::string name = "Nano.ytd";
	const std::string fullPath = path + name;

	int textureID;
	if (FileExists(fullPath))
		file_register(&textureID, fullPath.c_str(), true, name.c_str(), false);
	GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT("Logo", false);
}


namespace Globe
{
	int gGlareHandle; // int
	float gGlareDir;

	// Function(s)
	float conv360(float base, float min, float max) {
		// This is the function the script used. I just adapted it for C++.
		float fVar0;
		if (min == max) return min;
		fVar0 = max - min;

		if (base < min) base += fVar0;
		return base;
	}
	void drawGlare(float pX, float pY, float sX = 1, float sY = 1, int r = 255, int g = 255, int b = 255) {

		gGlareHandle = GRAPHICS::REQUEST_SCALEFORM_MOVIE("MP_MENU_GLARE");

		Vector3 rot = CAM::_GET_GAMEPLAY_CAM_ROT(2);

		float dir = conv360(rot.z, 0, 360);

		if ((gGlareDir == 0 || gGlareDir - dir > 0.5) || gGlareDir - dir < -0.5) {

			gGlareDir = dir;

			GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(gGlareHandle, "SET_DATA_SLOT");

			GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_FLOAT(gGlareDir);

			GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();
		}

		GRAPHICS::DRAW_SCALEFORM_MOVIE(gGlareHandle, pX, pY, sX, sY, r, g, b, 255, 0);

	}
};
float titlebox = 0.17f;
void Menu::Title(const char * title)
{
	Drawing::Text(title, Settings::titleText, { Settings::menuX, 0.095f }, { 0.85f, 0.85f }, true);
	Drawing::Spriter("Nano", "Header2", Settings::menuX, 0.1175f, 0.21f, 0.085f, 0, Settings::titleRect.r, Settings::titleRect.g, Settings::titleRect.b, Settings::titleRect.a);
	


	static struct {
		LPCSTR vehiclePreviewDict;
		char* vehicleName;
	} VehiclePreviews[267] = {
		{ "lgm_default", "adder" },
		{ "lgm_default", "banshee" },
		{ "lgm_default", "carboniz" },
		{ "lgm_default", "carbon" },
		{ "lgm_default", "cheetah" },
		{ "lgm_default", "cogcarbi" },
		{ "lgm_default", "comet2" },
		{ "lgm_default", "coquette" },
		{ "lgm_default", "elegy2" },
		{ "lgm_default", "entityxf" },
		{ "lgm_default", "exmplar" },
		{ "lgm_default", "feltzer" },
		{ "lgm_default", "hotknife" },
		{ "lgm_default", "infernus" },
		{ "lgm_default", "jb700" },
		{ "lgm_default", "khamel" },
		{ "lgm_default", "monroe" },
		{ "lgm_default", "ninef" },
		{ "lgm_default", "ninef2" },
		{ "lgm_default", "rapidgt" },
		{ "lgm_default", "rapidgt2" },
		{ "lgm_default", "stinger" },
		{ "lgm_default", "stingerg" },
		{ "lgm_default", "superd" },
		{ "lgm_default", "surano_convertable" },
		{ "lgm_default", "vacca" },
		{ "lgm_default", "voltic_tless" },
		{ "lgm_default", "ztype" },
		{ "lgm_dlc_apartments", "baller3_web_vehicle_regular_b" },
		{ "lgm_dlc_apartments", "baller4_web_vehicle_regular_b" },
		{ "lgm_dlc_apartments", "voltic_tless" },
		{ "lgm_dlc_apartments", "cog55_web_vehicle_regular_b" },
		{ "lgm_dlc_apartments", "cognosc_web_vehicle_regular_b" },
		{ "lgm_dlc_apartments", "mamba" },
		{ "lgm_dlc_apartments", "niteshad" },
		{ "lgm_dlc_apartments", "schafter3_web_vehicle_regular_b" },
		{ "lgm_dlc_apartments", "schafter4_web_vehicle_regular_b" },
		{ "lgm_dlc_apartments", "verlier" },
		{ "lgm_dlc_biker", "hakuchou2" },
		{ "lgm_dlc_biker", "raptor" },
		{ "lgm_dlc_biker", "shotaro" },
		{ "lgm_dlc_business", "alpha" },
		{ "lgm_dlc_business", "jester" },
		{ "lgm_dlc_business", "turismor" },
		{ "lgm_dlc_business2", "banshee_tless" },
		{ "lgm_dlc_business2", "coquette_tless" },
		{ "lgm_dlc_business2", "huntley" },
		{ "lgm_dlc_business2", "massacro" },
		{ "lgm_dlc_business2", "stinger_tless" },
		{ "lgm_dlc_business2", "thrust" },
		{ "lgm_dlc_business2", "voltic_htop" },
		{ "lgm_dlc_business2", "zentorno" },
		{ "lgm_dlc_executive1", "bestiagts" },
		{ "lgm_dlc_executive1", "fmj" },
		{ "lgm_dlc_executive1", "pfister811" },
		{ "lgm_dlc_executive1", "prototipo" },
		{ "lgm_dlc_executive1", "reaper" },
		{ "lgm_dlc_executive1", "seven70" },
		{ "lgm_dlc_executive1", "windsor2" },
		{ "lgm_dlc_executive1", "xls_web_vehicle_regular_b" },
		{ "lgm_dlc_heist", "casco" },
		{ "lgm_dlc_heist", "lectro" },
		{ "lgm_dlc_importexport", "penetrator" },
		{ "lgm_dlc_importexport", "tempesta" },
		{ "lgm_dlc_lts_creator", "furore" },
		{ "lgm_dlc_luxe", "brawler" },
		{ "lgm_dlc_luxe", "chino" },
		{ "lgm_dlc_luxe", "coquette3" },
		{ "lgm_dlc_luxe", "feltzer3" },
		{ "lgm_dlc_luxe", "osiris" },
		{ "lgm_dlc_luxe", "t20" },
		{ "lgm_dlc_luxe", "vindicator" },
		{ "lgm_dlc_luxe", "virgo" },
		{ "lgm_dlc_luxe", "windsor_windsor_lgm_1_b" },
		{ "lgm_dlc_pilot", "coquette2" },
		{ "lgm_dlc_pilot", "coquette2_tless" },
		{ "lgm_dlc_specialraces", "gp1" },
		{ "lgm_dlc_specialraces", "infernus2" },
		{ "lgm_dlc_specialraces", "ruston" },
		{ "lgm_dlc_specialraces", "turismo2" },
		{ "lgm_dlc_stunt", "le7b_lms_le7b_livery_1_b" },
		{ "lgm_dlc_stunt", "lynx_lms_lynx_livery_2_b" },
		{ "lgm_dlc_stunt", "sheava_lms_sheava_livery_2_b" },
		{ "lgm_dlc_stunt", "tyrus_lms_tyrus_livery_1_b" },
		{ "lgm_dlc_valentines", "roosevelt" },
		{ "lgm_dlc_valentines2", "roosevelt2" },
		{ "lsc_default", "buccaneer2" },
		{ "lsc_default", "chino2" },
		{ "lsc_default", "faction2" },
		{ "lsc_default", "moonbeam2" },
		{ "lsc_default", "primo2" },
		{ "lsc_default", "voodoo" },
		{ "lsc_dlc_import_export", "comet3" },
		{ "lsc_dlc_import_export", "diablous2" },
		{ "bruiser_scifi", "elegy" },
		{ "lsc_dlc_import_export", "fcr2" },
		{ "lsc_dlc_import_export", "italiagtb2" },
		{ "lsc_dlc_import_export", "nero2" },
		{ "lsc_dlc_import_export", "specter2" },
		{ "lsc_jan2016", "banshee2" },
		{ "lsc_jan2016", "sultan2" },
		{ "lsc_lowrider2", "faction3" },
		{ "lsc_lowrider2", "minivan2" },
		{ "lsc_lowrider2", "sabregt2" },
		{ "lsc_lowrider2", "slamvan3" },
		{ "lsc_lowrider2", "tornado5" },
		{ "lsc_lowrider2", "virgo2" },
		{ "sssa_default", "akuma" },
		{ "sssa_default", "baller2" },
		{ "sssa_default", "banshee" },
		{ "sssa_default", "bati" },
		{ "sssa_default", "bati2_sss_cerveza_b" },
		{ "sssa_default", "bfinject" },
		{ "sssa_default", "bifta" },
		{ "sssa_default", "bison" },
		{ "sssa_default", "blazer" },
		{ "sssa_default", "bodhi" },
		{ "sssa_default", "cavcade" },
		{ "sssa_default", "comet2" },
		{ "sssa_default", "dilettan" },
		{ "sssa_default", "double" },
		{ "sssa_default", "dune" },
		{ "sssa_default", "exemplar" },
		{ "sssa_default", "faggio" },
		{ "sssa_default", "felon" },
		{ "sssa_default", "felon2" },
		{ "sssa_default", "feltzer" },
		{ "sssa_default", "fugitive" },
		{ "sssa_default", "gauntlet" },
		{ "sssa_default", "hexer" },
		{ "sssa_default", "infernus" },
		{ "sssa_default", "issi2" },
		{ "sssa_default", "kalahari" },
		{ "sssa_default", "ninef" },
		{ "sssa_default", "oracle" },
		{ "sssa_default", "paradise_sss_logger_b" },
		{ "sssa_default", "pcj" },
		{ "sssa_default", "rebel" },
		{ "sssa_default", "rocoto" },
		{ "sssa_default", "ruffian" },
		{ "sssa_default", "sadler" },
		{ "sssa_default", "sanchez_sss_atomic_b" },
		{ "sssa_default", "sanchez2" },
		{ "sssa_default", "sandkin2" },
		{ "sssa_default", "sandking" },
		{ "sssa_default", "schwarze" },
		{ "sssa_default", "superd" },
		{ "sssa_default", "surano" },
		{ "sssa_default", "vacca" },
		{ "sssa_default", "vader" },
		{ "sssa_default", "vigero" },
		{ "sssa_default", "zion" },
		{ "sssa_default", "zion2" },
		{ "sssa_dlc_biker", "avarus" },
		{ "sssa_dlc_biker", "bagger" },
		{ "sssa_dlc_biker", "blazer4" },
		{ "sssa_dlc_biker", "chimera" },
		{ "sssa_dlc_biker", "daemon2" },
		{ "sssa_dlc_biker", "defiler" },
		{ "sssa_dlc_biker", "esskey" },
		{ "sssa_dlc_biker", "faggio3" },
		{ "sssa_dlc_biker", "faggion" },
		{ "sssa_dlc_biker", "manchez" },
		{ "sssa_dlc_biker", "nightblade" },
		{ "sssa_dlc_biker", "ratbike" },
		{ "sssa_dlc_biker", "sanctus" },
		{ "sssa_dlc_biker", "tornado6" },
		{ "sssa_dlc_biker", "vortex" },
		{ "sssa_dlc_biker", "woflsbane" },
		{ "sssa_dlc_biker", "youga2" },
		{ "sssa_dlc_biker", "zombiea" },
		{ "sssa_dlc_biker", "zombieb" },
		{ "sssa_dlc_business", "asea" },
		{ "sssa_dlc_business", "astrope" },
		{ "sssa_dlc_business", "bobcatxl" },
		{ "sssa_dlc_business", "cavcade2" },
		{ "sssa_dlc_business", "granger" },
		{ "sssa_dlc_business", "ingot" },
		{ "sssa_dlc_business", "intruder" },
		{ "sssa_dlc_business", "minivan" },
		{ "sssa_dlc_business", "premier" },
		{ "sssa_dlc_business", "radi" },
		{ "sssa_dlc_business", "rancherx" },
		{ "sssa_dlc_business", "stanier" },
		{ "sssa_dlc_business", "stratum" },
		{ "sssa_dlc_business", "washingt" },
		{ "sssa_dlc_business2", "dominato" },
		{ "sssa_dlc_business2", "f620" },
		{ "sssa_dlc_business2", "fusilade" },
		{ "sssa_dlc_business2", "penumbra" },
		{ "sssa_dlc_business2", "sentinel" },
		{ "sssa_dlc_business2", "sentinel_convertable" },
		{ "sssa_dlc_christmas_2", "jester2" },
		{ "sssa_dlc_christmas_2", "massacro2" },
		{ "sssa_dlc_christmas_2", "rloader2" },
		{ "sssa_dlc_christmas_2", "slamvan" },
		{ "sssa_dlc_christmas_3", "tampa" },
		{ "sssa_dlc_executive_1", "rumpo3" },
		{ "sssa_dlc_halloween", "btype2_sss_death_b" },
		{ "sssa_dlc_halloween", "lurcher_sss_hangman_b" },
		{ "sssa_dlc_heist", "blade" },
		{ "sssa_dlc_heist", "enduro" },
		{ "sssa_dlc_heist", "gburrito2" },
		{ "sssa_dlc_heist", "gresley" },
		{ "sssa_dlc_heist", "guardian" },
		{ "sssa_dlc_heist", "innovation" },
		{ "sssa_dlc_heist", "jackal" },
		{ "sssa_dlc_heist", "kuruma" },
		{ "sssa_dlc_heist", "kuruma2" },
		{ "sssa_dlc_heist", "landstalker" },
		{ "sssa_dlc_heist", "nemesis" },
		{ "sssa_dlc_heist", "oracle1" },
		{ "sssa_dlc_heist", "rumpo" },
		{ "sssa_dlc_heist", "schafter2" },
		{ "sssa_dlc_heist", "seminole" },
		{ "sssa_dlc_heist", "surge" },
		{ "sssa_dlc_hipster", "blade" },
		{ "sssa_dlc_hipster", "blazer3" },
		{ "sssa_dlc_hipster", "buffalo" },
		{ "sssa_dlc_hipster", "buffalo2" },
		{ "sssa_dlc_hipster", "glendale" },
		{ "sssa_dlc_hipster", "panto" },
		{ "sssa_dlc_hipster", "picador" },
		{ "sssa_dlc_hipster", "pigalle" },
		{ "sssa_dlc_hipster", "primo" },
		{ "sssa_dlc_hipster", "rebel2" },
		{ "sssa_dlc_hipster", "regina" },
		{ "sssa_dlc_hipster", "rhapsody" },
		{ "sssa_dlc_hipster", "surfer" },
		{ "sssa_dlc_hipster", "tailgater" },
		{ "sssa_dlc_hipster", "warrener" },
		{ "sssa_dlc_hipster", "youga" },
		{ "sssa_dlc_independence", "sovereign" },
		{ "sssa_dlc_lts_creator", "hakuchou" },
		{ "sssa_dlc_lts_creator", "innovation" },
		{ "sssa_dlc_lts_creator", "kalahari_topless" },
		{ "sssa_dlc_mp_to_sp", "blista2" },
		{ "sssa_dlc_mp_to_sp", "buffalo3" },
		{ "sssa_dlc_mp_to_sp", "dominator2" },
		{ "sssa_dlc_mp_to_sp", "dukes" },
		{ "sssa_dlc_mp_to_sp", "gauntlet2" },
		{ "sssa_dlc_mp_to_sp", "stalion2" },
		{ "sssa_dlc_mp_to_sp", "stallion" },
		{ "sssa_dlc_stunt", "bf400_sss_bf400_livery_1_b" },
		{ "sssa_dlc_stunt", "brioso_sss_brioso_livery_1_b" },
		{ "sssa_dlc_stunt", "cliffhanger_sss_cliffhanger_livery_1_b" },
		{ "sssa_dlc_stunt", "contender" },
		{ "sssa_dlc_stunt", "gargoyle_sss_gargoyle_livery_1_b" },
		{ "sssa_dlc_stunt", "omnis_sss_omnis_livery_1_b" },
		{ "sssa_dlc_stunt", "rallytruck_sss_dune_livery_1_b" },
		{ "sssa_dlc_stunt", "tampa2_sss_tampa2_livery_1_b" },
		{ "sssa_dlc_stunt", "trophy_sss_trophy_livery_1_b" },
		{ "sssa_dlc_stunt", "trophy2_sss_trophy2_livery_1_b" },
		{ "sssa_dlc_stunt", "tropos_sss_tropos_livery_1_b" },
		{ "sssa_dlc_valentines", "rloader" },
		{ "candc_gunrunning", "apc" },
		{ "candc_gunrunning", "ardent" },
		{ "candc_gunrunning", "dune3" },
		{ "candc_gunrunning", "halftrack" },
		{ "candc_gunrunning", "nightshark" },
		{ "candc_gunrunning", "oppressor" },
		{ "candc_gunrunning", "tampa3" },
		{ "candc_gunrunning", "trsmall2" },
		{ "lgm_dlc_gunrunning", "cheetah2" },
		{ "lgm_dlc_gunrunning", "torero" },
		{ "lgm_dlc_gunrunning", "vagner" },
		{ "lgm_dlc_gunrunning", "xa21" },
	};

	

	// Disable Controls
	UI::HIDE_HELP_TEXT_THIS_FRAME();
	CAM::SET_CINEMATIC_BUTTON_ACTIVE(0);
	UI::HIDE_HUD_COMPONENT_THIS_FRAME(10);
	UI::HIDE_HUD_COMPONENT_THIS_FRAME(6);
	UI::HIDE_HUD_COMPONENT_THIS_FRAME(7);
	UI::HIDE_HUD_COMPONENT_THIS_FRAME(9);
	UI::HIDE_HUD_COMPONENT_THIS_FRAME(8);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_NEXT_CAMERA, true);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_CHARACTER_WHEEL, true);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_MELEE_ATTACK_LIGHT, true);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_MELEE_ATTACK_HEAVY, true);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_MULTIPLAYER_INFO, true);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_PHONE, true);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_MELEE_ATTACK_ALTERNATE, true);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_VEH_CIN_CAM, true);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_MAP_POI, true);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_PHONE, true);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_VEH_RADIO_WHEEL, true);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_VEH_HEADLIGHT, true);
}

bool Menu::Option(const char * option)//
{
	Settings::optionCount++;
	bool onThis = Settings::currentOption == Settings::optionCount ? true : false;
	if (Settings::currentOption <= 16 && Settings::optionCount <= 16)
	{
		Drawing::Text(option, Settings::optionText, { Settings::menuX - 0.100f, (Settings::optionCount) * 0.035f + 0.125f }, { 0.45f, 0.45f }, false);
		Drawing::Rect(Settings::optionRect, { Settings::menuX, (Settings::optionCount) * 0.035f + 0.1415f }, { 0.21f, 0.035f });
		onThis ? Drawing::Rect(Settings::scroller, { Settings::menuX, (Settings::optionCount) * 0.035f + 0.1415f }, { 0.21f, 0.035f }) : NULL;
	}
	else if (Settings::optionCount > (Settings::currentOption - 16) && Settings::optionCount <= Settings::currentOption)
	{
		Drawing::Text(option, Settings::optionText, { Settings::menuX - 0.100f, (Settings::optionCount - (Settings::currentOption - 16)) * 0.035f + 0.125f }, { 0.45f, 0.45f }, false);
		Drawing::Rect(Settings::optionRect, { Settings::menuX,  (Settings::optionCount - (Settings::currentOption - 16)) * 0.035f + 0.1415f }, { 0.21f, 0.035f });
		onThis ? Drawing::Rect(Settings::scroller, { Settings::menuX,  (Settings::optionCount - (Settings::currentOption - 16)) * 0.035f + 0.1415f }, { 0.21f, 0.035f }) : NULL;
	}
	if (Settings::currentOption == Settings::optionCount)
	{
		if (Settings::selectPressed)
		{
			return true;
		}
	}
	return false;
}
bool Menu::Break(const char * option)
{
	Settings::optionCount++;
	bool onThis = Settings::currentOption == Settings::optionCount ? true : false;
	if (Settings::currentOption <= 16 && Settings::optionCount <= 16)
	{
		Drawing::Text(option, Settings::breakText, { Settings::menuX, (Settings::optionCount)*0.035f + 0.125f }, { 0.45f, 0.45f }, true);
		Drawing::Rect(Settings::optionRect, { Settings::menuX, (Settings::optionCount)*0.035f + 0.1415f }, { 0.21f, 0.035f });
	}
	else if (Settings::optionCount > (Settings::currentOption - 16) && Settings::optionCount <= Settings::currentOption)
	{
		Drawing::Text(option, Settings::breakText, { Settings::menuX, (Settings::optionCount - (Settings::currentOption - 16))*0.035f + 0.125f }, { 0.45f, 0.45f }, true);
		Drawing::Rect(Settings::optionRect, { Settings::menuX,  (Settings::optionCount - (Settings::currentOption - 16))*0.035f + 0.1415f }, { 0.21f, 0.035f });
	}
	return false;
}
bool Menu::MenuOption(const char * option, SubMenus newSub)
{
	Option(option);

	if (Settings::currentOption <= Settings::maxVisOptions && Settings::optionCount <= Settings::maxVisOptions)
		Drawing::Text("", Settings::titleText, { Settings::menuX + 0.099f, Settings::optionCount * 0.035f + 0.125f }, { 0.35f, 0.35f }, true);
	else if (Settings::optionCount > Settings::currentOption - Settings::maxVisOptions && Settings::optionCount <= Settings::currentOption)
		Drawing::Text("", Settings::titleText, { Settings::menuX + 0.099f, (Settings::optionCount - (Settings::currentOption - 16))*0.035f + 0.12f }, { 0.35f, 0.35f }, true);

	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) {
		MenuLevelHandler::MoveMenu(newSub);
		return true;
	}
	return false;
}


bool Menu::Toggle(const char* option, bool& b00l)
{
	Option(option);
	if (b00l) 
	{
		/*if (Settings::currentOption <= Settings::maxVisOptions && Settings::optionCount <= Settings::maxVisOptions)
			Drawing::Text("ON", Settings::integre, { Settings::menuX + 0.068f, Settings::optionCount * 0.035f + 0.128f }, { 0.32f, 0.32f }, true);
		//else if (Settings::optionCount > Settings::currentOption - Settings::maxVisOptions && Settings::optionCount <= Settings::currentOption)
			Drawing::Text("ON", Settings::integre, { Settings::menuX + 0.068f, (Settings::optionCount - (Settings::currentOption - 16))*0.035f + 0.12f }, { 0.32f, 0.32f }, true);
		//Drawing::Spriter("Psycho", b00l ? "On" : "Off", Settings::menuX + 0.095f, (Settings::optionCount * 0.035f + 0.141f), 0.02f, 0.03f, 0, 255, 255, 255, 200);
			//		Drawing::Spriter("Psycho", b00l ? "On" : "Off", Settings::menuX + 0.095f, ((Settings::optionCount - (Settings::currentOption - 16)) * 0.035f + 0.141f), 0.02f, 0.03f, 0, 255, 255, 255, 200);
			 //Here is the old ON / OFF Can be put back if wanted */

			if (Settings::currentOption <= Settings::maxVisOptions && Settings::optionCount <= Settings::maxVisOptions)
				Drawing::Spriter("commonmenu", "MP_SpecItem_Weed", Menu::Settings::menuX + 0.084f, (Settings::optionCount * 0.035f + 0.141f), 0.015f, 0.025f, 0, 0, 255, 0, 255);// Toggle on
			else if (Settings::optionCount > Settings::currentOption - Settings::maxVisOptions && Settings::optionCount <= Settings::currentOption)
				Drawing::Spriter("commonmenu", "MP_SpecItem_Weed", Settings::menuX + 0.084f, ((Settings::optionCount - (Settings::currentOption - 16)) * 0.035f + 0.141f), 0.015f, 0.025f, 0, 0, 255, 0, 255); 
			
		/*	if (Settings::currentOption <= Settings::maxVisOptions && Settings::optionCount <= Settings::maxVisOptions)//  Adjust those tho once injected to see the right placement( 0.015f, 0.025f )
		Drawing::Spriter("commonmenu", "Shop_Box_Tick", Menu::Settings::menuX + 0.068f, Settings::optionCount * 0.035f + 0.1390f, 0.0250f, 0.040f, 0, 255, 255, 255, 200);// Toggle on 
	else if (Settings::optionCount > Settings::currentOption - Settings::maxVisOptions && Settings::optionCount <= Settings::currentOption)
		Drawing::Spriter("commonmenu", "Shop_Box_Tick", Settings::menuX + 0.068f, (Settings::optionCount - (Settings::currentOption - 16))*0.035f + 0.0690f, 0.0250f, Menu::Settings::TickBoxHeightLevel, 0, 255, 255, 255, 200);
		*/	//                                                                / Up and down  y position = Option count - (current option - 16) multiply by 0.035f + new pos,
	} 
	else 
	{ 

		if (Settings::currentOption <= Settings::maxVisOptions && Settings::optionCount <= Settings::maxVisOptions)
			Drawing::Spriter("commonmenu", "MP_SpecItem_Weed", Menu::Settings::menuX + 0.084f, (Settings::optionCount * 0.035f + 0.141f), 0.015f, 0.025f, 0, 255, 0, 0, 255);// Toggle off
		else if (Settings::optionCount > Settings::currentOption - Settings::maxVisOptions && Settings::optionCount <= Settings::currentOption)
			Drawing::Spriter("commonmenu", "MP_SpecItem_Weed", Settings::menuX + 0.084f, ((Settings::optionCount - (Settings::currentOption - 16)) * 0.035f + 0.141f), 0.015f, 0.025f, 0, 255, 0, 0, 255);

		/*if (Settings::currentOption <= Settings::maxVisOptions && Settings::optionCount <= Settings::maxVisOptions) 
			Drawing::Text("OFF", Settings::integre, { Settings::menuX + 0.068f, Settings::optionCount * 0.035f + 0.128f }, { 0.32f, 0.32f }, true);

			//Drawing::Spriter("Psycho", b00l ? "On" : "Off", Settings::menuX + 0.095f, (Settings::optionCount * 0.035f + 0.141f), 0.02f, 0.03f, 0, 255, 255, 255, 200);
		else if (Settings::optionCount > Settings::currentOption - Settings::maxVisOptions && Settings::optionCount <= Settings::currentOption)
			Drawing::Text("OFF", Settings::integre, { Settings::menuX + 0.068f, (Settings::optionCount - (Settings::currentOption - 16))*0.035f + 0.12f }, { 0.32f, 0.32f }, true);

			//Drawing::Spriter("Psycho", b00l ? "On" : "Off", Settings::menuX + 0.095f, ((Settings::optionCount - (Settings::currentOption - 16)) * 0.035f + 0.141f), 0.02f, 0.03f, 0, 255, 255, 255, 200);
		//Here is the old ON / OFF Can be put back if wanted 

		if (Settings::currentOption <= Settings::maxVisOptions && Settings::optionCount <= Settings::maxVisOptions)
			Drawing::Spriter("commonmenu", "Shop_Box_Blank", Menu::Settings::menuX + 0.068f, Settings::optionCount * 0.035f + 0.1390f, 0.0250f, 0.040f, 0, 255, 255, 255, 200);// Toggle off
		else if (Settings::optionCount > Settings::currentOption - Settings::maxVisOptions && Settings::optionCount <= Settings::currentOption)
			Drawing::Spriter("commonmenu", "Shop_Box_Blank", Settings::menuX + 0.068, (Settings::optionCount - (Settings::currentOption - 18))*0.035f + 0.0690f, 0.0250f, 0.040f, 0, 255, 255, 255, 200);
		//Most of them don't come back */ 

	}
	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) {
		b00l ^= 1;
		return true;
	}
	return false;
}


bool Menu::Int(const char * option, int & _int, int min, int max)
{
	Option(option);

	if (Settings::optionCount == Settings::currentOption) {
		if (Settings::leftPressed) {
			_int < max ? _int++ : _int = min;
		}
		if (Settings::rightPressed) {
			_int >= min ? _int-- : _int = max;
		}
	}

	if (Settings::currentOption <= Settings::maxVisOptions && Settings::optionCount <= Settings::maxVisOptions)
		Drawing::Text(Tools::StringToChar("< " + std::to_string(_int) + " >"), Settings::integre, { Settings::menuX + 0.068f, Settings::optionCount * 0.035f + 0.128f }, { 0.32f, 0.32f }, true);
	else if (Settings::optionCount > Settings::currentOption - Settings::maxVisOptions && Settings::optionCount <= Settings::currentOption)
		Drawing::Text(Tools::StringToChar("< " + std::to_string(_int) + " >"), Settings::integre, { Settings::menuX + 0.068f, (Settings::optionCount - (Settings::currentOption - 16))*0.035f + 0.12f }, { 0.32f, 0.32f }, true);

	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::leftPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::rightPressed) return true;
	return false;
}
bool Menu::Int(const char * option, int & _int, int min, int max, int step)
{
	Option(option);

	if (Settings::optionCount == Settings::currentOption) {
		if (Settings::leftPressed) {
			_int < max ? _int += step : _int = min;
		}
		if (Settings::rightPressed) {
			_int >= min ? _int -= step : _int = max;
		}
	}

	if (Settings::currentOption <= Settings::maxVisOptions && Settings::optionCount <= Settings::maxVisOptions)
		Drawing::Text(Tools::StringToChar("< " + std::to_string(_int) + " >"), Settings::integre, { Settings::menuX + 0.068f, Settings::optionCount * 0.035f + 0.125f }, { 0.32f, 0.32f }, true);
	else if (Settings::optionCount > Settings::currentOption - Settings::maxVisOptions && Settings::optionCount <= Settings::currentOption)
		Drawing::Text(Tools::StringToChar("< " + std::to_string(_int) + " >"), Settings::integre, { Settings::menuX + 0.068f, (Settings::optionCount - (Settings::currentOption - 16))*0.035f + 0.12f }, { 0.32f, 0.32f }, true);

	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::leftPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::rightPressed) return true;
	return false;
}
#pragma warning(disable: 4244)
bool Menu::Float(const char * option, float & _float, int min, int max)
{
	Option(option);

	if (Settings::optionCount == Settings::currentOption) {
		if (Settings::leftPressed) {
			_float <= min ? _float = max : _float -= 0.005f;
		}
		if (Settings::rightPressed) {
			_float >= max ? _float = min : _float += 0.005f;
		}
		_float < min ? _float = max : _float > max ? _float = min : NULL;
	}

	if (Settings::currentOption <= Settings::maxVisOptions && Settings::optionCount <= Settings::maxVisOptions)
		Drawing::Text(Tools::StringToChar(std::to_string(_float)), Settings::optionText, { Settings::menuX + 0.068f, Settings::optionCount * 0.035f + 0.128f }, { 0.32f, 0.32f }, true);
	else if (Settings::optionCount > Settings::currentOption - Settings::maxVisOptions && Settings::optionCount <= Settings::currentOption)
		Drawing::Text(Tools::StringToChar(std::to_string(_float)), Settings::optionText, { Settings::menuX + 0.068f, (Settings::optionCount - (Settings::currentOption - 16))*0.035f + 0.12f }, { 0.32f, 0.32f }, true);

	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::leftPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::rightPressed) return true;
	return false;
}
bool Menu::Float(const char * option, float & _float, int min, int max, int step)
{
	Option(option);

	if (Settings::optionCount == Settings::currentOption) {
		if (Settings::leftPressed) {
			_float <= min ? _float = max : _float -= 0.1f;
		}
		if (Settings::rightPressed) {//0.005f
			_float >= max ? _float = min : _float += 0.1f;
		}
		_float < min ? _float = max : _float > max ? _float = min : NULL;
	}

	if (Settings::currentOption <= Settings::maxVisOptions && Settings::optionCount <= Settings::maxVisOptions)
		Drawing::Text(Tools::StringToChar(std::to_string(_float)), Settings::optionText, { Settings::menuX + 0.068f, Settings::optionCount * 0.035f + 0.128f }, { 0.32f, 0.32f }, true);
	else if (Settings::optionCount > Settings::currentOption - Settings::maxVisOptions && Settings::optionCount <= Settings::currentOption)
		Drawing::Text(Tools::StringToChar(std::to_string(_float)), Settings::optionText, { Settings::menuX + 0.068f, (Settings::optionCount - (Settings::currentOption - 16))*0.035f + 0.12f }, { 0.32f, 0.32f }, true);

	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::leftPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::rightPressed) return true;
	return false;
}
#pragma warning(default: 4244)
#pragma warning(disable: 4267)
bool Menu::IntVector(const char * option, std::vector<int> Vector, int & position)
{
	Option(option);

	if (Settings::optionCount == Settings::currentOption) {
		int max = Vector.size() - 1;
		int min = 0;
		if (Settings::leftPressed) {
			position >= 1 ? position-- : position = max;
		}
		if (Settings::rightPressed) {
			position < max ? position++ : position = min;
		}
	}

	if (Settings::currentOption <= Settings::maxVisOptions && Settings::optionCount <= Settings::maxVisOptions)
		Drawing::Text(Tools::StringToChar(std::to_string(Vector[position])), Settings::optionText, { Settings::menuX + 0.068f, Settings::optionCount * 0.035f + 0.125f }, { 0.5f, 0.5f }, true);
	else if (Settings::optionCount > Settings::currentOption - Settings::maxVisOptions && Settings::optionCount <= Settings::currentOption)
		Drawing::Text(Tools::StringToChar(std::to_string(Vector[position])), Settings::optionText, { Settings::menuX + 0.068f, (Settings::optionCount - (Settings::currentOption - 16))*0.035f + 0.12f }, { 0.5f, 0.5f }, true);

	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::leftPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::rightPressed) return true;
	return false;
}
bool Menu::FloatVector(const char * option, std::vector<float> Vector, int & position)
{
	Option(option);

	if (Settings::optionCount == Settings::currentOption) {
		size_t max = Vector.size() - 1;
		int min = 0;
		if (Settings::leftPressed) {
			position >= 1 ? position-- : position = max;
		}
		if (Settings::rightPressed) {
			position < max ? position++ : position = min;
		}
	}

	if (Settings::currentOption <= Settings::maxVisOptions && Settings::optionCount <= Settings::maxVisOptions)
		Drawing::Text(Tools::StringToChar(std::to_string(Vector[position])), Settings::optionText, { Settings::menuX + 0.068f, Settings::optionCount * 0.035f + 0.125f }, { 0.5f, 0.5f }, true);
	else if (Settings::optionCount > Settings::currentOption - Settings::maxVisOptions && Settings::optionCount <= Settings::currentOption)
		Drawing::Text(Tools::StringToChar(std::to_string(Vector[position])), Settings::optionText, { Settings::menuX + 0.068f, (Settings::optionCount - (Settings::currentOption - 16))*0.035f + 0.12f }, { 0.5f, 0.5f }, true);

	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::leftPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::rightPressed) return true;
	return false;
}
bool Menu::StringVector(const char * option, std::vector<std::string> Vector, int & position)
{
	Option(option);

	if (Settings::optionCount == Settings::currentOption) {
		size_t max = Vector.size() - 1;
		int min = 0;
		if (Settings::leftPressed) {
			position >= 1 ? position-- : position = max;
		}
		if (Settings::rightPressed) {
			position < max ? position++ : position = min;
		}
	}

	if (Settings::currentOption <= Settings::maxVisOptions && Settings::optionCount <= Settings::maxVisOptions)
		Drawing::Text(Tools::StringToChar((Vector[position])), Settings::optionText, { Settings::menuX + 0.068f, Settings::optionCount * 0.035f + 0.125f }, { 0.5f, 0.5f }, true);
	else if (Settings::optionCount > Settings::currentOption - Settings::maxVisOptions && Settings::optionCount <= Settings::currentOption)
		Drawing::Text(Tools::StringToChar((Vector[position])), Settings::optionText, { Settings::menuX + 0.068f, (Settings::optionCount - (Settings::currentOption - 16))*0.035f + 0.12f }, { 0.5f, 0.5f }, true);

	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::leftPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::rightPressed) return true;
	return false;
}


bool Menu::ListVector(const char * option, std::vector<char*> Vector, int & position)
{

	constexpr static const VECTOR2_2 textSize = { 0.32f, 0.32f };

	Option(option);
	if (Menu::Settings::optionCount == Menu::Settings::currentOption) {

		size_t max = Vector.size() - 1;
		int min = 0;
		if (Menu::Settings::leftPressed) {
			position >= 1 ? position-- : position = max;
		}
		if (Menu::Settings::rightPressed) {
			position < max ? position++ : position = min;
		}
	}
	if (Menu::Settings::currentOption <= Menu::Settings::maxVisOptions && Menu::Settings::optionCount <= Menu::Settings::maxVisOptions)
		Menu::Drawing::Text(Menu::Tools::StringToChar(" : " + (std::string)Vector[position] + " : "), Menu::Settings::optionText, { Menu::Settings::menuX + 0.068f, Menu::Settings::optionCount * 0.035f + 0.128f }, { 0.45f, 0.45f }, true);
	else if (Menu::Settings::optionCount > Menu::Settings::currentOption - Menu::Settings::maxVisOptions && Menu::Settings::optionCount <= Menu::Settings::currentOption)
		Menu::Drawing::Text(Menu::Tools::StringToChar(" : " + (std::string)Vector[position] + " : "), Menu::Settings::optionText, { Menu::Settings::menuX + 0.068f, Menu::Settings::optionCount * 0.035f + 0.128f }, { 0.45f, 0.45f }, true);

	if (Menu::Settings::optionCount == Menu::Settings::currentOption && Menu::Settings::selectPressed) return true;

	else
		return false;

}


bool Menu::StringVector(const char * option, std::vector<char*> Vector, int & position)
{
	Option(option);

	if (Settings::optionCount == Settings::currentOption) {
		size_t max = Vector.size() - 1;
		int min = 0;
		if (Settings::leftPressed) {
			position >= 1 ? position-- : position = max;
		}
		if (Settings::rightPressed) {
			position < max ? position++ : position = min;
		}
	}

	if (Settings::currentOption <= Settings::maxVisOptions && Settings::optionCount <= Settings::maxVisOptions)
		Drawing::Text(((Vector[position])), Settings::optionText, { Settings::menuX + 0.068f, Settings::optionCount * 0.035f + 0.125f }, { 0.5f, 0.5f }, true);
	else if (Settings::optionCount > Settings::currentOption - Settings::maxVisOptions && Settings::optionCount <= Settings::currentOption)
		Drawing::Text(((Vector[position])), Settings::optionText, { Settings::menuX + 0.068f, (Settings::optionCount - (Settings::currentOption - 16))*0.035f + 0.12f }, { 0.5f, 0.5f }, true);

	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::leftPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::rightPressed) return true;
	return false;
}
#pragma warning(default: 4267)
bool Menu::Teleport(const char * option, Vector3 coords)
{
	Option(option);

	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) {
		Entity handle;
		PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false) ? handle = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()) : PLAYER::PLAYER_PED_ID();
		ENTITY::SET_ENTITY_COORDS(handle, coords.x, coords.y, coords.z, false, false, false, false);
		return true;
	}
	return false;
}
bool Menu::Teleport(const char * option, Vector3 coords, std::function<void()> function)
{
	Teleport(option, coords);

	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) {
		function();
		return true;
	}
	return false;
}
void Menu::info(const char * info)
{
	if (Settings::currentOption <= 16 && Settings::optionCount <= 16)
	{
		if (bool onThis = true) { Drawing::Text(info, Settings::optionText, { Settings::menuX - 0.100f, 17 * 0.035f + 0.1600f }, { 0.25f, 0.25f }, false); }
	}
	else if (Settings::optionCount > (Settings::currentOption - 16) && Settings::optionCount <= Settings::currentOption)
	{
		if (bool onThis = true) { Drawing::Text(info, Settings::optionText, { Settings::menuX - 0.100f, (Settings::optionCount - (Settings::currentOption - 16))*0.035f + 0.1300f }, { 0.25f, 0.25f }, false); }
	}
}



void Menu::End()
{
	int opcount = Settings::optionCount;
	int currop = Settings::currentOption;
	if (opcount >= 16) { 
		Drawing::Text(" Nano", Settings::count, { Settings::menuX - 0.105f, 17 * 0.035f + 0.130f }, { 0.35f, 0.35f }, false); // Add a text message at the end of the break 
		Drawing::Text(Tools::StringToChar(std::to_string(currop) + " / " + std::to_string(opcount)), Settings::count, { Settings::menuX + 0.078f, 17 * 0.035f + 0.125f }, { 0.35f, 0.35f }, true);
		Drawing::Rect(Settings::optionRect, { Settings::menuX, 17 * 0.035f + 0.1415f }, { 0.21f, 0.035f });
		Drawing::Rect(Settings::line, { Settings::menuX, 17 * 0.035f + 0.1235f }, { 0.21f, 0.002f });
		Drawing::Spriter("commonmenu", "shop_arrows_upanddown", Settings::menuX, 17 * 0.035f + Settings::ArrowPosX, Settings::ArrowPosY, Settings::ArrowWidness, Settings::ArrowThicness, 255, 255, 255, 255);
		//Drawing::Spriter("commonmenu", "shop_arrows_upanddown", Settings::menuX, ((16 + 1) * 0.035f + 0.140f), 0.020f, 0.035f, 180, Settings::line.r, Settings::line.g, Settings::line.b, Settings::line.a);// Add arrows at then end 
	}
	else if (opcount > 0) { 
		Drawing::Text(" Nano", Settings::count, { Settings::menuX - 0.105f,(Settings::optionCount+ 1) * 0.035f + 0.130f }, { 0.35f, 0.35f }, false);// Add a text message at the end of the break 
		Drawing::Text(Tools::StringToChar(std::to_string(currop) + " / " + std::to_string(opcount)), Settings::count, { Settings::menuX + 0.078f, (Settings::optionCount + 1) * 0.035f + 0.125f }, { 0.35f, 0.35f }, true);
		Drawing::Rect(Settings::optionRect, { Settings::menuX, (Settings::optionCount + 1) * 0.035f + 0.1415f }, { 0.21f, 0.035f });
		Drawing::Rect(Settings::line, { Settings::menuX, (Settings::optionCount + 1) * 0.035f + 0.1235f }, { 0.21f, 0.002f });
		Drawing::Spriter("commonmenu", "shop_arrows_upanddown", Settings::menuX, ((Settings::optionCount + 1) * 0.035f + 0.1415f), 0.020f, 0.035f, 180, 255, 255, 255, 255);// Add arrows at then end 
		//Drawing::Spriter("commonmenu", "shop_arrows_upanddown", Settings::menuX, ((Settings::optionCount + 1) * 0.035f + 0.140f), 0.020f, 0.035f, 180, Settings::line.r, Settings::line.g, Settings::line.b, Settings::line.a);
	}
}


int IconNotification(char *text, char *text2, char *Subject)
{
	UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
	UI::_SET_NOTIFICATION_MESSAGE_CLAN_TAG("CHAR_GANGAPP", "CHAR_GANGAPP", false, 7, text2, Subject, 1.0, "___Menu");
	return UI::_DRAW_NOTIFICATION(1, 1);
}
void PlaySoundFrontend_default(char* sound_name)
{
	AUDIO::PLAY_SOUND_FRONTEND(-1, sound_name, "HUD_FRONTEND_DEFAULT_SOUNDSET", 0);
}
void PlaySoundFrontend_default2(char* sound_name)
{
	AUDIO::PLAY_SOUND_FRONTEND(-1, sound_name, "DLC_HEIST_BIOLAB_PREP_HACKING_SOUNDS", 0);
}

int Menu::Settings::keyPressDelay = 200;
int Menu::Settings::keyPressPreviousTick = GetTickCount();
int Menu::Settings::keyPressDelay2 = 100;
int Menu::Settings::keyPressPreviousTick2 = GetTickCount();
int Menu::Settings::keyPressDelay3 = 140;
int Menu::Settings::keyPressPreviousTick3 = GetTickCount();
int Menu::Settings::openKey = VK_F4;
int Menu::Settings::backKey = VK_NUMPAD0;
int Menu::Settings::upKey = VK_NUMPAD8;
int Menu::Settings::downKey = VK_NUMPAD2;
int Menu::Settings::leftKey = VK_NUMPAD4;
int Menu::Settings::rightKey = VK_NUMPAD6;
int Menu::Settings::selectKey = VK_NUMPAD5;
int Menu::Settings::arrowupKey = VK_UP;
int Menu::Settings::arrowdownKey = VK_DOWN;
int Menu::Settings::arrowleftKey = VK_LEFT;
int Menu::Settings::arrowrightKey = VK_RIGHT;
int Menu::Settings::enterKey = VK_RETURN;
int Menu::Settings::deleteKey = VK_BACK;

#pragma endregion

bool Menu::Settings::controllerinput = true;
void Menu::Checks::Controlls()
{
	Settings::selectPressed = false;
	Settings::leftPressed = false;
	Settings::rightPressed = false;
	if (GetTickCount() - Settings::keyPressPreviousTick > Settings::keyPressDelay) {
	if (GetTickCount() - Settings::keyPressPreviousTick2 > Settings::keyPressDelay2) {
		if (GetTickCount() - Settings::keyPressPreviousTick3 > Settings::keyPressDelay3) {
			if (IsKeyPressed(VK_F4) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlScriptRB) && CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlPhoneRight) && Settings::controllerinput) {
				Settings::menuLevel == 0 ? MenuLevelHandler::MoveMenu(SubMenus::mainmenu) : Settings::menuLevel == 1 ? MenuLevelHandler::BackMenu() : NULL;
				Settings::keyPressPreviousTick = GetTickCount();
			}
			else if (IsKeyPressed(VK_NUMPAD0) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlFrontendCancel) && Settings::controllerinput) {
				Settings::menuLevel > 0 ? MenuLevelHandler::BackMenu() : NULL;
				if (Settings::menuLevel > 0)
					PlaySoundFrontend_default("BACK");

				Settings::keyPressPreviousTick = GetTickCount();
			}
			else if (IsKeyPressed(VK_NUMPAD8) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlFrontendUp) && Settings::controllerinput) {
				Settings::currentOption > 1 ? Settings::currentOption-- : Settings::currentOption = Settings::optionCount;
				if (Settings::menuLevel > 0)
					PlaySoundFrontend_default("NAV_UP_DOWN");

				Settings::keyPressPreviousTick2 = GetTickCount();
			}
			else if (IsKeyPressed(VK_NUMPAD2) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlFrontendDown) && Settings::controllerinput) {
				Settings::currentOption < Settings::optionCount ? Settings::currentOption++ : Settings::currentOption = 1;
				if (Settings::menuLevel > 0)
					PlaySoundFrontend_default("NAV_UP_DOWN");

				Settings::keyPressPreviousTick2 = GetTickCount();
			}
			else if (IsKeyPressed(VK_NUMPAD6) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlPhoneRight) && Settings::controllerinput) {
				Settings::leftPressed = true;
				if (Settings::menuLevel > 0)
					PlaySoundFrontend_default("NAV_UP_DOWN");

				Settings::keyPressPreviousTick3 = GetTickCount();
			}
			else if (IsKeyPressed(VK_NUMPAD4) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlPhoneLeft) && Settings::controllerinput) {
				Settings::rightPressed = true;
				if (Settings::menuLevel > 0)
					PlaySoundFrontend_default("NAV_UP_DOWN");

				Settings::keyPressPreviousTick3 = GetTickCount();
			}
			else if (IsKeyPressed(VK_NUMPAD5) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlFrontendAccept) && Settings::controllerinput) {
				Settings::selectPressed = true;
				if (Settings::menuLevel > 0)
					PlaySoundFrontend_default("SELECT");

				Settings::keyPressPreviousTick = GetTickCount();
			}
		}
	}
  }
	Settings::optionCount = 0;
}
#pragma warning(default : 4018)
void Menu::MenuLevelHandler::MoveMenu(SubMenus menu)
{
	Settings::menusArray[Settings::menuLevel] = Settings::currentMenu;
	Settings::optionsArray[Settings::menuLevel] = Settings::currentOption;
	Settings::menuLevel++;
	Settings::currentMenu = menu;
	Settings::currentOption = 1;
}

void Menu::MenuLevelHandler::BackMenu()
{
	Settings::menuLevel--;
	Settings::currentMenu = Settings::menusArray[Settings::menuLevel];
	Settings::currentOption = Settings::optionsArray[Settings::menuLevel];
}

char * Menu::Tools::StringToChar(std::string string)
{
	return _strdup(string.c_str());
}

void Menu::Files::WriteStringToIni(std::string string, std::string file, std::string app, std::string key)
{
	WritePrivateProfileStringA(app.c_str(), key.c_str(), string.c_str(), file.c_str());
}

std::string Menu::Files::ReadStringFromIni(std::string file, std::string app, std::string key)
{
	char buf[100];
	GetPrivateProfileStringA(app.c_str(), key.c_str(), "NULL", buf, 100, file.c_str());
	return (std::string)buf;
}

void Menu::Files::WriteIntToIni(int intValue, std::string file, std::string app, std::string key)
{
	WriteStringToIni(std::to_string(intValue), file, app, key);
}

int Menu::Files::ReadIntFromIni(std::string file, std::string app, std::string key)
{
	return std::stoi(ReadStringFromIni(file, app, key));
}

void Menu::Files::WriteFloatToIni(float floatValue, std::string file, std::string app, std::string key)
{
	WriteStringToIni((std::to_string(floatValue)), file, app, key);
}

float Menu::Files::ReadFloatFromIni(std::string file, std::string app, std::string key)
{
	return std::stof(ReadStringFromIni(file, app, key));
}

void Menu::Files::WriteBoolToIni(bool b00l, std::string file, std::string app, std::string key)
{
	WriteStringToIni(b00l ? "true" : "false", file, app, key);
}

bool Menu::Files::ReadBoolFromIni(std::string file, std::string app, std::string key)
{
	return ReadStringFromIni(file, app, key) == "true" ? true : false;
}

void Menu::Vehicle(std::string texture1, std::string texture2)
{
	if (Menu::Settings::menuX < 0.78f)
	{
		if (Menu::Settings::optionCount == Menu::Settings::currentOption) { Menu::Drawing::Spriter(texture1, texture2, Menu::Settings::menuX + 0.24f, 0.2f, 0.11f, 0.11f, 0, 255, 255, 255, 255); }
	}
	else { if (Menu::Settings::optionCount == Menu::Settings::currentOption) { Menu::Drawing::Spriter(texture1, texture2, Menu::Settings::menuX - 0.24f, 0.2f, 0.11f, 0.11f, 0, 255, 255, 255, 255); } }
}
void Menu::Speedometer(char* text)
{
	Drawing::Text(text, Settings::titleText, { 0.84f, 0.8800f }, { 0.70f, 0.70f }, false);
}
void Menu::fps(char* text)
{
	Drawing::Text(text, Settings::optionText, { 0.84f, 0.050f }, { 0.70f, 0.70f }, false);
}
void Menu::AddSmallTitle(char* text)
{
	if (Menu::Settings::menuX < 0.78f)
	{
		Drawing::Text(text, Settings::titleText, { Settings::menuX + 0.175f, 0.090f }, { 0.425f, 0.425f }, true);
		Drawing::Spriter("CommonMenu", "", Settings::menuX + 0.175f, 0.1175f - 0.019f, 0.115f, 0.045f, 180, Settings::titleRect.r, Settings::titleRect.g, Settings::titleRect.b, Settings::titleRect.a);
	}
	else
	{
		Drawing::Text(text, Settings::titleText, { Settings::menuX - 0.175f, 0.090f }, { 0.425f, 0.425f }, true);
		Drawing::Spriter("CommonMenu", "", Settings::menuX - 0.175f, 0.1175f - 0.019f, 0.115f, 0.045f, 180, Settings::titleRect.r, Settings::titleRect.g, Settings::titleRect.b, Settings::titleRect.a);
	}
}
void Menu::AddSmallInfo(char* text, short line)
{
	if (Menu::Settings::menuX < 0.78f)
	{
		if (line == 1) {
			Drawing::Rect(Settings::optionRect, { Settings::menuX + 0.175f, ((16 * 0.035f) / 2.0f) + 0.159f - 0.135f }, { 0.115f, 16 * 0.035f + -0.193f });
		}
		Drawing::Text(text, Settings::count, { Settings::menuX + 0.120f, (line * 0.020f) + 0.123f }, { 0.375f, 0.375f }, false);
	}
	else
	{
		if (line == 1) {
			Drawing::Rect(Settings::optionRect, { Settings::menuX - 0.175f, ((16 * 0.035f) / 2.0f) + 0.159f - 0.135f }, { 0.115f, 16 * 0.035f + -0.193f });
		}
		Drawing::Text(text, Settings::count, { Settings::menuX - 0.228f, (line * 0.020f) + 0.123f }, { 0.375f, 0.375f }, false);
	}
}
void Menu::AddSmallTitle2(char* text)
{
	if (Menu::Settings::menuX < 0.78f)
	{
		Drawing::Text(text, Settings::titleText, { Settings::menuX + 0.175f, 0.090f }, { 0.425f, 0.425f }, true);
		Drawing::Spriter("CommonMenu", "", Settings::menuX + 0.175f, 0.1175f - 0.019f, 0.115f, 0.045f, 180, Settings::titleRect.r, Settings::titleRect.g, Settings::titleRect.b, Settings::titleRect.a);
	}
	else
	{
		Drawing::Text(text, Settings::titleText, { Settings::menuX - 0.175f, 0.090f }, { 0.425f, 0.425f }, true);
		Drawing::Spriter("CommonMenu", "", Settings::menuX - 0.175f, 0.1175f - 0.019f, 0.115f, 0.045f, 180, Settings::titleRect.r, Settings::titleRect.g, Settings::titleRect.b, Settings::titleRect.a);
	}
}

void Menu::AddSmallInfo2(char* text, short line)
{
	if (Menu::Settings::menuX < 0.78f)
	{
		if (line == 1) {
			Drawing::Rect(Settings::optionRect, { Settings::menuX + 0.175f, ((13 * 0.035f) / 2.0f) + 0.159f - 0.135f }, { 0.115f, 13 * 0.035f + -0.193f });
		}
		Drawing::Text(text, Settings::count, { Settings::menuX + 0.120f, (line * 0.020f) + 0.123f }, { 0.375f, 0.375f }, false);
	}
	else
	{
		if (line == 1) {
			Drawing::Rect(Settings::optionRect, { Settings::menuX - 0.175f, ((13 * 0.035f) / 2.0f) + 0.159f - 0.135f }, { 0.115f, 13 * 0.035f + -0.193f });
		}
		Drawing::Text(text, Settings::count, { Settings::menuX - 0.228f, (line * 0.020f) + 0.123f }, { 0.375f, 0.375f }, false);
	}
}

void Menu::AddSmallTitle3(char* text)
{
	if (Menu::Settings::menuX < 0.78f)
	{
		Drawing::Text(text, Settings::titleText, { Settings::menuX + 0.175f, 0.090f }, { 0.425f, 0.425f }, true);
		Drawing::Spriter("CommonMenu", "", Settings::menuX + 0.175f, 0.1175f - 0.019f, 0.115f, 0.045f, 180, Settings::titleRect.r, Settings::titleRect.g, Settings::titleRect.b, Settings::titleRect.a);
	}
	else
	{
		Drawing::Text(text, Settings::titleText, { Settings::menuX - 0.175f, 0.090f }, { 0.425f, 0.425f }, true);
		Drawing::Spriter("CommonMenu", "", Settings::menuX - 0.175f, 0.1175f - 0.019f, 0.115f, 0.045f, 180, Settings::titleRect.r, Settings::titleRect.g, Settings::titleRect.b, Settings::titleRect.a);
	}
}

void Menu::AddSmallInfo3(char* text, short line)
{
	if (Menu::Settings::menuX < 0.78f)
	{
		if (line == 1) {
			Drawing::Rect(Settings::optionRect, { Settings::menuX + 0.175f, ((11 * 0.035f) / 2.0f) + 0.159f - 0.135f }, { 0.115f, 11 * 0.035f + -0.193f });
		}
		Drawing::Text(text, Settings::count, { Settings::menuX + 0.120f, (line * 0.020f) + 0.123f }, { 0.375f, 0.375f }, false);
	}
	else
	{
		if (line == 1) {
			Drawing::Rect(Settings::optionRect, { Settings::menuX - 0.175f, ((11 * 0.035f) / 2.0f) + 0.159f - 0.135f }, { 0.115f, 11 * 0.035f + -0.193f });
		}
		Drawing::Text(text, Settings::count, { Settings::menuX - 0.228f, (line * 0.020f) + 0.123f }, { 0.375f, 0.375f }, false);
	}
}
