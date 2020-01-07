/*
===============================================================================
 Name        : Proj.c
 Author      : $Santeri Rauhala
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
 */

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "funk.h"

#define INITBALANCE 100
#define MAXINPUT 10
#define CARDS 10

int main(void)
{

#if defined (__USE_LPCOPEN)
	// Read clock settings and update SystemCoreClock variable
	SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
	// Set up and initialize all required blocks and
	// functions related to the board hardware
	Board_Init();
	// Set the LED to the state of "On"
	Board_LED_Set(0, true);
#endif
#endif

	srand(time(NULL));

	int deck[CARDS];

	int round=0;
	int balance = 0;

	char input[MAXINPUT];
	char *info = "Type:\n'play' - to start new game\n"
				 "'continue' - to continue your last game\n"
				 "'scores' - to print top10 scores of all time\n"
				 "'logs' - to print previous game scores\n"
				 "'help' - list of commands\n"
				 "'quit' - to stop playing\n\n";

	printf("Welcome to BLACKJACK\n\n");

	printf( "%s",info);

	printf(">> ");
	scanf("%s",input);

	while(strcmp(input, "quit") != 0) //stop playing when input its quit
	{

		if(strcmp(input,"play")==0) //starts the game
		{
			balance = INITBALANCE;
			play(deck,balance,round);
		}
		else if(strcmp(input,"continue")==0) //loads balance and round from "save.txt and then starts the game"
		{
			cont(&balance,&round);
			play(deck,balance,round);
		}
		else if(strcmp(input,"scores")==0) // prints top10 rounds from scores
		{
			scores();
		}
		else if(strcmp(input,"logs")==0) // prints top10 rounds from scores
		{
			logs();
		}
		else if(strcmp(input,"help")==0) // prints commands again
		{
			printf("%s",info);
		}

		printf("%s",info);
		printf(">> ");
		scanf("%s",input);

		printf("\n");
	}

	printf("Thanks for playing, see you soon! :)\n\n\n");


	// Force the counter to be placed into memory
	volatile static int i = 0 ;
	// Enter an infinite loop, just incrementing a counter
	while(1) {
		i++ ;
	}
	return 0 ;
}

