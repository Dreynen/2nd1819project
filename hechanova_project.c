#include "hechanova_project.h"

int main(void)
{
<<<<<<< HEAD
    //  intialization of variables
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
=======
    RunApplication();
>>>>>>> new

    return 0;
}