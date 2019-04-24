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
	if same day, validate time
	else check both before and after; must be valid for only before OR after // XOR ganon?
	spam user prompts? maybe latur
	use flight IDs and passport numbers as keys
	study doubly linked lists
*/

