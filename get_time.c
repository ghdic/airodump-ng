#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(){
	time_t timer, first_time;
	struct tm *t;
	first_time=time(NULL);
	while(1){
	timer = time(NULL);
	
	t=localtime(&timer);

	printf("[%02d-%02d-%02d %02d:%02d:%02d]\n", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
	timer=timer-first_time;
	printf("[Elapsed:");
	if(timer/3600 >0){
		printf(" %02d hour", timer/3600);
		timer=timer%3600;
	}

	if(timer/60>0){
		printf(" %02d min", timer/60);
		timer=timer%60;
	}

	printf(" %02d sec ]\n", timer);

	sleep(1);
	system("clear");
	}
	return 0;
}
