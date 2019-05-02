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
		case 5:
			add_passenger(passengers, passenger_count);
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
	if(!(*flights)
		|| check_dates((*flights) -> departure_date, new -> departure_date) == 1
		|| (check_dates((*flights) -> departure_date, new -> departure_date) == -1 && check_times((*flights) -> departure_time, new -> departure_time) == 1)
		|| (check_dates((*flights) -> departure_date, new -> departure_date) == -1 && check_times((*flights) -> departure_time, new -> departure_time) == -1 && compare_names((*flights) -> origin, new -> origin) > 0)
		|| (check_dates((*flights) -> departure_date, new -> departure_date) == -1 && check_times((*flights) -> departure_time, new -> departure_time) == -1 && compare_names((*flights) -> origin, new -> origin) == 0 && compare_names((*flights) -> destination, new -> destination) >= 0))
	//	if flight list is NULL or new node comes before head node
	{
		new -> next = *flights;
		*flights = new;
	}
	else
	{
		Flight *temp = *flights;
		while(temp)
		//	gets node before which new node should be placed; terminates with NULL if to be placed at tail
		{
			if(check_dates(new -> departure_date, temp -> departure_date) == 1
				|| (check_dates(new -> departure_date, temp -> departure_date) == -1 && check_times(new -> departure_time, temp -> departure_time) == 1)
				|| (check_dates(new -> departure_date, temp -> departure_date) == -1 && check_times(new -> departure_time, temp -> departure_time) == -1 && compare_names(new -> origin, temp -> origin) > 0)
				|| (check_dates(new -> departure_date, temp -> departure_date) == -1 && check_times(new -> departure_time, temp -> departure_time) == -1 && compare_names(new -> origin, temp -> origin) == 0 && compare_names(new -> destination, temp -> destination) >= 0))
			{
				temp = temp -> next;
			}
			else
			{
				break;
			}
		}

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

void del_flight(Flight **flights, int *flight_count)
{
	if(*flights)
	{
		printf("\nDELETE A FLIGHT\n");

		int ID = get_int("Enter Flight ID: ");
		Flight *flight = found_flight(*flights, ID);
		if(flight)
		{
			if(!(flight -> first))
			//	if flight passenger list is NULL
			{
				printf("Confirm [Y/N]: ");
				getchar();	//	get \n from last input
				char choice = getchar();
				if(choice == 'Y' || choice == 'y')
				{
					del_flight_node(flights, flight);

					printf("\nFlight deleted successfully.\n\n");
					(*flight_count)--;
				}
				else
				{
					printf("\nFlight not deleted.\n\n");
				}
			}


			else
			{
				printf("\nError: Flight still has reserved passengers.\n\n");
			}
		}
		else
		{
			printf("\nError: Flight not found.\n\n");
		}
	}
}

void del_flight_node(Flight **flights, Flight *to_delete)
{
	if(*flights == to_delete)
	//	if node to be deleted is head
	{
		*flights = to_delete -> next;
		free(to_delete);
	}
	else
	{
		for(Flight *prev = *flights; prev; prev = prev -> next)
		//	get node before node to be deleted
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

void add_passenger(Passenger **passengers, int *passenger_count)
{
	printf("\nADD A PASSENGER\n");
	int ID = get_int("Enter Passport Number: ");
	if(!found_passenger(*passengers, ID))
	{
		Passenger *new = malloc(sizeof(Passenger));
		get_names(new, "Enter first name: ", "Enter last name: ");
		new -> birthdate = get_date("Enter birthdate: ");
		new -> ID = ID;
		new -> miles = 0;
		new -> flight_count = 0;
		new -> first = NULL;
		new -> next = NULL;

		add_passenger_node(passengers, new);

		printf("\nPassenger added successfully.\n\n");
		(*passenger_count)++;
	}
	else
	{
		printf("\nPassenger with same passport number already exists.\n\n");
	}
}

void add_passenger_node(Passenger **passengers, Passenger *new)
{
	if(!(*passengers)
		|| compare_names((*passengers) -> lastname, new -> lastname) > 0
		|| (compare_names((*passengers) -> lastname, new -> lastname) == 0 && compare_names((*passengers) -> firstname, new -> firstname) >= 0))
	//	if passenger list is NULL or new node comes before head node
	{
		new -> next = *passengers;
		*passengers = new;
	}
	else
	{
		Passenger *temp = *passengers;
		while(temp)
		{
			if(compare_names(new -> lastname, temp -> lastname) > 0
				|| (compare_names(new -> lastname, temp -> lastname) == 0 && compare_names(new -> firstname, temp -> firstname) >= 0))
			{
				temp = temp -> next;
			}
			else
			{
				break;
			}
		}

		for(Passenger *prev = *passengers; prev; prev = prev -> next)
		//	get node before temp
		{
			if(prev -> next == temp)
			{
				prev -> next = new;
				new -> next = temp;
			}
		}
	}
}
