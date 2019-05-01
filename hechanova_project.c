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

