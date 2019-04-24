#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hechanova_project.h"

int main(void)
{
	Flight *flights = NULL;
	Passenger *passengers = NULL;
	int flight_count = 0, passenger_count = 0;
	while(1)
	{
		if(switchboard(&flights, &passengers, &flight_count, &passenger_count))
		{
			break;
		}
	}

	// save(flights, passengers, flight_count, passenger_count);
	// deallocate(&flights, &passengers);

	printf("\nGoodbye!...\n\n");

	return 0;
}

/*
	more structured user input methods
	validate date and time
	check if same date departure and arrival date
	if same, no need to validate departure and arrival time
	validate departure and arrival between two flights
		if same day, compare time
			departure must come after arrival
			or arrival must come before departure
			(before or after)
		else, compare date
			departure must come after arrival
			or arrival must come before departure
			(before or after pa rin)
	use flight IDs and passport numbers as keys
	how to properly reference flights from passenger??
	spam user prompts? maybe later

*/

