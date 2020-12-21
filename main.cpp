#include <iostream>

#include "Memory.h"
#include "Scan.h"

#include <fstream>
#include <filesystem>
#include <direct.h>

#include "Json.h"
using namespace nlohmann;

uint64_t sh_allow_address = 0;
static bool status = false;

std::string CurrentPath()
{
    char* cwd = _getcwd(0, 0);
    std::string directory(cwd);
    std::free(cwd);
    return directory;
}

void scan_scripthook()
{
    Scan* scan = new Scan(Memory::hProc, Memory::modEntry);

    const char* sh_allow_pattern = "48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8D 0D ? ? ? ? 48 83 C4 ? E9 ? ? ? ? 48 8D 0D ? ? ? ?";
    sh_allow_address = Memory::GetActualAddr((uint64_t)scan->PatternScan(sh_allow_pattern) + 0x3) + 0x8;

    delete scan;
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

    int hotkeyEnable = js["enable"];
    int hotkeyNormal = js["disable"];

    SetConsoleTitleA("SHBypass | FiveM | InsideExploit [UC]");

    Memory::pid = Memory::GetProcessId("FiveM_GTAProcess.exe");
    Memory::modEntry = Memory::GetModule(Memory::pid, "gta-core-five.dll");
    Memory::hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Memory::pid);

    if (!Memory::pid)
    {
        std::cout << "[ERROR] Could not get process id. [-1]\n";
        Sleep(1500);
        exit(-1);
    }

    if (Memory::modEntry.modBaseAddr == NULL || Memory::modEntry.modBaseSize == NULL)
    {
        std::cout << "[ERROR] Could not get base address. [-3]\n";
        Sleep(1500);
        exit(-3);
    }

    if (Memory::hProc == INVALID_HANDLE_VALUE)
    {
        std::cout << "[ERROR] Could not handle the process. [-2]\n";
        Sleep(1500);
        exit(-2);
    }

    scan_scripthook();

    std::cout << "[INFO] Please use an command.\n\n";
    std::cout << "[HOTKEY] F1: Enable.\n";
    std::cout << "[HOTKEY] F2: Disable.\n";

    std::cout << "\n[IMPORTANT] Remember, that one showed key are the default one.\n";

    while (true)
    {
        Sleep(1);

        if (GetKeyState(hotkeyEnable))
        {
            status = true;
            WriteProcessMemory(Memory::hProc, (LPVOID)(Memory::modEntry.modBaseAddr + sh_allow_address), &status, sizeof(status), NULL);
        }

        if (GetKeyState(hotkeyNormal))
        {
            status = false;
            WriteProcessMemory(Memory::hProc, (LPVOID)(Memory::modEntry.modBaseAddr + sh_allow_address), &status, sizeof(status), NULL);
        }
    }
}
