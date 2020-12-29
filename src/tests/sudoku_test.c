#include <string.h>
#include <stdio.h>
#include "types.h"

#define BOARD_SIZE 9
#define BLOCK_SIZE 3
#define BLANK_CELL 0xFF

void print_board(U8 * board)
{
  for (U32 block_row = 0; block_row < BLOCK_SIZE; block_row++)
  {
      for (U32 i = 0; i < (BOARD_SIZE*3 + BLOCK_SIZE*2); i++)
      {
        putchar('-');
      }
      putchar('\n');
      for (U32 cell_row = block_row*BLOCK_SIZE; cell_row < (block_row+1)*BLOCK_SIZE; cell_row++)
      {
        for (U32 block_column = 0; block_column < BLOCK_SIZE; block_column++)
        {
          putchar('|');
          for (U32 cell_col = block_column*BLOCK_SIZE; cell_col < (block_column+1)*BLOCK_SIZE; cell_col++)
          {
            putchar(' ');
            U8 val = board[cell_col + cell_row*BOARD_SIZE];
            if (val == BLANK_CELL)
            {
              putchar(' ');
            }
            else
            {
              putchar('0' + val);
            }
            putchar(' ');
          }
          putchar('|');
        }
        putchar('\n');
      }
      for (U32 i = 0; i < (BOARD_SIZE*3 + BLOCK_SIZE*2); i++)
      {
        putchar('-');
      }
      putchar('\n');
  }
}

U8 test_board(U8 * board)
{
  U32 errors = 0;
  
  // Test rows
  U8 tests[BOARD_SIZE];
  for (U32 i = 0; i < BOARD_SIZE; i++)
  {
    memset(tests, 0, BOARD_SIZE);
    for (U32 j = 0; j < BOARD_SIZE; j++)
    {
      U8 val = board[i*BOARD_SIZE + j];
      if (val > BOARD_SIZE)
      {
        errors++;
      }
      else
      {
        if (tests[val] > 0)
        {
          errors++;
        }
        tests[val] = 1;
      }
    }
  }
  
  // Test cols
  for (U32 i = 0; i < BOARD_SIZE; i++)
  {
    memset(tests, 0, BOARD_SIZE);
    for (U32 j = 0; j < BOARD_SIZE; j++)
    {
      U8 val = board[i + j*BOARD_SIZE];
      if (val > BOARD_SIZE)
      {
        errors++;
      }
      else
      {
        if (tests[val] > 0)
        {
          errors++;
        }
        tests[val] = 1;
      }
    }
  }
  
  // Test blocks
  for (U32 i = 0; i < BLOCK_SIZE; i++)
  {
    for (U32 j = 0; j < BLOCK_SIZE; j++)
    {
      memset(tests, 0, BOARD_SIZE);
      for (U32 k = 0; k < BLOCK_SIZE; k++)
      {
        for (U32 l = 0; l < BLOCK_SIZE; l++)
        {
          U8 val = board[(i*BLOCK_SIZE + k)*BOARD_SIZE + (j*BLOCK_SIZE + l)];
          if (val > BOARD_SIZE)
          {
            errors++;
          }
          else
          {
            if (tests[val] > 0)
            {
              errors++;
            }
            tests[val] = 1;
          }
        }
      }
    }
  }
}

void gen_board()
{
  
}

int main(int argc, char * argv[])
{
  U8 board[BOARD_SIZE*BOARD_SIZE];
  memset(board, BLANK_CELL, sizeof(board));
  
  print_board(board);

}
