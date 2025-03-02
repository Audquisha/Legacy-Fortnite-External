#include "engine.h"

uintptr_t UWorld;
uintptr_t Levels;
INT32 levelCount;
uintptr_t persistentLevel;
uintptr_t GameInstance;
uintptr_t GameState;
uintptr_t localPlayers;
uintptr_t localPlayer;
uintptr_t localPlayerController;
uintptr_t localPlayerPiece;
int localPlayerID;

std::vector<AFortPlayerState> targetPawns;
std::vector<AFortPlayerState> enemyPawns;

std::vector<FortItem> worldItems;

uintptr_t playerArray;

AimSettings currAimSetting;


AFortPlayerState localPlayerState;
int aimTarg = -1;
AFortPlayerState aimPiece;


ImVec2 boxDimensions;

FMinimalViewInfo viewInfo;

clock_t worldUpdateTimer = -4000;
clock_t localPlayerUpdateTimer = -4000;
clock_t pieceBaseUpdateTimer = -4000;
clock_t pieceUpdateTimer = -2000;
clock_t levelUpdateTimer = -4000;

unsigned int dword_5A1DA78 = 0;

static void fname_decrypt(char* inputBuf, int namelength)
{
    char decname[1024];
    ZeroMemory(decname, sizeof(decname));

    __int64 outputBuff = reinterpret_cast<__int64>(&decname);

    if (dword_5A1DA78 == 0 && modBase != 0)
    {
        dword_5A1DA78 = util::Read<unsigned int>(modBase + 0x5A1DA78, sizeof(dword_5A1DA78));
    }
    char v9;
    char v4 = *inputBuf;
    unsigned int v5 = dword_5A1DA78 % 0x21;
    unsigned int v6 = dword_5A1DA78 % 0x21 + (dword_5A1DA78 ^ 0x9C677CC5);
    *decname = static_cast<BYTE>(v6 ^ *inputBuf);
    if (v4 != (BYTE)v6)
    {
        char* v7 = decname;
        __int64 v8 = (uint64_t)inputBuf - outputBuff;
        do
        {
            v6 += v5 + (DWORD)++v7 - (DWORD)outputBuff;
            v9 = v6 ^ v7[v8];
            *v7 = v9;
        } while (v9);
    }


    memcpy(inputBuf, decname, sizeof(decname));
}

std::string GetNameFromFName(int key)
{


    constexpr auto ElementsPerChunk = 0x4000;

    enum { NAME_SIZE = 1024 };

    char name[NAME_SIZE] = { 0 };
    SecureZeroMemory(name, NAME_SIZE);


    const int32_t Index = key;


    const int32_t ChunkIndex = Index / ElementsPerChunk;
    const int32_t WithinChunkIndex = Index % ElementsPerChunk;

    //GNAMES_POOL_OFFSET exists as theres always a offset for whatever reason. Check this in IDA!!!!!!!!!
    const uint64_t ElementType = util::Read<uint64_t>(GNames + 8 * ChunkIndex + 112, sizeof(ElementType));

    //WithinChunkIndex * 8 as its full of pointers
    const auto FNameEntryPtrPtr = ElementType + (WithinChunkIndex * 8);

    const auto FNameEntryPtr = util::Read<uint64_t>(FNameEntryPtrPtr, sizeof(uint64_t));


    const uint64_t AnsiName = FNameEntryPtr + 0xC;


    int nameLength = NAME_SIZE - 1;
    ReadProcessMemory(hProc, reinterpret_cast<void*>(AnsiName), name, nameLength, NULL);
    fname_decrypt(name, nameLength);
    std::string finalName = name;
    finalName = finalName.substr(0, nameLength);
    //delete[] name;

    return finalName;
}

/*
std::vector<AActor> getAActors(uintptr_t levelBaseAddress) {

    uintptr_t pAActors = util::Read<uintptr_t>((levelBaseAddress + 0x98), sizeof(pAActors));

    int actorCount = util::Read<int>((levelBaseAddress + 0xA0), sizeof(actorCount));

    std::vector<AActor> AActors(actorCount);

    for (int i = 0; i < actorCount; i++) {

        AActors[i] = AActor(util::Read<uintptr_t>((pAActors + (8 * i)), sizeof(uintptr_t)));
    
        AActors[i].getPos();
    }

    return AActors;

}
*/
bool updateWorld() {


    uintptr_t buffer = UWorld;

    clock_t tempTime = clock();
    if (tempTime >= worldUpdateTimer + 4000) {

        UWorld = util::Read<uintptr_t>(GWorld + modBase, sizeof(UWorld));


            Levels = util::Read<uintptr_t>(UWorld + offsets::UWorld::Levels, sizeof(Levels));

            levelCount = util::Read<INT32>(UWorld + 0x140, sizeof(levelCount));

            persistentLevel = util::Read<uintptr_t>((UWorld + offsets::UWorld::PersistentLevel), sizeof(persistentLevel));

            GameInstance = util::Read<uintptr_t>((UWorld + offsets::UWorld::OwningGameInstance), sizeof(GameInstance));

            GameState = util::Read<uintptr_t>((UWorld + offsets::UWorld::GameState), sizeof(GameState));

            localPlayers = util::Read<uintptr_t>((GameInstance + 0x38), sizeof(localPlayers));

            localPlayer = util::Read<uintptr_t>(localPlayers, sizeof(localPlayer));



            worldUpdateTimer = tempTime;

            return true;

        }



  }



bool updateLocalPlayer() {

    clock_t tempTime = clock();

    if (tempTime >= localPlayerUpdateTimer + 3000) {

        localPlayerController = util::Read<uintptr_t>(localPlayer + 0x30, sizeof(localPlayerController));

        localPlayerState = AFortPlayerState(util::Read<uintptr_t>((localPlayerController + 0x338), sizeof(uintptr_t)));

        localPlayerUpdateTimer = tempTime;
        return true;
    }
    return false;

}

/*
std::string getPlayerName(AActor player) {

   

    int nameLength = util::Read<int>((player.playerStateBase + 0x340), sizeof(nameLength));

    

    wchar_t* buffer = new wchar_t[nameLength * 2];
    SecureZeroMemory(buffer, nameLength * 2);

    uintptr_t nameAddr = util::Read<uintptr_t>((player.playerStateBase + 0x338), sizeof(nameAddr));

    ReadProcessMemory(hProc, (BYTE*)(nameAddr), buffer, nameLength * 2, NULL);

    WORD* pNameBufferPointer;
    int i; // ecx
    char v25; // al
    int v26; // er8
    int v29; // eax

    v25 = nameLength - 1;
    v26 = 0;
    pNameBufferPointer = (WORD*)buffer;

    for (i = (v25) & 3; ; *pNameBufferPointer++ += i & 7)
    {
        v29 = nameLength - 1;
        if (!(DWORD)nameLength)
            v29 = 0;

        if (v26 >= v29)
            break;

        i += 3;
        ++v26;
    }

    std::wstring ws(buffer);
    std::string str(ws.begin(), ws.end());
    delete[] buffer;

    return str;



}
*/


FMinimalViewInfo getViewInfo() {


    int screenScaleOffset = 0xD08;

    FMinimalViewInfo currViewInfo;
    uintptr_t viewStates = util::Read<uintptr_t>(localPlayer + 0xb0, sizeof(viewStates));

    D3DMATRIX invProjMatrix = util::Read<D3DMATRIX>(viewStates + (screenScaleOffset - 0x2A8), sizeof(invProjMatrix));

    currViewInfo.Location.x = invProjMatrix.m[3][0];
    currViewInfo.Location.y = invProjMatrix.m[3][1];
    currViewInfo.Location.z = invProjMatrix.m[3][2];

    currViewInfo.Rotation.x = (rad2Deg(asinf(invProjMatrix.m[2][2])));
    currViewInfo.Rotation.y = rad2Deg(atan2f(invProjMatrix.m[2][1], invProjMatrix.m[2][0]));
    currViewInfo.Rotation.z = (rad2Deg((atan2f(invProjMatrix.m[0][2], invProjMatrix.m[2][0]))));
    float fov = atanf(1 / util::Read<float>(viewStates + 0x960, sizeof(float))) * 2;
    currViewInfo.FOV = rad2Deg(fov);





    if (currViewInfo.Rotation.y >= 90.f || currViewInfo.Rotation.y <= 90.f) {

        currViewInfo.FOV *= -1;
    }



    viewInfo = currViewInfo;

    return viewInfo;

}

/*
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
*/

AimTab currWeaponType() {

    uintptr_t currWeapon = util::Read<uintptr_t>(localPlayerState.playerPawn.baseAddress + 0x820, sizeof(currWeapon));

    std::string currWeaponName = GetNameFromFName(util::Read<int>(currWeapon + 0x18, sizeof(int)));
    
    if (std::strstr(currWeaponName.c_str(), "Assault") != nullptr) {

        return AR;

    }
    else if (std::strstr(currWeaponName.c_str(), "Shotgun") != nullptr) {

        return SG;

    }
    else if (std::strstr(currWeaponName.c_str(), "Pistol_PDW") != nullptr || std::strstr(currWeaponName.c_str(), "Pistol_Auto") != nullptr || std::strstr(currWeaponName.c_str(), "Minigun") != nullptr) {

        return SMG;

    }
    else if (std::strstr(currWeaponName.c_str(), "Sniper") != nullptr || std::strstr(currWeaponName.c_str(), "Rifle_NoScope") != nullptr) {

        return SNIPER;

    }
    else if (std::strstr(currWeaponName.c_str(), "Pistol") != nullptr) {

        return PISTOL;

    }
    else {
        return NOAIM;
    }





}

bool refreshPawns() {
    
    clock_t tempTime = clock();
    if (tempTime >= pieceUpdateTimer + 4000) {

        SecureZeroMemory(&targetPawns, sizeof(targetPawns));
        SecureZeroMemory(&enemyPawns, sizeof(enemyPawns));

        INT32 playerSize = util::Read<INT32>(GameState + 0x350, sizeof(playerSize));
        playerArray = util::Read<uintptr_t>(GameState + 0x348, sizeof(playerArray));

        targetPawns = std::vector<AFortPlayerState>();


        for (int i = 0; i < playerSize; i++) {

            AFortPlayerState tempPlayerState = AFortPlayerState(util::Read<uintptr_t>(playerArray + (i * 0x8), sizeof(uintptr_t)));
          
        
            tempPlayerState.playerPawn.refreshSkeleton();

            if (!tempPlayerState.playerPawn.isDead()) {

                targetPawns.push_back(tempPlayerState);
            }

        }


        enemyPawns.clear();
        enemyPawns.shrink_to_fit();
        for (int id = 0; id < targetPawns.size(); id++) {
            if (!targetPawns[id].playerPawn.skeleton[0][0].isZero()) {

                if (targetPawns[id].playerPawn.playerID != localPlayerState.playerPawn.playerID) {


                    enemyPawns.push_back(targetPawns[id]);


                }
                else {

                    localPlayerState = targetPawns[id];
                }

            }
        }

        pieceUpdateTimer = tempTime;
        return true;
    }




    else {


        for (int i = 0; i < targetPawns.size(); i++) {


      

            targetPawns[i].playerPawn.refreshSkeleton();

            targetPawns[i].playerPawn.getDistance();

            targetPawns[i].playerPawn.getW2S();

            if (targetPawns[i].playerPawn.isDead()) {



                    if (i >= 0 && i < targetPawns.size()) {
                        targetPawns.erase(targetPawns.begin() + i);
                    }


                
            }



        }



        enemyPawns.clear();
        enemyPawns.shrink_to_fit();
        for (int id = 0; id < targetPawns.size(); id++) {

            if (!targetPawns[id].playerPawn.skeleton[0][0].isZero()) {


          
            if (targetPawns[id].playerPawn.playerID != localPlayerState.playerPawn.playerID) {


                enemyPawns.push_back(targetPawns[id]);


            }
            else {

                localPlayerState = targetPawns[id];
            }
        }

        }

    }

    return false;

}





void drawSkeleton(AFortPlayerState player) {


    BoneInfo previous(0, 0, 0, 0);
    BoneInfo current(0, 0, 0, 0);
    Vector3 p1, c1;
    for (int boneSet = 0; boneSet < player.playerPawn.skeleton.size(); boneSet++)
    {
        previous = BoneInfo(0, 0, 0, 0);
        for (int bone = 0; bone < player.playerPawn.skeleton[boneSet].size(); bone++)
        {
            current = player.playerPawn.skeleton[boneSet][bone];
            if (previous.x == 0.f)
            {
                previous = current;
                continue;
            }

            overlay::drawLine(Vector3(previous.W2S.x, previous.W2S.y, 0), Vector3(current.W2S.x, current.W2S.y, 0));
            previous = current;
        }
    }


}


void drawBoneIDs(AFortPlayerState player) {

    INT32 boneArrSize = util::Read<INT32>(player.playerPawn.meshAddress + 0x5c0 + 0x8, sizeof(boneArrSize));
    for (int i = 0; i < boneArrSize; i++) {

        FTransform bone = player.playerPawn.GetBoneIndex(i);

        FTransform ComponentToWorld = util::Read<FTransform>(player.playerPawn.meshAddress + 0x190, sizeof(FTransform));

        D3DMATRIX Matrix;
        Matrix = util::MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());
        Vector3 boneWorld = Vector3(Matrix._41, Matrix._42, Matrix._43);

        overlay::drawText(util::WorldToScreen(boneWorld, viewInfo), 0.f, std::to_string(i).c_str());

    }


}


AFortPlayerState findPlayerByID(int32_t ID) {

    for (int i = 0; i < enemyPawns.size(); i++) {

        if (enemyPawns[i].playerPawn.playerID == ID) {
            return enemyPawns[i];
        }



    }
    SecureZeroMemory(&aimPiece, sizeof(aimPiece));
    return aimPiece;


}

ImColor getColorByRarity(INT8 rarity) {



    switch (rarity) {

    case(0): {

        return { 0.741f, 0.729f, 0.729f, 1.f };

    }
    case(1): {

        return { 0.039f, 0.769f, 0.f ,1.f };
    }
    case(2): {

        return { 0.02f, 0.529f, 0.769f ,1.f };
    }

    case(3): {

        return { 0.855f, 0.f, 0.922f , 1.f };
    }
    case(4): {

        return { 0.922f, 0.529f, 0.f , 1.f };
    }

           //chest
    case(5): {


        return{ 0.922f, 0.847f, 0.f , 1.f };
    }

           //ammoBox
    case(6): {

        return { 0.553f, 0.961f, 0.553f ,1.f };
    }


    }

}


void drawDebugAActors() {

    uintptr_t levelArray = util::Read<uintptr_t>(UWorld + 0x138, sizeof(levelArray));
    INT32 levelCount = util::Read<INT32>(UWorld + 0x138 + 0x8, sizeof(levelCount));

    for (int level = 0; level < levelCount; level++) {

        uintptr_t levelBase = util::Read<uintptr_t>(levelArray + 0x8 * level, sizeof(levelBase));

        uintptr_t  actorCluster = util::Read<uintptr_t>(levelBase + 0x98, sizeof(actorCluster));

        INT32 actorClusterSize = util::Read<INT32>(levelBase + 0x98 + 0x8, sizeof(actorClusterSize));

        for (int i = 0; i < actorClusterSize; i++) {



            AActor temp(util::Read<uintptr_t>(actorCluster + 0x8 * i, sizeof(uintptr_t)));
            temp.getDistance();
            temp.getW2S();
          
            overlay::drawText(temp.W2S, 0.f, temp.Name.c_str());


        }

    }
}


void cacheLevel() {

    clock_t tempTime = clock();
    if (tempTime >= levelUpdateTimer + 10000) {

        worldItems.clear();
        worldItems.shrink_to_fit();

        uintptr_t levelArray = util::Read<uintptr_t>(UWorld + 0x138, sizeof(levelArray));
        INT32 levelCount = util::Read<INT32>(UWorld + 0x138 + 0x8, sizeof(levelCount));

        for (int level = 0; level < levelCount; level++) {

            uintptr_t levelBase = util::Read<uintptr_t>(levelArray + 0x8 * level, sizeof(levelBase));

            uintptr_t  actorCluster = util::Read<uintptr_t>(levelBase + 0x98, sizeof(actorCluster));

            INT32 actorClusterSize = util::Read<INT32>(levelBase + 0x98 + 0x8, sizeof(actorClusterSize));

            for (int i = 0; i < actorClusterSize; i++) {


                AActor tempActor = AActor(util::Read<uintptr_t>(actorCluster + 0x8 * i, sizeof(uintptr_t)));

      
                if (!strcmp("FortPickupAthena", tempActor.Name.c_str())) {
                   
                   FortItem bufferItem(util::Read<uintptr_t>(tempActor.baseAddress + 0x380, sizeof(uintptr_t)), tempActor.getPos());

                    worldItems.push_back(bufferItem);
                    continue;

                }
                else if (std::strstr(tempActor.Name.c_str(), "Tiered_Short_Ammo") != nullptr) {

                    FortItem bufferItem(tempActor.baseAddress, World_AmmoBox);

                    if (!bufferItem.isSearched) {


                        worldItems.push_back(bufferItem);

                    }
                    continue;
                }
                else if (std::strstr(tempActor.Name.c_str(), "Tiered_Chest") != nullptr) {

                   
                    FortItem bufferItem(tempActor.baseAddress, World_Chest);

                    if (!bufferItem.isSearched) {


                        worldItems.push_back(bufferItem);

                    }

                    continue;

                }


            }
        }
        levelUpdateTimer = tempTime;
    }
   

    for (int i = 0; i < worldItems.size(); i++) {

      
        worldItems[i].getW2S(); 
        worldItems[i].getDistance();

     
        
    }



}



void drawWorldItems() {


    if (itemESP::enabled) {


        for (int i = 0; i < worldItems.size(); i++) {

            if (worldItems[i].Distance/100.f < itemESP::maxDistance) {

                if (itemESP::chests) {

                    if (worldItems[i].itemType == World_Chest) {

                        overlay::drawText(worldItems[i].W2S, 0.f, worldItems[i].drawColor, worldItems[i].itemName.c_str());


                    }

                }

                if (itemESP::ammoBox) {

                    if (worldItems[i].itemType == World_AmmoBox) {

                        overlay::drawText(worldItems[i].W2S, 0.f, worldItems[i].drawColor, worldItems[i].itemName.c_str());


                    }

                }

                if (itemESP::ammo) {

                    if (worldItems[i].itemType == Item_AMMO) {

                        overlay::drawText(worldItems[i].W2S, 0.f, worldItems[i].drawColor, worldItems[i].itemName.c_str());

                    }


                }
                if (itemESP::consumable) {

                    if (worldItems[i].itemType == Item_Consumable) {

                        overlay::drawText(worldItems[i].W2S, 0.f, worldItems[i].drawColor, worldItems[i].itemName.c_str());


                    }


                }
                if (itemESP::traps) {


                    if (worldItems[i].itemType == Item_Trap) {

                        overlay::drawText(worldItems[i].W2S, 0.f, worldItems[i].drawColor, worldItems[i].itemName.c_str());


                    }

                }
                if (itemESP::materials) {

                    if (worldItems[i].itemType == Item_Material) {

                        overlay::drawText(worldItems[i].W2S, 0.f, worldItems[i].drawColor, worldItems[i].itemName.c_str());


                    }

                }

                if (itemESP::weaponESP::enabled) {

                    if (itemESP::weaponESP::AR) {

                        if (worldItems[i].weaponType == WEAPON_AR) {

                            overlay::drawText(worldItems[i].W2S, 0.f, worldItems[i].drawColor, worldItems[i].itemName.c_str());



                        }

                    }
                    if (itemESP::weaponESP::SG) {

                        if (worldItems[i].weaponType == WEAPON_SG) {


                            overlay::drawText(worldItems[i].W2S, 0.f, worldItems[i].drawColor, worldItems[i].itemName.c_str());


                        }

                    }
                    if (itemESP::weaponESP::SMG) {

                        if (worldItems[i].weaponType == WEAPON_SMG) {


                            overlay::drawText(worldItems[i].W2S, 0.f, worldItems[i].drawColor, worldItems[i].itemName.c_str());


                        }

                    }
                    if (itemESP::weaponESP::Sniper) {

                        if (worldItems[i].weaponType == WEAPON_SNIPER) {


                            overlay::drawText(worldItems[i].W2S, 0.f, worldItems[i].drawColor, worldItems[i].itemName.c_str());


                        }

                    }
                    if (itemESP::weaponESP::Pistol) {

                        if (worldItems[i].weaponType == WEAPON_PISTOL) {

                            overlay::drawText(worldItems[i].W2S, 0.f, worldItems[i].drawColor, worldItems[i].itemName.c_str());



                        }

                    }




                }


            }

        }




    }




}


void projectileTP() {


    uintptr_t currWeapon = util::Read<uintptr_t>(localPlayerState.playerPawn.baseAddress + 0x820, sizeof(currWeapon));

 
    uintptr_t weaponChildren = util::Read<uintptr_t>(currWeapon + 0x148, sizeof(weaponChildren));

    INT32 szWeaponChildren = util::Read<INT32>(currWeapon + 0x148 + 0x8, sizeof(szWeaponChildren));


    for (int i = 0; i < szWeaponChildren; i++) {

        uintptr_t projBase = util::Read<uintptr_t>(weaponChildren + 0x8 * i, sizeof(projBase));

        int projFnameID = util::Read<int>(projBase + 0x18, sizeof(projFnameID));

        std::string projName = GetNameFromFName(projFnameID);

        if (std::strstr(projName.c_str(), "Prj") != nullptr) {

            uintptr_t weaponRootComp = util::Read<uintptr_t>(projBase + offsets::AActor::RootComponent, sizeof(weaponRootComp));


            util::Write<Vector3>(weaponRootComp + 0x1A0, Vector3(aimPiece.playerPawn.skeleton[0][2].x, aimPiece.playerPawn.skeleton[0][2].y, aimPiece.playerPawn.skeleton[0][2].z));

        }
      

    }
        



}


void gatherProjectileInfo() {

    uintptr_t currWeapon = util::Read<uintptr_t>(localPlayerState.playerPawn.baseAddress + 0x820, sizeof(currWeapon));


    uintptr_t weaponChildren = util::Read<uintptr_t>(currWeapon + 0x148, sizeof(weaponChildren));

    INT32 szWeaponChildren = util::Read<INT32>(currWeapon + 0x148 + 0x8, sizeof(szWeaponChildren));


    for (int i = 0; i < szWeaponChildren; i++) {

        AActor projBase(util::Read<uintptr_t>(weaponChildren + 0x8 * i, 8));

       
        if (std::strstr(projBase.Name.c_str(), "Prj") != nullptr) {

           



        }


    }



}

void mainLoop() {


    hProc = OpenProcess(PROCESS_ALL_ACCESS, NULL, procID);


    util::updateWindow();




    if (updateWorld()) {


    }



    if (UWorld && GameState) {


        getViewInfo();

        if (updateLocalPlayer()) {

        }


        if (refreshPawns()) {


        }


        if (enemyPawns.size()) {



            if (!bAimActive || !aimPiece.playerPawn.playerID) {


                aimPiece = aimbot::getAimTarget();


            }

            if (aimPiece.playerPawn.playerID != 0) {
                    
                aimPiece = findPlayerByID(aimPiece.playerPawn.playerID);
            }
        }
        else {
            SecureZeroMemory(&aimPiece, sizeof(aimPiece));

        }



        currAimSetting = aimSettings[currWeaponType()];

     //   drawDebugAActors();

        if (itemESP::enabled) {

            cacheLevel();
            drawWorldItems();

        }


       
    }



    std::vector<AFortPlayerState> pawnsToLoop;

    currAimSetting.fovRadius = abs((currAimSetting.fov / viewInfo.FOV) * winProperties.height*(winProperties.height/winProperties.width));


    if (bDrawFov) {

        ImGui::GetBackgroundDrawList()->AddCircle({ winProperties.width / 2 + winProperties.x, winProperties.height / 2 + winProperties.y }, currAimSetting.fovRadius, ImColor(1.f, 0.f, 0.f, 1.f), 100, 2.f);

    }

    if (localESP) {

        pawnsToLoop = targetPawns;

    }
    else {

        pawnsToLoop = enemyPawns;
    }

    if (currAimSetting.bAimBot && aimPiece.playerPawn.playerID)
    {
       


            if (currAimSetting.aimBone == 0) {
                aimbot::aimbot(currAimSetting.aimKey, currAimSetting.smoothX, currAimSetting.smoothY, Vector3(aimPiece.playerPawn.skeleton[0][2].x, aimPiece.playerPawn.skeleton[0][2].y, aimPiece.playerPawn.skeleton[0][2].z));

            

            }
            else if (currAimSetting.aimBone == 1) {

                aimbot::aimbot(currAimSetting.aimKey, currAimSetting.smoothX, currAimSetting.smoothY, Vector3(aimPiece.playerPawn.skeleton[3][1].x, aimPiece.playerPawn.skeleton[3][1].y, aimPiece.playerPawn.skeleton[3][1].z));


            }
            else if (currAimSetting.aimBone == 2) {

                aimbot::aimbot(currAimSetting.aimKey, currAimSetting.smoothX, currAimSetting.smoothY, Vector3(aimPiece.playerPawn.skeleton[3][3].x, aimPiece.playerPawn.skeleton[3][3].y, aimPiece.playerPawn.skeleton[3][3].z));

            }

           
        
    }


    if (bulletTP && aimPiece.playerPawn.playerID) {

        projectileTP();
    }

    for (int i = 0; i < pawnsToLoop.size(); i++) {



      
        

        if (pawnsToLoop[i].playerPawn.Distance > 0.f ) {




            boxDimensions = { (60 / (pawnsToLoop[i].playerPawn.Distance / 1000.f)) * (winProperties.width / 1920) , (110 / (pawnsToLoop[i].playerPawn.Distance / 1000.f)) * (winProperties.height / 1080) };

            if (boxESP) {

                if (bRounded) {
                    overlay::drawBox(pawnsToLoop[i].playerPawn.W2S, rounding);

                }
                else {

                    overlay::drawBox(pawnsToLoop[i].playerPawn.W2S);


                }

            }

            if (lineESP) {


                overlay::drawLine(pawnsToLoop[i].playerPawn.W2S);

            }

            if (skeletonESP) {

                drawSkeleton(pawnsToLoop[i]);

            }

            if (distanceESP) {

                char distText[50];
                sprintf_s(distText, "%i Meters", ((int)pawnsToLoop[i].playerPawn.Distance / 100));
                overlay::drawText(pawnsToLoop[i].playerPawn.W2S, boxDimensions.y + 15.f, distText);
            }
            if (playerIDESP) {


                overlay::drawText(pawnsToLoop[i].playerPawn.W2S, -boxDimensions.y - 10.f, std::to_string(pawnsToLoop[i].playerPawn.playerID).c_str());
            }



        }

    }

    if (drawTPS) {

        char TPSBuffer[50];
        sprintf_s(TPSBuffer, "TPS: %i", TPS);
        overlay::drawText(Vector3(winProperties.x + 40.f, winProperties.y + 20.f, 0), 0.f, TPSBuffer);

        char TotalBuf[50];
        sprintf_s(TotalBuf, "Average TPS %i", totalTPS / totalTime);
        overlay::drawText(Vector3(winProperties.x + 65.f, winProperties.y + 35.f, 0), 0.f, TotalBuf);

    }

   


}

















