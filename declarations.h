#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Date Date;
typedef struct _Time Time;
typedef struct _Flight Flight;
typedef struct _Passenger Passenger;
typedef struct _Flight_list Flight_list;
typedef struct _Passenger_list Passenger_list;

int get_int(char *);
Date get_date(char *);
int valid_date(Date);
Time get_time(char *);
int valid_time(Time);
void get_locations(Flight *, char *, char *);
void get_dates(Flight *, char *, char *);
int check_dates(Date, Date);
void get_times(Flight *, char *, char *);
int check_times(Time, Time);
Flight *found_flight(Flight *, int);
int compare_names(char *, char *);
void capitalize(char *);
Flight *flightcpy(Flight *);
Passenger *found_passenger(Passenger *, int);
void get_names(Passenger *, char *, char *);

void RunApplication();
void menu_out();
int switchboard(Flight **, Passenger **, int *, int *);
void add_flight(Flight **, int *);
void add_flight_node(Flight **, Flight *);
void edit_flight(Flight **);
void view(Flight *);
void view_flight(Flight *);
void del_flight(Flight **, int*);
void del_flight_node(Flight **, Flight *);
void add_passenger(Passenger **, int *);
void add_passenger_node(Passenger **, Passenger *);
void edit_passenger(Passenger **);

