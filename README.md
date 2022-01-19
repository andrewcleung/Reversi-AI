# APS105 Reversi AI project


## Description

----

This project was a school assignment that includes the following requirements

1. Visualise the Reversi game board on the terminal.  

2. Allows two players to compete against each other.

3. Design a bot that evaluates the position using a simple heuristic function and compete against the player.

4. (Optional) Enhance the bot and compete against bots that were created by other students

    * The runtime of the bot is limited under 1 second

## Technical Challenges

-----

### Optimizing the algorithm

In order to enhance the bot, I have chosen to implement the [minimax](https://www.geeksforgeeks.org/minimax-algorithm-in-game-theory-set-1-introduction/) algorithm. However, two main problem has arised as

1. Minimax's runtime limits the strengh of the bot

2. There are no pre-built dynamic data structure for storing the result of the positions.

#### Addressing minimax problem 1

By using [alpha-beta pruning](https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning) , I have able to eliminate positions that doesn't have to be analysed, which allows me to increase the depth of the bot from 3 to 5 within the runtime limit

#### Addressing minimax problem 2

Instead of storing the position, I utilise the fact that only the best position with the highest score is needed. In order to address the problem:

1. Create a **minimax** helper function that **only** returns the score of the position, since we do not care about where are the preceeding positions are.

2. Use a `for` loop to evaluate all the current position position, and check if the position has a higher score than the current one.

### Heuristic Function

----

The Heuristic Function determines how does the computer determines if the move is "good". The better the position, the higher the score it has, which feds into the minimax algorithm. However, the criteria of analysis relies on:

1. Including all the criteria that should be analysed

2. Having optimal parameters for the analysis.

#### Addressing Heruistic Function problem 1

I tried playing a few games with my family to understand the important factors of winning the game. Includes:

* Corners are the most important squares

* Squares around the corners are the weakeest squares

* Do not try to grab an edge before securing a corner

* Position that are next to the edge are also weak

#### Addressing Heruistic Function problem 2

After obtaining the criteria for analysis, I need to adjust the parameters to make sure not to over/under estimate the score of the position. I created two computer bots to play against each other, and adjusting the parameters manually to observe the win rate of the specific combination of the parameters. (which is the `reversi.c` file in this repo)


## Possible improvements

### Algorithm

----

1. Using a more advance algorithm to analyse the position, in order to increase the efficiency

2. Switch to `c++` lol

### Heuristic

----

1. Instead of manually adjusting the parameters, use a ML model to complete the testing and obtain the parameters

2. Explore more criters to analyse for the position

### Code reusabiliy

----

1. The code utilise the boolean flag in the  function `checkLegalFlipDirection(tempBoard, i, j, 'B', n, true);` to both check and place the position on the board. A helper function with more definite purpose should be created.

2. More descriptive comments should be implemented

3. Utilise version control (Haven't learn git version control at the moment)