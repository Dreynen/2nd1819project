#include "projectlib.h"

void RunApplication()
//	initialization of variables and call to switch function
{
	Flight *flights = NULL;
	Passenger *passengers = NULL;
	int flight_count = 0, passenger_count = 0;

	do
	{
		menu_out();
	}
	while(switchboard(&flights, &passengers, &flight_count, &passenger_count));

	// save(flights);
	// save(passengers);
	// deallocate(flights);
	// deallocate(passengers);
}

void menu_out()
{
	printf("[1] Add A Flight\n");
	printf("[2] Edit A Flight\n");
	printf("[3] View Flights\n");
	printf("[4] Delete A Flight\n");
	printf("[5] Add A Passenger\n");
	printf("[6] Edit Passenger Details\n");
	printf("[7] Book A Flight Reservation\n");
	printf("[8] Remove a Flight Reservation\n");
	printf("[9] View Reservations\n");
	printf("[10] Exit\n");
}

int switchboard(Flight **flights, Passenger **passengers, int *flight_count, int *passenger_count)
//	switch for selection of menu option
{
	int choice = get_int("Choice: ");

	switch(choice)
	{
		case 1:
			add_flight(flights, flight_count);
			break;
		case 10:
			break;
		default:
			printf("\n1 - 10 only.\n\n");
	}

	return 10 - choice;
}

void add_flight(Flight **flights, int *flight_count)
{
	printf("\nADD A FLIGHT\n");
	int flight_ID = get_int("Enter Flight ID: ");
	if(!found_flight(*flights, flight_ID))
	{
		Flight *new = malloc(sizeof(Flight));
		new -> flight_ID = flight_ID;
		get_locations(new, "Enter destination: ", "Enter origin: ");
		get_dates(new, "Enter departure date: ", "Enter arrival date: ");
		get_times(new, "Enter departure time: ", "Enter arrival time: ");
		new -> passenger_count = 0;
		new -> max_passengers = get_int("Enter maximum number of passengers: ");
		new -> bonus_miles = get_int("Enter bonus miles: ");
		new -> first = NULL;
		new -> next = NULL;

		add_flight_node(flights, new);

		printf("\nFlight added successfully.\n\n");
		(*flight_count)++;
	}
	else
	{
		printf("\nFlight with same flight ID already exists.\n\n");
	}
}

void add_flight_node(Flight **flights, Flight *new)
{
	if(!(*flights) || (check_dates((*flights) -> departure_date, new -> departure_date) == -1 && check_times((*flights) -> departure_time, new -> departure_time)) || check_dates((*flights) -> departure_date, new -> departure_date))
	//	if flight list is NULL or new node comes before head node
	{
		new -> next = *flights;
		*flights = new;
	}
	else
	{
		Flight *temp = *flights;
		while(check_dates(new -> departure_date, temp -> departure_date))
		//	traverse list and compare dates and times; loop terminates if new should be before temp
		{
			if(check_dates(new -> departure_date, temp -> departure_date) == -1 && check_times(new -> departure_time, temp -> departure_time))
			//	if same departure dates, compare times
			{
				temp = temp -> next;
			}
		}

		if(temp)
		//	if temp is not NULL, place new node before temp
		{
			for(Flight *prev = *flights; prev; prev = prev -> next)
			{
				if(prev -> next == temp)
				{
					prev -> next = new;
					new -> next = temp;
				}
			}
		}
		else
		//	place node at end of list
		{
			temp -> next = new;
		}
	}
}
