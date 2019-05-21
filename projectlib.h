#include "declarations.h"

struct _F_info
{
    int flight_ID;
	char destination[30];
	char origin[30];
	struct tm departure;
	struct tm arrival;
	int passenger_count;
	int max_passengers;
	int bonus_miles;
	Passenger *passengers;
	F_info *next;
};

struct _P_info
{
    int passport_num;
	char firstname[30];
	char lastname[30];
	struct tm birthdate;
	int miles;
	int flight_count;
	Flight *flights;
	P_info *next;
};

struct _Flight
{
    F_info *info;
    Flight *next;
};

struct _Passenger
{
    P_info *info;
    Passenger *next;
};

//	prompts user for a positive integer
int get_int(char *prompt)
{
	char s[10], *end;
	int i;

	do
	{
		i = 0;

		printf("%s", prompt);
		scanf("%10s", s);

		if(strisdigits(s))
		{
			i = atoi(s);
		}
		else
		{
			continue;
		}
	}
	while(i <= 0);

	return i;
}

//	returns 1 if a string passed has digits for all characters; else returns 0
int strisdigits(char *s)
{
    for(int i = 0, len = strlen(s); i < len; i++)
    {
        if(isdigit(s[i]))
        {
            continue;
        }
        else
        {
            return 0;
        }
    }

    return 1;
}

//	gets month, day, and year from user
struct tm get_date(char *prompt, struct tm new_tm)
{
	char s[11];
	int count;

	do
	{
		printf("%s", prompt);
		scanf("%10s", s);
		count = 0;
		for(char *temp = strtok(s, "/"); temp; temp = strtok(NULL, "/"))
		{
			if(strisdigits(temp))
			{
				if(count == 0) new_tm.tm_mon = atoi(temp) - 1;
				if(count == 1) new_tm.tm_mday = atoi(temp);
				if(count == 2) new_tm.tm_year = atoi(temp) -  1900;
			}
			else
			{
				break;
			}
			count++;
		}

		if(count != 3)
			continue;
	}
	while(!valid_date(new_tm));

	return new_tm;
}

//	returns 1 if user input is within proper bounds; else returns 0
int valid_date(struct tm date)
{
	//	minimum year is 1900
	if(date.tm_year > 0)
	{
		switch(date.tm_mon)
		{
			case 0:
				if(date.tm_mday >= 1 && date.tm_mday <= 31) break;
				else return 0;
			case 1:
				if(date.tm_mday >= 1 && date.tm_mday <= 28) break;
				else return 0;
			case 2:
				if(date.tm_mday >= 1 && date.tm_mday <= 31) break;
				else return 0;
			case 3:
				if(date.tm_mday >= 1 && date.tm_mday <= 30) break;
				else return 0;
			case 4:
				if(date.tm_mday >= 1 && date.tm_mday <= 31) break;
				else return 0;
			case 5:
				if(date.tm_mday >= 1 && date.tm_mday <= 30) break;
				else return 0;
			case 6:
				if(date.tm_mday >= 1 && date.tm_mday <= 31) break;
				else return 0;
			case 7:
				if(date.tm_mday >= 1 && date.tm_mday <= 31) break;
				else return 0;
			case 8:
				if(date.tm_mday >= 1 && date.tm_mday <= 31) break;
				else return 0;
			case 9:
				if(date.tm_mday >= 1 && date.tm_mday <= 31) break;
				else return 0;
			case 10:
				if(date.tm_mday >= 1 && date.tm_mday <= 30) break;
				else return 0;
			case 11:
				if(date.tm_mday >= 1 && date.tm_mday <= 31) break;
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

//	returns 1 if the date of time0 occurs after time1, returns -1 if the two dates are the same; else returns 0
int valid_dates(struct tm time0, struct tm time1)
{
	if(time0.tm_year > time1.tm_year
		|| (time0.tm_year == time1.tm_year && time0.tm_mon > time1.tm_mon)
		|| (time0.tm_year == time1.tm_year && time0.tm_mon == time1.tm_mon && time0.tm_mday >= time1.tm_mday))
	{
		if(time0.tm_year == time1.tm_year && time0.tm_mon == time1.tm_mon && time0.tm_mday == time1.tm_mday)
		{
			return -1;
		}
		return 1;
	}
	return 0;
}

//	get hour and minute from user
struct tm get_time(char *prompt, struct tm new_tm)
{
	char s[6];
	int count;

	do
	{
		printf("%s", prompt);
		scanf("%5s", s);
		count = 0;
		for(char *temp = strtok(s, ":"); temp; temp = strtok(NULL, ":"))
		{
			if(strisdigits(temp))
			{
				if(count == 0) new_tm.tm_hour = atoi(temp);
				if(count == 1) new_tm.tm_min = atoi(temp);
			}
			else
			{
				break;
			}
			count++;
		}
	}
	while(!valid_time(new_tm));

	return new_tm;
}

int valid_time(struct tm new_tm)
{
	if((new_tm.tm_hour >= 0 || new_tm.tm_hour < 24) || (new_tm.tm_min >= 0 || new_tm.tm_min < 60))
	{
		return 1;
	}
	return 0;
}

//	returns 1 if the hour:min of time0 occurs after time1, returns -1 if the two hour:min are the same; else returns 0
int valid_times(struct tm time0, struct tm time1)
{
	if(time0.tm_hour > time1.tm_hour || (time0.tm_hour == time1.tm_hour && time0.tm_min >= time1.tm_min))
	{
		if(time0.tm_hour == time1.tm_hour && time0.tm_min == time1.tm_min)
		{
			return -1;
		}
		return 1;
	}
	return 0;
}

//	gets strings and stores them into flight; reprompts if destination and origin are the same
void get_locations(F_info *flight, char *prompt0, char *prompt1)
{
	do
	{
		printf("%s", prompt0);
		scanf(" %29[^\n]", flight -> destination);
		printf("%s", prompt1);
		scanf(" %29[^\n]", flight -> origin);
	}
	while(!namecmp(flight -> destination, flight -> origin));
}

//	gets the departure and arrival dates of a flight
void get_dates(F_info *flight, char *prompt0, char *prompt1)
{
	time_t current_t = time(NULL);
	struct tm current_tm = *(gmtime(&current_t));
	current_tm.tm_hour+=8;
	//	normalize current_tm
	mktime(&current_tm);

	do
	{
		printf("\nFormat: [MM/DD/YYYY]\n");
		flight -> departure = get_date(prompt0, flight -> departure);
		flight -> arrival = get_date(prompt1, flight -> arrival);
	}
	while(!valid_dates(flight -> departure, current_tm) || !valid_dates(flight -> arrival, flight -> departure));
}

//	gets the departure and arrival times of a flight; reprompts if departure occurs after arrival
void get_times(F_info *flight, char *prompt0, char *prompt1)
{
	do
	{
		printf("\nFormat: [HH:MM]\n");
		flight -> departure = get_time(prompt0, flight -> departure);
		flight -> arrival = get_time(prompt1, flight -> arrival);

		//	if departure date and arrival date of flight are different
		if(valid_dates(flight -> arrival, flight -> departure) > 0)
		{
			break;
		}
	}
	while(!valid_times(flight -> arrival, flight -> departure));
}

//	strcmp capitalized copies of s and t
int namecmp(char *s, char *t)
{
	char namecpy0[30];
	char namecpy1[30];
	strcpy(namecpy0, s);
	strcpy(namecpy1, t);
	capitalize(namecpy0);
	capitalize(namecpy1);

	return strcmp(namecpy0, namecpy1);
}

/* replaces string s with a capitalized string */
void capitalize(char *s)
{
	for(int i = 0, n = strlen(s); i < n; i++)
	{
		s[i] = toupper(s[i]);
	}
}

//	returns a F_info pointer if a flight with flight_ID is found; else returns NULL
F_info *found_F_info(F_info *flights, int flight_ID)
{
	for(F_info *flight = flights; flight; flight = flight -> next)
	{
		if(flight -> flight_ID == flight_ID) return flight;
	}
	return NULL;
}

//	returns 1 if flight0 occurs after flight1; else return 0
int F_infocmp(F_info *flight0, F_info *flight1)
{
	if(valid_dates(flight0 -> departure, flight1 -> departure) > 0
		|| (valid_dates(flight0 -> departure, flight1 -> departure) < 0 && valid_times(flight0 -> departure, flight1 -> departure) > 0)
		|| (valid_dates(flight0 -> departure, flight1 -> departure) < 0 && valid_times(flight0 -> departure, flight1 -> departure) < 0 && namecmp(flight0 -> origin, flight1 -> origin) > 0)
		|| (valid_dates(flight0 -> departure, flight1 -> departure) < 0 && valid_times(flight0 -> departure, flight1 -> departure) < 0 && namecmp(flight0 -> origin, flight1 -> origin) < 0 && namecmp(flight0 -> destination, flight1 -> destination) >= 0))
	{
		return 1;
	}

	return 0;
}

//	returns a P_info pointer if a passenger with passport_num is found; else returns NULL
P_info *found_P_info(P_info *passengers, int passport_num)
{
	for(P_info *passenger = passengers; passenger; passenger = passenger -> next)
	{
		if(passenger -> passport_num == passport_num) return passenger;
	}
	return NULL;
}

//	gets two strings and stores them into passenger
void get_names(P_info *passenger, char *prompt0, char *prompt1)
{
	printf("%s", prompt0);
	scanf(" %[^\n]", passenger -> firstname);
	printf("%s", prompt1);
	scanf(" %[^\n]", passenger -> lastname);
}

//	returns flight pointer if a flight with flight_ID is found from a flight list; else returns NULL
Flight *found_flight(Flight *flights, int flight_ID)
{
	for(Flight *flight = flights; flight; flight = flight -> next)
	{
		if(flight -> info -> flight_ID == flight_ID)
		{
			return flight;
		}
	}

	return NULL;
}

//	returns a passenger pointer if passenger with passport_num is found from a passenger list; else returns NULL
Passenger *found_passenger(Passenger *passengers, int passport_num)
{
	for(Passenger *passenger = passengers; passenger; passenger = passenger -> next)
	{
		if((passenger -> info -> passport_num) == passport_num)
		{
			return passenger;
		}
	}
	return NULL;
}

//	disconnects a node from list without freeing it; preparation for reinsertion
void rm_F_info_node(F_info **flights, F_info *to_rm)
{
	if(*flights == to_rm)
	{
		*flights = to_rm -> next;
	}
	else
	{
		for(F_info *prev = *flights; prev; prev = prev -> next)
		{
			if(prev -> next == to_rm)
			{
				prev -> next = to_rm -> next;
				break;
			}
		}
	}
}


//	disconnects a node from list without freeing it; preparation for reinsertion
void rm_P_info_node(P_info **passengers, P_info *to_rm)
{
	//	if node to be deleted is head
	if(*passengers == to_rm)
	{
		*passengers = to_rm -> next;
	}
	else
	{
		for(P_info *passenger = *passengers; passenger; passenger = passenger -> next)
		{
			if(passenger -> next == to_rm)
			{
				passenger -> next = to_rm -> next;
				break;
			}
		}
	}
}