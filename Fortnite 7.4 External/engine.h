#pragma once
#include "precomp.h"







bool updateWorld();

std::string GetNameFromFName(int key);



bool updateLocalPlayer();




FMinimalViewInfo getViewInfo();



AimTab currWeaponType();

bool refreshPawns();


void drawSkeleton(AFortPlayerState player);


void drawBoneIDs(AFortPlayerState player);


AFortPlayerState findPlayerByID(int32_t ID);
ImColor getColorByRarity(INT8 rarity);


void drawDebugAActors();


void cacheLevel();



void drawWorldItems();


void projectileTP();


void mainLoop();

















