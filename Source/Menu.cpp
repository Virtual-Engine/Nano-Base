
#pragma onceMenu::Title("Player List");
#include "stdafx.h"
#include "Functions.h"
#include "Features.h"

bool g_running = true;

char* CharKeyboard(char* windowName = "", int maxInput = 21, char* defaultText = "") {
	GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(0, "", "", defaultText, "", "", "", maxInput);
	while (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 0) WAIT(0);
	if (!GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT()) return "";
	return GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT();
}
int NumberKeyboard() {
	GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(1, "", "", "", "", "", "", 10);
	while (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 0) WAIT(0);
	if (!GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT()) return 0;
	return atof(GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT());
}

int MenuNotification(char* text, char* text2, char* Subject)
{
	YTDNotification();
	UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
	UI::_SET_NOTIFICATION_MESSAGE_CLAN_TAG("CHAR_PEGASUS_DELIVERY", "CHAR_PEGASUS_DELIVERY", true, 7, text2, Subject, 1.0f, "Naruto");
	return UI::_DRAW_NOTIFICATION(1, 1);

}

bool casinostealth = false;
bool godmode = false;
bool blackout = false;
bool invisible = false;
void Funktion()
{
	if (godmode)
	{
		ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), true);
	}
	else
	{
		ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), false);
	}

	if (invisible)
	{
		ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), false, 0);
	}
	else
	{
		ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), true, 0);
	}

	if (blackout) {
		GRAPHICS::SET_ARTIFICIAL_LIGHTS_STATE(true);
	}
	else
	{
		GRAPHICS::SET_ARTIFICIAL_LIGHTS_STATE(false);
	}
}

void main() {
	YTDNotification();
	MenuNotification("Open F5 Menu", "Naruto", "Welcome");

	Funktion();
	//Superjump()
	Menu::Drawing::YTD();

	while (true) {
		Menu::Checks::Controlls();

		switch (Menu::Settings::currentMenu) {

		case mainmenu:
		{
			Menu::Title("Nano");
			Menu::MenuOption("Self", self);
			Menu::MenuOption("Network", self);
			Menu::MenuOption("Teleport", self);
			Menu::MenuOption("Weapon", self);
			Menu::MenuOption("Vehicle", self);
			Menu::MenuOption("Recovery", self);
			Menu::MenuOption("Protections", self);
			Menu::MenuOption("Misc", self);
			Menu::MenuOption("Settings", settings);
		}
		break;

		case self:
		{
			Menu::Title("Self");
			Menu::Toggle("God Mode", godmode);
			Menu::Toggle("Invisible", invisible);
		}
		break;

		case settings:
		{
			Menu::Title("Settings");
			if (Menu::Int("Scroll Delay", Menu::Settings::keyPressDelay2, 1, 200))
			{
				if (IsKeyPressed(VK_NUMPAD5) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlFrontendAccept)) {
					Menu::Settings::keyPressDelay2 = NumberKeyboard();
				}
			}
			if (Menu::Int("Int Delay", Menu::Settings::keyPressDelay3, 1, 200))
			{
				if (IsKeyPressed(VK_NUMPAD5) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlFrontendAccept)) {
					Menu::Settings::keyPressDelay3 = NumberKeyboard();
				}
			}
			if (Menu::Option("MenuX plus")) {
				if (Menu::Settings::menuX < 0.81f) Menu::Settings::menuX += 0.01f;
			}
			if (Menu::Option("MenuX minus")) {
				if (Menu::Settings::menuX > 0.17f) Menu::Settings::menuX -= 0.01f;
			}
			Menu::MenuOption("~r~KILL GAME", exitgta);
		}
		break;
		case exitgta:
		{
			Menu::Title("Unload?");
			if (Menu::Option("Yes"))
			{
				Hooking::Cleanup();
				g_running = false;
			}
		    
		}
		break;

#pragma endregion
		}
		Menu::End();
		WAIT(0);
		YTDNotification();
	}
}

void ScriptMain() {
	srand(GetTickCount());

	main();
}