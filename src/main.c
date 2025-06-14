/*
 *  File: main.c
 *  Date: 4/9/2025
 *  Author: Andrew Ament
 *  Purpose: Make the card game UNO
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <ctype.h>
#include "setup.h"
#include "play.h"

// Define colors for text
#define RED "\033[1;31m"
#define YELLOW "\033[1;33m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"

int main(void) {

    // Initialize variables
    srand(time(NULL));
    int numPlayers;
    char junk;
    int numRead;
    int curPlayer;
    int minTime = 3000;
    int maxTime = 5000;
    char chosenColor[7];

    // Create Deck
    Deck* unoDeck = NULL;
    unoDeck = (Deck*) malloc(sizeof(Deck));
    if (unoDeck == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }
    CreateDeck(unoDeck);

    // Create discard pile
    Deck* discardPile = NULL;
    discardPile = (Deck*) malloc(sizeof(Deck));
    if (discardPile == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }
    discardPile->cards = malloc(108 * sizeof(Card));  // allocate space for up to 108 cards
    if (discardPile->cards == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }
    discardPile->size = 0;

    // Print start game screen
    StartGame();

    // Shuffle deck
    Shuffle(unoDeck);

    // Prompt number of players
    do {
        printf("How many players (2, 3, or 4): ");
        numRead = scanf("%d", &numPlayers);

        if (numRead != 1) {
            printf("Error: Invalid input.\n");
        } else if (numPlayers < 2 || numPlayers > 4) {
            printf("Error: Invalid player count.\n");
        }

        do {
            scanf("%c", &junk);
        } while(junk != '\n');

    } while(numPlayers < 2 || numPlayers > 4 || numRead != 1);

    // Create players' hands
    Deck** playerHands = NULL;
    playerHands = (Deck**) malloc(sizeof(Deck*) * numPlayers);
    if (playerHands == NULL) {
        printf("Error: Failed to allocate player hands\n");
        return 1;
    }

    for (int i = 0; i < numPlayers; i++) {
        playerHands[i] = malloc(sizeof(Deck));
        if (playerHands[i] == NULL) {
            printf("Error: Failed to allocate player hand\n");
            return 1;
        }
        playerHands[i]->cards = malloc(7 * sizeof(Card));
        if (playerHands[i]->cards == NULL) {
            printf("Error: Failed to allocate player cards\n");
            return 1;
        }
        playerHands[i]->size = 0;
        for (int j = 0; j < 7; j++) {
            DrawCard(numPlayers, playerHands[i], unoDeck, discardPile);
        }
    }

    // Decide which player will start
    curPlayer = rand() % numPlayers;

    // Set play direction (1 = clockwise, -1 = counter-clockwise)
    int direction = 1;
    
    // Set game status
    int gameOver = 0;

    // Set playerDrew flag
    int playerDrew;

    // Set first card - cannot be a skip, +2, reverse, wild, or +4 wild
    do {
        discardPile->cards[discardPile->size] = unoDeck->cards[unoDeck->size-1];
        discardPile->size++;
        unoDeck->size--;
    } while (discardPile->cards[discardPile->size-1].num > 9);

    do {
        // Current player's turn
        printf("It is player %d's turn\nThe card in play is: ", curPlayer+1);
        PrintCard(discardPile->cards[discardPile->size-1]);
        if (!strcmp(discardPile->cards[discardPile->size - 1].color, "black")) {
            if (!strcmp(chosenColor, "red")) {
                printf(RED " (%s)" RESET, chosenColor);
            } else if (!strcmp(chosenColor, "yellow")) {
                printf(YELLOW " (%s)" RESET, chosenColor);
            } else if (!strcmp(chosenColor, "green")) {
                printf(GREEN " (%s)" RESET, chosenColor);
            } else if (!strcmp(chosenColor, "blue")) {
                printf(BLUE " (%s)" RESET, chosenColor);
            }
        }
        printf("\n");

        // Check if player is human or computer
        if (curPlayer == 0) {
            playerDrew = HumanTurn(playerHands, numPlayers, playerHands[curPlayer], unoDeck, discardPile, chosenColor);
        } else {
            playerDrew = ComputerTurn(numPlayers, playerHands[curPlayer], unoDeck, discardPile, chosenColor, &curPlayer);
            Sleep(1000);       // Pauses program for 1000 milliseconds
        }

        // Check if the player has UNO
        if (playerHands[curPlayer]->size == 1) {

            // Give player time to say UNO accordingly
            if (curPlayer) {
                minTime = 0;
                maxTime = 2000;
            } else {
                minTime = 3000;
                maxTime = 5000;
            }
            
            DWORD timeOut = minTime + rand() % (maxTime - minTime);
            
            char buffer[25];

            // Prompt user to say UNO
            if (TimedInput(buffer, sizeof(buffer), timeOut, curPlayer)) {
                for (int i = 0; buffer[i]; i++) {
                    buffer[i] = tolower((unsigned char)buffer[i]);
                }

                if (!strcmp(buffer, "uno")) {
                    printf("\nYou said UNO in time!\n");
                    if (curPlayer) {
                        DrawCard(numPlayers, playerHands[curPlayer], unoDeck, discardPile);
                    }
                } else {
                    printf("\nYou did not say UNO!\n");
                    if (!curPlayer) {
                        DrawCard(numPlayers, playerHands[curPlayer], unoDeck, discardPile);
                    } 
                }
            } else {
                printf("\nYou did not say UNO in time!\n");
                if (!curPlayer) {
                    DrawCard(numPlayers, playerHands[curPlayer], unoDeck, discardPile);
                }
            }
        }

        // Check if a player won
        if (playerHands[curPlayer]->size == 0) {
            printf("Player %d wins!\n", curPlayer+1);
            gameOver = 1;
            break;
        }

        // Check for special condition cards
        CheckSkip(numPlayers, discardPile, &curPlayer, &direction, playerDrew);
        CheckDraw2(playerHands, numPlayers, unoDeck, discardPile, &curPlayer, &direction, playerDrew);
        CheckReverse(numPlayers, discardPile, &direction, playerDrew);
        CheckWild(playerHands, numPlayers, unoDeck, discardPile, chosenColor, &curPlayer, &direction, playerDrew);

        // Reveal the number of cards the computer player has
        if (curPlayer != 0) {
            printf("Player %d has %d cards in their hand\n", curPlayer+1, playerHands[curPlayer]->size);
            Sleep(1000);       // Pauses program for 1000 milliseconds
        }

        // Move to next player
        curPlayer += direction;

        // Check if curPlayer is out of bounds
        if (curPlayer >= numPlayers) {
            curPlayer = 0;
        } else if (curPlayer < 0) {
            curPlayer = numPlayers - 1;
        }
        
        printf("\n");

    } while (!gameOver);

    return 0;
}