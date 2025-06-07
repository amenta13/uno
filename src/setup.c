/*
 *  File: setup.c
 *  Date: 4/9/2025
 *  Author: Andrew Ament
 *  Purpose: Make the functions related to setting up the card game UNO
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include "setup.h"
#include "play.h"

// Define colors for text
#define RED "\033[1;31m"
#define YELLOW "\033[1;33m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"

/* Create cards:
 * Four colors: red, yellow, green, blue
 * One zero and two of each number 1-9 and action card (skip=10, +2=11, reverse=12) per color
 * Four wilds (black and 13) and four +4 wilds (black and 14)
 * Total of 108 cards
 */

Card CreateCard (Card card, int i, int j) {
    // Assign color to card
    switch (i) {
        case 0:
            strcpy(card.color,"red");
            break;
        case 1:
            strcpy(card.color,"yellow");
            break;
        case 2:
            strcpy(card.color,"green");
            break;
        case 3:
            strcpy(card.color,"blue");
            break;
        case 4:
            strcpy(card.color,"black");
            break;
    }
    // Assign number to card
    card.num = j;
    return card;
}

void CreateDeck (Deck* deck) {
    deck->size = 108;
    int numCard = 0;

    deck->cards = malloc(deck->size * sizeof(Card));
    if (deck->cards == NULL) {
        printf("Error: Memory allocation for cards failed\n");
        return;
    }

    for(int i = 0; i < 4; i++) {
        deck->cards[numCard] = CreateCard(deck->cards[numCard], i, 0);
        numCard++;
        for(int j = 1; j < 13; j++) {
            deck->cards[numCard] = CreateCard(deck->cards[numCard], i, j);
            numCard++;
            deck->cards[numCard] = CreateCard(deck->cards[numCard], i, j);
            numCard++;
        }
    }
    for(int i = 0; i < 4; i++) {
        deck->cards[numCard] = CreateCard(deck->cards[numCard],4, 13);
        numCard++;
    }
    for(int i = 0; i < 4; i++) {
        deck->cards[numCard] = CreateCard(deck->cards[numCard],4, 14);
        numCard++;
    }
}

void Shuffle (Deck* deck) {
    // Using Fisher-Yates Shuffle technique
    for (int i = deck->size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Card temp = deck->cards[i];
        deck->cards[i] = deck->cards[j];
        deck->cards[j] = temp;
    }
    printf("Deck Shuffled!\n");
}

void StartGame() {
    printf("\n");
    printf(RED    "=====================================\n");
    printf(YELLOW "|                                   |\n");
    printf(GREEN  "|           WELCOME TO              |\n");
    printf(BLUE   "|             U N O !               |\n");
    printf(RED    "|                                   |\n");
    printf(YELLOW "=====================================\n");
    printf(RESET "\n");
    printf("Match cards by color or number. Use special cards to turn the game around!\n");
    printf("First to run out of cards wins!\n");
    printf("\nPress Enter to begin...\n");

    char junk;
    do {
        scanf("%c", &junk);
    } while (junk != '\n');
}

void PrintCard(Card card) {
    // Check card color
    if (!strcmp(card.color, "red")) {

        // Check card type
        if (card.num == 10) {
            printf(RED "%s skip" RESET, card.color);
        } else if (card.num == 11) {
            printf(RED "%s +2" RESET, card.color);
        } else if (card.num == 12) {
            printf(RED "%s reverse" RESET, card.color);
        } else {
            printf(RED "%s %d" RESET, card.color, card.num);
        }

    } else if (!strcmp(card.color, "yellow")) {

        // Check card type
        if (card.num == 10) {
            printf(YELLOW "%s skip" RESET, card.color);
        } else if (card.num == 11) {
            printf(YELLOW "%s +2" RESET, card.color);
        } else if (card.num == 12) {
            printf(YELLOW "%s reverse" RESET, card.color);
        } else {
            printf(YELLOW "%s %d" RESET, card.color, card.num);
        }

    } else if (!strcmp(card.color, "green")) {

        // Check card type
        if (card.num == 10) {
            printf(GREEN "%s skip" RESET, card.color);
        } else if (card.num == 11) {
            printf(GREEN "%s +2" RESET, card.color);
        } else if (card.num == 12) {
            printf(GREEN "%s reverse" RESET, card.color);
        } else {
            printf(GREEN "%s %d" RESET, card.color, card.num);
        }

    } else if (!strcmp(card.color, "blue")) {

        // Check card type
        if (card.num == 10) {
            printf(BLUE "%s skip" RESET, card.color);
        } else if (card.num == 11) {
            printf(BLUE "%s +2" RESET, card.color);
        } else if (card.num == 12) {
            printf(BLUE "%s reverse" RESET, card.color);
        } else {
            printf(BLUE "%s %d" RESET, card.color, card.num);
        }

    } else if (!strcmp(card.color, "black")) {

        // Check card type
        if (card.num == 13) {
            printf("wild");
        } else if (card.num == 14) {
            printf("wild +4");
        }

    } else {
        printf("%s %d", card.color, card.num); // fallback
    }
}

// Thread function for reading user input
DWORD WINAPI InputThread(LPVOID userInput) {
    char *buffer = (char*)userInput;
    fgets(buffer, 25, stdin);
    return 0;
}