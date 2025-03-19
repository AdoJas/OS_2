#pragma once

#ifdef DLL_APP_EXPORTS
#define DLL_FUNC __declspec(dllexport)
#else
#define DLL_FUNC __declspec(dllimport)
#endif

extern "C" {
    DLL_FUNC void StartTimer();
    DLL_FUNC void EndTimer();
    DLL_FUNC void OpenControlPanelComponent();
    DLL_FUNC void CreateFolders();
    DLL_FUNC void ComputeTschirnhausen(double F, double x0, double xn, double dx);
    DLL_FUNC void MergeAndClean(double F);
    DLL_FUNC void DeleteFolders();
}