/*
 *  File: setup.c
 *  Date: 4/9/2025
 *  Author: Andrew Ament
 *  Purpose: Make the header functions related to playing the card game UNO
 */

 // Function headers
 void DrawCard(int numPlayers, Deck* player, Deck* deck, Deck* discard);
 void RebuildDeck(int numPlayers, Deck* deck, Deck* discard);
 int HumanTurn(Deck** playerList,  int numPlayers, Deck* playerHand, Deck* deck, Deck* discard, char* chosenColor);
 int ComputerTurn(int numPlayers, Deck* playerHand, Deck* deck, Deck* discard, char* chosenColor, int * curPlayer);
 int CheckCard(Card* userCard, Card* inPlay, char* chosenColor);
 void PlayCard(Deck* playerHand, Deck* discard, int cardIndex, char* chosenColor);
 void CheckSkip(int numPlayers, Deck* discard,int* curPlayer, int* direction, int playerDrew);
 void CheckDraw2(Deck** playerList, int numPlayers, Deck* deck, Deck* discard,int* curPlayer, int* direction, int playerDrew);
 void CheckReverse(int numPlayers, Deck* discard, int* direction, int playerDrew);
 void CheckWild(Deck** playerList, int numPlayers, Deck* deck, Deck* discard, char* chosenColor, int* curPlayer, int* direction, int playerDrew);