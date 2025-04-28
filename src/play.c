/*
 *  File: setup.c
 *  Date: 4/9/2025
 *  Author: Andrew Ament
 *  Purpose: Make the functions relating to playing the card game UNO
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "setup.h"
#include "play.h"

// Define colors for text
#define RED "\033[1;31m"
#define YELLOW "\033[1;33m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"

// Draw one card
void DrawCard(int numPlayers, Deck* player, Deck* deck, Deck* discard) {
    if (deck->size == 0) {
        printf("Draw pile is empty!\n");
        RebuildDeck(numPlayers, deck, discard);
        Shuffle(deck);
        if (deck->size == 0) {
            printf("No cards remaining in deck.\n");
            return;
        }
    }

    // Take the top card from the draw pile
    Card drawn = deck->cards[deck->size-1];
    deck->size--;

    // Reallocate player hand to hold one more card
    Card* temp = realloc(player->cards, sizeof(Card) * (player->size + 1));
    if (temp == NULL) {
        printf("Error: Failed to allocate memory for player hand\n");
        exit(1);
    }
    player->cards = temp;

    player->cards[player->size] = drawn;
    player->size++;
}

void RebuildDeck(int numPlayers, Deck* deck, Deck* discard) {
    // Check discard pile
    if (discard->size <= 1) {
        printf("No cards to draw\n");
        return;
    }

    // Save previously played card
    Card topCard = discard->cards[discard->size - 1];

    // Create temp deck to rebuild
    Card* newDeck = malloc(sizeof(Card) * discard->size-1);
    if (newDeck == NULL) {
        printf("Error: New deck allocation failed\n");
        return;
    }

    // Copy over all cards except for previously played card
    for (int i = 0; i < discard->size-1; i++) {
        newDeck[i] = discard->cards[i];
    }

    free(deck->cards);

    deck->cards = newDeck;
    deck->size = discard->size-1;

    discard->cards[0] = topCard;
    discard->size = 1;
}

int HumanTurn(Deck** playerList,  int numPlayers, Deck* playerHand, Deck* deck, Deck* discard, char* chosenColor) {
    int userChoice;
    int numRead;
    char junk;

    // Display user's hand
    printf("Your cards:\n");
    for (int i = 0; i < playerHand->size; i++) {
        printf("%d: ", i+1);
        PrintCard(playerHand->cards[i]);
        printf("\n");
    }
    printf("\n");

    int validTurn = 0;

    do {
        // Prompt user for their turn
        printf("Choose a card to play (1, 2, ...) or draw (0): ");
        numRead = scanf("%d", &userChoice);

        // Check user input
        if (numRead != 1) {
            printf("Invalid input\n");
        } else if (userChoice < 0 || userChoice > playerHand->size) {
            printf("Invalid input\n");
        } else if (userChoice == 0) {
            DrawCard(numPlayers, playerHand, deck, discard);
            printf("You drew a card\n");
            validTurn = 1;
            return 1;
        } else {
            if (CheckCard(&playerHand->cards[userChoice-1], &discard->cards[discard->size-1], chosenColor)) {
                PlayCard(playerHand, discard, userChoice-1, chosenColor);
                validTurn = 1;
                return 0;
            } else {
                printf("That card cannot be played\n");
            }
        }

        do {
            scanf("%c", &junk);
        } while(junk != '\n');

    } while (!validTurn);
    return 0;
}

int ComputerTurn(int numPlayers, Deck* playerHand, Deck* deck, Deck* discard, char* chosenColor, int * curPlayer) {
    int validIndices[playerHand->size];
    int validCount = 0;

    // Scan the hand for valid cards if valid, save index to validIndices and increment validCount
    for (int i = 0; i < playerHand->size; i++) {
        if (CheckCard(&playerHand->cards[i], &discard->cards[discard->size - 1], chosenColor)) {
            validIndices[validCount] = i;
            validCount++;
        }
    }

    // Randomly choose one of the valid cards to play
    if (validCount > 0) {
        // Randomly choose a valid card to play
        int randCard = rand() % validCount;  // Get a random index from valid cards

        // Play the randomly chosen valid card
        int cardIndex = validIndices[randCard];
        PlayCard(playerHand, discard, cardIndex, chosenColor);
        printf("Player %d played: ", *curPlayer+1);
        PrintCard(discard->cards[discard->size-1]);
        printf("\n");
        return 0;
    } else {
        // Draw a card if no valid cards are found
        DrawCard(numPlayers, playerHand, deck, discard);
        printf("Player %d drew a card\n", *curPlayer+1);
        return 1;
    }
}

int CheckCard(Card* userCard, Card* inPlay, char* chosenColor) {
    if (!strcmp(userCard->color, inPlay->color) || userCard->num == inPlay->num || !strcmp(userCard->color, "black")) {
        return 1;
    } else if (!strcmp(inPlay->color, "black") && !strcmp(userCard->color, chosenColor)) {
        return 1;
    } else {
        return 0;
    }
}

void PlayCard(Deck* playerHand, Deck* discard, int cardIndex, char* chosenColor) {
    // Move card from the player's hand to the discard pile
    Card playedCard = playerHand->cards[cardIndex];

    // Add card to the discard pile
    discard->cards[discard->size] = playedCard;
    discard->size++;

    // Remove the card from the player's hand
    for (int i = cardIndex; i < playerHand->size - 1; i++) {
        playerHand->cards[i] = playerHand->cards[i + 1];
    }
    playerHand->size--;

    // Resize hand
    Card* temp = realloc(playerHand->cards, sizeof(Card) * playerHand->size);
    if (temp == NULL && playerHand->size > 0) { // Only check if size > 0
        printf("Error: Failed to resize player hand\n");
        exit(1);
    }
}

void CheckSkip(int numPlayers, Deck* discard, int* curPlayer, int* direction, int playerDrew) {
    // Check if played card is a skip
    if (discard->cards[discard->size-1].num == 10 && !playerDrew) {

        // Increment curPlayer
        *curPlayer += *direction;

        // Check if curPlayer is still in bounds
        if (*curPlayer >= numPlayers) {
            *curPlayer = 0;
        } else if (*curPlayer < 0) {
            *curPlayer = numPlayers - 1;
        }
    }    
}

void CheckDraw2(Deck** playerList, int numPlayers, Deck* deck, Deck* discard,int* curPlayer, int* direction, int playerDrew) {
    // Check if played card is a +2
    if (discard->cards[discard->size-1].num == 11 && !playerDrew) {

        // Reveal the number of cards the computer player has
        if (curPlayer != 0) {
            printf("Player %d has %d cards in their hand\n", *curPlayer+1, playerList[*curPlayer]->size);
        }

        // Increment curPlayer
        *curPlayer += *direction;

        // Check if curPlayer is still in bounds
        if (*curPlayer >= numPlayers) {
            *curPlayer = 0;
        } else if (*curPlayer < 0) {
            *curPlayer = numPlayers - 1;
        }

        // Next player draws two cards
        for (int i = 0; i < 2; i++) {
            DrawCard(numPlayers, playerList[*curPlayer], deck, discard);
        }
    }    
}

void CheckReverse(int numPlayers, Deck* discard, int* direction, int playerDrew) {

    // Check if played card is a reverse
    if (discard->cards[discard->size-1].num == 12 && !playerDrew) {

        // Flip direction
        *direction *= -1;
    }
}

void CheckWild(Deck** playerList, int numPlayers, Deck* deck, Deck* discard, char* chosenColor, int* curPlayer, int* direction, int playerDrew) {
    int userChoice;
    int numRead;
    char junk;

    // Check if played card is a wild
    if (!strcmp(discard->cards[discard->size-1].color, "black") && playerDrew == 0) {
        // If human, prompt for color
        if (*curPlayer == 0) {
            do {
                printf("Colors:\n");
                printf(RED "1. red\n" RESET);
                printf(YELLOW "2. yellow\n" RESET);
                printf(GREEN "3. green\n" RESET);
                printf(BLUE "4. blue\n" RESET);
                printf("Choose a color to change it to: ");
                numRead = scanf("%d", &userChoice);

                if (numRead != 1) {
                    printf("Invalid input");
                } else if (userChoice < 1 || userChoice > 4) {
                    printf("Invalid input");
                } else {
                    switch (userChoice) {
                        case 1:
                            strcpy(chosenColor, "red");
                            printf("You chose ");
                            printf(RED "%s\n" RESET, chosenColor);
                            break;
                        case 2:
                            strcpy(chosenColor, "yellow");
                            printf("You chose ");
                            printf(YELLOW "%s\n" RESET, chosenColor);
                            break;
                        case 3:
                            strcpy(chosenColor, "green");
                            printf("You chose ");
                            printf(GREEN "%s\n" RESET, chosenColor);
                            break;
                        case 4:
                            strcpy(chosenColor, "blue");
                            printf("You chose ");
                            printf(BLUE "%s\n" RESET, chosenColor);
                            break;
                    }
                    
                }

                do {
                    scanf("%c", &junk);
                } while(junk != '\n');
            } while (numRead != 1 || userChoice < 1 || userChoice > 4);
        } else {

            // Smart selection using weighted random selection
            // Initialize card color counts
            int redCards = 0;
            int yellowCards = 0;
            int greenCards = 0;
            int blueCards = 0;

            // Scan the hand for the number of colors of cards
            for (int i = 0; i < playerList[*curPlayer]->size; i++) {
                if (strcmp(playerList[*curPlayer]->cards[i].color, "red")) {
                    redCards++;
                } else if (strcmp(playerList[*curPlayer]->cards[i].color, "yellow")) {
                    yellowCards++;
                } else if (strcmp(playerList[*curPlayer]->cards[i].color, "green")) {
                    greenCards++;
                } else if (strcmp(playerList[*curPlayer]->cards[i].color, "blue")) {
                    blueCards++;
                }
            }

            // Choose a weighted random color
            userChoice = rand() % (redCards + yellowCards + greenCards + blueCards);
            if (userChoice < redCards) {
                strcpy(chosenColor, "red");
                printf("Player %d chose ", *curPlayer+1);
                printf(RED "%s\n" RESET, chosenColor);
            } else if (userChoice < redCards + yellowCards) {
                strcpy(chosenColor, "yellow");
                printf("Player %d chose ", *curPlayer+1);
                printf(YELLOW "%s\n" RESET, chosenColor);
            } else if (userChoice < redCards + yellowCards + greenCards) {
                strcpy(chosenColor, "green");
                printf("Player %d chose ", *curPlayer+1);
                printf(GREEN "%s\n" RESET, chosenColor);
            } else {
                strcpy(chosenColor, "blue");
                printf("Player %d chose ", *curPlayer+1);
                printf(BLUE "%s\n" RESET, chosenColor);
            }
        }
    }

    // Check if it is a +4 wild
    if (discard->cards[discard->size-1].num == 14) {

        // Reveal the number of cards the computer player has
        if (curPlayer != 0) {
            printf("Player %d has %d cards in their hand\n", *curPlayer+1, playerList[*curPlayer]->size);
        }

        // Increment curPlayer
        *curPlayer += *direction;
        
        // Check if curPlayer value is still in bounds
        if (*curPlayer >= numPlayers) {
            *curPlayer = 0;
        } else if (*curPlayer < 0) {
            *curPlayer = numPlayers - 1;
        }

        // Next player draw four cards
        for (int i = 0; i < 4; i++) {
            DrawCard(numPlayers, playerList[*curPlayer], deck, discard);
        }
    }
}