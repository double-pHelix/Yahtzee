/*
 Description : COMP1917 16s2 Assignment 1 - Yahtzee Program
              (http://www.cse.unsw.edu.au/~cs1917/16s2/ass/1/spec.pdf)
 */
#include <stdio.h>
#include <stdlib.h>

#define NUM_DICE 6
#define NUM_DICE_VAL 7
#define SUCCESS 0
#define ERROR 1

#define REROLL_SOME 1
#define REROLL_ALL 2
#define KEEP 3

int diceInputAll(int* diceValues);
int diceValuation(int* diceValues);
void printDiceValues(int* diceValues);
int existsInArray(int* array, int val, int max);
void intialiseArray(int* array, int size);
int rerollChoice();
int diceInputSome(int* diceValues);

int main(void) {
	setbuf(stdout, NULL); //to prevent having to fflush() all the time
	int diceValues[NUM_DICE];

	if(diceInputAll(diceValues) == ERROR){
		return EXIT_FAILURE;
	}
	printDiceValues(diceValues);

	int userChoice = rerollChoice();
	while(userChoice != KEEP){

		if(userChoice == REROLL_SOME){
			if(diceInputSome(diceValues) == ERROR){
				return EXIT_FAILURE;
			}
		} else if(userChoice == REROLL_ALL){
			if(diceInputAll(diceValues) == ERROR){
				return EXIT_FAILURE;
			}
		} else {
			//invalid choice
			return EXIT_FAILURE;
		}

		printDiceValues(diceValues);
		userChoice = rerollChoice();
	}

	if(diceValuation(diceValues) == ERROR){
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}

void intialiseArray(int* array, int size){
	int i = 0;
	while(i < size){
		array[i] = 0;
		i++;
	}
}

int diceInputAll(int* diceValues){
	printf("Please enter dice values:\n");
	char c = 0;
	int i = 1;

	c = getchar();
	if(c == '\n'){ //to prevent getchar reading in from buffer the previous "enter" and leaving loop early
		c = ' ';
	}
	while (c != '\n' && i < NUM_DICE){
		if(c >= '1' && c <= '6'){
			diceValues[i] = c - '0';
			i++;
		} else if(c == '0' || (c >= '7' && c <= '9')){
			printf("Dice Number Out of Range.\n");
			return ERROR;
		} else if (c != ' '){
			printf("Incorrect Input Format.\n");
			return ERROR;
		}
		c = getchar();
	}

	if(i != NUM_DICE){
		//not enough input
		printf("Incorrect Number of Values.\n");
		return ERROR;
	}

	printf("\n");
	return SUCCESS;
}

int rerollChoice(){
	printf("Please choose:\n");
	printf("1 -> Reroll some dice\n");
	printf("2 -> Reroll all dice\n");
	printf("3 -> Keep dice\n");
	int userChoice = 0;
	scanf("%d", &userChoice);
	if(!(userChoice >= 1 && userChoice <=3)){
		printf("Invalid Choice.\n");
	}
	
	printf("\n");
	return userChoice;
}

int diceInputSome(int* diceValues){
	char c;
	int i = 0;
	int numEntered = 0;
	int diceToReroll[NUM_DICE];
	intialiseArray(diceToReroll, NUM_DICE);

	printf("Please enter dice to reroll (1 - 5):\n");
	c = getchar();
	if(c == '\n'){ //to prevent getchar reading in from buffer the previous "enter" and leaving loop early
		c = ' ';
	}
	while (c != '\n' && i < NUM_DICE){
		if(!existsInArray(diceToReroll, (c - '0'), numEntered)){
			if(c >= '1' && c <= '5'){
				diceToReroll[i] = c - '0';
				i++;
				numEntered++;
			} else if (c != ' '){
				printf("Incorrect Input Format.\n");
				return ERROR;
			} else if(c == '0' || (c >= '6' && c <= '9')){
				printf("Dice Number Out of Range.\n");
				return ERROR;
			}
		}
		c = getchar();
	}

	printf("\n");
	i = 0;
	printf("Please enter %d values\n", numEntered);
	c = getchar();
	if(c == '\n'){ //to prevent getchar reading in from buffer the previous "enter" and leaving loop early
		c = ' ';
	}
	while (c != '\n' && i <= numEntered){
		if(c >= '1' && c <= '6'){
			int diceToChange = diceToReroll[i];
			diceValues[diceToChange] =  c - '0';
			i++;
		} else if (c != ' '){
			printf("Incorrect Input Format.\n");
			c = '\n'; //break;
			return ERROR;
		} else if(c == '0' || (c >= '7' && c <= '9')){
			printf("Dice Number Out of Range.\n");
			return ERROR;
		}
		c = getchar();
	}

	printf("\n");
	return SUCCESS;
}

int existsInArray(int* array, int val, int max){
	int i = 0;
	while(i <= max){
		if(array[i] == val){
			return 1;
		}
		i++;
	}
	return 0;
}

void printDiceValues(int* diceValues){
	int i = 1;
	printf("Your dice values are:");
	while(i < NUM_DICE){
		printf("%d ", diceValues[i]);
		i++;
	}
	printf("\n");
}

int diceValuation(int* diceValues){
	int diceValFreq[NUM_DICE_VAL];
	intialiseArray(diceValFreq, NUM_DICE_VAL);
	int scoreChoice[6];
	int i = 1;

	while(i < NUM_DICE){
		if(diceValues[i] == 1){
			diceValFreq[1]++;
		} else if(diceValues[i] == 2){
			diceValFreq[2]++;
		} else if(diceValues[i] == 3){
			diceValFreq[3]++;
		} else if(diceValues[i] == 4){
			diceValFreq[4]++;
		} else if(diceValues[i] == 5){
			diceValFreq[5]++;
		} else if(diceValues[i] == 6){
			diceValFreq[6]++;
		}
		i++;
	}

	//calculate the sum
	int sum = 0;
	i = 1;
	while(i < NUM_DICE_VAL){
		sum = sum + diceValFreq[i]*i;
		i++;
	}

	printf("Your score options are:\n");
	int currChoiceNum = 1;
	//Three of a Kind
	i = 1;
	while(i < NUM_DICE_VAL){
		if(diceValFreq[i] == 3){
			printf("%d -> Three of a Kind ", currChoiceNum);
			printf("(%d points)\n", sum);
			scoreChoice[currChoiceNum] = sum;
			i = NUM_DICE_VAL; //to break
			currChoiceNum++;
		}
		i++;
	}

	//Four of a Kind
	i = 1;
	while(i < NUM_DICE_VAL){
		if(diceValFreq[i] == 4){
			printf("%d -> Four of a Kind ", currChoiceNum);
			printf("(%d points)\n", sum);
			scoreChoice[currChoiceNum] = sum;
			i = NUM_DICE_VAL; //to break
			currChoiceNum++;
		}
		i++;
	}

	//Full house
	i = 1;
	while(i < NUM_DICE_VAL){
		if(diceValFreq[i] == 3){
			i = 1;
			while(i < NUM_DICE_VAL){
				if(diceValFreq[i] == 2){
					printf("%d -> Full House (25 points)\n", currChoiceNum);
					scoreChoice[currChoiceNum] = 25;
					i = NUM_DICE_VAL; //to break
					currChoiceNum++;
				}
				i++;
			}
		}
		i++;
	}

	//Small Straight/Straight
	i = 1;
	int currConseq = 0;
	int currCombo = 1;
	while(i < NUM_DICE){
		if(currConseq != 0){
			if(diceValues[i] == currConseq + 1){
				currCombo++;
			} else {
				currCombo = 1;
			}
		}
		currConseq = diceValues[i];
		i++;
	}
	if(currCombo >= 4){
		printf("%d -> Small Straight (30 points)\n", currChoiceNum);
		scoreChoice[currChoiceNum] = 30;
		currChoiceNum++;
	}
	if(currCombo >= 5){
		printf("%d -> Straight (40 points)\n", currChoiceNum);
		scoreChoice[currChoiceNum] = 40;
		currChoiceNum++;
	}

	//Yahtzee
	i = 1;
	while(i < NUM_DICE_VAL){
		if(diceValFreq[i] == 5){
			printf("%d -> Yahtzee (50 points)\n", currChoiceNum);
			scoreChoice[currChoiceNum] = 50;
			i = NUM_DICE_VAL; //to break
			currChoiceNum++;
		}
		i++;
	}

	//Chance
	printf("%d -> Chance", currChoiceNum);
	printf("(%d points)\n", sum);
	scoreChoice[currChoiceNum] = sum;

	printf("\n");
	//User Chooses
	int userChoice = 0;
	scanf("%d", &userChoice);
	if(userChoice > 0 && userChoice <= currChoiceNum){
		printf("Your score is: %d points\n", scoreChoice[userChoice]);
	} else {
		printf("Invalid Choice.\n");
		return ERROR;
	}

	return SUCCESS;
}
