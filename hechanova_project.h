#include "projectlib.h"

void RunApplication()
//	initialization of variables and call to switch function
{
	Flight *flights = NULL;
	Passenger *passengers = NULL;
	int flight_count = 0, passenger_count = 0;

	load(&flights, &passengers, &flight_count, &passenger_count);

	do
	{
		menu_out();
	}
	while(switchboard(&flights, &passengers, &flight_count, &passenger_count));

	save(flights, passengers, flight_count, passenger_count);
	deallocate(&flights, &passengers);
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
			edit_flight(flights, passengers);
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
		case 6:
			edit_passenger(passengers);
			break;
		case 7:
			book_reservation(*flights, *passengers);
			break;
		case 8:
			rm_reservation(*flights, *passengers);
			break;
		case 9:
			view_reservations(*passengers);
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
			get_locations(new, "Enter destination: ", "Enter origin:     ");
			get_dates(new, "Enter departure date: ", "Enter arrival date  : ");
			get_times(new, "Enter departure time: ", "Enter arrival time  : ");
			new -> passenger_count = 0;
			new -> max_passengers = get_int("Enter maximum number of passengers: ");
			new -> bonus_miles = get_int("Enter bonus miles: ");
			new -> passengers = NULL;
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
	if(!(*flights) || compare_flights(*flights, new))
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
			if(compare_flights(new, temp))
			//	if new node comes after node in list
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

void edit_flight(Flight **flights, Passenger **passengers)
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
				Flight *new = flightcpy(flight);
				switch(choice)
				{
					case 1:
						flight -> max_passengers = get_int("Enter new maximum value: ");
						break;
					case 2:
						get_dates(new, "Enter new departure date: ", "Enter new arrival date:   ");
						break;
					case 3:
						get_times(new, "Enter new departure time: ", "Enter new arrival time:   ");
						break;
				}

				for(Passenger *passenger = *passengers; passenger; passenger = passenger -> next)
				{
					for(Flight *temp = passenger -> flights; temp; temp = temp -> next)
					{
						if(flight_conflict(new, temp) > 0)
						{
							printf("\nError: New flight schedule has conflict with a passenger reservation.\n\n");
							free(new);
							return;
						}
					}
				}

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
	printf("Destination: %s\n", flight -> destination);
	printf("Origin:      %s\n", flight -> origin);
	printf("Departure: %.2d/%.2d/%.4d %.2d:%.2d\n", flight -> departure_date.mm, flight -> departure_date.dd, flight -> departure_date.yyyy, flight -> departure_time.hh, flight -> departure_time.mm);
	printf("Arrival:   %.2d/%.2d/%.4d %.2d:%.2d\n", flight -> arrival_date.mm, flight -> arrival_date.dd, flight -> arrival_date.yyyy, flight -> arrival_time.hh, flight -> arrival_time.mm);
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
			if(!(flight -> passengers))
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
				printf("\nError: Unable to delete flight with passengers.\n\n");
			}
		}
		else
		{
			printf("\nError: Flight not found.\n\n");
		}
	}
	else
	{
		printf("Error: No flights in list yet.\n\n");
	}
}

void del_flight_node(Flight **flights, Flight *to_delete)
{
	if((*flights) -> ID == to_delete -> ID)
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
		new -> flights = NULL;
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

void edit_passenger(Passenger **passengers)
{
	if(*passengers)
	{
		printf("\nEDIT A PASSENGER\n");
		int ID = get_int("Enter Passport Number: ");
		Passenger *passenger = found_passenger(*passengers, ID);
		if(passenger)
		{
			printf("[1] Edit last name\n");
			printf("[2] Edit birthdate\n");
			printf("[3] Back\n");
			int choice = get_int("Choice: ");

			if(choice > 0 && choice < 3)
			{
				switch(choice)
				{
					case 1:
						printf("Enter new lastname: ");
						scanf("%s", passenger -> lastname);
						break;
					case 2:
						passenger -> birthdate = get_date("Format: [MM/DD/YYYY]\nEnter new birthdate: ");
						break;
				}

				Passenger *new = passengercpy(passenger);
				del_passenger_node(passengers, passenger);
				add_passenger_node(passengers, new);
			}
			else if(choice != 3)
			{
				printf("\n1 - 3 only.\n\n");
			}
		}
		else
		{
			printf("\nError: Passenger not found.\n\n");
		}
	}
	else
	{
		printf("\nError: No passengers in list yet.\n\n");
	}
}

void del_passenger_node(Passenger **passengers, Passenger *passenger)
{
	if((*passengers) -> ID == passenger -> ID)
	//	if node to be deleted is head
	{
		Passenger *temp = *passengers;
		*passengers = temp -> next;
		free(passenger);
	}
	else
	{
		for(Passenger *temp = *passengers; temp; temp = temp -> next)
		{
			if(temp -> next == passenger)
			{
				temp -> next = passenger -> next;
				free(passenger);
				break;
			}
		}
	}
}

void book_reservation(Flight *flights, Passenger *passengers)
{
	if(flights)
	{
		if(passengers)
		{
			printf("\nBOOK A FLIGHT RESERVATION\n");
			int pass_ID = get_int("Enter Passport Number: ");
			Passenger *passenger = found_passenger(passengers, pass_ID);
			if(passenger)
			{
				int flight_ID = get_int("Enter Flight ID: ");
				Flight *flight = found_flight(flights, flight_ID);
				if(flight)
				{
					if((flight -> passenger_count) < (flight -> max_passengers))
					{
						int conflict = flight_conflict(passenger -> flights, flight);
						if(!conflict)
						{
							Passenger *new_p = malloc(sizeof(Passenger));
							new_p = passengercpy(passenger);
							Flight *new_f= malloc(sizeof(Flight));
							new_f = flightcpy(flight);

							add_passenger_node(&(flight -> passengers), new_p);
							add_flight_node(&(passenger -> flights), new_f);

							(passenger -> miles)++;
							(passenger -> flight_count)++;
							(flight -> passenger_count)++;
							printf("\nFlight reservation booked successfully.\n\n");
						}
						else if(conflict < 0)
						{
							printf("\nError: Passenger has already booked this flight.\n\n");
						}
						else
						{
							printf("\nError: Flight has conflict with a currently booked flight.\n\n");
						}
					}
					else
					{
						printf("\nError: Flight has reached maximum number of passengers.\n\n");
					}
				}
				else
				{
					printf("\nError: Flight not found.\n\n");
				}
			}
			else
			{
				printf("\nError: Passenger not found.\n\n");
			}
		}
		else
		{
			printf("\nError: No passengers in list yet.\n\n");
		}
	}
	else
	{
		printf("\nError: No flights in list yet.\n\n");
	}
}

int flight_conflict(Flight *flights, Flight *flight)
//	returns 1 if a flight conflict is found; else return 0
{
	for(Flight *tmp = flights; tmp; tmp = tmp -> next)
	{
		//	if flight in list departs after arrival of flight to be booked
		if((check_dates(tmp -> departure_date, flight -> arrival_date) > 0
			|| (check_dates(tmp -> departure_date, flight -> arrival_date) < 0 && check_times(tmp -> departure_time, flight -> arrival_time) > 0))
		//	if flight to be booked departs after arrival of flight in list
		|| (check_dates(flight -> departure_date, tmp -> arrival_date) > 0
			|| (check_dates(flight -> departure_date, tmp -> arrival_date) < 0 && check_times(flight -> departure_time, tmp -> arrival_time) > 0)))
		{
			continue;
		}
		else if(tmp -> ID == flight -> ID)
		{
			return -1;
		}
		else
		{
			return 1;
		}
	}

	return 0;
}

void rm_reservation(Flight *flights, Passenger *passengers)
{
	if(flights)
	{
		if(passengers)
		{
			int pass_ID = get_int("Enter Passport Number: ");
			Passenger *passenger = found_passenger(passengers, pass_ID);
			if(passenger)
			{
				if(passenger -> flights)
				{
					int flight_ID = get_int("Enter Flight ID: ");
					Flight *flight = found_flight(flights, flight_ID);
					if(flight)
					{
						if(flight -> passengers)
						{
							Passenger *to_delete_p = found_passenger(flight -> passengers, pass_ID);
							if(passenger)
							{
								Flight *to_delete_f = found_flight(passenger -> flights, flight_ID);
								del_passenger_node(&(flight -> passengers), to_delete_p);
								del_flight_node(&(passenger -> flights), to_delete_f);

								(flight -> passenger_count)--;
								(passenger -> flight_count)--;
								printf("\nFlight reservation removed successfully.\n\n");
							}
							else
							{
								printf("\nError: Passenger not in flight.\n\n");
							}
						}
						else
						{
							printf("\nError: No passengers in flight.\n\n");
						}
					}
					else
					{
						printf("\nError: Flight not in list.\n\n");
					}
				}
				else
				{
					printf("\nError: Passenger has no flight reservations.\n\n");
				}
			}
			else
			{
				printf("\nError: Passenger not in list.\n\n");
			}
		}
		else
		{
			printf("\nError: No passengers in list yet.\n\n");
		}
	}
	else
	{
		printf("\nError: No flights in list yet.\n\n");
	}
}

void view_reservations(Passenger *passengers)
{
	if(passengers)
	{
		printf("\nVIEW RESERVATIONS\n");
		int pass_ID = get_int("Enter passport number: ");
		Passenger *passenger = found_passenger(passengers, pass_ID);
		if(passenger)
		{
			if(passenger -> flights)
			{
				for(Flight *flight = passenger -> flights; flight; flight = flight -> next)
				{
					view_flight(flight);
				}
				printf("\n");
			}
			else
			{
				printf("\nPassenger has no reservations.\n\n");
			}
		}
		else
		{
			printf("\nPassenger not in list.\n\n");
		}
	}
	else
	{
		printf("\nNo passengers in list yet.\n\n");
	}
}

void save(Flight *flights, Passenger *passengers, int flight_count, int passenger_count)
{
	FILE *file = fopen("files/flights.txt", "w");
	if(file)
	{
		fprintf(file, "%d\n", flight_count);
		for(Flight *flight = flights; flight; flight = flight -> next)
		{
			fprintf(file, "%d|", flight -> ID);
			fprintf(file, "%s|", flight -> destination);
			fprintf(file, "%s|", flight -> origin);
			fprintf(file, "%d/%d/%d|", flight -> departure_date.mm, flight -> departure_date.dd, flight -> departure_date.yyyy);
			fprintf(file, "%d/%d/%d|", flight -> arrival_date.mm, flight -> arrival_date.dd, flight -> arrival_date.yyyy);
			fprintf(file, "%d:%d|", flight -> departure_time.hh, flight -> departure_time.mm);
			fprintf(file, "%d:%d|", flight -> arrival_time.hh, flight -> arrival_time.mm);
			fprintf(file, "%d/%d|", flight -> passenger_count, flight -> max_passengers);
			fprintf(file, "%d\n", flight -> bonus_miles);
		}
		fclose(file);
	}

	file = fopen("files/passengers.txt", "w");
	if(file)
	{
		fprintf(file, "%d\n", passenger_count);
		for(Passenger *passenger = passengers; passenger; passenger = passenger -> next)
		{
			fprintf(file, "%d|", passenger -> ID);
			fprintf(file, "%s|", passenger -> firstname);
			fprintf(file, "%s|", passenger -> lastname);
			fprintf(file, "%d/%d/%d|", passenger -> birthdate.mm, passenger -> birthdate.dd, passenger -> birthdate.yyyy);
			fprintf(file, "%d|", passenger -> miles);
			fprintf(file, "%d\n", passenger -> flight_count);
		}
		fclose(file);
	}

	file = fopen("files/flight_passengers.txt", "w");
	if(file)
	{
		for(Flight *flight = flights; flight; flight = flight -> next)
		{
			for(Passenger *passenger = flight -> passengers; passenger; passenger = passenger -> next)
			{
				fprintf(file, "%d|", passenger -> ID);
			}
			fprintf(file, "\n");
		}
		fclose(file);
	}

	file = fopen("files/passenger_flights.txt", "w");
	if(file)
	{
		for(Passenger *passenger = passengers; passenger; passenger = passenger -> next)
		{
			for(Flight *flight = passenger -> flights; flight; flight = flight -> next)
			{
				fprintf(file, "%d|", flight -> ID);
			}
			fprintf(file, "\n");
		}
		fclose(file);
	}
}

void load(Flight **flights, Passenger **passengers, int *flight_count, int *passenger_count)
{
	FILE *file = fopen("files/flights.txt", "r");
	if(file)
	{
		fscanf(file, "%d\n", flight_count);
		for(int i = 0; i < (*flight_count); i++)
		{
			Flight *new = malloc(sizeof(Flight));
			fscanf(file, "%d|", &(new -> ID));
			fscanf(file, "%[^|]|", new -> destination);
			fscanf(file, "%[^|]|", new -> origin);
			fscanf(file, "%d/%d/%d|", &(new -> departure_date.mm), &(new -> departure_date.dd), &(new -> departure_date.yyyy));
			fscanf(file, "%d/%d/%d|", &(new -> arrival_date.mm), &(new -> arrival_date.dd), &(new -> arrival_date.yyyy));
			fscanf(file, "%d:%d|", &(new -> departure_time.hh), &(new -> departure_time.mm));
			fscanf(file, "%d:%d|", &(new -> arrival_time.hh), &(new -> arrival_time.mm));
			fscanf(file, "%d/%d|", &(new -> passenger_count), &(new -> max_passengers));
			fscanf(file, "%d\n", &(new -> bonus_miles));
			new -> passengers = NULL;
			new -> next = NULL;
			add_flight_node(flights, new);
		}
		fclose(file);
	}

	file = fopen("files/passengers.txt", "r");
	if(file)
	{
		fscanf(file, "%d\n", passenger_count);
		for(int i = 0; i < (*passenger_count); i++)
		{
			Passenger *new = malloc(sizeof(Passenger));
			fscanf(file, "%d|", &(new -> ID));
			fscanf(file, " %[^|]|", new -> firstname);
			fscanf(file, " %[^|]|", new -> lastname);
			fscanf(file, "%d/%d/%d|", &(new -> birthdate.mm), &(new -> birthdate.dd), &(new -> birthdate.yyyy));
			fscanf(file, "%d|", &(new -> miles));
			fscanf(file, "%d\n", &(new -> flight_count));
			new -> flights = NULL;
			new -> next = NULL;
			add_passenger_node(passengers, new);
		}
		fclose(file);
	}

	file = fopen("files/flight_passengers.txt", "r");
	if(file)
	{
		int ID;
		for(Flight *flight = *flights; flight; flight = flight -> next)
		{
			for(int i = 0; i < flight -> passenger_count; i++)
			{
				fscanf(file, " %d|", &ID);
				Passenger *passenger = found_passenger(*passengers, ID);
				Passenger *new = passengercpy(passenger);
				add_passenger_node(&(flight -> passengers), new);
			}
		}
		fclose(file);
	}

	file = fopen("files/passenger_flights.txt", "r");
	if(file)
	{
		int ID;
		for(Passenger *passenger = *passengers; passenger; passenger = passenger -> next)
		{
			for(int i = 0; i < passenger -> flight_count; i++)
			{
				fscanf(file, " %d|", &ID);
				Flight *flight = found_flight(*flights, ID);
				Flight *new = flightcpy(flight);
				add_flight_node(&(passenger -> flights), new);
			}
		}
		fclose(file);
	}
}

void deallocate(Flight ** flights, Passenger **passengers)
{
	while(*flights)
	{
		while((*flights) -> passengers)
		{
			//	delete first element in passenger list of flight
			del_passenger_node(&((*flights) -> passengers), (*flights) -> passengers);
		}
		//	delete first element in flight list
		del_flight_node(flights, *flights);
	}

	while(*passengers)
	{
		while((*passengers) -> flights)
		{
			//	delete first element in flight list of passenger
			del_flight_node(&((*passengers) -> flights), (*passengers) -> flights);
		}
		//	delete first element in passenger list
		del_passenger_node(passengers, *passengers);
	}
}
