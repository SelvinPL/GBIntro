#include <gb/gb.h>
#include <stdio.h>
#include "console.h"

const char long_time_ago[] = "\n\n\n\n\n\n\n"
"\a\a\a"
"   A  long time ago,\n\a"
"in  a   galaxy  far,\n\a"
"far away...\a\a\a\n"
"   It is a period of\n\a"
"civil   war.   Rebel\n\a"
"spaceships, striking\n\a"
"from a hidden  base,\n\a"
"have won their first\n\a"
"victory  against the\n\a"
"evil Galactic Empire\n"
"...\a\a\a"
"\n\n\n\n\n\n\n";

void main()
{
	printf("\n\n\n    PRESS START\n\n\n\n~~~~~~~~~~~~~~~~~~~~\n\n\n\n\n\n\n~~~~~~~~~~~~~~~~~~~~");
	consoleSetup(long_time_ago);
	while (TRUE)
	{
		wait_vbl_done();
		consoleLoop();
	}
}