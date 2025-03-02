#include "unrealClasses.h"





	UObject::UObject(uintptr_t baseAddy)
	{

		this->baseAddress = baseAddy;
		this->FNameID = util::Read<int>(this->baseAddress + 0x18, sizeof(FNameID));
		this->Name = GetNameFromFName(this->FNameID);

	}
	UObject::UObject() {

	}

	bool UObject::isValid() {

		int ObjFlags = util::Read<int>(this->baseAddress + 0x8, sizeof(ObjFlags));
		if (ObjFlags & PendingKill) {

			return false;
		}
		else {
			return true;
		}
	}








	AActor::AActor(uintptr_t baseAddy) : UObject(baseAddy) {


		this->RootComponent.RelativeLocation = util::Read<Vector3>(util::Read<uintptr_t>(this->baseAddress + offsets::AActor::RootComponent, sizeof(uintptr_t)) + offsets::USceneComponent::RelativeLocation, sizeof(this->RootComponent.RelativeLocation));



	}

	AActor::AActor() : UObject(0){}
	 Vector3 AActor::getPos() {
		this->RootComponent.RelativeLocation = util::Read<Vector3>(util::Read<uintptr_t>(this->baseAddress + offsets::AActor::RootComponent, sizeof(uintptr_t)) + offsets::USceneComponent::RelativeLocation, sizeof(this->RootComponent.RelativeLocation));
		return this->RootComponent.RelativeLocation;

	}
	float AActor::getDistance() {

		this->Distance = util::getDistance(viewInfo.Location, this->RootComponent.RelativeLocation);
		return this->Distance;


	}
	Vector3 AActor::getW2S() {

		this->W2S = util::WorldToScreen(this->RootComponent.RelativeLocation, viewInfo);
		return this->W2S;

	}




	AFortPawn::AFortPawn(uintptr_t baseAddy) : AActor(baseAddy) {

		this->playerID = util::Read<int>(this->baseAddress + 0x818, sizeof(this->playerID));
		//meshnAddress
		this->meshAddress = util::Read<uintptr_t>(this->baseAddress + 0x390, sizeof(this->meshAddress));
		//Bone Array Addr
		this->boneArrayAddr = util::Read<uintptr_t>(this->meshAddress + 0x5c0, sizeof(this->boneArrayAddr));
		//Bone Array Count
		this->boneArrayCount = util::Read<INT32>(this->meshAddress + 0x5c8, sizeof(this->boneArrayCount));

		std::vector<BoneInfo> upper = { BoneInfo(0,0,0,64),		BoneInfo(0,0,0,65),		BoneInfo(0,0,0, 66) };
		std::vector<BoneInfo> right = { BoneInfo(0,0,0,64),		BoneInfo(0,0,0,91),		BoneInfo(0,0,0, 38), BoneInfo(0,0,0, 41) };

		std::vector<BoneInfo> left = { BoneInfo(0,0,0,64),		BoneInfo(0,0,0,9),		BoneInfo(0,0,0, 10), BoneInfo(0,0,0, 25) };
		std::vector<BoneInfo> spineBones = { BoneInfo(0,0,0,64),		BoneInfo(0,0,0,7),		BoneInfo(0,0,0, 4), BoneInfo(0,0,0, 2) };
		std::vector<BoneInfo> lowerright = { BoneInfo(0,0,0,2),		BoneInfo(0,0,0,80),		BoneInfo(0,0,0, 75), BoneInfo(0,0,0, 83) };
		std::vector<BoneInfo> lowerleft = { BoneInfo(0,0,0,2),		BoneInfo(0,0,0,73),		BoneInfo(0,0,0, 68), BoneInfo(0,0,0, 82) };

		this->skeleton = std::vector<std::vector<BoneInfo>>{ upper, right, left, spineBones, lowerright, lowerleft };

		

	}
	AFortPawn::AFortPawn() : AActor(0){}

	bool AFortPawn::isDead() {

		int deathFlags = util::Read<int>(this->baseAddress + 0x7c8, sizeof(deathFlags));

		if (deathFlags & 1 << 2) {

			this->isHiddenForDeath = true;

		}
		else {
			this->isHiddenForDeath = false;
		}

		return this->isHiddenForDeath;
	}
	FTransform AFortPawn::GetBoneIndex(int index)
	{
		return util::Read<FTransform>(this->boneArrayAddr + (index * 0x30), sizeof(FTransform));
	}

	BoneInfo AFortPawn::GetBoneWithRotation(BoneInfo toRefresh)
	{
		FTransform bone = GetBoneIndex(toRefresh.boneID);

		if (!bone.scale.x) {

			return toRefresh;
		}



		//CompToWorld
		FTransform ComponentToWorld = util::Read<FTransform>(this->meshAddress + 0x190, sizeof(FTransform));

		D3DMATRIX Matrix;
		Matrix = util::MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());
		return BoneInfo(Matrix._41, Matrix._42, Matrix._43, toRefresh.boneID);
	}

	void AFortPawn::refreshSkeleton() {


		for (int i = 0; i < this->skeleton.size(); i++) {


			for (int j = 0; j < this->skeleton[i].size(); j++) {

				BoneInfo temp = GetBoneWithRotation(this->skeleton[i][j]);
				if (temp.isZero() && !this->skeleton[i][j].isZero()) {

					if (!this->skeleton[i][j].isZero()) {

						this->skeleton[i][j].W2S = util::WorldToScreen(Vector3(this->skeleton[i][j].x, this->skeleton[i][j].y, this->skeleton[i][j].z), viewInfo);

					}
					

				}
				else {
					this->skeleton[i][j] = temp;
					this->skeleton[i][j].W2S = util::WorldToScreen(Vector3(this->skeleton[i][j].x, this->skeleton[i][j].y, this->skeleton[i][j].z), viewInfo);
				}
			}

		}

	}

	UObject AFortPawn::getCurrentWeapon() {


		this->currentWeaponPtr = util::Read<uintptr_t>(this->baseAddress + 0x820, sizeof(this->currentWeaponPtr));

		return UObject(this->currentWeaponPtr);

	}

	Vector3 AFortPawn::getPos()  {

		this->location = Vector3(this->skeleton[4][0].x, this->skeleton[4][0].y, this->skeleton[4][0].z);
		return this->location;

	}

	float AFortPawn::getDistance()  {

		this->Distance = util::getDistance(viewInfo.Location, Vector3(this->skeleton[0][0].x, this->skeleton[0][0].y, this->skeleton[0][0].z));
		return this->Distance;


	}
	Vector3 AFortPawn::getW2S()  {

		this->W2S = this->skeleton[4][0].W2S;
		return this->W2S;


	}







	AFortPlayerState::AFortPlayerState(uintptr_t baseAddy) : UObject(baseAddy) {

		this->pawnPtr = util::Read<uintptr_t>(this->baseAddress + 0x3B8, sizeof(this->pawnPtr));
		this->playerPawn = AFortPawn(pawnPtr);

	}

	AFortPlayerState::AFortPlayerState() : UObject(0) {}




	FortItem::FortItem(uintptr_t baseAddy, Vector3 worldPos) : AActor(baseAddy) {

		this->RootComponent.RelativeLocation = worldPos;

		uintptr_t itemNameCont = util::Read<uintptr_t>(this->baseAddress + 0x80, sizeof(itemNameCont));

		uintptr_t FTEXTPTR = util::Read<uintptr_t>(itemNameCont + 0x28, sizeof(FTEXTPTR));

		INT32 ftextSz = util::Read<INT32>(itemNameCont + 0x30, sizeof(ftextSz));

		wchar_t* buffer = new wchar_t[ftextSz * 2];
		SecureZeroMemory(buffer, ftextSz * 2);

		ReadProcessMemory(hProc, (BYTE*)(FTEXTPTR), buffer, ftextSz * 2, NULL);

		std::wstring ws(buffer);
		std::string str(ws.begin(), ws.end());

		delete[] buffer;

		this->itemName = str;

		this->itemRarity = util::Read<INT8>(this->baseAddress + 0x50, sizeof(this->itemRarity));

		this->drawColor = getColorByRarity(this->itemRarity);

		this->itemType = (fortItemType)util::Read<INT8>(this->baseAddress + 0x51, sizeof(this->itemType));



		if (std::strstr(this->itemName.c_str(), "Assault") != nullptr) {

			this->weaponType = WEAPON_AR;

		}
		else if (std::strstr(this->itemName.c_str(), "Shotgun") != nullptr) {

			this->weaponType = WEAPON_SG;

		}
		else if (std::strstr(this->itemName.c_str(), "Submachine") != nullptr || std::strstr(this->itemName.c_str(), "Minigun") != nullptr) {

			this->weaponType = WEAPON_SMG;

		}
		else if (std::strstr(this->itemName.c_str(), "Sniper") != nullptr || std::strstr(this->itemName.c_str(), "Hunting") != nullptr || std::strstr(this->itemName.c_str(), "Infantry") != nullptr) {

			this->weaponType = WEAPON_SNIPER;

		}
		else if (std::strstr(this->itemName.c_str(), "Pistol") != nullptr || std::strstr(this->itemName.c_str(), "Hand Cannon") != nullptr) {

			this->weaponType = WEAPON_PISTOL;

		}

		else {
			this->weaponType = WEAPON_NA;
		}


	}

	FortItem::FortItem(uintptr_t baseAddy, fortItemType itemType) : AActor(baseAddy) {


		this->itemType = itemType;

		if (itemType == World_AmmoBox) {

			this->drawColor = getColorByRarity(6);

			this->itemName = "Ammo Box";


		}
		else if (itemType == World_Chest) {

			this->drawColor = getColorByRarity(5);

			this->itemName = "Chest";


		}


		int searchFlags = util::Read<int>(this->baseAddress + 0xcf1, sizeof(searchFlags));

		if (searchFlags & 1 << 3) {

			this->isSearched = true;

		}
		else {
			this->isSearched = false;
		}


	}

	FortItem::FortItem() : AActor(0) {}





