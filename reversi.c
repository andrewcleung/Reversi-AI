/*
 * File:        project_reversi_skeleton.c
 * Author:      APS105H1 Teaching Team
 * Modified by: * Andrew Chapman Leung *
 *
 * Date: 14 Mar 2021
 */

#include "project_reversi_skeleton.h" // DO NOT modify this line
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

// Note: You may want to add more function declarations here
// =========   Function declaration  ==========
void initializeBoard(char board[][26], int n);
void configBoard(char board[][26], int n);
void checkLegalInDirectionHelper(char board[][26], int dim, char colour);
bool checkLegalInDirectionHelper2(char board[][26], int dim, int row, int col, char colour);
int checkLegalFlipDirection(char board[][26], int row, int col, char colour, int n, bool flip);
char oppositeColour(char colour);
int flipInDirection(char board[][26], int row, int col, char colour, int n, int deltaRow, int deltaCol, bool flip);
bool isGameEnded(char board[][26], int dim);
void boardCopy(const char board[][26], char tempBoard[][26], int dim);
int countTiles(char board[][26], int dim, char colour);
char checkVictory(char board[][26], int dim);
bool isValidMove(char board[][26], int dim, char colour);
//minimax
void makeMoveHelper(char board[][26], int dim, int row, int col, char colour);
int miniMax(char board[][26], int n, char turn, int depth, int alpha, int beta, int parameter[][26]);
int getScore(char board[][26], int n, int parameter[][26], char turn);
void getParameter(int board[][26], int n);
int getLegalMoves(char board[][26], int n, char turn);
int min(int a, int b);
int max(int a, int b);
int cornerStab(char board[][26], int n, int parameter[][26]);

// ========= Function implementation ==========

/*
 * Function:  initializeBoard
 * --------------------
 * initizalize the Board
 */

void initializeBoard(char board[][26], int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            board[i][j] = 'U';
        }
    }
    //intilize white
    board[n / 2 - 1][n / 2 - 1] = 'W';
    board[n / 2][n / 2] = 'W';
    //intilize black
    board[n / 2 - 1][n / 2] = 'B';
    board[n / 2][n / 2 - 1] = 'B';

    return;
}

/*
 * Function:  printBoard 
 * --------------------
 * Print the whole board to the terminal window.
 */
void printBoard(char board[][26], int n)
{
    //initalize the first row
    printf("  "); //prints two spaces
    for (int j = 0; j < n; j++)
        printf("%c", 'a' + j);

    for (int i = 0; i < n; i++)
    {
        printf("\n");
        printf("%c ", 'a' + i);
        for (int j = 0; j < n; j++)
        {
            printf("%c", board[i][j]);
        }
    }
    return;
}
//debug
void printBoard2(int board[][26], int n)
{
    //initalize the first row
    printf("  "); //prints two spaces
    for (int j = 0; j < n; j++)
        printf("%c", 'a' + j);

    for (int i = 0; i < n; i++)
    {
        printf("\n");
        printf("%c ", 'a' + i);
        for (int j = 0; j < n; j++)
        {
            printf("%d", board[i][j]);
        }
    }
    return;
}

/*
 * Function:  positionInBounds 
 * --------------------
 * Checks whether the specified (row, col) lies within the board dimensions.
 */
bool positionInBounds(int n, int row, int col)
{

    if (row >= 0 && row < n && col >= 0 && col < n)
        return true;

    return false;
}

/*
 * Function:  checkLegalInDirection 
 * --------------------
 * Checks whether (row, col) is a legal position for a tile of colour by “looking” in the direction 
 * specified by deltaRow and deltaCol.
 */
bool checkLegalInDirection(char board[][26], int n, int row,
                           int col, char colour, int deltaRow, int deltaCol)
{

    //opposite Colour
    char oppColour = oppositeColour(colour);

    /* 
    This part could be modified to use a parameter instead of stepcount
    However, it is kept to see if there is a need to use this method in the future
    */
    int stepRow = 0;
    int stepCol = 0;
    stepRow = deltaRow;
    stepCol = deltaCol;

    if (board[row][col] == 'U')
    {
        while ((positionInBounds(n, row + stepRow, col + stepCol)) && (board[row + stepRow][col + stepCol] == oppColour))
        {
            /* as going into the whileloop, the colour is alredy been checked once. Thus, the incrementing process will be done once before checking the colour again*/

            stepRow += deltaRow;
            stepCol += deltaCol;
            if (board[row + stepRow][col + stepCol] == colour) //start checking after checking the first entry
                return true;
        }
    }
    return false;
}
/*
 * Function:  checkLegalInDirectionHelper
 * --------------------
 * Helps checkLegalInDirection function to scan the entire board. Returns the number of possible tiles
 */
void checkLegalInDirectionHelper(char board[][26], int dim, char colour)
{

    for (int row = 0; row < dim; row++)
    {
        for (int col = 0; col < dim; col++)
        {
            if (checkLegalInDirectionHelper2(board, dim, row, col, colour))
            {
                printf("\n%c", 'a' + row);
                printf("%c", 'a' + col);
            }
        }
    }
    return;
}

/*
 * Function:  checkLegalInDirectionHelper
 * --------------------
 * Helps checkLegalInDirectionHelpoer function to scan the entire board. Returns the number of possible tiles
 */
bool checkLegalInDirectionHelper2(char board[][26], int dim, int row, int col, char colour)
{
    for (int dRow = -1; dRow <= 1; dRow++)
    {
        for (int dCol = -1; dCol <= 1; dCol++)
        {
            if (!(dRow == 0 && dCol == 0))
            {
                if (checkLegalInDirection(board, dim, row, col, colour, dRow, dCol))
                    return true;
            }
        }
    }
    return false;
}

/*
 * Function:  configBoard 
 * --------------------
 * configure the board
 */
void configBoard(char board[][26], int dim)
{

    int count = 0;
    char ch;
    char pos[3];

    do
    {
        for (int i = 0; i < 3; i++)
        {
            if ((ch = getchar()) != '\n')
                pos[i] = ch;
            else
                i = -1; //reset to i=0, i is set to -1 as the for loop will increment the i
        }
        getchar(); //for eliminating the \n buffer

        for (int i = 0; i < 3; i++)
        {
            if (pos[i] == '!')
                count++;
        }

        if (positionInBounds(dim, (int)(pos[1] - 'a'), (int)(pos[2] - 'a')))
            board[(int)pos[1] - 'a'][(int)pos[2] - 'a'] = pos[0];

        for (int i = 0; i < 3; i++)
        {
            pos[i] = '\0';
        }
    } while (count < 3);
    return;
}

/*
 * Function:  checkLegalFlipDirection
 * --------------------
 * Check if the position is valid, if so, and bool flip ==true, flip
 * Returns the score of the position, use for determining the computer AI
 */
int checkLegalFlipDirection(char board[][26], int row, int col, char colour, int n, bool flip)
{
    int countFlip = 0;
    for (int dRow = -1; dRow <= 1; dRow++)
    {
        for (int dCol = -1; dCol <= 1; dCol++)
        {
            if (!(dRow == 0 && dCol == 0))
            {
                if (checkLegalInDirection(board, n, row, col, colour, dRow, dCol))
                {
                    countFlip += flipInDirection(board, row, col, colour, n, dRow, dCol, flip);
                }
            }
        }
    }
    if ((flip) && (countFlip))
        board[row][col] = colour;
    return countFlip;
}

/*
 * Function:  flipInDirection
 * --------------------
 * if flip == true, flip the tiles
 * return the number of increment for score calculation
 */
int flipInDirection(char board[][26], int row, int col, char colour, int n, int deltaRow, int deltaCol, bool flip)
{

    int countFlip = 0;
    char oppColour = oppositeColour(colour);
    do
    {
        countFlip++;
        if (flip)
        {
            board[row + deltaRow * countFlip][col + deltaCol * countFlip] = colour;
        }
    } while (board[row + deltaRow * (countFlip + 1)][col + deltaCol * (countFlip + 1)] == oppColour);

    return countFlip;
}

/*
 * Function:  isGameEnded
 * --------------------
 * Check is the game finished
 */
bool isGameEnded(char board[][26], int dim)
{
    for (int row = 0; row < dim; row++)
    {
        for (int col = 0; col < dim; col++)
        {
            for (int dRow = -1; dRow <= 1; dRow++)
            {
                for (int dCol = -1; dCol <= 1; dCol++)
                {
                    if (!(dRow == 0 && dCol == 0))
                    {
                        if (checkLegalInDirection(board, dim, row, col, 'B', dRow, dCol) || checkLegalInDirection(board, dim, row, col, 'W', dRow, dCol))
                        {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

/*
 * Function:  IsValidMove
 * --------------------
 * Check if there is still any valid move for the player
 */
bool isValidMove(char board[][26], int dim, char colour)
{
    for (int row = 0; row < dim; row++)
    {
        for (int col = 0; col < dim; col++)
        {
            for (int dRow = -1; dRow <= 1; dRow++)
            {
                for (int dCol = -1; dCol <= 1; dCol++)
                {
                    if (!(dRow == 0 && dCol == 0))
                    {
                        if (checkLegalInDirection(board, dim, row, col, colour, dRow, dCol))
                            return true;
                    }
                }
            }
        }
    }
    return false;
}

/*
 * Function:  boardCopy
 * --------------------
 * copy the constant board to a tempoary board
 */
void boardCopy(const char board[][26], char tempBoard[][26], int dim)
{

    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            tempBoard[i][j] = board[i][j];
        }
    }
    return;
}

/*
 * Function:  boardCopy2
 * --------------------
 * copy the  board to a tempoary board
 */
void boardCopy2(const char board[][26], char tempBoard[][26], int dim)
{

    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            tempBoard[i][j] = board[i][j];
        }
    }
    return;
}

/*
 * Function:  countTiles
 * --------------------
 * copy the constant board to a tempoary board
 */
int countTiles(char board[][26], int dim, char colour)
{

    int count = 0;
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            if (board[i][j] == colour)
                count++;
        }
    }
    return count;
}

/*
 * Function:  checkVictory
 * --------------------
 * check who wins the game
 */
char checkVictory(char board[][26], int dim)
{
    if (isGameEnded(board, dim))
    {
        if (countTiles(board, dim, 'B') > countTiles(board, dim, 'W'))
            return 'B';
        else if (countTiles(board, dim, 'B') < countTiles(board, dim, 'W'))
            return 'W';
        else
            return 'T';
    }
    return 'U';
}

/*
 * Function:  oppositeColour
 * --------------------
 * returns the opposite colour
 */
char oppositeColour(char colour)
{
    if (colour == 'W')
        return 'B';
    else
        return 'W';
}

/*
 * Function:  makeMove 
 * --------------------
 * The player "turn" makes a move at (row, col).
 * Note: This function MUST NOT change board, only row and col can be changed to reflect the move.
 */
int makeMove(const char board[26][26], int n, char turn, int *row, int *col)
{

    typedef struct aiMove
    {
        int x;
        int y;
        int score;
    } AImove;

    *row = 0;
    *col = 0;
    int count = 0;

    //depth of the AI, modify this to increase/decrrease the strength of the AI
    int depth = 4;

    //temparary for evaluation
    char tempBoard[26][26];
    int parameter[26][26];
    AImove bestEval, eval;
    //generating the parameter for the board
    getParameter(parameter, n);
    boardCopy(board, tempBoard, n);

    if (turn == 'W')
    {
        bestEval.score = -10000000;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (checkLegalInDirectionHelper2(tempBoard, n, i, j, 'W'))
                {
                    checkLegalFlipDirection(tempBoard, i, j, 'W', n, true);
                    eval.x = i;
                    eval.y = j;
                    eval.score = miniMax(tempBoard, n, 'B', depth, -10000000, 10000000, parameter);
                    boardCopy(board, tempBoard, n);
                    if (eval.score >= bestEval.score)
                    {
                        bestEval.score = eval.score;
                        bestEval.x = eval.x;
                        bestEval.y = eval.y;
                    }
                }
            }
        }
    }

    else
    {
        bestEval.score = 10000000;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (checkLegalInDirectionHelper2(tempBoard, n, i, j, 'B'))
                {
                    checkLegalFlipDirection(tempBoard, i, j, 'B', n, true);
                    eval.x = i;
                    eval.y = j;
                    eval.score = miniMax(tempBoard, n, 'W', depth, -10000000, 10000000, parameter);
                    boardCopy(board, tempBoard, n);
                    if (eval.score <= bestEval.score)
                    {
                        bestEval.score = eval.score;
                        bestEval.x = eval.x;
                        bestEval.y = eval.y;
                    }
                }
            }
        }
    }
    *row = bestEval.x;
    *col = bestEval.y;
    return bestEval.score;
}

/* 
 * Function:  minimax 
 * --------------------
 * Find the optimal move for the position
 */
int miniMax(char board[][26], int n, char turn, int depth, int alpha, int beta, int parameter[][26])    
{

    int eval = 0;
    char tempBoard[26][26];
    boardCopy2(board, tempBoard, n);

    //base cases
    //victory
    if (checkVictory(board, n) == 'W')
        return 10000000;
    else if (checkVictory(board, n) == 'B')
        return -10000000;
    else if (checkVictory(board, n) == 'T')
        return 0;   
    //maximum depth
    else if (depth <= 0)
    {
        return getScore(board, n, parameter, turn);
    }
    //no move for one player
    else if (turn == 'B' && isValidMove(board, n, 'W') == false)
        goto minimizer;
    else if (turn == 'W' && isValidMove(board, n, 'B') == false)
        goto maximizer;

    //maximize the value
    if (turn == 'W')
    {
    maximizer:;
        int maxEval = -10000000;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (checkLegalInDirectionHelper2(tempBoard, n, i, j, 'W'))
                {
                    checkLegalFlipDirection(tempBoard, i, j, 'W', n, true);
                    eval = miniMax(tempBoard, n, 'B', depth - 1, alpha, beta, parameter); //alpha beta pruning
                    boardCopy2(board, tempBoard, n);
                    maxEval = max(maxEval, eval);
                    alpha = max(alpha, maxEval);
                    if (beta <= alpha)
                        goto finishMax;
                }
            }
        }
    finishMax:;
        return maxEval;
    }
    //minimize the value
    else
    {
    minimizer:;
        int minEval = 10000000;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (checkLegalInDirectionHelper2(tempBoard, n, i, j, 'B'))
                {
                    checkLegalFlipDirection(tempBoard, i, j, 'B', n, true);
                    eval = miniMax(tempBoard, n, 'W', depth - 1, alpha, beta, parameter); //alpha beta pruning
                    boardCopy2(board, tempBoard, n);
                    minEval = min(minEval, eval);
                    beta = min(beta, minEval);
                    if (beta <= alpha)
                        goto finishMin;
                }
            }
        }
    finishMin:;
        return minEval;
    }
}

/* 
 * Function:  min
 * --------------------
 * output the minimum of the two
 */
int min(int a, int b)
{
    if (a < b)
        return a;
    else if (a > b)
        return b;
    else
        return a;
}

/* 
 * Function:  max
 * --------------------
 * output the maximum of the two
 */
int max(int a, int b)
{
    if (a < b)
        return b;
    else if (a > b)
        return a;
    else
        return a;
}

/* 
 * Function:  getScore
 * --------------------
 * get the score for the board position
 */
int getScore(char board[][26], int n, int parameter[][26], char turn)
{
    int score = 0;
    //disk locations
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (board[i][j] == 'B')
            {
                score -= parameter[i][j];
            }
            if (board[i][j] == 'W')
            {
                score += parameter[i][j];
            }
        }
    }
    score += 2 * cornerStab(board, n, parameter);

    //Legal moves available
    if (turn == 'W')
    {
        score += 100 * n * (getLegalMoves(board, n, 'W') - getLegalMoves(board, n, 'B'));
    }
    else
    {
        score -= 100 * n * (getLegalMoves(board, n, 'B') - getLegalMoves(board, n, 'W'));
    }
    return score;
}

/* 
 * Function:  cornerStab
 * --------------------
 * get the score for the board position
 */
int cornerStab(char board[][26], int n, int parameter[][26])
{
    int score = 0;
    //black
    if (board[0][0] == 'B')
    {
        for (int i = 0; i <= 1; i++)
        {
            for (int j = 0; j <= 1; j++)
            {
                if (board[i][j] == 'B' && i != 0 && j != 0)
                {
                    score += 3 * parameter[i][j];
                }
            }
        }
    }
    if (board[0][n - 1] == 'B')
    {
        for (int i = 0; i <= 1; i++)
        {
            for (int j = n - 2; j <= n - 1; j++)
            {
                if (board[i][j] == 'B' && i != 0 && j != 0)
                {
                    score += 3 * parameter[i][j];
                }
            }
        }
    }
    if (board[n - 1][0] == 'B')
    {
        for (int i = n - 2; i <= n - 1; i++)
        {
            for (int j = 0; j <= 1; j++)
            {
                if (board[i][j] == 'B' && i != 0 && j != 0)
                {
                    score += 3 * parameter[i][j];
                }
            }
        }
    }
    if (board[n - 1][n - 1] == 'B')
    {
        for (int i = n - 2; i <= n - 1; i++)
        {
            for (int j = n - 2; j <= n - 1; j++)
            {
                if (board[i][j] == 'B' && i != 0 && j != 0)
                {
                    score += 3 * parameter[i][j];
                }
            }
        }
    }
    //white
    if (board[0][0] == 'W')
    {
        for (int i = 0; i <= 1; i++)
        {
            for (int j = 0; j <= 1; j++)
            {
                if (board[i][j] == 'W' && i != 0 && j != 0)
                {
                    score -= 3 * parameter[i][j];
                }
            }
        }
    }
    if (board[0][n - 1] == 'W')
    {
        for (int i = 0; i <= 1; i++)
        {
            for (int j = n - 2; j <= n - 1; j++)
            {
                if (board[i][j] == 'W' && i != 0 && j != 0)
                {
                    score -= 3 * parameter[i][j];
                }
            }
        }
    }
    if (board[n - 1][0] == 'W')
    {
        for (int i = n - 2; i <= n - 1; i++)
        {
            for (int j = 0; j <= 1; j++)
            {
                if (board[i][j] == 'W' && i != 0 && j != 0)
                {
                    score -= 3 * parameter[i][j];
                }
            }
        }
    }
    if (board[n - 1][n - 1] == 'W')
    {
        for (int i = n - 2; i <= n - 1; i++)
        {
            for (int j = n - 2; j <= n - 1; j++)
            {
                if (board[i][j] == 'W' && i != 0 && j != 0)
                {
                    score -= 3 * parameter[i][j];
                }
            }
        }
    }
    return score;
}

/* 
 * Function:  getLegalMoves
 * --------------------
 * output the number of the legal moves for a colour
 */
int getLegalMoves(char board[][26], int n, char turn)
{
    int count = 0;

    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            for (int dRow = -1; dRow <= 1; dRow++)
            {
                for (int dCol = -1; dCol <= 1; dCol++)
                {
                    if (!(dRow == 0 && dCol == 0))
                    {
                        if (checkLegalInDirection(board, n, row, col, turn, dRow, dCol))
                            count++;
                    }
                }
            }
        }
    }
    return count;
}

/* 
 * Function:  getParameter
 * --------------------
 * generate the parameter for the board using an array
 */
void getParameter(int parameter[][26], int n)
{

    //parameters, change these values to modify the AI
    const int edge = 2 * n;
    const int edgePlus = 5 * n;
    const int edgeMiddle = 2 * n;
    const int other = 10 * n;
    const int corner = 150 * n;
    const int xposEdge = -70 * n;
    const int xposDiag = -100 * n;

    //edges
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            parameter[i][j] = edge;
        }
    }

    //edge+1
    for (int i = 1; i < n - 1; i++)
    {
        for (int j = 1; j < n - 1; j++)
        {
            parameter[i][j] = edgePlus;
        }
    }

    //others
    for (int i = 2; i < n - 2; i++)
    {
        for (int j = 2; j < n - 2; j++)
        {
            parameter[i][j] = other;
        }
    }

    //middle edge
    for (int i = (n / 2) - 1; i <= (n / 2); i++)
    {
        for (int j = 0; j < n; j += (n - 1))
        {
            parameter[i][j] = edgeMiddle;
        }
    }

    for (int j = (n / 2) - 1; j <= n / 2; j++)
    {
        for (int i = 0; i < n; i += (n - 1))
        {
            parameter[i][j] = edgeMiddle;
        }
    }

    //corner
    for (int i = 0; i < n; i += (n - 1))
    {
        for (int j = 0; j < n; j += (n - 1))
        {
            parameter[i][j] = corner;
        }
    }
    //x position
    for (int i = 1; i < n - 1; i += (n - 3))
    {
        for (int j = 1; j < n - 1; j += (n - 3))
        {
            parameter[i][j] = xposDiag;
        }
    }

    for (int i = 0; i < n; i += (n - 1))
    {
        for (int j = 1; j < n - 1; j += (n - 3))
        {
            parameter[i][j] = xposEdge;
        }
    }
    for (int i = 1; i < n - 1; i += (n - 3))
    {
        for (int j = 0; j < n; j += (n - 1))
        {
            parameter[i][j] = xposEdge;
        }
    }
}

/*
 * Function:  makeMoveHelper
 * --------------------
 * make the move for the computer using the row and col returned
 */
void makeMoveHelper(char board[][26], int dim, int row, int col, char colour)
{
    checkLegalFlipDirection(board, row, col, colour, dim, true);
    return;
}

#ifndef COMPETE_MODE // DO NOT DELETE THIS LINE
int main(void)
{
    // Complete your main function here

    //variable initialization

    //The playing board that contains the ACTUAL value of each player
    char playBoard[26][26];

    //dimension of the board
    int dim = 0;

    //player and computer colour, and the winner
    char playerColour, computerColour;

    //The row and column for input
    int row, col = 0;

    //Boolean variable for determining if we are only analysing or actually fliping the board
    bool flipOrNot = false;

    //DEBUG
    int a;

    //Board dimension and initialization
    printf("Enter the board dimension: ");
    scanf("%d", &dim);
    initializeBoard(playBoard, dim);
    getchar();

    //computer colour
    printf("Computer plays (B/W) : ");
    scanf("%c", &computerColour);
    playerColour = oppositeColour(computerColour);
    getchar();
    printBoard(playBoard, dim);

    /*------------------ lab 7
    //board configuration
    printf("\nEnter board configuration:\n");
    configBoard(playBoard, dim);
    printBoard(playBoard,dim);

    //prinf possible moves
    printf("\nAvailable moves for W:");
    checkLegalInDirectionHelper(playBoard,dim,'W');
    printf("\nAvailable moves for B:");
    checkLegalInDirectionHelper(playBoard,dim,'B');
--------------------*/

    //black goes first
    if (computerColour == 'B')
    {
        makeMove(playBoard, dim, computerColour, &row, &col);
        makeMoveHelper(playBoard, dim, row, col, computerColour);
        printf("\nComputer places %c at %c%c.\n", computerColour, row + 'a', col + 'a');
        printBoard(playBoard, dim);
    }

    do
    {

        // colour = getchar(); Lab 7
        /* 
        if (isValidMove(playBoard, dim, playerColour)){
            printf("\nEnter move for colour %c (RowCol): ", playerColour);
            row = getchar()-'a';
            col = getchar()-'a';
            getchar();
            flipOrNot = true;

            if (checkLegalFlipDirection(playBoard, row, col, playerColour, dim, flipOrNot)){
                printBoard(playBoard,dim);
            }
            else{
                printf("Invalid move.\n");
                printf("%c player wins.\n", computerColour);
                return 0;
            }
        }
        else
            printf("\n%c player has no valid move.\n", playerColour);

        */
        int totalTime;
        struct rusage usage;       // a structure to hold "resource usage" (including time)
        struct timeval start, end; // will hold the start and end times
        getrusage(RUSAGE_SELF, &usage);
        start = usage.ru_utime;
        double timeStart = start.tv_sec + start.tv_usec / 1000000.0; // in seconds
        // PLACE THE CODE YOU WISH TO TIME HERE
        if (isValidMove(playBoard, dim, playerColour))
        {
            a = makeMove(playBoard, dim, playerColour, &row, &col);
            printf("%d%d", row, col);
            makeMoveHelper(playBoard, dim, row, col, playerColour);
            printf("\nComputer1 places %c at %c%c%d.\n", playerColour, row + 'a', col + 'a', a);
            printBoard(playBoard, dim);
        }
        else if (isGameEnded(playBoard, dim) == false)
            printf("\n%c player has no valid move.", playerColour);
        getrusage(RUSAGE_SELF, &usage);
        end = usage.ru_utime;
        double timeEnd = end.tv_sec + end.tv_usec / 1000000.0; // in seconds double
        totalTime = timeEnd - timeStart;                       // totalTime now holds the time (in seconds) it takes to run your code
        printf("total time %d", totalTime);
        if (totalTime >= 1)
        {
            printf("\nreduce depth, terminating program");
            return 0;
        }

        /*
        if (isValidMove(playBoard, dim, playerColour)){
            a= makeMove(playBoard, dim, playerColour, &row, &col);
            makeMoveHelper(playBoard, dim, row, col, playerColour);
            printf("\nComputer1 places %c at %c%c%d.\n", playerColour, row+'a', col+'a',a);
            printBoard(playBoard,dim);
        }
        else if(isGameEnded(playBoard,dim)==false)
            printf("\n%c player has no valid move.", playerColour);
        */
        if (isValidMove(playBoard, dim, computerColour))
        {
            a = makeMove(playBoard, dim, computerColour, &row, &col);
            makeMoveHelper(playBoard, dim, row, col, computerColour);
            printf("\nComputer2 places %c at %c%c%d.\n", computerColour, row + 'a', col + 'a', a);
            printBoard(playBoard, dim);
        }
        else if (isGameEnded(playBoard, dim) == false)
            printf("\n%c player has no valid move.", computerColour);

    } while (isGameEnded(playBoard, dim) == false);

    if (checkVictory(playBoard, dim) == 'B')
        printf("\nB player wins.");
    else if (checkVictory(playBoard, dim) == 'W')
        printf("\nW player wins.");
    else
        printf("\nDraw!");

    return 0;
}
#endif // DO NOT DELETE THIS LINE
