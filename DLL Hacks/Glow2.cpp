#include <iostream>
#include <Windows.h>

#define m_iTeamNum 0xF4
#define dwLocalPlayer 0xDE6964
#define dwEntityList 0x4DFBE44
#define m_iCrosshairId 0x11838
#define dwGlowObjectManager  0x53568F8
#define m_iGlowIndex 0x10488

uintptr_t moduleBase;

DWORD WINAPI MainThread(HMODULE hModule)
{
    //Create Console
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    std::cout << "Welcome\n";

    DWORD moduleBase = (DWORD)GetModuleHandle("client.dll");

    while (true)
    {
        DWORD LPlayer = *(DWORD*)(moduleBase + dwLocalPlayer);
        DWORD GlowManager = *(DWORD*)(moduleBase + dwGlowObjectManager);
        int localTeam = *(int*)(LPlayer + m_iTeamNum);

        if (GetAsyncKeyState(VK_END) & 1)
        {
            break;
        }

        if (GetAsyncKeyState(VK_MENU)) //alt_key
        {

            for (int i = 1; i < 32; i++)
            {

                DWORD entity = *(DWORD*)((moduleBase + dwEntityList) + i * 0x10);

                if (entity == NULL)
                    continue;

                int glowIndex = *(int*)(entity + m_iGlowIndex);
                int entityteam = *(int*)(entity + m_iTeamNum);

                if (entityteam == localTeam)
                {
                    *(float*)((GlowManager + glowIndex * 0x38 + 0x8)) = 0.f; //Red
                    *(float*)((GlowManager + glowIndex * 0x38 + 0xC)) = 1.f; //Green
                    *(float*)((GlowManager + glowIndex * 0x38 + 0x10)) = 0.f; //Blue
                    *(float*)((GlowManager + glowIndex * 0x38 + 0x14)) = 2.6f; //Alpha

                }
                else
                {
                    *(float*)((GlowManager + glowIndex * 0x38 + 0x8)) = 1.f;
                    *(float*)((GlowManager + glowIndex * 0x38 + 0xC)) = 0.f;
                    *(float*)((GlowManager + glowIndex * 0x38 + 0x10)) = 0.f;
                    *(float*)((GlowManager + glowIndex * 0x38 + 0x14)) = 2.6f;
                }

                *(bool*)((GlowManager + glowIndex * 0x38 + 0x28)) = true;
                *(bool*)((GlowManager + glowIndex * 0x38 + 0x29)) = false;
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