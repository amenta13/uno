# Welcome to UNO
created by Andrew Ament

This is a command line interface only version of the card game UNO written in C language.
The game follows the classic rules of UNO. A special ruleset may be added in the future.

### Setup:
- Ensure that you have a compiler installed such as MYSYS2 @ https://www.msys2.org/
- You will need both gcc and make in order to run and play the game
- Simply type "make run" in your terminal to compile and start the game

### How To Play:
- First choose how many players (currently supporting up to 4 total players, you included!)
- The game will choose a random player to go first.
- Before each turn, the current player and card in play will be displayed.
- On your turn, choose which card to play with the associated number to the left of the card description or draw with "0".
- When playing a wild card (wild or wild +4), choose a color to make the wild card using the associated number to the left of the color when prompted. 
- If you drew, your drawn card will be shown. You will be prompted to play the card you drew if possible. Answer using "Y" for yes or "N" for no (not case sensitive)
- During your opponents' turns, you will see the card they played or if they drew followed by an updated number of how many cards are left in their hand.
- If during a turn a card is played that forces another player to draw (+2 or wild +4), the player who drew's total number of cards will be revealed.
- When a player (you or an opponent) gets to one card remaining, you will be prompted to type in "UNO" (not case sensative) as fast as you can. The amount of time you have varies depending on if you or your opponent has one card left.
