#include "precomp.h"




INT APIENTRY WinMain(HINSTANCE instance, HINSTANCE, PSTR, int cmd_show) {


	if (util::init())
	{
		overlay::Render();
		
			
		
	}
	return 0;

}