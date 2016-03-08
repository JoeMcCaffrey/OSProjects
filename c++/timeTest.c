#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int main() {

    time_t current_time;
   // char* c_time_string;
    int i;
for(i =0; i < 100000000000; i++){
   
    /* Convert to local time format. */
   // c_time_string = ctime(&current_time);
    current_time = time(NULL);
//	sleep(4);
    printf("Current time is %ld\n", current_time);
}
    return 0;
}
