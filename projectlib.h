#include "declarations.h"

struct _Date
{
	int mm;
	int dd;
	int yyyy;
};

struct _Time
{
	int hh;
	int mm;
};

struct _Flight
{
	int ID;
	char destination[30];
	char origin[30];
	Date departure_date;
	Date arrival_date;
	Time departure_time;
	Time arrival_time;
	int passenger_count;
	int max_passengers;
	int bonus_miles;
	Passenger_list *first;
	Flight *next;
};

struct _Passenger
{
	char firstname[30];
	char lastname[30];
	Date birthdate;
	int ID;
	int miles;
	int flight_count;
	Flight_list *first;
	Passenger *next;
};

struct _Flight_list
{
	Flight *info;
	Flight_list *next;
};

struct _Passenger_list
{
	Passenger *info;
	Passenger_list *next;
};

int get_int(char *prompt)
//	prompts user for a positive integer
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
//	prompts user for a valid date
{
	Date new;

	do
	{
		printf("%s", prompt);
		scanf("%d/%d/%d", &new.mm, &new.dd, &new.yyyy);
	}
	while(!valid_date(new));

	return new;
}

int valid_date(Date date)
//	checks if month, day, and year values are valid
{
	if(date.yyyy < 10000)
	{
		switch(date.mm)
		{
			case 1:
				if(date.dd >= 1 && date.dd <= 31) break;
				else return 0;
			case 2:
				if(date.dd >= 1 && date.dd <= 28) break;
				else return 0;
			case 3:
				if(date.dd >= 1 && date.dd <= 31) break;
				else return 0;
			case 4:
				if(date.dd >= 1 && date.dd <= 30) break;
				else return 0;
			case 5:
				if(date.dd >= 1 && date.dd <= 31) break;
				else return 0;
			case 6:
				if(date.dd >= 1 && date.dd <= 30) break;
				else return 0;
			case 7:
				if(date.dd >= 1 && date.dd <= 31) break;
				else return 0;
			case 8:
				if(date.dd >= 1 && date.dd <= 31) break;
				else return 0;
			case 9:
				if(date.dd >= 1 && date.dd <= 31) break;
				else return 0;
			case 10:
				if(date.dd >= 1 && date.dd <= 31) break;
				else return 0;
			case 11:
				if(date.dd >= 1 && date.dd <= 30) break;
				else return 0;
			case 12:
				if(date.dd >= 1 && date.dd <= 31) break;
				else return 0;
			default:
				return 0;
		}
	}
	else
	{
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
		scanf("%d:%d", &new.hh, &new.mm);
	}
	while(!valid_time(new));

	return new;
}

int valid_time(Time new)
//	checks if hour and minute values are valid
{
	if(new.hh >= 00 && new.hh <= 24 && new.mm >= 0 && new.mm <= 60)
	{
		return 1;
	}

	return 0;
}

void get_locations(Flight *flight, char *prompt0, char *prompt1)
{
	do
	{
		printf("%s", prompt0);
		scanf("%s", flight -> destination);
		printf("%s", prompt1);
		scanf("%s", flight -> origin);
	}
	while(compare_names(flight -> destination, flight -> origin) == 0);
}

void get_dates(Flight *flight, char *prompt0, char *prompt1)
{
	do
	{
		printf("\nFormat: [MM/DD/YYYY]\n");
		flight -> departure_date = get_date(prompt0);
		flight -> arrival_date = get_date(prompt1);
	}
	while(!check_dates(flight -> arrival_date, flight -> departure_date));
}

int check_dates(Date date0, Date date1)
// returns 1 if date0 is after date1, returns -1 if dates are the same; else return 0
{
	if(date0.yyyy >= date1.yyyy)
	{
		if(date0.yyyy == date1.yyyy)
		{
			if(date0.mm >= date1.mm)
			{
				if(date0.mm == date1.mm)
				{
					if(date0.dd >= date1.dd)
					{
						if(date0.dd == date1.dd)
						{
							return -1;
						}
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

void get_times(Flight *flight, char *prompt0, char *prompt1)
{
	do
	{
		printf("\nFormat: [HH:MM]\n");
		flight -> departure_time = get_time(prompt0);
		flight -> arrival_time = get_time(prompt1);
		if(check_dates(flight -> departure_date, flight -> arrival_date) == -1)
		// if departure and arrival dates of flight are same, don't validate
		{
			break;
		}
	}
	while(!check_times(flight -> arrival_time, flight -> departure_time));
}

int check_times(Time time0, Time time1)
// returns 1 if time0 is after time1; else return 0
{
	if(time0.hh >= time1.hh)
	{
		if(time0.hh == time1.hh)
		{
			if(time0.mm >= time1.mm)
			{
				if(time0.mm == time1.mm)
				{
					return -1;
				}
				return 1;
			}
			return 0;
		}
		return 1;
	}
	return 0;
}

Flight *found_flight(Flight *flights, int ID)
{
	for(Flight *flight = flights; flight; flight = flight -> next)
	{
		if(flight -> ID == ID)
		{
			return flight;
		}
	}

	return NULL;
}

int compare_names(char *s, char *t)
//	strcmp capitalized copies of s and t
{
	char namecpy0[30];
	char namecpy1[30];
	strcpy(namecpy0, s);
	strcpy(namecpy1, t);
	capitalize(namecpy0);
	capitalize(namecpy1);

	return strcmp(namecpy0, namecpy1);
}

void capitalize(char *s)
/* replaces string s with a capitalized string */
{
	for(int i = 0, n = strlen(s); i < n; i++)
	{
		s[i] = toupper(s[i]);
	}
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
	new -> first = flight -> first;
	new -> next = NULL;

	return new;
}

Passenger *found_passenger(Passenger *passengers, int ID)
{
	for(Passenger *passenger = passengers; passenger; passenger = passenger -> next)
	{
		if((passenger -> ID) == ID)
		{
			return passenger;
		}
	}
	return NULL;
}

void get_names(Passenger *passenger, char *prompt0, char *prompt1)
{
	printf("%s", prompt0);
	scanf("%s", passenger -> firstname);
	printf("%s", prompt1);
	scanf("%s", passenger -> lastname);
}