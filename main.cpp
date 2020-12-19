#include <iostream>

#include "Memory.h"

#include <fstream>
#include <filesystem>
#include <direct.h>

#include "Json.h"
using namespace nlohmann;

memory* mem;

std::string CurrentPath()
{
    char* cwd = _getcwd(0, 0);
    std::string directory(cwd);
    std::free(cwd);
    return directory;
}

int main()
{
    json js;

    std::string configPath = CurrentPath() + "\\config.json";

    std::ifstream configR(configPath);

    if (!configR)
    {

        std::cout << "[LOADING] Creating the json config file because is missing...\n";

        Sleep(300);

        std::ofstream configW;
        configW.open(configPath);
        configW << "{\n";
        configW << "  \"sh_allow\": 903864,\n";
        configW << "  \"enable\": 112,\n";
        configW << "  \"disable\": 113\n";
        configW << "}";

        configW.close();

        Sleep(300);

        std::cout << "[SUCCESS] The json config file got created... please restart application.\n";

        Sleep(500);

        _exit(false);
    }

    configR >> js;

    uintptr_t offset = js["sh_allow"];
    int hotkeyEnable = js["enable"];
    int hotkeyNormal = js["disable"];
    //float version = js["Version"];

    /*
    if (version != Config::Program::version)
    {
        std::cout << "[-] Please delete the old config.json";
        exit(-7);
    }
    */

    SetConsoleTitleA("SHBypass | FiveM | InsideExploit[UC]");

    DWORD ProcessID = mem->get_pid(L"FiveM_GTAProcess.exe"); if (!ProcessID) return false;
    DWORD gta_base = mem->get_base(ProcessID, L"FiveM_GTAProcess.exe"); if (!gta_base) return false;
    unsigned long long base = mem->get_base(ProcessID, L"gta-core-five.dll"); if (!base) return false;
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID); if (!hProcess) return false;

    if (!ProcessID)
    {
        std::cout << "[ERROR] Could not get process id. [-1]\n";
        Sleep(1500);
        exit(-1);
    }

    if (!hProcess)
    {
        std::cout << "[ERROR] Could not handle the process. [-2]\n";
        Sleep(1500);
        exit(-2);
    }

    if (!base)
    {
        std::cout << "[ERROR] Could not get base address. [-3]\n";
        Sleep(1500);
        exit(-3);
    }

    std::cout << "[INFO] Please use an command.\n\n";
    std::cout << "[HOTKEY] F1: Enable.\n";
    std::cout << "[HOTKEY] F2: Disable.\n";

    std::cout << "\n[IMPORTANT] Remember, that one showed key are the default one.\n";


    std::cout << "" << std::endl;


    uint8_t enable[] = { 0x1 };
    uint8_t disable[] = { 0x0 };

    while (1)
    {
        if (GetAsyncKeyState(hotkeyEnable) & 0x1)
        {
            std::cout << "[SUCCESS] ScriptHook enabled. [5.0]\n";
            WriteProcessMemory(hProcess, (LPVOID)(base + offset), enable, sizeof(enable), NULL);
            Sleep(1);
        }

        if (GetAsyncKeyState(hotkeyNormal) & 0x1)
        {
            std::cout << "[SUCCESS] ScriptHook disabled. [5.1]\n";
            WriteProcessMemory(hProcess, (LPVOID)(base + offset), disable, sizeof(disable), NULL);
            Sleep(1);
        }
    }
    Sleep(1);
}