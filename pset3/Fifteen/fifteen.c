/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */

#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();

        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }

    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).
 */
void init(void)
{
    // TODO - FOR EACH ROW AND FOR EACH COLUMN SET VAUE FOR TILE
    int max_size = d*d;
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            board[i][j] = --max_size;
        }
    }

    //IF MAX_SIZE IS EVEN AND NUMBER OF TILES ODD SWAP 1 AND 2 SO THAT GAME CAN BE WON
   if ((d*d) % 2 == 0)
    {
        int swap = board[d - 1][d - 2];
        board[d - 1][d - 2] = board[d - 1][d - 3];
        board[d - 1][d - 3] = swap;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // ITERATE OVER THE ROWS AND COLUMMNS
    for (int i = 0; i < d; i++)
    {
        for( int j = 0; j < d; j++)
        {
            if (board[i][j] < 10)
            {
                // IF NUMBER IS 0 PRINT SPACE
                if (board[i][j] == 0)
                {
                    printf ("_");
                }
                else
                {
                    printf ("%d", board[i][j]);
                }
            }
        }
        printf ("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false.
 */
bool move(int tile)
{
    int row;
    int col;
    int space;


    // CHECK VALIDITY OF TILE
    if (tile > (d*d) - 1 || tile < 1)
    {
        return false;
    }
    // SEARCH BOARD FOR ROW AND COLUMN
    for (int i = 0; i < d; i++)
    {
        for( int j = 0; j < d; j++)
        {
            if (board[i][j] == tile)
            {
                row = i;
                col = j;
            }
        }
    }
    // SEARCH THROUGH ROWS AND COLUMNS AGAIN
    for (int i = 0; i < d; i++)
    {
        for( int j = 0; j < d; j++)
        {
            if ( 0 == board[i][j])
            {
                space = board[i][j];
                // CHECK IF SPACES NEXT TO EACH TILE IS EMPTY
                if ((row == i && col == j + 1) || (row == i && col == j - 1) ||
                   (col == j && row == i + 1) || (col == j && row == i - 1))
                   {
                       // SWAP TILE WITH BLANK TILE
                       int swap = tile;
                       tile = space;
                       space = swap;
                       // REASSIGN BLANK TILE VALUES
                       board[row][col] = tile;
                       board[i][j] = space;
                       return true;
                   }
                else
                {
                    return false;
                }
            }
        }
    }
return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration),
 * else false.
 */
bool won(void)
{
    int number = 0;

    // CHECK ORDER OF ROWS AND TILES
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            // IF ANY TILE DOES NOT EQUAL NUMBER COUNTING UP FROM 0 - RETURN FALSE
            if (++number != (d*d) && board[i][j] != number)
            {
                return false;
            }
        }
    }
    //OTHERWISE ALL TILES COUNT UP FROM 0 AND GAME IS WON
    return true;
}
