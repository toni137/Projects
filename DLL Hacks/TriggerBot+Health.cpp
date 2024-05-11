#include <iostream>
#include <Windows.h>

#define m_iTeamNum 0xF4
#define dwLocalPlayer 0xDE6964
#define dwEntityList 0x4DFBE44
#define m_iCrosshairId 0x11838
#define dwGlowObjectManager 0x53568F8
#define m_iGlowIndex 0x10488
#define m_iHealth 0x100

uintptr_t moduleBase;

uintptr_t getLocalPlayer() { //This will get the address to localplayer. 
    return *(uintptr_t*)(moduleBase + dwLocalPlayer);
}

uintptr_t getPlayer(int index) {  //Each player in the game has an index.
    return *(uintptr_t*)(moduleBase + dwEntityList + index * 0x10); //We use index times 0x10 because the distance between each player 0x10.
}

uintptr_t GlowManager()
{
    return *(uintptr_t*)(moduleBase + dwGlowObjectManager);
}

int getTeam(uintptr_t player) { //get team number of player
    return *(int*)(player + m_iTeamNum);
}

int getCrosshairID(uintptr_t player) { // get crosshair number of player
    return *(int*)(player + m_iCrosshairId);
}



DWORD WINAPI MainThread(HMODULE hModule)
{
    //Create Console
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    std::cout << "Code injected successfully\n";

    moduleBase = (DWORD)GetModuleHandle("client.dll");

    while (true)
    {

        if (GetAsyncKeyState(VK_END) & 1)
        {
            break;
        }

        uintptr_t LocalPlayer = getLocalPlayer();
        uintptr_t GlowObjectManager = GlowManager();
        int CrosshairID = getCrosshairID(LocalPlayer); //crosshairid of the local player
        int OppositeTeam = getTeam(getPlayer(CrosshairID - 1)); //number of the opposite team of local player
        int LocalTeam = getTeam(LocalPlayer); // number of the team of local player

        int* health = (int*)(LocalPlayer + m_iHealth);
        int health_index = *health;


        if (GetAsyncKeyState(VK_MENU /*alt_key*/)) //press alt key to restore health to 100
        {

            if (*health > 0 && *health < 100)
                *health = 100;


        }


        if (CrosshairID > 0 && CrosshairID < 32)
        {
            if (LocalTeam != OppositeTeam) //if the player is in the opposite team, turn red glow on and automatically shoot
            {
                uintptr_t player = *(uintptr_t*)((moduleBase + dwEntityList) + (CrosshairID - 1) * 0x10);
                if (player == NULL) continue;
                int glowIndex = *(int*)(player + m_iGlowIndex);
                int entityTeam = *(int*)(player + m_iTeamNum);
                *(float*)((GlowObjectManager + glowIndex * 0x38 + 0x8)) = 1.7f; //R
                *(float*)((GlowObjectManager + glowIndex * 0x38 + 0xC)) = 0.f; //G
                *(float*)((GlowObjectManager + glowIndex * 0x38 + 0x10)) = 0.f; //B
                *(float*)((GlowObjectManager + glowIndex * 0x38 + 0x14)) = 1.f; //A
                *(bool*)((GlowObjectManager + glowIndex * 0x38 + 0x28)) = true;
                *(bool*)((GlowObjectManager + glowIndex * 0x38 + 0x29)) = false;
                mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, 0, 0); //automatically shoot the enemy
                mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, 0, 0);
                Sleep(100); //Optional


            }
            else if (LocalTeam == OppositeTeam) // if player is part of localPlayer's team, turn green glow on
            {
                uintptr_t player = *(uintptr_t*)((moduleBase + dwEntityList) + (CrosshairID - 1) * 0x10);
                if (player == NULL) continue;
                int glowIndex = *(int*)(player + m_iGlowIndex);
                int entityTeam = *(int*)(player + m_iTeamNum);
                *(float*)((GlowObjectManager + glowIndex * 0x38 + 0x8)) = 0.f; //R
                *(float*)((GlowObjectManager + glowIndex * 0x38 + 0xC)) = 1.7f; //G
                *(float*)((GlowObjectManager + glowIndex * 0x38 + 0x10)) = 0.f; //B
                *(float*)((GlowObjectManager + glowIndex * 0x38 + 0x14)) = 1.f; //A
                *(bool*)((GlowObjectManager + glowIndex * 0x38 + 0x28)) = true;
                *(bool*)((GlowObjectManager + glowIndex * 0x38 + 0x29)) = false;
            }

        }

    }

    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
