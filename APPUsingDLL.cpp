#include <iostream>
#include <windows.h>
#include <string>

typedef void(*StartTimerFunc)(); //Declarinu pointerius dll funkcijoms
typedef void(*EndTimerFunc)();
typedef void(*OpenControlPanelFunc)();
typedef void(*CreateFoldersFunc)();
typedef void(*ComputeTschirnhausenFunc)(double, double, double, double);
typedef void(*MergeAndCleanFunc)(double);
typedef void(*DeleteFoldersFunc)();

int main() {
    HMODULE dll = LoadLibrary(L"Dll_APP.dll"); //Loadinu DLL
    if (!dll) {
        std::cerr << "DLL load failed!" << std::endl;
        return 1;
    }

    auto StartTimer = (StartTimerFunc)GetProcAddress(dll, "StartTimer"); //Priskiriu pointerius 
    auto EndTimer = (EndTimerFunc)GetProcAddress(dll, "EndTimer");
    auto OpenControlPanel = (OpenControlPanelFunc)GetProcAddress(dll, "OpenControlPanelComponent");
    auto CreateFolders = (CreateFoldersFunc)GetProcAddress(dll, "CreateFolders");
    auto ComputeTschirnhausen = (ComputeTschirnhausenFunc)GetProcAddress(dll, "ComputeTschirnhausen");
    auto MergeAndClean = (MergeAndCleanFunc)GetProcAddress(dll, "MergeAndClean");
    auto DeleteFolders = (DeleteFoldersFunc)GetProcAddress(dll, "DeleteFolders");

    // Verify all functions
    if (!StartTimer || !EndTimer || !OpenControlPanel || !CreateFolders ||
        !ComputeTschirnhausen || !MergeAndClean || !DeleteFolders) {
        FreeLibrary(dll);
        std::cerr << "Function load failed!" << std::endl;
        return 1;
    }

    OpenControlPanel();

    StartTimer();

    const long long student_ID = 2313977; 
    const double x0 = -(student_ID % 19);
    const double xn = student_ID % 25;
    const double dx = student_ID / 1000000000000.0;

    CreateFolders();
    std::cout << "Folders Created" << std::endl;

    // Process all F values
    const double F_values[] = { -2, -1, 0, 1, 2 };
    for (double F : F_values) {
        ComputeTschirnhausen(F, x0, xn, dx);
        std::cout << "ComputeTschirnhausen done for " << std::to_string(F) << std::endl;
    }

    DeleteFolders();
    EndTimer();

    FreeLibrary(dll);
    return 0;
}