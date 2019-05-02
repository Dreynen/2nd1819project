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
		case 2:
			edit_flight(flights);
			break;
		case 3:
			view(*flights);
			break;
		case 4:
			del_flight(flights, flight_count);
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
		new -> ID = flight_ID;
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
		while(temp)
		{
			if((check_dates(new -> departure_date, temp -> departure_date) == -1 && check_times(new -> departure_time, temp -> departure_time)) || check_dates(new -> departure_date, temp -> departure_date))
			{
				temp = temp -> next;
				continue;
			}

			break;
		}


		if(temp)
		//	if temp is not NULL, place new node before temp
		{
			for(Flight *prev = *flights; prev; prev = prev -> next)
			//	get node before temp
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
			for(Flight *flight = *flights; flight; flight = flight -> next)
			{
				if(!(flight -> next))
				{
					flight -> next = new;
					break;
				}
			}
		}
	}
}

void edit_flight(Flight **flights)
{
	if(*flights)
	{
		printf("\nEDIT A FLIGHT\n");

		int ID = get_int("Enter flight ID: ");
		Flight *flight = found_flight(*flights, ID);
		if(flight)
		{
			printf("[1] Edit Max Count of Passengers\n");
			printf("[2] Edit Departure and Arrival Date\n");
			printf("[3] Edit Departure and Arrival Time\n");
			printf("[4] Back\n");
			int choice = get_int("Choice: ");

			if(choice > 0 && choice < 4)
			{
				view_flight(flight);
				switch(choice)
				{
					case 1:
						flight -> max_passengers = get_int("Enter new maximum value: ");
						break;
					case 2:
						get_dates(flight, "Enter new departure date: ", "Enter new arrival date:   ");
						break;
					case 3:
						get_times(flight, "Enter new departure time: ", "Enter new arrival time:   ");
						break;
				}

				Flight *new = flightcpy(flight);
				del_flight_node(flights, flight);
				add_flight_node(flights, new);

				printf("\nFlight edited successfully.\n\n");
			}
			else if(choice != 4)
			{
				printf("\n1-4 only.\n\n");
			}
		}
		else
		{
			printf("\nError: Flight not found.\n\n");
		}
	}
	else
	{
		printf("\nError: No flights in list yet.\n\n");
	}
}

void view_flight(Flight *flight)
{
	printf("\nFlight ID: %d\n", flight -> ID);
	printf("Departure: %s %.2d/%.2d/%.4d %.2d:%.2d\n", flight -> origin, flight -> departure_date.mm, flight -> departure_date.dd, flight -> departure_date.yyyy, flight -> departure_time.hh, flight -> departure_time.mm);
	printf("Arrival:   %s %.2d/%.2d/%.4d %.2d:%.2d\n", flight -> destination, flight -> arrival_date.mm, flight -> arrival_date.dd, flight -> arrival_date.yyyy, flight -> arrival_time.hh, flight -> arrival_time.mm);
	printf("Number of Passengers: %d/%d\n", flight -> passenger_count, flight -> max_passengers);
	printf("Bonus Miles: %d\n", flight -> bonus_miles);
}

void view(Flight *flights)
{
	if(flights)
	{
		printf("\nVIEW FLIGHT\n");
		printf("[1] View Specific Flight\n");
		printf("[2] View All Available Flights\n");
		printf("[3] View All Fully-Booked Flights\n");
		printf("[4] View All Flights\n");
		printf("[5] Back\n");
		int choice = get_int("Choice: ");
		switch(choice)
		{
			case 1:
			// if a specific flight is in list, prints details
			{
				int ID = get_int("Enter flight ID: ");
				Flight *flight = found_flight(flights, ID);
				if(flight)
				{
					view_flight(flight);
				}
				else
				{
					printf("\nError: Flight not found.\n\n");
				}
				break;
			}
			case 2:
			// prints all flights that can still accomodate passengers
			{
				for(Flight *flight = flights; flight; flight = flight -> next)
				{
					if((flight -> passenger_count) < (flight -> max_passengers))
					{
						view_flight(flight);
					}
				}
				break;
			}
			case 3:
			// prints all flights that are fully booked
			{
				for(Flight *flight = flights; flight; flight = flight -> next)
				{
					if((flight -> passenger_count) == (flight -> max_passengers))
					{
						view_flight(flight);
					}
				}
				break;
			}
			case 4:
			// prints all flights
			{
				for(Flight *flight = flights; flight; flight = flight -> next)
				{
					view_flight(flight);
				}
				break;
			}
			case 5:
				break;
			default:
				printf("\n1-5 only.\n");
		}
		printf("\n");
	}
	else
	{
		printf("\nError: No flights in list yet.\n\n");
	}
}

void del_flight(Flight **flights)
{
	if(*flights)
	{
		printf("\nDELETE A FLIGHT\n\n");
		printf("")
	}
}

void del_flight_node(Flight **flights, Flight *to_delete)
{
	if(*flights == to_delete)
	//	if node to be deleted is head
	{
		Flight *temp = *flights;
		*flights = temp -> next;
		free(to_delete);
	}
	else
	{
		for(Flight *prev = *flights; prev; prev = prev -> next)
		{
			if(prev -> next == to_delete)
			{
				prev -> next = to_delete -> next;
				free(to_delete);
				break;
			}
		}
	}
}