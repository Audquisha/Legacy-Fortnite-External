#include "process.h"

DWORD procID;
uintptr_t modBase;
HANDLE hProc;
HWND targetWindow;
RECT windowRect;
windowProperties winProperties;




uintptr_t GNames = 0x5B8D390;
uintptr_t GObjects = 0x5B918B8;

clock_t winUpdateTimer = -6000;
clock_t checkProcActiveTime = -6000;

std::chrono::steady_clock::time_point startTime;
int ticks;
int bufTicks;

int totalTPS;
int totalTime;


namespace util {




    DWORD getProcID(const char* procName) {

        DWORD procID = 0;
        PROCESSENTRY32 procEntry;
        procEntry.dwSize = sizeof(PROCESSENTRY32);
        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
        if (Process32First(hSnap, &procEntry)) {


            do {


                if (!strcmp(procName, procEntry.szExeFile)) {


                    procID = procEntry.th32ProcessID;
                    break;

                }


            } while (Process32Next(hSnap, &procEntry));


        }
        if (!procID) {


            printf("Failed To Locate Process With Name \"%s\" ", procName);
        }


        return procID;

    }

    uintptr_t GetModuleBaseAddress(DWORD ProcID, const char* procName) {

        uintptr_t modBaseAddr = 0;

        HANDLE hSnap = 0;

        hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, ProcID);

        MODULEENTRY32 modEntry;

        modEntry.dwSize = sizeof(modEntry);

        if (Module32First(hSnap, &modEntry)) {

            do {

                if (!strcmp(modEntry.szModule, procName)) {


                    modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                    break;

                }
            } while (Module32Next(hSnap, &modEntry));


        }
        CloseHandle(hSnap);
        return modBaseAddr;

    }
    template<typename T> T Read(uintptr_t address, int size) {


        T buffer;

        ReadProcessMemory(hProc, (BYTE*)address, &buffer, size, NULL);

        return buffer;


    }

    template<typename T> bool Write(uintptr_t address, T toWrite) {


        return WriteProcessMemory(hProc, (BYTE*)address, &toWrite, sizeof(toWrite), NULL);

    }


    bool updateWindow() {

        //  targetWindow = FindWindow("UnrealWindow", "FPS Chess  ");

        clock_t tempTime = clock();
        if (tempTime >= winUpdateTimer + 5000) {

            if (targetWindow) {


                LONG_PTR windowStyle = GetWindowLongPtr(targetWindow, GWL_STYLE);

                GetWindowRect(targetWindow, &windowRect);

                if (windowStyle & WS_CAPTION) {

                    winProperties.bWindowed = true;
                    winProperties.width = (float)(windowRect.right - windowRect.left);
                    winProperties.height = float(windowRect.bottom - windowRect.top + 19.5f);
                    winProperties.x = (float)(windowRect.left);
                    winProperties.y = (float)windowRect.top;
                }
                else {

                    winProperties.bWindowed = false;
                    winProperties.width = (float)(windowRect.right - windowRect.left);
                    winProperties.height = float(windowRect.bottom - windowRect.top);
                    winProperties.x = (float)(windowRect.left);
                    winProperties.y = (float)windowRect.top;

                }




                winUpdateTimer = tempTime;

                return true;
            }

        }
        return false;

    }


    bool init() {

        procID = getProcID("FortniteClient-Win64-Shipping.exe");
        if (procID) {

            modBase = GetModuleBaseAddress(procID, "FortniteClient-Win64-Shipping.exe");
            hProc = OpenProcess(PROCESS_ALL_ACCESS, NULL, procID);

            GNames += modBase;
            GNames = util::Read<uintptr_t>(GNames, sizeof(GNames));
            GObjects += modBase;
            targetWindow = FindWindow("UnrealWindow", "Fortnite  ");

           startTime = std::chrono::high_resolution_clock::now();

            return true;
        }
        else { return false; }
    }




    void toRead() {

        Read<uintptr_t>(1, 0x0);
        Read<INT32>(1, 0x0);
        Read<int>(1, 0x0);
        Read<float>(1, 0x0);
        Read<FVector>(1, 0x0);
        Read<INT64>(1, 0x0);
        Read<INT8>(1, 0x0);
        Read<short>(1, 0x0);
        Read<bool>(1, 0x0);
        Read<FMinimalViewInfo>(1, 0x0);
        Read<Vector3>(1, 0x0);
        Read<unsigned int>(1, 0x0);
        Read<D3DMATRIX>(1, 0x0);
        Read<FTransform>(1, 0x0);

        Write<float>(0x0, 1);
        Write<int>(0x0, 1);
        Write<Vector3>(0x0, Vector3(0, 0, 0));



    }

    int setAimKey() {


        int key = -1;
        while (key == -1) {


            for (int i = 0; i < 256; i++) {


                if (GetAsyncKeyState(i) & 0x8000) {


                    return i;

                }



            }
        }
        return -1;


    }
    bool isActive() {

        clock_t tempTime = clock();

        if (tempTime >= checkProcActiveTime + 6000) {


            DWORD active;
            GetExitCodeProcess(hProc, &active);

            if (!active) {
                return false;
            }
            else {
                return true;
            }
        }
        else return true;

    }

    D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2)
    {
        D3DMATRIX pOut;
        pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
        pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
        pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
        pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
        pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
        pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
        pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
        pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
        pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
        pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
        pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
        pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
        pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
        pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
        pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
        pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

        return pOut;
    }


    float getDistance(Vector3 localPos, Vector3 enemyPos) {


        float xDiff = (localPos.x - enemyPos.x);
        float yDiff = (localPos.y - enemyPos.y);
        float zDiff = (localPos.z - enemyPos.z);

        return sqrt((xDiff * xDiff) + (yDiff * yDiff) + (zDiff * zDiff));



    }

    float get2DDistance(Vector3 W2S, Vector3 screenCenter) {


        float xDiff = (W2S.x - screenCenter.x);
        float yDiff = (W2S.y - screenCenter.y);
        return sqrt((xDiff * xDiff) + (yDiff * yDiff));

    }

    D3DMATRIX Matrix(Vector3 rot)
    {

        Vector3 origin = Vector3(0, 0, 0);
        float radPitch = (rot.x * float(3.1415926535897932384626433832795) / 180.f);
        float radYaw = (rot.y * float(3.1415926535897932384626433832795) / 180.f);
        float radRoll = (rot.z * float(3.1415926535897932384626433832795) / 180.f);

        float SP = sinf(radPitch);
        float CP = cosf(radPitch);
        float SY = sinf(radYaw);
        float CY = cosf(radYaw);
        float SR = sinf(radRoll);
        float CR = cosf(radRoll);

        D3DMATRIX matrix;
        matrix.m[0][0] = CP * CY;
        matrix.m[0][1] = CP * SY;
        matrix.m[0][2] = SP;
        matrix.m[0][3] = 0.f;

        matrix.m[1][0] = SR * SP * CY - CR * SY;
        matrix.m[1][1] = SR * SP * SY + CR * CY;
        matrix.m[1][2] = -SR * CP;
        matrix.m[1][3] = 0.f;

        matrix.m[2][0] = -(CR * SP * CY + SR * SY);
        matrix.m[2][1] = CY * SR - CR * SP * SY;
        matrix.m[2][2] = CR * CP;
        matrix.m[2][3] = 0.f;

        matrix.m[3][0] = origin.x;
        matrix.m[3][1] = origin.y;
        matrix.m[3][2] = origin.z;
        matrix.m[3][3] = 1.f;

        return matrix;
    }

    Vector3 WorldToScreen(Vector3 WorldLocation, FMinimalViewInfo camInfo)
    {
        Vector3 Screenlocation = Vector3(0, 0, 0);

        auto POV = camInfo;

        Vector3 Rotation = POV.Rotation; // FRotator

        D3DMATRIX tempMatrix = util::Matrix(Rotation); // Matrix

        Vector3 vAxisX, vAxisY, vAxisZ;


        vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
        vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
        vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

        Vector3 vDelta = WorldLocation - POV.Location;
        Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

        if (vTransformed.z < 1.f)
            vTransformed.z = 1.f;

        float FovAngle = POV.FOV;

        float ScreenCenterX = (winProperties.width / 2.0f);
        float ScreenCenterY = (winProperties.height / 2.0f);

        if (camInfo.Rotation.y > -90.f && camInfo.Rotation.y < 90.f) {
            Screenlocation.x = (ScreenCenterX - vTransformed.x * (ScreenCenterX / tanf(FovAngle * (float)3.1415926535897932384626433832795 / 360.f)) / vTransformed.z) + winProperties.x;
            Screenlocation.y = (ScreenCenterY + vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)3.1415926535897932384626433832795 / 360.f)) / vTransformed.z) + winProperties.y;
        }
        else {

            Screenlocation.x = (ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(FovAngle * (float)3.1415926535897932384626433832795 / 360.f)) / vTransformed.z) + winProperties.x;
            Screenlocation.y = (ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)3.1415926535897932384626433832795 / 360.f)) / vTransformed.z) + winProperties.y;
        }
        return Screenlocation;
    }

    int measureTicksPerSecond() {

        auto endTime = std::chrono::high_resolution_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();

        if (elapsedTime <= 1) {
            ticks++;
        }

        else if (elapsedTime == 0) {
            return static_cast<int>(static_cast<double>(bufTicks));// Calculate ticks per second
        }
        else {
            startTime = std::chrono::high_resolution_clock::now();

            bufTicks = ticks;
            ticks = 0;

            totalTime++;

            totalTPS += static_cast<int>(static_cast<double>(bufTicks));


            return static_cast<int>(static_cast<double>(bufTicks));// Calculate ticks per second
        }
    }
    
}