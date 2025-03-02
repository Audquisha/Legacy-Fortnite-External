#pragma once
#include "precomp.h"


namespace util {

    template<typename T> T Read(uintptr_t address, int size);

    template<typename T> bool Write(uintptr_t address, T toWrite);
    DWORD getProcID(const char* procName);

    uintptr_t GetModuleBaseAddress(DWORD ProcID, const char* procName);

    bool updateWindow();

    bool init();

    void toRead();

    int setAimKey();

    bool isActive();

    D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2);

    D3DMATRIX Matrix(Vector3 rot);


    float getDistance(Vector3 localPos, Vector3 enemyPos);

    float get2DDistance(Vector3 W2S, Vector3 screenCenter);
    Vector3 WorldToScreen(Vector3 WorldLocation, FMinimalViewInfo camInfo);
    
    int measureTicksPerSecond();

  

}