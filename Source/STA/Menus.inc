#if defined MENUS_INCLUDED
#endinput
#endif
#define MENUS_INCLUDED

/*
	Main plugin menu entries
*/
enum
{	
	/*
		Menu commands before or while a replay is being created
	*/
	SEG_Start,
	SEG_Resume,
	SEG_Pause,
	SEG_GoBack,
	SEG_Play,
	SEG_Stop,
	SEG_LoadFromFile,
	
	/*
		Menu commands after a replay is created
	*/	
	MOV_Resume,
	MOV_NewFrom,
	MOV_Stop,
	MOV_Pause,
	MOV_ContinueFrom,
	MOV_SaveToFile,
	
	ALL_RewindSpeed,
	ALL_JumpToStart,
	ALL_JumpToEnd,
};

/*
	Converts the enum value "data" into a string and sets it as the info value to a menu item.
*/
public void Menu_AddEnumEntry(Handle menu, any data, char[] displaystr)
{
	char buf[64];
	IntToString(data, buf, sizeof(buf));
	AddMenuItem(menu, buf, displaystr);
}
