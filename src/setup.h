/*
 *  File: setup.c
 *  Date: 4/9/2025
 *  Author: Andrew Ament
 *  Purpose: Make the structures and header functions related to setting up the card game UNO
 */

#include <windows.h>

typedef struct Card_Struct {
    char color[7];
    int num;
} Card;

typedef struct Deck_Struct {
    Card* cards;
    int size;
} Deck;

Card CreateCard (Card card, int i, int j);
void CreateDeck (Deck* deck);
void Shuffle (Deck* deck);
void StartGame();
void PrintCard(Card card);
int TimedInput(char *buffer, size_t bufsize, DWORD timeOut);