/*
	Projekt: gra Skyscrapers, projekt 2 z PRI
	Autor: Magdalena Zych
	Data: 12.2017
*/
#include<gtk/gtk.h>
#include<stdlib.h> //rand
#include<time.h> //srand

#include "board.h"

#define BOARD_SIZE 6
#define WINDOW_WIDTH 300
#define SMALL_WINDOW_WIDTH 250
#define SMALL_WINDOW_HEIGHT 250

const char* labels[BOARD_SIZE+1] = {"1", "2", "3", "4", "5", "6", "C"};
const char* marked_labels[BOARD_SIZE] = {"<b>1</b>", "<b>2</b>", "<b>3</b>", "<b>4</b>", "<b>5</b>", "<b>6</b>"};

int i, j, k, i_chosen, j_chosen, k_chosen, correct=0;

struct grid
    {
        int corr;
        int corr_filled;
        GtkWidget *button;
    } sqr[BOARD_SIZE][BOARD_SIZE];//elements of the board

void rules_click(GtkWidget *pressed, GtkWidget *wind);
void board_click(GtkWidget *pressed, GtkWidget *numb_buttons);
void numb_click(GtkWidget *pressed, GtkWidget *numb_buttons);
void tab_to_struct();//rewriting the table to the structure
void console_show_board();
void create_main_window();
void create_rules_window();
void create_board();
void create_new_board();
void new_board();
void show_solution();
void nothing(){};
void create_end_window();
void hide_window();
void win();

extern int clue[4][BOARD_SIZE];
extern int tab[BOARD_SIZE][BOARD_SIZE];

GtkWidget *put_numb[BOARD_SIZE+1], *menu_rules, *rules_ok, *menu_new_board, *menu_solution, *menu_exit, *end_new_board, *end_exit;//buttons
GtkWidget *rules_window, *window, *end_window;//windows
GtkWidget *rules_container, *container, *end_container;
GtkWidget *box;//box with the buttons on the right side
GtkWidget *clue_label[4][BOARD_SIZE], *rules_label, *end_label;//labels

int main(int argc, char* argv[])
{
    srand(time(NULL));
    gtk_init(&argc, &argv);

    create_main_window();
    create_rules_window();
    create_end_window();

    generate_board();
    tab_to_struct();
    //console_show_board();//if you want to see the solution on the console
    create_board();

    gtk_widget_show_all(window);//all widgets in the window are visible
    gtk_widget_hide(box);//at the beginning the right buttons should not be visible

    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(rules_ok), "clicked", G_CALLBACK(hide_window), rules_window);//hiding the window with rules
    g_signal_connect(G_OBJECT(rules_window), "delete-event", G_CALLBACK(nothing), NULL);
    g_signal_connect(G_OBJECT(end_window), "delete-event", G_CALLBACK(nothing), NULL);

    g_signal_connect(menu_rules, "clicked", G_CALLBACK(rules_click), rules_window);
    g_signal_connect(menu_new_board, "clicked", G_CALLBACK(new_board), NULL);
    g_signal_connect(menu_solution, "clicked", G_CALLBACK(show_solution), NULL);
    g_signal_connect(menu_exit, "clicked", G_CALLBACK(gtk_main_quit), NULL);

    g_signal_connect(end_exit, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(end_new_board, "clicked", G_CALLBACK(new_board), NULL);

    for(i=0;i<BOARD_SIZE;i++)
    {
        for(j=0;j<BOARD_SIZE;j++)
        {
            g_signal_connect(sqr[i][j].button, "clicked", G_CALLBACK(board_click), box);
        }
    }

    for(k=0;k<BOARD_SIZE+1; k++)
    {
            g_signal_connect(put_numb[k], "clicked", G_CALLBACK(numb_click), box);
    }

    gtk_main();
    return 0;
}

void console_show_board()//showing the solution on the console
{
    for(i=0;i<BOARD_SIZE;i++)
        {
            for(j=0;j<BOARD_SIZE;j++)
            printf("%d ", sqr[i][j].corr);

            printf("\n");
        }
        printf("\n\n");
}

void tab_to_struct()//filling in the structure
{
    for(i=0;i<BOARD_SIZE;i++)
        {
            for(j=0;j<BOARD_SIZE;j++)
            {
            sqr[i][j].corr=tab[i][j];
            sqr[i][j].corr_filled=0;
            }
        }
}

void board_click(GtkWidget *pressed, GtkWidget *numb_buttons)//when you press one of the buttons creating the board
{
    int x, y;
    gtk_widget_show(numb_buttons);//right-hand buttons become visible
    for (y=0;y<BOARD_SIZE;y++)
    {
        for(x=0;x<BOARD_SIZE;x++)
        {
            if(pressed==sqr[y][x].button)
            {
                i_chosen=y;
                j_chosen=x;
                break;
            }
        }
    }
}

void numb_click(GtkWidget *pressed, GtkWidget *numb_buttons)//when you press one of the right-hand buttons
{
    gtk_widget_hide(numb_buttons);

    int z;
    for(z=0;z<BOARD_SIZE+1;z++)
        {
            if(pressed==put_numb[z])
                k_chosen=z;
        }

    if(k_chosen==6)
    {
        gtk_button_set_label (GTK_BUTTON(sqr[i_chosen][j_chosen].button), "");

        if(sqr[j_chosen][i_chosen].corr_filled)
        {
            sqr[j_chosen][i_chosen].corr_filled=0;
            correct--;
        }
    }
    else
    {
        gtk_button_set_label (GTK_BUTTON(sqr[i_chosen][j_chosen].button), labels[k_chosen]);

        if(sqr[j_chosen][i_chosen].corr_filled)
        {
            if(sqr[j_chosen][i_chosen].corr!=k_chosen+1)
            {
                sqr[j_chosen][i_chosen].corr_filled=0;
                correct--;
            }
        }
        else
        {
            if(sqr[j_chosen][i_chosen].corr==k_chosen+1)
            {
                sqr[j_chosen][i_chosen].corr_filled=1;
                correct++;
            }
        }
    }

    //printf("%d", correct);//when you want to show on the console the number of the correctly filled grids
    if(correct==36)//when the board is filled correctly
    {
        for(i=0; i<BOARD_SIZE;i++)
        for(j=0;j<BOARD_SIZE;j++)
            {
            gtk_widget_set_sensitive(sqr[j][i].button, FALSE);
            }
        win();
    }
}
void rules_click(GtkWidget *pressed, GtkWidget *wind)
{
    gtk_widget_show_all(wind);
}
void create_main_window()
{
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_title (GTK_WINDOW(window), "Skyscrapers");
    gtk_widget_set_size_request(window, WINDOW_WIDTH, WINDOW_WIDTH);
    gtk_window_set_resizable (GTK_WINDOW(window), FALSE);//changing the size of the window is impossible

    container = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window),container);

    box = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(container),box);

    for(i=0;i<BOARD_SIZE+1;i++)
    {
        put_numb[i]= gtk_button_new_with_label (labels[i]);
        gtk_widget_set_size_request(put_numb[i], 30, 30);
        gtk_fixed_put(GTK_FIXED(box), put_numb[i], 250,80+i*30);
    }//the right-hand buttons

    menu_rules = gtk_button_new_with_label("Rules");//the menu-button
    gtk_widget_set_size_request(menu_rules, 70, 35);
    gtk_fixed_put(GTK_FIXED(container), menu_rules, 0, 0);

    menu_new_board = gtk_button_new_with_label("New board");//the menu-button
    gtk_widget_set_size_request(menu_new_board, 80, 35);
    gtk_fixed_put(GTK_FIXED(container), menu_new_board, 70, 0);

    menu_solution = gtk_button_new_with_label("Solution");//the menu-button
    gtk_widget_set_size_request(menu_solution, 75, 35);
    gtk_fixed_put(GTK_FIXED(container), menu_solution, 150, 0);

    menu_exit = gtk_button_new_with_label("Exit");//the menu-button
    gtk_widget_set_size_request(menu_exit, 75, 35);
    gtk_fixed_put(GTK_FIXED(container), menu_exit, 225, 0);
}
void create_rules_window()
{
    rules_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(rules_window), GTK_WIN_POS_CENTER);
    gtk_window_set_title (GTK_WINDOW(rules_window), "Rules");
    gtk_widget_set_size_request(rules_window, SMALL_WINDOW_WIDTH, SMALL_WINDOW_HEIGHT);
    gtk_window_set_resizable (GTK_WINDOW(rules_window), FALSE);//changing the size of the window is impossible

    rules_container = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(rules_window),rules_container);
    rules_label= gtk_label_new("The puzzle consists of an 6x6 grid with \n"
                               "some clues along its sides. The object\n"
                               "is to place a skyscraper in each square,\n"
                               "with a height between 1 and 6, so that \n"
                               "no two skyscrapers in a row or column \n"
                               "have the same number of floors. \n"
                               "In addition, the number of visible\n"
                               "skyscrapers, as viewed from the direction\n"
                               "of each clue, is equal to the value \n"
                               "of the clue. Note that higher skyscrapers\n"
                               "block the view of lower skyscrapers \n"
                               "located behind them.");
    gtk_widget_set_size_request(rules_label, SMALL_WINDOW_WIDTH-10, SMALL_WINDOW_HEIGHT-40);
    gtk_fixed_put(GTK_FIXED(rules_container), rules_label, 7, 0);

    rules_ok = gtk_button_new_with_label("OK");
    gtk_widget_set_size_request(rules_ok, 60, 30);
    gtk_fixed_put(GTK_FIXED(rules_container), rules_ok, 85, 210);
}
void create_board()
{
    for(i=0;i<BOARD_SIZE;i++)
    {
        for(j=0;j<BOARD_SIZE;j++)//board buttons
        {
            sqr[i][j].button = gtk_button_new_with_label ("");//creating the button
            gtk_widget_set_size_request(sqr[i][j].button, 30, 30);//button's size
            gtk_fixed_put(GTK_FIXED(container), sqr[i][j].button, 30+i*30,80+j*30);//adding the button to the container and determining its position
        }
    }

    for( i=0;i<BOARD_SIZE;i++)
          {
              for(j=0;j<4;j++)
              {
                  clue_label[j][i] = gtk_label_new (NULL);
                  gtk_label_set_markup(GTK_LABEL(clue_label[j][i]), marked_labels[clue[j][i]-1]);
              }
            gtk_fixed_put(GTK_FIXED(container), clue_label[0][i], 15,85+i*30);//left
            gtk_fixed_put(GTK_FIXED(container), clue_label[1][i], 220,85+i*30);//right
            gtk_fixed_put(GTK_FIXED(container), clue_label[2][i], 40+i*30,60);//top
            gtk_fixed_put(GTK_FIXED(container), clue_label[3][i], 40+i*30,260);//bottom
          }   //showing numbers around the board(visible skyscrapers)
}
void new_board()
{
    generate_board();
    tab_to_struct();
    //console_show_board();//if you want to see the solution on the console
    create_new_board();
    correct=0;
    gtk_widget_hide(end_window);

    for(i=0; i<BOARD_SIZE;i++)
        for(j=0;j<BOARD_SIZE;j++)
            {
            gtk_widget_set_sensitive(sqr[j][i].button, TRUE);//pressing the board buttons becomes possible
            }
}
void create_new_board()
{
    for(i=0;i<BOARD_SIZE;i++)
    {
        for(j=0;j<BOARD_SIZE;j++)//board buttons
        gtk_button_set_label (GTK_BUTTON(sqr[i][j].button), " ");
    }

    for( i=0;i<BOARD_SIZE;i++)
          {
              for(j=0;j<4;j++)
                  gtk_label_set_markup(GTK_LABEL(clue_label[j][i]), marked_labels[clue[j][i]-1]);
          }   //showing numbers around the board(visible skyscrapers)
}
void show_solution()
{
    for(i=0; i<BOARD_SIZE;i++)
        for(j=0;j<BOARD_SIZE;j++)
            {
            gtk_button_set_label (GTK_BUTTON(sqr[j][i].button), labels[sqr[i][j].corr-1]);
            gtk_widget_set_sensitive(sqr[j][i].button, FALSE);//pressing the board buttons becomes impossible
            }
}
void create_end_window()
{
    end_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(end_window), GTK_WIN_POS_CENTER);
    gtk_window_set_title (GTK_WINDOW(end_window), "The end");
    gtk_widget_set_size_request(end_window, 200, 100);
    gtk_window_set_resizable (GTK_WINDOW(end_window), FALSE);//changing the size of the window is impossible
    end_container = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(end_window),end_container);
    end_label= gtk_label_new("          Congratulations!\n You solved the puzzle correctly");
    gtk_fixed_put(GTK_FIXED(end_container), end_label, 10, 10);

    end_new_board = gtk_button_new_with_label("New board");
    gtk_widget_set_size_request(end_new_board, 80, 30);
    gtk_fixed_put(GTK_FIXED(end_container), end_new_board, 15, 60);

    end_exit = gtk_button_new_with_label("Exit");
    gtk_widget_set_size_request(end_exit, 80, 30);
    gtk_fixed_put(GTK_FIXED(end_container), end_exit, 105, 60);

}
void hide_window(GtkWidget *pressed, GtkWidget *wind)
{
    gtk_widget_hide(wind);
}
void win()
{
    gtk_widget_show_all(end_window);
}
