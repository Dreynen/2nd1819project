#include "projectlib.h"

//	initialization of variables
void RunApplication()
{
	//	initialize variables
	F_info *flights = NULL;
    P_info *passengers = NULL;
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

int switchboard(F_info **flights, P_info **passengers, int *flight_count, int *passenger_count)
{
	int choice = get_int("Choice: ");

	switch(choice)
	{
		case 1:
			add_F_info(flights, flight_count);
			break;
		case 2:
			edit_F_info(flights, *passengers);
			break;
		case 3:
			view(*flights);
			break;
		case 4:
			del_F_info(flights, flight_count);
			break;
		case 5:
			add_P_info(passengers, passenger_count);
			break;
		case 6:
			edit_P_info(*flights, passengers);
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

void add_F_info(F_info **flights, int *flight_count)
{
	printf("\nADD A FLIGHT\n");
	int flight_ID = get_int("Enter Flight ID: ");
	if(!found_F_info(*flights, flight_ID))
	{
		//	initialize new node
		F_info *new = malloc(sizeof(F_info));
		new -> flight_ID = flight_ID;
		get_locations(new, "Enter destination: ", "Enter origin:      ");
		get_dates(new, "Enter departure date: ", "Enter arrival date  : ");
		get_times(new, "Enter departure time: ", "Enter arrival time  : ");
		new -> passenger_count = 0;
		new -> max_passengers = get_int("Enter maximum number of passengers: ");
		new -> bonus_miles = get_int("Enter bonus miles: ");
		new -> passengers = NULL;
		new -> next = NULL;

		add_F_info_node(flights, new);

		printf("\nFlight added successfully.\n");
		(*flight_count)++;
	}
	else
	{
		printf("\nFlight with same flight ID already exists.\n");
	}
	printf("\n");
}

void add_F_info_node(F_info **flights, F_info *new)
{
	if(!(*flights) || F_infocmp(*flights, new))
	{
		new -> next = *flights;
		*flights = new;
	}
	else
	{
		F_info *flight = *flights;
		while(flight)
		{
			if(F_infocmp(new, flight))
			{
				flight = flight -> next;
			}
			else
			{
				break;
			}
		}

		for(F_info *prev = *flights; prev; prev = prev -> next)
		{
			if(prev -> next == flight)
			{
				prev -> next = new;
				new -> next = flight;
				break;
			}
		}

	}
}

void edit_F_info(F_info **flights, P_info *passengers)
{
	if(*flights)
	{
		printf("\nEDIT A FLIGHT\n");
		int flight_ID = get_int("Enter flight ID: ");
		F_info *flight = found_F_info(*flights, flight_ID);
		if(flight)
		{
			view_flight(flight);
			printf("\n[1] Edit Max Count of Passengers\n");
			printf("[2] Edit Departure and Arrival Date\n");
			printf("[3] Edit Departure and Arrival Time\n");
			printf("[4] Back\n");
			int choice = get_int("Choice: ");

			if(choice > 0 && choice < 4)
			{
				int conflict = 0;
				//	make copies of departure and arrival of flight in case a conflict exists
				struct tm old_dep = flight -> departure, old_arr = flight -> arrival;
				switch(choice)
				{
					case 1:
						flight -> max_passengers = get_int("Enter new maximum value: ");
						break;
					case 2:
						get_dates(flight, "Enter new departure date: ", "Enter new arrival date:   ");
						//	check if flight arrival is after departure; else conflict = 1
						if(valid_dates(flight -> arrival, flight -> departure) < 0 && !valid_times(flight -> arrival, flight -> departure))
						{
							conflict = -1;
						}
						break;
					case 3:
						get_times(flight, "Enter new departure time: ", "Enter new arrival time:   ");
						break;
				}

				//	if dates are valid, check for conflict with passenger flights
				if(conflict == 0)
				{
					for(P_info *passenger = passengers; passenger; passenger = passenger -> next)
					{
						if(flight_conflict(passenger -> flights, flight))
						{
							conflict = 1;
							break;
						}
					}
				}

				if(conflict == 0)
				{
					//	disconnect node from list
					rm_F_info_node(flights, flight);
					//	reinsert node to list
					add_F_info_node(flights, flight);
					printf("\nFlight edited successfully.\n");
				}
				else
				{
					//	revert edit
					flight -> departure = old_dep;
					flight -> arrival = old_arr;
					if(conflict == 1)
					{
						printf("\nError: A passenger reservation has a conflict with the new schedule.\n");
					}
					else if(conflict == -1)
					{
						printf("\nError: Departure and arrival times of new schedule is invalid.\n");
					}
				}
			}
			else if(choice != 4)
			{
				printf("\n1-4 only.\n");
			}
		}
		else
		{
			printf("\nError: Flight not found.\n");
		}
	}
	else
	{
		printf("\nError: No flights in list yet.\n");
	}
	printf("\n");
}

void view(F_info *flights)
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
			//	asks for specific flight and prints flight
			case 1:
			{
				int flight_ID = get_int("Enter flight ID: ");
				F_info *flight = found_F_info(flights, flight_ID);
				if(flight)
				{
					view_flight(flight);
				}
				else
				{
					printf("\nError: Flight not found.\n");
				}
				break;
			}
			// prints all flights that can still accomodate passengers
			case 2:
			{
				for(F_info *flight = flights; flight; flight = flight -> next)
				{
					if((flight -> passenger_count) < (flight -> max_passengers))
					{
						view_flight(flight);
					}
				}
				break;
			}
			// prints all flights that are fully booked
			case 3:
			{
				for(F_info *flight = flights; flight; flight = flight -> next)
				{
					if((flight -> passenger_count) == (flight -> max_passengers))
					{
						view_flight(flight);
					}
				}
				break;
			}
			// prints all flights
			case 4:
			{
				for(F_info *flight = flights; flight; flight = flight -> next)
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
		printf("\nError: No flights in list yet.\n");
	}
	printf("\n");
}

void view_flight(F_info *flight)
{
	printf("\nFlight ID: %d\n", flight -> flight_ID);
	printf("Destination: %s\n", flight -> destination);
	printf("Origin:      %s\n", flight -> origin);
	printf("Departure: %.2d/%.2d/%.4d %.2d:%.2d\n", flight -> departure.tm_mon + 1, flight -> departure.tm_mday, flight -> departure.tm_year + 1900, flight -> departure.tm_hour, flight -> departure.tm_min);
	printf("Arrival:   %.2d/%.2d/%.4d %.2d:%.2d\n", flight -> arrival.tm_mon + 1, flight -> arrival.tm_mday, flight -> arrival.tm_year + 1900, flight -> arrival.tm_hour, flight -> arrival.tm_min);
	printf("Number of Passengers: %d/%d\n", flight -> passenger_count, flight -> max_passengers);
	printf("Bonus Miles: %d\n", flight -> bonus_miles);
}

void del_F_info(F_info **flights, int *flight_count)
{
	if(*flights)
	{
		printf("\nDELETE A FLIGHT\n");

		int ID = get_int("Enter Flight ID: ");
		F_info *flight = found_F_info(*flights, ID);
		if(flight)
		{
			if(!(flight -> passengers))
			{
				view_flight(flight);
				printf("\nConfirm Delete[Y/N]: ");
				char choice; scanf(" %c", &choice);

				if(choice == 'Y' || choice == 'y')
				{
					del_F_info_node(flights, flight);

					printf("\nFlight deleted successfully.\n");
					(*flight_count)--;
				}
				else
				{
					printf("\nFlight not deleted.\n");
				}
			}
			else
			{
				printf("\nError: Unable to delete flight with passengers.\n");
			}
		}
		else
		{
			printf("\nError: Flight not found.\n");
		}
	}
	else
	{
		printf("Error: No flights in list yet.\n");
	}
	printf("\n");
}

void del_F_info_node(F_info **flights, F_info *to_delete)
{
	if(*flights == to_delete)
	{
		*flights = to_delete -> next;
	}
	else
	{
		for(F_info *prev = *flights; prev; prev = prev -> next)
		{
			if(prev -> next == to_delete)
			{
				prev -> next = to_delete -> next;
				break;
			}
		}
	}
	free(to_delete);
}

void add_P_info(P_info **passengers, int *passenger_count)
{
	printf("\nADD A PASSENGER\n");
	int passport_num = get_int("Enter Passport Number: ");
	if(!found_P_info(*passengers, passport_num))
	{
		//	initialize new node
		P_info *new = malloc(sizeof(P_info));
		get_names(new, "Enter first name: ", "Enter last name: ");
		new -> birthdate = get_date("Format: [MM/DD/YYYY]\nEnter birthdate: ", new -> birthdate);
		new -> passport_num = passport_num;
		new -> miles = 0;
		new -> flight_count = 0;
		new -> flights = NULL;
		new -> next = NULL;

		add_P_info_node(passengers, new);
		(*passenger_count)++;

		printf("\nPassenger added successfully.\n");
	}
	else
	{
		printf("\nPassenger with same passport number already exists.\n");
	}
	printf("\n");
}

void add_P_info_node(P_info **passengers, P_info *new)
{
	if(!(*passengers)
		|| namecmp((*passengers) -> lastname, new -> lastname) > 0
		|| (namecmp((*passengers) -> lastname, new -> lastname) == 0 && namecmp((*passengers) -> firstname, new -> firstname) >= 0))
	{
		new -> next = *passengers;
		*passengers = new;
	}
	else
	{
		P_info *passenger = *passengers;
		while(passenger)
		{
			//	if new node is after a node in passengers
			if(namecmp(new -> lastname, passenger -> lastname) > 0
				|| (namecmp(new -> lastname, passenger -> lastname) == 0 && namecmp(new -> firstname, passenger -> firstname) >= 0))
			{
				passenger = passenger -> next;
			}
			else
			{
				break;
			}
		}

		//	get node before which new node will be added
		for(P_info *prev = *passengers; prev; prev = prev -> next)
		{
			if(prev -> next == passenger)
			{
				prev -> next = new;
				new -> next = passenger;
				break;
			}
		}
	}
}

void edit_P_info(F_info *flights, P_info **passengers)
{
	if(*passengers)
	{
		printf("\nEDIT A PASSENGER\n");
		int passport_num = get_int("Enter Passport Number: ");
		P_info *passenger = found_P_info(*passengers, passport_num);
		if(passenger)
		{
			printf("Passport Number: %d\n", passenger ->  passport_num);
			printf("Name: %s %s\n", passenger -> firstname, passenger -> lastname);
			printf("Birthdate: %d/%d/%d\n", passenger -> birthdate.tm_mon, passenger -> birthdate.tm_mday, passenger -> birthdate.tm_year);
			printf("Miles: %d\n", passenger -> miles);
			printf("# of Flights: %d\n", passenger -> flight_count);

			printf("[1] Edit last name\n");
			printf("[2] Edit birthdate\n");
			printf("[3] Back\n");
			int choice = get_int("Choice: ");

			if(choice > 0 && choice < 3)
			{
				switch(choice)
				{
					case 1:
					{
						printf("Enter new lastname: ");
						scanf(" %s", passenger -> lastname);

						//	disconnect node from list
						rm_P_info_node(passengers, passenger);
						//	reinsert node to list
						add_P_info_node(passengers, passenger);
						break;
					}

					case 2:
					{
						struct tm old_date = passenger -> birthdate;

						time_t current_t = time(NULL);
						struct tm current_tm = *(gmtime(&current_t));
						current_tm.tm_hour+=8;
						//	normalize current_tm
						mktime(&current_tm);

						passenger -> birthdate = get_date("Format: [MM/DD/YYYY]\nEnter new birthdate: ", passenger -> birthdate);
						//	if new birthday is after current date
						if(valid_dates(passenger -> birthdate, current_tm))
						{
							passenger -> birthdate = old_date;
							printf("\nError: New birthdate is invalid.\n");
						}
						break;
					}
				}

				printf("\nPassenger edited successfully.\n");
			}
			else if(choice != 3)
			{
				printf("\n1 - 3 only.\n");
			}
		}
		else
		{
			printf("\nError: Passenger not found.\n");
		}
	}
	else
	{
		printf("\nError: No passengers in list yet.\n");
	}
	printf("\n");
}

void del_P_info_node(P_info **passengers, P_info *to_delete)
{
	//	if node to be deleted is head
	if(*passengers == to_delete)
	{
		*passengers = to_delete -> next;
	}
	else
	{
		//	get node before node to be deleted
		for(P_info *passenger = *passengers; passenger; passenger = passenger -> next)
		{
			if(passenger -> next == to_delete)
			{
				passenger -> next = to_delete -> next;
				break;
			}
		}
	}
	free(to_delete);
}

void book_reservation(F_info *flights, P_info *passengers)
{
	if(flights)
	{
		if(passengers)
		{
			printf("\nBOOK A FLIGHT RESERVATION\n");
			int passport_num = get_int("Enter Passport Number: ");
			P_info *passenger = found_P_info(passengers, passport_num);
			if(passenger)
			{
				int flight_ID = get_int("Enter Flight ID: ");
				F_info *flight = found_F_info(flights, flight_ID);
				if(flight)
				{
					if(found_passenger(flight -> passengers, passenger -> passport_num))
					{
						printf("\nError: Passenger has already booked this flight.\n");
					}
					else if(!flight_conflict(passenger -> flights, flight))
					{
						if(flight -> passenger_count < flight -> max_passengers)
						{
							//	make new passenger node to be added to passenger list of flight
							Passenger *new_p = malloc(sizeof(Passenger));
							new_p -> info = passenger;
							new_p -> next = NULL;
							add_passenger_node(&(flight -> passengers), new_p);

							//	make new flight node to be added to flight list of passenger
							Flight *new_f = malloc(sizeof(Flight));
							new_f -> info = flight;
							new_f -> next = NULL;
							add_flight_node(&(passenger -> flights), new_f);

							(flight -> passenger_count)++;
							(passenger -> flight_count)++;
							(passenger -> miles)++;
							printf("\nFlight reservation booked successfully.\n");
						}
						else
						{
							printf("\nError: Flight has reached max number of passengers.\n");
						}
					}
					else
					{
						printf("\nError: Flight has conflict with a currently booked flight.\n");
					}
				}
				else
				{
					printf("\nError: Flight not found.\n");
				}
			}
			else
			{
				printf("\nError: Passenger not found.\n");
			}
		}
		else
		{
			printf("\nError: No passengers in list yet.\n");
		}
	}
	else
	{
		printf("\nError: No flights in list yet.\n");
	}
	printf("\n");
}

int flight_conflict(Flight *flights, F_info *to_book)
/*
	returns 1 if to_book departs before the arrival of a flight in flights or
	to_book arrives after the departure of a flight in flights; else return 0
*/
{
	for(Flight *flight = flights; flight; flight = flight -> next)
	{
		//	if flight is the same as to_book
		if(flight -> info -> flight_ID == to_book -> flight_ID
			//	if flight in list departs after arrival of to_book
			|| (valid_dates(flight -> info -> departure, to_book -> arrival) > 0
				|| (valid_dates(flight -> info -> departure, to_book -> arrival) < 0 && valid_times(flight -> info -> departure, to_book -> arrival) > 0))
			//	if to_book departs after arrival of flight in list
			|| (valid_dates(to_book -> departure, flight -> info -> arrival) > 0
				|| (valid_dates(to_book -> departure, flight -> info -> arrival) < 0 && valid_times(to_book -> departure, flight -> info -> arrival) > 0)))
		{
			continue;
		}
		else
		{
			return 1;
		}
	}
	return 0;
}

void add_flight_node(Flight **flights, Flight *new)
{
	if(!(*flights) || F_infocmp((*flights) -> info, new -> info))
	{
		new -> next = *flights;
		*flights = new;
	}
	else
	{
		Flight *flight = *flights;
		while(flight)
		{
			if(F_infocmp(new -> info, flight -> info))
			{
				flight = flight -> next;
			}
			else
			{
				break;
			}
		}

		for(Flight *prev = *flights; prev; prev = prev -> next)
		{
			if(prev -> next == flight)
			{
				prev -> next = new;
				new -> next = flight;
				break;
			}
		}
	}
}

void add_passenger_node(Passenger **passengers, Passenger *new)
{
	if(!(*passengers)
		|| namecmp((*passengers) -> info -> lastname, new -> info -> lastname) > 0
		|| (namecmp((*passengers) -> info -> lastname, new -> info -> lastname) < 0 && namecmp((*passengers) -> info -> firstname, new -> info -> firstname) >= 0))
	{
		new -> next = *passengers;
		*passengers = new;
	}
	else
	{
		Passenger *passenger = *passengers;
		while(passenger)
		{
			if(namecmp(new -> info -> lastname, passenger -> info -> lastname) > 0
				|| (namecmp(new -> info -> lastname, passenger -> info -> lastname) < 0 && namecmp(new -> info -> firstname, passenger -> info -> firstname) >= 0))
			{
				passenger = passenger -> next;
			}
			else
			{
				break;
			}
		}

		for(Passenger *prev = *passengers; prev; prev = prev -> next)
		{
			if(prev -> next == passenger)
			{
				prev -> next = new;
				new -> next = passenger;
				break;
			}
		}
	}
}

void rm_reservation(F_info *flights, P_info *passengers)
{
	if(flights)
	{
		if(passengers)
		{
			printf("\nREMOVE A FLIGHT RESERVATION\n");
			int passport_num = get_int("Enter Passport Number: ");
			P_info *passenger = found_P_info(passengers, passport_num);
			if(passenger)
			{
				if(passenger -> flights)
				{
					int flight_ID = get_int("Enter Flight ID: ");
					F_info *flight = found_F_info(flights, flight_ID);
					if(flight)
					{
						if(flight -> passengers)
						{
							Passenger *to_delete_p = found_passenger(flight -> passengers, passport_num);
							if(to_delete_p)
							{
								view_flight(flight);
								printf("\nConfirm Delete[Y/N]: ");
								char choice; scanf(" %c", &choice);

								if(choice == 'Y' || choice == 'y')
								{
									Flight *to_delete_f = found_flight(passenger -> flights, flight_ID);
									del_passenger_node(&(flight -> passengers), to_delete_p);
									del_flight_node(&(passenger -> flights), to_delete_f);

									(flight -> passenger_count)--;
									(passenger -> flight_count)--;
									printf("\nFlight reservation removed successfully.\n");
								}
								else
								{
									printf("\nFlight reservation not deleted.\n");
								}
							}
							else
							{
								printf("\nError: Passenger not in flight.\n");
							}
						}
						else
						{
							printf("\nError: No passengers in flight.\n");
						}
					}
					else
					{
						printf("\nError: Flight not in list.\n");
					}
				}
				else
				{
					printf("\nError: Passenger has no flight reservations.\n");
				}
			}
			else
			{
				printf("\nError: Passenger not in list.\n");
			}
		}
		else
		{
			printf("\nError: No passengers in list yet.\n");
		}
	}
	else
	{
		printf("\nError: No flights in list yet.\n");
	}
	printf("\n");
}

void del_flight_node(Flight **flights, Flight *to_delete)
{
	if(*flights == to_delete)
	{
		*flights = to_delete -> next;
	}
	else
	{
		for(Flight *prev = *flights; prev; prev = prev -> next)
		{
			if(prev -> next == to_delete)
			{
				prev -> next = to_delete -> next;
				break;
			}
		}
	}
	free(to_delete);
}

void del_passenger_node(Passenger **passengers, Passenger *to_delete)
{
	if(*passengers == to_delete)
	{
		*passengers = to_delete -> next;
	}
	else
	{
		for(Passenger *prev = *passengers; prev; prev = prev -> next)
		{
			if(prev -> next == to_delete)
			{
				prev -> next = to_delete -> next;
				break;
			}
		}
	}
	free(to_delete);
}

void view_reservations(P_info *passengers)
{
	if(passengers)
	{
		printf("\nVIEW RESERVATIONS\n");
		int passport_num = get_int("Enter passport number: ");
		P_info *passenger = found_P_info(passengers, passport_num);
		if(passenger)
		{
			if(passenger -> flights)
			{
				for(Flight *flight = passenger -> flights; flight; flight = flight -> next)
				{
					view_flight(flight -> info);
				}
				printf("\n");
			}
			else
			{
				printf("\nPassenger has no reservations.\n");
			}
		}
		else
		{
			printf("\nPassenger not in list.\n");
		}
	}
	else
	{
		printf("\nNo passengers in list yet.\n");
	}
	printf("\n");
}

void deallocate(F_info **flights, P_info **passengers)
{
	while(*flights)
	{
		while((*flights) -> passengers)
		{
			del_passenger_node(&((*flights) -> passengers), (*flights) -> passengers);
		}
		del_F_info_node(flights, *flights);
	}

	while(*passengers)
	{
		while((*passengers) -> flights)
		{
			del_flight_node(&((*passengers) -> flights), (*passengers) -> flights);
		}
		del_P_info_node(passengers, *passengers);
	}
}

void save(F_info *flights, P_info *passengers, int flight_count, int passenger_count)
{
	FILE *file = fopen("files/flights.txt", "w");
	if(file)
	{
		fprintf(file, "%d\n", flight_count);
		for(F_info *flight = flights; flight; flight = flight -> next)
		{
			fprintf(file, "%d|", flight -> flight_ID);
			fprintf(file, "%s|", flight -> destination);
			fprintf(file, "%s|", flight -> origin);
			fprintf(file, "%d/%d/%d|", flight -> departure.tm_mon, flight -> departure.tm_mday, flight -> departure.tm_year);
			fprintf(file, "%d/%d/%d|", flight -> arrival.tm_mon, flight -> arrival.tm_mday, flight -> arrival.tm_year);
			fprintf(file, "%d:%d|", flight -> departure.tm_hour, flight -> departure.tm_min);
			fprintf(file, "%d:%d|", flight -> arrival.tm_hour, flight -> arrival.tm_min);
			fprintf(file, "%d/%d|", flight -> passenger_count, flight -> max_passengers);
			fprintf(file, "%d\n", flight -> bonus_miles);
		}
		fclose(file);
	}

	file = fopen("files/passengers.txt", "w");
	if(file)
	{
		fprintf(file, "%d\n", passenger_count);
		for(P_info *passenger = passengers; passenger; passenger = passenger -> next)
		{
			fprintf(file, "%d|", passenger -> passport_num);
			fprintf(file, "%s|", passenger -> firstname);
			fprintf(file, "%s|", passenger -> lastname);
			fprintf(file, "%d/%d/%d|", passenger -> birthdate.tm_mon, passenger -> birthdate.tm_mday, passenger -> birthdate.tm_year);
			fprintf(file, "%d|", passenger -> miles);
			fprintf(file, "%d\n", passenger -> flight_count);
		}
		fclose(file);
	}

	file = fopen("files/flight_passengers.txt", "w");
	if(file)
	{
		for(F_info *flight = flights; flight; flight = flight -> next)
		{
			fprintf(file, "%d|", flight -> flight_ID);
			for(Passenger *passenger = flight -> passengers; passenger; passenger = passenger -> next)
			{
				fprintf(file, "%d|", passenger -> info -> passport_num);
			}
			fprintf(file, "\n");
		}
		fclose(file);
	}

	file = fopen("files/passenger_flights.txt", "w");
	if(file)
	{
		for(P_info *passenger = passengers; passenger; passenger = passenger -> next)
		{
			fprintf(file, "%d|", passenger -> passport_num);
			for(Flight *flight = passenger -> flights; flight; flight = flight -> next)
			{
				fprintf(file, "%d|", flight -> info -> flight_ID);
			}
			fprintf(file, "\n");
		}
		fclose(file);
	}
}

void load(F_info **flights, P_info **passengers, int *flight_count, int *passenger_count)
{
	FILE *file = fopen("files/flights.txt", "r");
	if(file)
	{
		fscanf(file, "%d\n", flight_count);
		for(int i = 0; i < (*flight_count); i++)
		{
			struct tm departure = {}, arrival = {};
			F_info *new = malloc(sizeof(F_info));
			fscanf(file, "%d|", &(new -> flight_ID));
			fscanf(file, "%[^|]|", new -> destination);
			fscanf(file, "%[^|]|", new -> origin);
			fscanf(file, "%d/%d/%d|", &departure.tm_mon, &departure.tm_mday, &departure.tm_year);
			fscanf(file, "%d/%d/%d|", &arrival.tm_mon, &arrival.tm_mday, &arrival.tm_year);
			fscanf(file, "%d:%d|", &departure.tm_hour, &departure.tm_min);
			fscanf(file, "%d:%d|", &arrival.tm_hour, &arrival.tm_min);
			fscanf(file, "%d/%d|", &(new -> passenger_count), &(new -> max_passengers));
			fscanf(file, "%d\n", &(new -> bonus_miles));
			new -> departure = departure;
			new -> arrival = arrival;
			new -> passengers = NULL;
			new -> next = NULL;
			add_F_info_node(flights, new);
		}
		fclose(file);
	}

	file = fopen("files/passengers.txt", "r");
	if(file)
	{
		fscanf(file, "%d\n", passenger_count);
		for(int i = 0; i < (*passenger_count); i++)
		{
			struct tm birthdate = {};
			P_info *new = malloc(sizeof(P_info));
			fscanf(file, "%d|", &(new -> passport_num));
			fscanf(file, " %[^|]|", new -> firstname);
			fscanf(file, " %[^|]|", new -> lastname);
			fscanf(file, "%d/%d/%d|", &birthdate.tm_mon, &birthdate.tm_mday, &birthdate.tm_year);
			fscanf(file, "%d|", &(new -> miles));
			fscanf(file, "%d\n", &(new -> flight_count));
			new -> birthdate = birthdate;
			new -> flights = NULL;
			new -> next = NULL;
			add_P_info_node(passengers, new);
		}
		fclose(file);
	}

	file = fopen("files/flight_passengers.txt", "r");
	if(file)
	{
		int flight_ID, passport_num;
		for(int i = 0; i < (*flight_count); i++)
		{
			fscanf(file, " %d|", &flight_ID);
			F_info *flight = found_F_info(*flights, flight_ID);
			for(int j = 0; j < flight -> passenger_count; j++)
			{
				fscanf(file, "%d|", &passport_num);
				Passenger *new = malloc(sizeof(Passenger));
				new -> info = found_P_info(*passengers, passport_num);
				new -> next = NULL;
				add_passenger_node(&(flight -> passengers), new);
			}
		}
		fclose(file);
	}

	file = fopen("files/passenger_flights.txt", "r");
	if(file)
	{
		int passport_num, flight_ID;
		for(int i = 0; i < (*passenger_count); i++)
		{
			fscanf(file, " %d|", &passport_num);
			P_info *passenger = found_P_info(*passengers, passport_num);
			for(int j = 0; j < passenger -> flight_count; j++)
			{
				fscanf(file, " %d|", &flight_ID);
				Flight *new = malloc(sizeof(Flight));
				new -> info = found_F_info(*flights, flight_ID);
				new -> next = NULL;
				add_flight_node(&(passenger -> flights), new);
			}
		}
		fclose(file);
	}
}
