#include <iostream>
#include <thread>
#include <Windows.h>
#include <string>

#include "lua.hpp"

void DumpError(lua_State* L)
{
	//Hämta toppen av lua stacken och kolla om det är en sträng
	if (lua_gettop(L) > 0 && lua_isstring(L, -1))
	{
		std::cout << "Lua error: " << lua_tostring(L, -1) << std::endl;

		//Ta bort meddelandet från stacken
		lua_pop(L, 1);
	}
}

//Function som körs parallelt av tråd
void ConsoleThreadFunction(lua_State* L)
{

	//Läs console input
	std::string input;
	while (GetConsoleWindow())
	{
		std::cout << "> ";
		std::getline(std::cin, input);

		//Försök köra strängen som lua kod
		if (luaL_dostring(L, input.c_str()) != LUA_OK)
		{
			DumpError(L);
		}
	}
}

int main()
{
	//Rekommenderat att ha ett men går att ha flera om det behövs
	lua_State* L = luaL_newstate();

	//Öppnar standardbibliotek för lua, gör så att kodsträngen går att köra
	luaL_openlibs(L);

	//Skapa tråd
	std::thread consoleThread(ConsoleThreadFunction, L);

	bool running = true;
	while (running)
	{
		//Game & render loop
	}

	std::cout << "Hello world!" << std::endl;
	return 0;
}