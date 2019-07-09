#include "board.h"

#define BOARD_SIZE 6

int i, j, max0, max1, max2, max3, tab_p[BOARD_SIZE][BOARD_SIZE], tab_pos[BOARD_SIZE][BOARD_SIZE], order[BOARD_SIZE];
int change, tab[BOARD_SIZE][BOARD_SIZE], clue[4][BOARD_SIZE];

void generate_board()
{
    for(i=0;i<BOARD_SIZE;i++)
    {
        for(j=0;j<BOARD_SIZE;j++)
                if(j+1+i<=BOARD_SIZE)
                    tab_p[i][j]=j+i+1;
                else
                    tab_p[i][j]=j+1+i-BOARD_SIZE;
        } //filling the basic board

    for( i=0;i<BOARD_SIZE;i++)
        order[i]=-1;//filling the table of order as empty

        for(i=0;i<BOARD_SIZE;i++)
        {
        change=rand()%BOARD_SIZE;
        while(order[change]!=-1)
            change=(change+1)%BOARD_SIZE;
        order[change]=i;
        }//determining the order of rows

        for(i =0;i<BOARD_SIZE;i++)
        {
            for(j=0;j<BOARD_SIZE;j++)
            {
                tab_pos[i][j]=tab_p[j][order[i]];
            }
        }//filling the intermediate table based on the basic table

        for(i=0;i<BOARD_SIZE;i++)
        order[i]=-1;//filling the table of order as empty

        for(i=0;i<BOARD_SIZE;i++)
        {
        change=rand()%BOARD_SIZE;
        while(order[change]!=-1)
            change=(change+1)%BOARD_SIZE;
        order[change]=i;
        }//determining the order of columns

        for(i =0;i<BOARD_SIZE;i++)
        {
            for(j=0;j<BOARD_SIZE;j++)
            {
                tab[i][j]=tab_pos[j][order[i]];
            }
        }


     for(i=0;i<BOARD_SIZE;i++)
        {
            clue[0][i]=clue[1][i]=clue[2][i]=clue[3][i]=0;
            max0=max1=max2=max3=0;

            for(j=0;j<BOARD_SIZE;j++)
            {
                if(tab[i][j]>max0)
                {
                    max0=tab[i][j];
                    clue[0][i]++;
                }//filling the table of left clues

                if(tab[j][i]>max2)
                {
                    max2=tab[j][i];
                    clue[2][i]++;
                }//filling the table of upper clues

            }

             for(j=BOARD_SIZE-1;j>=0;j--)
            {
                if(tab[i][j]>max1)
                {
                    max1=tab[i][j];
                    clue[1][i]++;
                }//filling the table of right clues

                if(tab[j][i]>max3)
                {
                    max3=tab[j][i];
                    clue[3][i]++;
                }//filling the table of bottom clues
            }
        }
}
