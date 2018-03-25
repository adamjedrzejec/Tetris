#include "primlib.h"
#include <stdlib.h>

int main(int argc, char* argv[])
{
	if(initGraph())
	{
		exit(3);
	}
	/* clear screen */
	filledRect(0, 0, screenWidth() - 1, screenHeight() - 1, BLACK);
	filledCircle(screenWidth() / 2, screenHeight() / 2, 100, YELLOW);
	updateScreen();
	getkey();
	return 0;
}
