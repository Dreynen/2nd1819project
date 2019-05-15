#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char *strtok(char *, const char *);

typedef struct _F_info F_info;
typedef struct _P_info P_info;
typedef struct _Flight Flight;
typedef struct _Passenger Passenger;

void RunApplication();

void menu_out();
int switchboard(F_info **, P_info **, int *, int *);
void add_F_info(F_info **, int *);
void add_F_info_node(F_info **, F_info *);
void edit_F_info(F_info **, P_info *);
void view(F_info *);
void view_flight(F_info *);
void del_F_info(F_info **, int *);
void del_F_info_node(F_info **, F_info *);
void add_P_info(P_info **, int *);
void add_P_info_node(P_info **, P_info *);
void edit_P_info(F_info *, P_info **);
void del_P_info_node(P_info **, P_info *);
void book_reservation(F_info *, P_info *);
int flight_conflict(Flight *, F_info *);
void add_flight_node(Flight **, Flight *);
void add_passenger_node(Passenger **, Passenger *);
void rm_reservation(F_info *, P_info *);
void del_flight_node(Flight **, Flight *);
void del_passenger_node(Passenger **, Passenger *);
void view_reservations(P_info *);
void deallocate(F_info **, P_info **);
void save(F_info *, P_info *, int, int);
void load(F_info **, P_info **, int *, int *);

int get_int(char *);
int strisdigits(char *);
struct tm get_date(char *, struct tm);
int valid_date(struct tm);
int valid_dates(struct tm, struct tm);
struct tm get_time(char *, struct tm);
int valid_times(struct tm, struct tm);
void get_locations(F_info *, char *, char *);
void get_dates(F_info *, char *, char *);
void get_times(F_info *, char *, char *);
int namecmp(char *, char *);
void capitalize(char *);
F_info *found_F_info(F_info *, int);
int F_infocmp(F_info *, F_info *);
P_info *found_P_info(P_info *, int);
void get_names(P_info *, char *, char *);
P_info *P_infocpy(P_info *);
Flight *found_flight(Flight *, int);
Passenger *found_passenger(Passenger *, int);
void rm_F_info_node(F_info **, F_info *);
void rm_P_info_node(P_info **, P_info *);
