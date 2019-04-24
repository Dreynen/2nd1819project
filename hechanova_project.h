#include "prototypes.h"

struct Date
{
	int mm;
	int dd;
	int yyyy;
};

struct Time
{
	int hh;
	int mm;
};

struct Passenger
{
	char firstname[30];
	char lastname[30];
	Date birthdate;
	int ID;
	int miles;
	struct Flights *first;
	struct Passenger *next;
	struct Passenger *prev;
};

struct Flight
{
	int ID;
	char destination[20];
	char origin[20];
	Date departure_date;
	Date arrival_date;
	Time departure_time;
	Time arrival_time;
	int passenger_count;
	int max_passengers;
	int bonus_miles;
	struct Passengers *first;
	struct Flight *next;
	struct Flight *prev;
};

struct Flights
{
	int ID;
	Flights *next;
};

struct Passengers
{
	int ID;
	Passengers *next;
};

int switchboard(Flight ** flights, Passenger ** passengers, int *flight_count, int *passenger_count)
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
			view(flights);
			break;
		case 4:
			del_flight(flights, flight_count);
			break;
		case 5:
			add_passenger(passengers, passenger_count);
			break;
// 		case 6:
// 			edit_passenger(passengers);
// 			break;
// 		case 7:
// 			book_flight(flights, passengers);
// 			break;
// 		case 8:
// 			rm_reservation(flights, passengers);
// 			break;
// 		case 9:
// 			view_reservations(passengers);
// 			break;
		case 10:
			return 1;
		default:
			printf("\n1-10 only.\n\n");
	}
	return 0;
}

int get_int(char *prompt)
{
	int i;
	do
	{
		printf("%s", prompt);
		scanf("%d", &i);
	}
	while(i < 0);

	return i;
}

Date get_date(char *prompt)
{
	Date new;
	do
	{
		printf("%s", prompt);
		scanf("%d/%d/%d", &(new.mm), &(new.dd), &(new.yyyy));
	}
	while(!valid_date(new));

	return new;
}

int valid_date(Date new)
{
	switch(new.mm)
	{
		case 1:
			if(new.dd >= 1 && new.dd <= 31)	break;
			else return 0;
		case 2:
			if(new.dd >= 1 && new.dd <= 28)	break;
			else return 0;
		case 3:
			if(new.dd >= 1 && new.dd <= 31)	break;
			else return 0;
		case 4:
			if(new.dd >= 1 && new.dd <= 30)	break;
			else return 0;
		case 5:
			if(new.dd >= 1 && new.dd <= 31)	break;
			else return 0;
		case 6:
			if(new.dd >= 1 && new.dd <= 30)	break;
			else return 0;
		case 7:
			if(new.dd >= 1 && new.dd <= 31)	break;
			else return 0;
		case 8:
			if(new.dd >= 1 && new.dd <= 31)	break;
			else return 0;
		case 9:
			if(new.dd >= 1 && new.dd <= 31)	break;
			else return 0;
		case 10:
			if(new.dd >= 1 && new.dd <= 31)	break;
			else return 0;
		case 11:
			if(new.dd >= 1 && new.dd <= 30)	break;
			else return 0;
		case 12:
			if(new.dd >= 1 && new.dd <= 31)	break;
			else return 0;
		default:
			printf("\nError: Invalid date.\n\n");
			return 0;
	}
	return 1;
}

Time get_time(char *prompt)
{
	Time new;
	do
	{
		printf("%s", prompt);
		scanf("%d:%d", &(new.hh), &(new.mm));
	}
	while(!valid_time(new));

	return new;
}

int valid_time(Time new)
{
	if(new.hh >= 0 && new.hh <= 24 && new.mm >= 0 && new.mm <= 59)
	{
		return 1;
	}
	printf("\nError: Invalid time.\n\n");
	return 0;
}

void add_flight(Flight **flights, int *flight_count)
{
	printf("\nADD A FLIGHT\n");
	int ID = get_int("Enter Flight ID: ");
	if(!found_flight(flights, ID))
	{
		Flight *new = malloc(sizeof(Flight));
		new -> ID = ID;
		get_locations(new, "Enter destination: ", "Enter origin: ");
		get_dates(new, "Enter departure date: ", "Enter arrival date:   ");
		get_times(new, "Enter departure time: ", "Enter arrival time:   ");
		new -> passenger_count = 0;
		new -> max_passengers = get_int("Enter maximum count of passengers: ");
		new -> bonus_miles = get_int("Enter bonus miles: ");
		new -> first = NULL;
		new -> next = NULL;
		add_flight_node(flights, new);

	    printf("\nFlight added successfully.\n\n");
		(*flight_count)++;
	}
	else
	{
		printf("\nError: A flight with such flight ID already exists.\n\n");
	}
}

Flight *found_flight(Flight **flights, int ID)
{
	for(Flight *flight = *flights; flight; flight = flight -> next)
	{
		if((flight -> ID) == ID)
		{
			return flight;
		}
	}
	return NULL;
}


void get_locations(Flight *new, char *destination, char *origin)
{
	printf("%s", destination);
	scanf("%s", new -> destination);
	printf("%s", origin);
	scanf("%s", new -> origin);
}

void get_dates(Flight *new, char *departure, char *arrival)
{
	do
	{
		printf("\nFormat: [MM/DD/YYYY]\n");
		new -> departure_date = get_date(departure);
		new -> arrival_date = get_date(arrival);
	}
	while(!valid_dates(new -> departure_date, new -> arrival_date));
}

int valid_dates(Date departure, Date arrival)
// checks if arrival comes after departure
{
	if(arrival.yyyy >= departure.yyyy)
	{
		if(arrival.yyyy == departure.yyyy)
		{
			if(arrival.mm >= departure.mm)
			{
				if(arrival.mm == departure.mm)
				{
					if(arrival.dd >= departure.dd)
					{
						return 1;
					}
					return 0;
				}
				return 1;
			}
			return 0;
		}
		return 1;
	}
	return 0;
}

void get_times(Flight *new, char *departure, char *arrival)
{
	do
	{
		printf("\nFormat: [HH:MM]\n");
		new -> departure_time = get_time(departure);
		new -> arrival_time = get_time(arrival);
	}
	while(!valid_times(new, new -> departure_time, new -> arrival_time));
}

int valid_times(Flight *new, Time departure, Time arrival)
// checks if arrival comes before departure; returns 1 if it does, else return 0.
{
	if(new -> arrival_date.yyyy == new -> departure_date.yyyy && new -> arrival_date.mm == new -> departure_date.mm && new -> arrival_date.dd == new -> departure_date.dd)
	{
		if(arrival.hh >= departure.hh)
		{
			if(arrival.hh == departure.hh)
			{
				if(arrival.mm >= departure.mm)
				{
					return 1;
				}
				return 0;
			}
			return 1;
		}
		return 0;
	}
	return 1;
}

void add_flight_node(Flight **flights, Flight *new)
{
	if(*flights)
	{
		for(Flight *flight = *flights; flight; flight = flight -> next)
		{
			//	valid_dates used to determine which date comes first
			if(valid_dates(flight -> departure_date, new -> departure_date))
			{
				continue;
			}
			else
			{
				if(!(flight -> prev))
				{
					add_flight_head(flights, new);
				}
				else
				{
					new -> next = flight;
					new -> prev = flight -> prev;
					new -> prev -> next = new;
					new -> next -> prev = new;
				}
				break;
			}
		}

		if(!(new -> next))
		{
			add_flight_tail(flights, new);
		}
	}
	else
	{
		add_flight_head(flights, new);
	}
}

void add_flight_head(Flight **flights, Flight *new)
{
	new -> next = *flights;
	new -> prev = NULL;
	*flights = new;
	if(new -> next)
	{
		new -> next -> prev = new;
	}
}

void add_flight_tail(Flight **flights, Flight *new)
{
	for(Flight *flight = *flights; flight; flight = flight -> next)
	{
		if(!(flight -> next))
		{
			flight -> next = new;
			new -> prev = flight;
			break;
		}
	}
}

void edit_flight(Flight **flights)
{
	if(*flights)
	{
		printf("\nEDIT A FLIGHT\n");

		int ID = get_int("Enter flight ID: ");
		Flight *flight = found_flight(flights, ID);
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
	printf("Arrival:   %s %.2d/%.2d/%.2d %.2d:%.2d\n", flight -> destination, flight -> arrival_date.mm, flight -> arrival_date.dd, flight -> arrival_date.yyyy, flight -> arrival_time.hh, flight -> arrival_time.mm);
	printf("Number of Passengers: %d/%d\n", flight -> passenger_count, flight -> max_passengers);
	printf("Bonus Miles: %d\n", flight -> bonus_miles);
}

Flight *flightcpy(Flight *flight)
{
	Flight *new = malloc(sizeof(Flight));
	new -> ID = flight -> ID;
	strcpy(new -> destination, flight -> destination);
	strcpy(new -> origin, flight -> origin);
	new -> departure_date = flight -> departure_date;
	new -> arrival_date = flight -> arrival_date;
	new -> departure_time = flight -> departure_time;
	new -> arrival_time = flight -> arrival_time;
	new -> passenger_count = flight -> passenger_count;
	new -> max_passengers = flight -> max_passengers;
	new -> bonus_miles = flight -> bonus_miles;

	return new;
}

void view(Flight **flights)
{
	if(*flights)
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
				for(Flight *flight = *flights; flight; flight = flight -> next)
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
				for(Flight *flight = *flights; flight; flight = flight -> next)
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
				for(Flight *flight = *flights; flight; flight = flight -> next)
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
		Flight *flight = found_flight(flights, ID);
		if(flight)
		{
			if(!(flight -> first))
			{
				printf("Confirm [Y/N]: ");
				getchar();
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

void del_flight_node(Flight **flights, Flight *flight)
{
	if(!(flight -> prev))
	{
		del_flight_head(flights);
	}
	else if(!(flight -> next))
	{
		del_flight_tail(flights);
	}
	else
	{
		flight -> next -> prev = flight -> prev;
		flight -> prev -> next = flight -> next;
		free(flight);
	}
}

void del_flight_head(Flight **flights)
{
	Flight *flight = *flights;
	*flights = flight -> next;
	if(*flights)
	{
		flight -> next -> prev = NULL;
		free(flight);
	}
}

void del_flight_tail(Flight **flights)
{
	for(Flight *flight = *flights; flight; flight = flight -> next)
	{
		if(!(flight -> next -> next))
		{
			free(flight -> next);
			flight -> next = NULL;
		}
	}
}

void add_passenger(Passenger **passengers, int *passenger_count)
{
	printf("\nADD A PASSENGER\n");
	int ID = get_int("Enter passport number: ");
	if(!found_passenger(passengers, ID))
	{
		Passenger *new = malloc(sizeof(Passenger));
		get_names(new, "Enter first name: ", "Enter last name: ");
		new -> birthdate = get_date("Format: [MM/DD/YYYY]\nEnter birthdate: ");
		new -> ID = ID;
		new -> miles = 0;
		new -> first = NULL;
		new -> next = NULL;
		add_passenger_node(passengers, new);
		(*passenger_count)++;
	}
	else
	{
		printf("\nError: Passenger with same passport number exists.\n\n");
	}
}

Passenger *found_passenger(Passenger **passengers, int ID)
{
	for(Passenger *passenger = *passengers; passenger; passenger = passenger -> next)
	{
		if((passenger -> ID) == ID)
		{
			return passenger;
		}
	}
	return NULL;
}

void get_names(Passenger *new, char *prompt0, char *prompt1)
{
	printf("%s", prompt0);
	scanf("%s", new -> firstname);
	printf("%s", prompt1);
	scanf("%s", new -> lastname);
}

void add_passenger_node(Passenger **passengers, Passenger *new)
{
	if(*passengers)
	{
		for(Passenger *passenger = *passengers; passenger; passenger = passenger -> next)
		{
			if(passenger_cmp(passenger, new))
			{
				continue;
			}
			else
			{
				if(!(passenger -> prev))
				{
					add_passenger_head(passengers, new);
				}
				else
				{
					new -> next = passenger;
					new -> prev = passenger -> prev;
					new -> prev -> next = new;
					new -> next -> prev = new;
				}
				break;
			}
		}

		if(!(new -> next))
		{
			add_passenger_tail(passengers, new);
		}
	}
	else
	{
		add_passenger_head(passengers, new);
	}
}

int passenger_cmp(Passenger *passenger, Passenger *new)
{
	if(strcmp(new -> lastname, passenger -> lastname) >= 0)
	{
		if(strcmp(new -> lastname, passenger -> lastname) == 0)
		{
			if(strcmp(new -> firstname, passenger -> firstname) >= 0)
			{
				return 1;
			}
			return 0;
		}
		return 1;
	}
	return 0;
}

void add_passenger_head(Passenger **passengers, Passenger *new)
{
	new -> next = *passengers;
	new -> prev = NULL;
	*passengers = new;
	if(!(new -> next))
	{
		new -> next -> prev = new;
	}
}

void add_passenger_tail(Passenger **passengers, Passenger *new)
{
	for(Passenger *passenger = *passengers; passenger; passenger = passenger -> next)
	{
		if(!(passenger -> next))
		{
			passenger -> next = new;
			new -> prev = passenger;
			break;
		}
	}
}