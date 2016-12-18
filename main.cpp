#include "Display.h"

int main(int argc, char** argv) {

	Display display = Display(1024, 1024, "Manhattan");

	display.Run();

	return 0;
}