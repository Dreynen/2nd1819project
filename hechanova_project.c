#include "hechanova_project.h"

int main(void)
{
	RunApplication();

	return 0;
}

/*
	more structured user input methods
	validate date and time
	if flight has same departure and arrival date, validate times.
	validate departure and arrival between two flights
		if same day, validate time
			departure time after arrival time or arrival time before departure time
		else, compare date
			departure date after arrival date or arrival date before departure date
	how to keep things sorted even after editing????
	use flight IDs and passport numbers as keys
	how to properly cross-reference flights and passengers??
	spam user prompts? maybe later

*/