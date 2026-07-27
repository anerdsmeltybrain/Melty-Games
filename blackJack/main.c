#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct hand {
  int * cards;
  int capacity;
  int counter;
};

//hand functions
void initHand(struct hand *);
void addCard(struct hand *);
void sumPrintHand(struct hand *);
int sumHand(struct hand *);


int main() {
  srand(time(NULL));

  int balance = 1000;
  struct hand player;
  struct hand dealer;

  printf("Welcome to the table :D\n");

init:
  initHand(&player);
  initHand(&dealer);

  while(balance < 5000 && balance > 50) {
    char buffer[2048];
    char answer;
    int bet = 0;

    printf("Bet amount? min $50: ");
    fgets(buffer, 1, stdin);
    sscanf(buffer, "%d", &bet);

    while(bet < 50) {
      printf("Please enter a valid amount\n");
      printf("Bet amount? min $50: ");
      scanf("%d", &bet);
    }

    sumPrintHand(&player);
    printf("Would you like to take a card?(y)(n): \n");
    scanf("%c", &answer);

    if(tolower(answer) == 'y') {
      addCard(&player);
    }

    if(tolower(answer) == 'n') {
      while(sumHand(&dealer) < 17) {
        printf("dealer's hand");
        sumPrintHand(&dealer);
        addCard(&dealer);
    }
        
      printf("player hand : %d | dealer hand : %d\n", sumHand(&player), sumHand(&dealer));
      if((sumHand(&player) < sumHand(&dealer) && sumHand(&dealer) <= 21) || (sumHand(&player) > 21)) {
        balance -= bet;
      } else if ((sumHand(&dealer) < sumHand(&player) && sumHand(&player) <= 21) || (sumHand(&dealer) > 21)) {
        balance += (bet * 2);
      }
      goto init;
    }

  }

  if (balance > 10000) {
    printf("YOU WIN!!!... gtfo~\n");
  } else {
    printf("YOU LOSE HA HA!!!!!!!!!\n");
  }

  return 0;
}

void initHand(struct hand * h) {
  h->cards = malloc(2 * sizeof(int));
  h->capacity = 2;
  h->counter = 1;

  h->cards[0] = rand() % 11;
  h->cards[1] = rand() % 10;
}

void addCard(struct hand * h) {

  h->capacity++;
  h->counter++;

  h->cards = realloc(h->cards, h->capacity * sizeof(int));

  h->cards[h->counter] = rand() % 11;
}

void sumPrintHand(struct hand * h) {

  int sum = 0;

  for(int i  = 0; i <= h->counter; i++) {
    sum += h->cards[i];
  }


  printf("Current Hand : %d\n", sum);
}

int sumHand(struct hand * h) {
  
  int sum = 0;

  for(int i  = 0; i <= h->counter; i++) {
    sum += h->cards[i];
  }

  return sum;
}

