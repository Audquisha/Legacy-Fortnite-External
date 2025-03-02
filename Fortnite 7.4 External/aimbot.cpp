#include "aimbot.h"

bool bAimActive;

void aimbot::aimbot(int vKey, float smoothX, float smoothY, Vector3 BoneAimLoc) {


  


    if (GetAsyncKeyState(vKey)) {

        bAimActive = true;

        float currValY = viewInfo.Rotation.y;
        float currValX = viewInfo.Rotation.x;
        float atanYX;
        float asinXY;





        atanYX = atan2(BoneAimLoc.y - viewInfo.Location.y, BoneAimLoc.x - viewInfo.Location.x);
        asinXY = asin((BoneAimLoc.z - viewInfo.Location.z) / util::getDistance(viewInfo.Location, BoneAimLoc));




        float yChange = rad2Deg(atanYX);
        float xChange = rad2Deg(asinXY);

     

        float bufferY = yChange - currValY;

        float bufferX = xChange - currValX;


       

        if ((bufferY) >= 180.f) {

            bufferY =  abs(bufferY)-360.f;

        }
        else if (bufferY <= -180.f) {

            bufferY =  360.f - abs(bufferY);

        }

        if (abs(bufferX) >= 250) {

            bufferX = 360 - abs(bufferX);
        }


        //RotationInput
        util::Write<float>(localPlayerController + 0x538, bufferX / smoothX);
        util::Write<float>(localPlayerController + 0x53C, bufferY / smoothY);
    }
    else {

        bAimActive = false;
    }
}

AFortPlayerState aimbot::getAimTarget() {

    float distBuffer = 999999999.f;
    int currTarget = -1;


    for (int i = 0; i < enemyPawns.size(); i++) {

        float distFromCrossHair = util::get2DDistance(enemyPawns[i].playerPawn.getW2S(), Vector3(winProperties.width / 2 + winProperties.x, winProperties.height / 2 + winProperties.y, 0));


        if (!enemyPawns[i].playerPawn.skeleton[0][0].isZero()) {

            if (currAimSetting.bUseFov) {

                if (distFromCrossHair > currAimSetting.fovRadius) {

                    continue;
                }

            }


            if (aimPriority == 0)
            {

                //Distance from screen center

                if (distFromCrossHair < distBuffer) {

                    distBuffer = distFromCrossHair;
                    currTarget = i;

                }


            }
            else if (aimPriority == 1)
            {

                //3D distance in world

                if (enemyPawns[i].playerPawn.Distance < distBuffer) {

                    distBuffer = enemyPawns[i].playerPawn.Distance;
                    currTarget = i;

                }
            }
        }

    }

        if (currTarget < enemyPawns.size() && currTarget >= 0) {
            return enemyPawns[currTarget];
        }
        return AFortPlayerState(NULL);
    

}

