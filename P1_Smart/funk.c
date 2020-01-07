#include "funk.h"

#define LINESIZE 20
#define MAXNAME 15
#define SCORENUM 10
#define CARDS 52
#define MAXCARDFACE 10
#define MAXCARDSUIT 10
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct score { //used in printing scores
	int round;
	char name[MAXNAME];
};

//function that is called to play the game. *deck is the deck, balance is how many coins you have and round is what round it is.
void play(int *deck, int balance, int round)
{

	//intialvalues that dont come from main
	int pot = 0;
	int conti = 0; //to check if player wants to keep playing
	int *pdeck=deck;
	char input[21];

	//One round...
	printf("You have %d credits and the round is %d.\n",balance,round);
	while(balance > 9 && conti == 0){

		bet(&balance,&pot);
		initialize(pdeck);
		shuffle(pdeck);
		turn(&balance,&pot,pdeck);
		round++;

		printf("Do you want to play again? (y/n)\n>> ");
		while(1)
		{
			scanf("%s",input);

			if(strcmp(input,"y")==0)
			{
				break;
			}
			else if (strcmp(input,"n")==0)
			{
				quit(round,balance);
				conti++;
				break;
			}
			else
			{
				printf("You need to input 'y' or 'n'.\n");
			}
		}

	}

	//checking if you have run out of credits
	if(balance<9)
	{

		FILE *scores;
		scores = fopen("scores.txt", "a");

		if (scores == NULL) {
			printf("Unable to open %s\n", "scores");
		}
		else
		{

			printf("Give name to save in scores: (max 20 letters)\n>>  ");
			scanf("%s",input);

			fprintf(scores,"%d-%s\n",round,input);
			fclose(scores);

			printf("Your score has been saved.\n");

		}

		printf("Type 'play' to play another game or 'quit' to stop playing.\n\n");
	}
	printf("\nType:\n'play' - to start new game\n'continue' - to continue your last game\n'scores' - to print top10 scores of all time\n'logs' - to print previous game scores\n'help' - list of commands\n'quit' - to stop playing\n\n");
}

//function that is called to stop playing once you have started, asks if u want to save the game and saves round and balance to save.txt
void quit(int round,int balance)
{
	FILE *save;
	char input[10];

	while(strcmp(input,"n")!=0)
	{
		printf("Would you like to save? (y/n)\n>> ");
		scanf("%s",input);

		if(strcmp(input,"y")==0) //saves the round and current credit balance to save.txt
		{

			save = fopen("save.txt", "w");

			if (save == NULL) {
				printf("Unable to open %s\n", "save.txt");
			}
			else
			{
				fprintf(save, "%d %d",round, balance);
			}
			fclose(save);
			break;
		}
		else if(strcmp(input,"n")==0)
		{
			break;
		}
		else
		{
			printf("You entered invalid choice, try again.\n");
		}
	}
}

//used to print top10 scores of all time
void scores(void)
{

	FILE *scores;
	struct score *scorelist;
	scorelist = (struct score *)malloc(sizeof(struct score));

	int i=0;
	char line[LINESIZE];

	scores = fopen("scores.txt", "r");

	if (scores == NULL) {
		printf("Unable to open %s\n", "scores");
	}
	else
	{

		while (!feof(scores)) //puts all scores into array "scorelist"
		{
			if (fgets(line, LINESIZE, scores) != NULL) {
				if (sscanf(line, "%d-%s", &scorelist[i].round,scorelist[i].name) == 2) {
					i++;
					scorelist=(struct score *)realloc(scorelist,sizeof(struct score)*(i+1));
				}
				else {
					printf("Invalid line at line %d\n",i);
				}
			}
		}

		qsort(scorelist,i,sizeof(struct score),sortscore); //sorts the list from highest to lowest

		for(int j=0;j<SCORENUM;j++) //prints top10 scores
		{
			printf("%2d: Player %s survived %3d rounds.\n",j+1,scorelist[j].name,scorelist[j].round);
		}

		free(scorelist);
		fclose(scores);
		printf("\n\n");
	}


}

//used in scores(void) to sort the records with qsort, sorts from highest to lowest
int sortscore (const void * a, const void * b)
{
	const struct score *p1 = (struct score *)a;
	const struct score *p2 = (struct score *)b;

	return (p2->round - p1->round); //highest to lowest
}

//used to load saved round and balance before play is called
void cont(int *balance, int *round)
{
	char line[LINESIZE];

	FILE *save;
	save = fopen("save.txt", "r");
	int temp1;
	int temp2;

	if (save == NULL) {
		printf("Unable to open %s\n", "save.txt");
	}
	else //gets values for balance and round from save.txt file
	{

		if (fgets(line, LINESIZE, save) != NULL) {
			if (sscanf(line, "%d %d", &temp1, &temp2) == 2) {}
			else
			{
				printf("Invalid line.\n");
			}
		}

		fclose(save);
		*balance = temp2;
		*round = temp1;
	}
}

//used in play function to play one game of blackjack
void turn(int *balance, int *pot, int *deck)
{
	int inc = rand()%52;
	int ptotal = 0;
	int dtotal = 0;

	printf("Pot is %d\n",*pot);

	//first starting card draw
	int card = hit(inc, deck);
	char csuit[MAXCARDSUIT];
	strcpy(csuit, suit(card));
	char cface[MAXCARDFACE];
	strcpy(cface, face(card));
	int cval = cvalue(card);
	ptotal += cval;
	checkAce(cval,&ptotal);

	printf("You got a %s of %s.\n", cface, csuit);

	//second starting card draw
	inc = rand()%52;
	card = hit(inc,deck);
	strcpy(csuit , suit(card));
	strcpy(cface , face(card));
	cval = cvalue(card);
	ptotal += cval;
	checkAce(cval,&ptotal);


	printf("and a %s of %s.\n", cface, csuit);

	//dealers first card draw
	inc = rand()%52;
	card = hit(inc,deck);
	strcpy(csuit , suit(card));
	strcpy(cface , face(card));
	cval = cvalue(card);
	dtotal += cval;

	printf("The dealer's face up card is a %s of %s.\n", cface, csuit);

	//delers hidden card draw
	inc = rand()%52;
	int hidden = hit(inc,deck);
	char *hSuit = suit(hidden);
	char *hFace = face(hidden);
	int hiddenCV = cvalue(hidden);

	printf("You have a total of %d points, and the dealer has %d.\n\n", ptotal, dtotal);

	if(ptotal == 21) //21=blackjack
	{
		printf("BLACKJACK! Your payout is 2-to-1, %d.\n", (int)((*pot)*2)); // no cents in this world
		*balance += ((*pot)*2);
	}
	else //drawing cards with hit or stop drawing with stand
	{
		char input[6];

		printf("type 'stand' to not get more cards or 'hit' to get new card.\n>> ");
		scanf("%5s", input);

		while(strcmp(input,"stand")!=0)
		{
			if(strcmp(input,"hit") == 0)
			{
				inc = rand()%52;
				card = hit(inc,deck);
				strcpy(csuit , suit(card));
				strcpy(cface , face(card));
				cval = cvalue(card);
				ptotal += cval;

				printf("You got a %s of %s.\n", cface, csuit);
				if(ptotal < 21)
				{
					if(cval==1)
					{
						checkAce(cval,&ptotal);

						if(ptotal==21)
						{
							printf("Congrats! You got 21!\n");
							break;
						}
					}
				}
				else if(ptotal == 21)
				{
					printf("Congrats! You got 21!\n");
					break;
				}
				else
				{
					printf("Oh no, you've busted with %d. Try Again!\n", ptotal);
					break;
				}
			}

			else printf("Invalid command, try again.\n");

			printf("Your new total is %d.\n", ptotal);
			printf("type 'stand' to not get more cards or 'hit' to get new card.\n>> ");
			scanf("%5s", input);
		}
	}

	if(ptotal < 21){ //if you didnt get 21 dealer starts drawing untill hes >=16

		printf("The dealer's flips a %s of %s worth %d.\n", hFace, hSuit, hiddenCV);
		dtotal += hiddenCV;
		if(hiddenCV == 1)
		{
			if(dtotal+11 < 21) dtotal += 10;
		}

		if(dtotal >= 16){
			printf("The dealer stands with %d.\n", dtotal);
		}

		while(dtotal < 16)
		{
			inc = rand()%52;
			card = hit(inc,deck);
			strcpy(csuit , suit(card));
			strcpy(cface , face(card));
			cval = cvalue(card);
			dtotal += cval;

			printf("The dealer got a %s of %s.\n", cface, csuit);
			if(dtotal < 16)
			{
				if(cval==1)
				{
					if(dtotal+11<21) dtotal += 10;
				}
			}
			else if(dtotal == 21)
			{
				printf("The dealer just got 21.\n");
				break;
			}
			else if(dtotal > 21)
			{
				printf("The dealer busted with %d! You win!\n", dtotal);
				break;
			}
			else
			{
				printf("The dealer stands with %d.\n", dtotal);
				break;
			}
			printf("The dealer's new total is %d.\n", dtotal);
		}

		if(dtotal<ptotal)
		{
			printf("\nYou beat the dealer! Your payout is %d.\n", (int)(*pot*1.5));
			*balance += (*pot*1.5);
		}

		else if(dtotal>=ptotal && dtotal <= 21)
		{
			printf("Oh no! Looks like the dealer won. Try again!\n");
		}
		else
		{
			printf("\nYou beat the dealer! Your payout is %d.\n", (int)(*pot*1.5));
			*balance += (*pot*1.5);
		}
	}

	FILE *logs;

	logs = fopen("logs.txt", "a"); //saves a log of the round

	if (logs == NULL) {
		printf("Unable to open %s\n", "scores");
	}
	else
	{
		fprintf(logs,"%d %d\n",ptotal,dtotal);
		fclose(logs);
	}

	if(*balance > 0)	printf("\nYour new balance is %d.\n", *balance);
}

//used in play function to decide if u want to have your ace as 11 or 1
void checkAce(int val, int *total)
{
	int input;

	if(*total<=10) //checks if its even nessasarry to check
	{
		if(val==1)
		{
			printf("You have an ace do you want it to be 11 or 1.\n>> ");
			while(1)
			{
				scanf("%d",&input);
				if(input==11)
				{
					printf("10 added to you total.\n");
					*total= *total + 10;
					break;
				}
				else if(input==1)
				{
					break;
				}
				else printf("You need to enter 11 or 1.\n");

			}
		}
	}

}

//used in play function to get value of the card u got
int cvalue(int val)
{
	if((val%13)<10)
	{
		return (val%13)+1;
	}
	else
	{
		return 10;
	}
}

//used in play function to get face of the card u got
char *face(int val)
{
	const char* cardFaces[] = {"Ace","Two","Three","Four","Five","Six","Seven","Eight","Nine","Ten","Jack","Queen","King"};

	return (char *)cardFaces [ val % 13 ];
}

//used in play function to get suit of the card u got
char *suit(int val)
{
	const char* cardSuits[] = {"Hearts", "Clubs", "Diamonds", "Spades"};

	return (char *)cardSuits [ val % 4 ];
}

//used in play function to "draw" a card
int hit(int i, int *deck)
{
	int a = deck[i];
	return a;
}

//used in play function to make a bet for the round
void bet(int *balance, int *pot)
{
	char input2[10];
	int val;
	printf("Enter an amount you would like to 'bet $' (max $=%d, min $=10)\n>> ",*balance);

	scanf("%s %d",input2,&val);

	while(strcmp(input2, "bet") != 0)
	{

		if(strcmp(input2,"bet")!=0)
		{
			printf("You've entered an invalid command. Type 'bet $' to place bet.\n");
		}

		printf(">> ");
		scanf("%s %d",input2,&val);
	}

	while(val > *balance || (val < 10  && val > 0) || val <=0){ //checks if you have enough balance
		if(val > *balance)
		{
			printf("You cannot bet more than your balance. Your balance is: %d.\n", *balance);
		}
		else if(val <= 0)
		{
			printf("You must make a bet. Enter a valid bet amount.\n");
		}
		else
		{
			printf("Invalid entry. Try again.\n");
		}
		printf("Enter valid bet (10-%d)\n>> ", *balance);
		scanf("%d", &val);
	}
	*balance = *balance - val;
	printf("You've made a bet of %d. Good luck!\n", val);
	*pot = val;
}

//used in play function to initialize the deck
void initialize(int *pdeck)
{
	for(int co=0;co<CARDS;co++)
	{
		pdeck[co] = co;
	}
}

//used to shuffle the deck before a turn
void shuffle(int *pdeck)
{

	for (int i = CARDS-1; i > 0; i--)
	{
		// Pick a random index from 0 to i
		int j = rand() % (i+1);

		// Swap arr[i] with the element at random index
		swap(&pdeck[i], &pdeck[j]);
	}
}

//used in suffle to help with shuffling
void swap (int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

//used to print last game scores
void logs(void)
{
	printf("\n");
	FILE *logs;

	int i = 0;
	char line[LINESIZE];
	int pscore;
	int dscore;


	logs = fopen("logs.txt", "r");

	if (logs == NULL) {
		printf("Unable to open %s\n", "scores");
	}
	else//prints log lines
	{
		while (!feof(logs))
		{
			if (fgets(line, LINESIZE, logs) != NULL)
			{
				if (sscanf(line, "%d %d", &pscore,&dscore) == 2) {
					i++;
					printf("%2d: %2d players end score, %2d dealers end score\n",i,pscore,dscore);
				}
				else {
					printf("Invalid line at line %d\n",i);
				}
			}
		}
		fclose(logs);
	}
	printf("\n");
}







