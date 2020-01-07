
void play(int *deck, int balance, int round);

void quit( int round, int balance);

void scores(void);

//used to sort scorelist in scores
int sortscore (const void * a, const void * b);

void cont(int *balance, int *round);

void turn(int *balance, int *pot, int *deck);

void checkAce(int val, int *total);

int cvalue(int v);

//get card suit
char *suit(int val);

//get card face
char *face(int val);

//get card ^for face and suit
int hit(int i, int *deck);

void bet(int *balance, int *pot);

void initialize(int *pdeck);

void shuffle(int *pdeck);

//used in shuffle
void swap (int *a, int *b);

void logs(void);
