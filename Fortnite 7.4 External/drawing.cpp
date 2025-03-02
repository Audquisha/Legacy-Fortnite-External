#include "Overlay.h"

bool boxESP;
bool bRounded;
float rounding;

bool lineESP;
bool pieceESP;
bool playerNameESP;
bool skeletonESP;
bool distanceESP;
bool playerIDESP;
bool localESP;	
bool bDrawFov;
bool bAimbot;

int aimKey = 0;
char keyName[128] = "Aim Key";
static const char* aimModes[]{ "CrossHair", "Distance" };
static const char* aimBones[]{ "Head", "Chest",	"Pelvis"};
int  aimPriority = 0;
int aimBone = 0;
float smoothX = 1.f;
float smoothY = 1.f;

bool bulletTP;
bool drawTPS;

int syncInterval;

namespace itemESP {

	 bool enabled;
	 float maxDistance;
	 bool consumable;
	 bool traps;
	 bool ammo;
	 bool materials;
	 bool chests;
	 bool ammoBox;

	namespace weaponESP {

		 bool enabled;
		 bool AR;
		 bool SG;
		 bool SMG;
		 bool Sniper;
		 bool Pistol;
	}


}

ImFont* Font;
std::vector<AimSettings> aimSettings(6);

VisualTab visualProfile = playerESP;
AimTab currProfile = AR;

void overlay::menuDrawing()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImFont* Font = ImGui::GetFont();
	
	ImVec2 menuDimensions = ImVec2(600, 400);

	ImGui::SetNextWindowSize(menuDimensions);


	ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoDecoration);
	{

		ImGui::SetCursorPos({ 15.f, 15.f });

		ImGui::BeginChild("##VisualSettings", ImVec2(4* menuDimensions.x / 9, menuDimensions.y -30.f), true, ImGuiWindowFlags_NoDecoration);
		{

			float yPos = ImGui::GetCursorPosY() + 10.f;

			float xSpace = (4 * menuDimensions.x / 9) - 32.f;
			float xSize = (xSpace / 9)*4;
			float ySize = xSize / 2;
			float xSpacer = xSize /4;
			ImGui::SetCursorPosX(12.f);

			float xPos = ImGui::GetCursorPosX();

			ImGui::SetCursorPos({ xPos ,yPos });

			if (visualProfile == playerESP) {

				ImGui::PushStyleColor(ImGuiCol_Button, { .43f, .15f, 1.f, 1.f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { .43f, .7f, 1.f, 1.f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, { .2f, 0.f, 1.f, 1.f });
				ImGui::ImageButton(playerESPIconImg, { xSize,ySize });
				ImGui::PopStyleColor(3);
			}
			else {

				if (ImGui::ImageButton(playerESPIconImg, { xSize,ySize })) {

					visualProfile = playerESP;
				}

			}

			xPos += xSpacer + xSize;
			ImGui::SetCursorPos({ xPos ,yPos });

			if (visualProfile == worldESP) {

				ImGui::PushStyleColor(ImGuiCol_Button, { .43f, .15f, 1.f, 1.f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { .43f, .7f, 1.f, 1.f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, { .2f, 0.f, 1.f, 1.f });
				ImGui::ImageButton(itemESPIconImg, { xSize,ySize });
				ImGui::PopStyleColor(3);
			}
			else {

				if (ImGui::ImageButton(itemESPIconImg, { xSize,ySize })) {

					visualProfile = worldESP;
				}

			}
		
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.f);

			if (visualProfile == playerESP) {

				ImGui::Checkbox("Box", &boxESP);

				if (boxESP) {

					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20.f);
					ImGui::Checkbox("Rounded", &bRounded);
					if (bRounded) {
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20.f);

						ImGui::SliderFloat("Box Rounding", &rounding, 0.f, 15.f);
					}


				}


				ImGui::Checkbox("Line", &lineESP);

				ImGui::Checkbox("Skeleton", &skeletonESP);

				ImGui::Checkbox("Player ID", &playerIDESP);
				ImGui::Checkbox("Distance", &distanceESP);

				ImGui::Checkbox("Draw Aimbot Fov", &bDrawFov);

				ImGui::Checkbox("Draw Local Player", &localESP);
			}

			if (visualProfile == worldESP) {

				ImGui::Checkbox("Items", &itemESP::enabled);

				if (itemESP::enabled) {


					ImGui::SliderFloat("Max Distance", &itemESP::maxDistance, 0.f, 150.f);

					ImGui::Checkbox("Chests", &itemESP::chests);
					ImGui::Checkbox("Ammo Box", &itemESP::ammoBox);


					ImGui::Checkbox("Ammo", &itemESP::ammo);
					ImGui::Checkbox("Consumable", &itemESP::consumable);
					ImGui::Checkbox("Materials", &itemESP::materials);
					ImGui::Checkbox("Traps", &itemESP::traps);


					ImGui::Checkbox("Weapons", &itemESP::weaponESP::enabled);
					if (itemESP::weaponESP::enabled) {

						ImGui::Checkbox("Assault Rifles", &itemESP::weaponESP::AR);
						ImGui::Checkbox("Shotguns", &itemESP::weaponESP::SG);
						ImGui::Checkbox("SMGs", &itemESP::weaponESP::SMG);
						ImGui::Checkbox("Snipers", &itemESP::weaponESP::Sniper);
						ImGui::Checkbox("Pistols", &itemESP::weaponESP::Pistol);




					}
				}
			}

			ImGui::Checkbox("Draw TPS", &drawTPS);
			ImGui::SliderInt("Sync Interval", &syncInterval, 0, 4);



		}
		ImGui::EndChild();

		ImGui::SetCursorPos({( 5 * menuDimensions.x / 9)-15, 15.f });

		ImGui::BeginChild("##AimSettings", ImVec2((4 * menuDimensions.x / 9), menuDimensions.y-30.f), true, ImGuiWindowFlags_NoDecoration);
		{


			float yPos = ImGui::GetCursorPosY() + 10.f;

			float xSpace = (4 * menuDimensions.x / 9) - 32.f;
			float xSize = (xSpace / 6);
			float xSpacer = xSize / 4;
			ImGui::SetCursorPosX(12.f);

			float xPos = ImGui::GetCursorPosX();

			ImGui::SetCursorPos({ xPos ,yPos });

			if (currProfile == AR) {

				ImGui::PushStyleColor(ImGuiCol_Button, { .43f, .15f, 1.f, 1.f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { .43f, .7f, 1.f, 1.f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, { .2f, 0.f, 1.f, 1.f });
				ImGui::ImageButton(arIconImg, { xSize,xSize });
				ImGui::PopStyleColor(3);
			}
			else {

				if (ImGui::ImageButton(arIconImg, { xSize,xSize })) {

					currProfile = AR;
				}

			}

			xPos += xSpacer + xSize;
			ImGui::SetCursorPos({ xPos ,yPos });

			if (currProfile == SG) {

				ImGui::PushStyleColor(ImGuiCol_Button, { .43f, .15f, 1.f, 1.f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { .43f, .7f, 1.f, 1.f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, { .2f, 0.f, 1.f, 1.f });
				ImGui::ImageButton(sgIconImg, { xSize,xSize });
				ImGui::PopStyleColor(3);


			}
			else {

				if (ImGui::ImageButton(sgIconImg, { xSize,xSize })) {

					currProfile = SG;
				};

			}
			xPos += xSpacer + xSize;
			ImGui::SetCursorPos({ xPos ,yPos });

			if (currProfile == SMG) {

				ImGui::PushStyleColor(ImGuiCol_Button, { .43f, .15f, 1.f, 1.f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { .43f, .7f, 1.f, 1.f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, { .2f, 0.f, 1.f, 1.f });
				ImGui::ImageButton(smgIconImg, { xSize,xSize });
				ImGui::PopStyleColor(3);


			}
			else {

				if (ImGui::ImageButton(smgIconImg, { xSize,xSize })) {
					currProfile = SMG;
				}

			}
			xPos += xSpacer + xSize;
			ImGui::SetCursorPos({ xPos ,yPos });

			if (currProfile == SNIPER) {

				ImGui::PushStyleColor(ImGuiCol_Button, { .43f, .15f, 1.f, 1.f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { .43f, .7f, 1.f, 1.f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, { .2f, 0.f, 1.f, 1.f });
				ImGui::ImageButton(sniperIconImg, { xSize,xSize });
				ImGui::PopStyleColor(3);


			}
			else {

				if (ImGui::ImageButton(sniperIconImg, { xSize,xSize })) {
					currProfile = SNIPER;
				}

			}
			xPos += xSpacer + xSize;
			ImGui::SetCursorPos({ xPos ,yPos });

			if (currProfile == PISTOL) {

				ImGui::PushStyleColor(ImGuiCol_Button, { .43f, .15f, 1.f, 1.f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { .43f, .7f, 1.f, 1.f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, { .2f, 0.f, 1.f, 1.f });
				ImGui::ImageButton(pistolIconImg, { xSize,xSize });
				ImGui::PopStyleColor(3);


			}
			else {

				if (ImGui::ImageButton(pistolIconImg, { xSize,xSize })) {
					currProfile = PISTOL;
				}

			}

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.f);
			ImGui::Checkbox("Aimbot", &aimSettings[currProfile].bAimBot);

			if (aimSettings[currProfile].bAimBot) {

				ImGui::SameLine(0, 20.f);
				if (ImGui::Button(aimSettings[currProfile].keyName, { 80, 20 })) {

					aimSettings[currProfile].aimKey = util::setAimKey();
					SecureZeroMemory(aimSettings[currProfile].keyName, sizeof(aimSettings[currProfile].keyName));
					GetKeyNameTextA(MapVirtualKey(aimSettings[currProfile].aimKey, 0) << 16, aimSettings[currProfile].keyName, sizeof(aimSettings[currProfile].keyName));

				}
				
			
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20.f);

				ImGui::Checkbox("Use Aimbot Fov", &aimSettings[currProfile].bUseFov);

				if (aimSettings[currProfile].bUseFov) {

					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20.f);

					ImGui::SliderFloat("Aimbot Fov", &aimSettings[currProfile].fov, 0.f, 360.f);

				}

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20.f);

				ImGui::Combo("Aim Priority", &aimSettings[currProfile].aimPriority, aimModes, IM_ARRAYSIZE(aimModes));

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20.f);

				ImGui::Combo("Aim Bone", &aimSettings[currProfile].aimBone, aimBones, IM_ARRAYSIZE(aimBones));

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20.f);

				ImGui::SliderFloat("Smooth X", &aimSettings[currProfile].smoothX, 1.f, 30.f);

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20.f);

				ImGui::SliderFloat("Smooth Y", &aimSettings[currProfile].smoothY, 1.f, 30.f);

				

			}

			ImGui::Checkbox("Bullet TP", &bulletTP);


		}
		ImGui::EndChild();
		

		




	

		

		//ImGui::Image((void*)vtLogoImg, { 348, 61 });




	}
	ImGui::End();
}


void overlay::drawText(Vector3 World2Screen, float yOffset, const char* text)
{
	Font = ImGui::GetFont();

	ImVec2 textSize = ImGui::CalcTextSize(text);

	Font->Scale = 1.5f;

	ImGui::PushFont(Font);

	ImGui::GetBackgroundDrawList()->AddText({ (World2Screen.x - (textSize.x / 2) * (Font->Scale)) - 1.f, World2Screen.y + yOffset - textSize.y }, ImColor{ 0.f, 0.f, 0.f, 1.f }, text);
	ImGui::GetBackgroundDrawList()->AddText({ (World2Screen.x - (textSize.x / 2) * (Font->Scale)) + 1.f, World2Screen.y + yOffset - textSize.y }, ImColor{ 0.f, 0.f, 0.f, 1.f }, text);
	ImGui::GetBackgroundDrawList()->AddText({ (World2Screen.x - (textSize.x / 2) * (Font->Scale)) - 1.f, (World2Screen.y + yOffset - textSize.y) - 1.f }, ImColor{ 0.f, 0.f, 0.f, 1.f }, text);
	ImGui::GetBackgroundDrawList()->AddText({ (World2Screen.x - (textSize.x / 2) * (Font->Scale)) + 1.f, (World2Screen.y + yOffset - textSize.y) + 1.f }, ImColor{ 0.f, 0.f, 0.f, 1.f }, text);
	ImGui::GetBackgroundDrawList()->AddText({ World2Screen.x - (textSize.x / 2) * (Font->Scale) , World2Screen.y + yOffset - textSize.y }, ImColor{ 1.f, 1.f, 1.f, 1.f }, text);

	Font->Scale = 1.f;

	ImGui::PopFont();



}
void overlay::drawText(Vector3 World2Screen, float yOffset, ImColor color, const char* text)
{
	Font = ImGui::GetFont();

	ImVec2 textSize = ImGui::CalcTextSize(text);

	Font->Scale = 1.f;

	ImGui::PushFont(Font);

	ImGui::GetBackgroundDrawList()->AddText({ (World2Screen.x - (textSize.x / 2) * (Font->Scale)) - 1.f, World2Screen.y + yOffset - textSize.y }, ImColor{ 0.f, 0.f, 0.f, 1.f }, text);
	ImGui::GetBackgroundDrawList()->AddText({ (World2Screen.x - (textSize.x / 2) * (Font->Scale)) + 1.f, World2Screen.y + yOffset - textSize.y }, ImColor{ 0.f, 0.f, 0.f, 1.f }, text);
	ImGui::GetBackgroundDrawList()->AddText({ (World2Screen.x - (textSize.x / 2) * (Font->Scale)) - 1.f, (World2Screen.y + yOffset - textSize.y) - 1.f }, ImColor{ 0.f, 0.f, 0.f, 1.f }, text);
	ImGui::GetBackgroundDrawList()->AddText({ (World2Screen.x - (textSize.x / 2) * (Font->Scale)) + 1.f, (World2Screen.y + yOffset - textSize.y) + 1.f }, ImColor{ 0.f, 0.f, 0.f, 1.f }, text);
	ImGui::GetBackgroundDrawList()->AddText({ World2Screen.x - (textSize.x / 2) * (Font->Scale) , World2Screen.y + yOffset - textSize.y }, color, text);

	Font->Scale = 1.f;

	ImGui::PopFont();



}


void overlay::drawBox(Vector3 World2Screen) 
{


	
		if (World2Screen.x <= 4000.f && World2Screen.x >= -2000.f) {

			if (World2Screen.y <= 4000.f && World2Screen.y >= -2000.f) {

				ImGui::GetBackgroundDrawList()->AddRect({ World2Screen.x - boxDimensions.x - 1.75f , World2Screen.y - boxDimensions.y - 1.75f }, { World2Screen.x + boxDimensions.x + 1.75f, World2Screen.y + boxDimensions.y + 1.75f }, ImColor(0.f, 0.f, 0.f, 1.f), 0.f, 0, 1.5f);
				ImGui::GetBackgroundDrawList()->AddRect({ World2Screen.x - boxDimensions.x , World2Screen.y - boxDimensions.y }, { World2Screen.x + boxDimensions.x , World2Screen.y + boxDimensions.y }, ImColor(1.f, 0.f, 0.f), 0.f, 0, 2.f);
				ImGui::GetBackgroundDrawList()->AddRect({ World2Screen.x - boxDimensions.x + 1.75f , World2Screen.y - boxDimensions.y + 1.75f }, { World2Screen.x + boxDimensions.x - 1.75f, World2Screen.y + boxDimensions.y - 1.75f }, ImColor(0.f, 0.f, 0.f, 1.f), 0.f, 0, 1.5f);

			}


		}
	
}
void overlay::drawBox(Vector3 World2Screen, float rounding)
{



	if (World2Screen.x <= 4000.f && World2Screen.x >= -2000.f) {

		if (World2Screen.y <= 4000.f && World2Screen.y >= -2000.f) {

			ImGui::GetBackgroundDrawList()->AddRect({ World2Screen.x - boxDimensions.x - 1.75f , World2Screen.y - boxDimensions.y - 1.75f }, { World2Screen.x + boxDimensions.x + 1.75f, World2Screen.y + boxDimensions.y + 1.75f }, ImColor(0.f, 0.f, 0.f, 1.f), rounding, 0, 1.5f);
			ImGui::GetBackgroundDrawList()->AddRect({ World2Screen.x - boxDimensions.x , World2Screen.y - boxDimensions.y }, { World2Screen.x + boxDimensions.x , World2Screen.y + boxDimensions.y }, ImColor(1.f, 0.f, 0.f), rounding, 0, 2.f);
			ImGui::GetBackgroundDrawList()->AddRect({ World2Screen.x - boxDimensions.x + 1.75f , World2Screen.y - boxDimensions.y + 1.75f }, { World2Screen.x + boxDimensions.x - 1.75f, World2Screen.y + boxDimensions.y - 1.75f }, ImColor(0.f, 0.f, 0.f, 1.f), rounding, 0, 1.5f);

		}


	}

}


void overlay::drawLine(Vector3 World2Screen) 
{
	ImGui::GetBackgroundDrawList()->AddLine({ winProperties.width / 2 + winProperties.x, winProperties.height / 2  + winProperties.y}, { World2Screen.x, World2Screen.y }, ImColor{ 1.f, 0.f, 0.f }, 2.f);
}

void overlay::drawLine(Vector3 World2Screen, Vector3 origin)
{
	ImGui::GetBackgroundDrawList()->AddLine({ origin.x, origin.y }, { World2Screen.x, World2Screen.y }, ImColor{ 1.f, 0.f, 0.f }, 2.f);
}

void overlay::drawHealth(Vector3 World2Screen, float health)
{
	if (health > 0.f && health <= 100.f) {

		ImColor color;

		if (health == 100.f) {

			color = ImColor(0.f, 1.f, 0.f, 1.f);

		}
		else if (health >= 75.f) {

			color = ImColor(1.f, 1.f, 0.f, 1.f);

		}
		else if (health >= 50.f) {

			color = ImColor(1.f, 0.5f, 0.f, 1.f);

		}
		else if (health >= 25.f) {

			color = ImColor(1.f, 0.f, 0.f, 1.f);

		}
		else if (health > 0.f) {

			color = ImColor(.5f, 0.2f, 0.f, 1.f);

		}

		ImGui::GetBackgroundDrawList()->AddLine({ World2Screen.x - boxDimensions.x - 5.f, (World2Screen.y + boxDimensions.y) - (2 * boxDimensions.y * (health / 100.f))}, { World2Screen.x - boxDimensions.x - 5.f, World2Screen.y + boxDimensions.y  }, ImColor(0.f, 0.f, 0.f, 1.f), 5.f);

		ImGui::GetBackgroundDrawList()->AddLine({ World2Screen.x - boxDimensions.x - 5.f, (World2Screen.y + boxDimensions.y) - (2 * boxDimensions.y * (health / 100.f)) +1.f }, { World2Screen.x - boxDimensions.x - 5.f, World2Screen.y + boxDimensions.y -1.f }, color, 3.f);
	}
}





