#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <Windows.h>

bool row_check (char **board, int row, int col)
{return col > 2 && (board [row][col] == board [row][col - 1])
						&& (board [row][col] == board [row][col - 2]);
}

bool col_check (char **board, int row, int col)
{return row > 2 && (board [row][col] == board [row - 1][col])
						 && (board [row][col] == board [row - 2][col]);
}

char **create_board (int n, int n_symbols, char* symbols)
{char ** board;
 board = (char **)malloc((n + 1) * sizeof(char *));
 for (int i = 1; i <= n; i++) {
	 board [i] = (char *)malloc((n + 1) * sizeof(char));
	 for (int j = 1; j <= n; j++) {
		 board [i][j] = symbols [rand () % n_symbols];
		 while (row_check (board, i, j) || col_check (board, i ,j)) {
			 board [i][j] = symbols [rand () % n_symbols];
		 }
	 }
 }
 return board;
}

void print_board (char **board, int n)
{printf("        ");
 for (int i = 1; i <= n; i++) {
	 printf("%2d ", i);
 }
 printf("\n       ");
 int k = 1;
 while (k <= n) {
	 printf("---");
	 k++;
 }
 printf("--\n");
 for (int i = 1; i <= n; i++) {
	printf("   %2d |  ", i);
	for (int j = 1; j <= n; j++) {
		printf("%c  ", board [i][j]);
	}
	printf("|\n");
 }
 printf("       ");
 int m = 1;
 while (m <= n) {
	 printf("---");
	 m++;
 }
 printf("--\n\n");
}

bool check_range (int row, int col, int n)
{return row > 0 && row <= n && col > 0 && col <= n;
}

bool check_adjacent (int row1, int col1, int row2, int col2)
{return (row1 == row2 && abs(col1 - col2) == 1) ||
			(col1 == col2 && abs(row1 - row2) == 1);
}

bool row_repeat_symbols (int row, int col, char **board) 
{return (board [row][col] == board [row][col + 1] && board [row][col] == board [row][col + 2]) ||
			(board [row][col] == board [row][col + 1] && board [row][col] == board [row][col + 2] && board [row][col] == board [row][col + 3]) ||
			(board [row][col] == board [row][col + 1] && board [row][col] == board [row][col + 2] && board [row][col] == board [row][col + 3] && board [row][col] == board [row][col + 4]);
}

bool check_row_repeat_symbols (int row, int col, int n, char **board)
{if (col == 1) 
	return row_repeat_symbols (row, col, board);
 if (col == 2) 
	 return row_repeat_symbols (row, col, board) || row_repeat_symbols (row, col - 1, board);
 if (col == n) 
	 return row_repeat_symbols (row, col - 2, board);
 if (col == n - 1) 
	 return row_repeat_symbols (row, col - 1, board) || row_repeat_symbols (row, col - 2, board);
 return row_repeat_symbols (row, col, board) || row_repeat_symbols (row, col - 1, board) || row_repeat_symbols (row, col - 2, board);
}

bool col_repeat_symbols (int row, int col, char **board)
{return (board [row][col] == board [row + 1][col] && board [row ][col] == board [row + 2][col]) ||
			(board [row][col] == board [row + 1][col] && board [row][col] == board [row + 2][col] && board [row][col] == board [row + 3][col]) ||
			(board [row][col] == board [row + 1][col] && board [row][col] == board [row + 2][col] && board [row][col] == board [row + 3][col] && board [row][col] == board [row + 4][col]);
}

bool col_repeat_symbols_bottom (int row, int col, char **board)
{return (board [row][col] == board [row - 1][col] && board [row][col] == board [row - 2][col]) ||
			(board [row][col] == board [row - 1][col] && board [row][col] == board [row - 2][col] && board [row][col] == board [row - 3][col]) ||
			(board [row][col] == board [row - 1][col] && board [row][col] == board [row - 2][col] && board [row][col] == board [row - 3][col] && board [row][col] == board [row - 4][col]);
}

bool check_col_repeat_symbols (int row, int col, int n, char **board)
{if (row == 1) 
	 return col_repeat_symbols (row, col, board); 
 if (row == 2) 
	 return col_repeat_symbols (row, col, board) || col_repeat_symbols (row - 1, col, board);
 if (row == n) 
	 return col_repeat_symbols (row - 2, col, board);
 if (row == n - 1) 
	 return col_repeat_symbols (row - 1, col, board) || col_repeat_symbols (row - 2, col, board);
 return col_repeat_symbols (row, col, board) || col_repeat_symbols (row - 1, col, board) || col_repeat_symbols (row - 2, col, board);
}

bool overall_check_seq (int row, int col, int n, char **board)
{return check_row_repeat_symbols (row, col, n, board) || check_col_repeat_symbols (row, col, n, board);
}

void move_cursor (HANDLE hConsole, int a, int b)
{COORD coordScreen = {a, b}; 
 SetConsoleCursorPosition (hConsole, coordScreen);
}

void cursor(HANDLE hConsole, int a)
{CONSOLE_CURSOR_INFO info;
 info.dwSize = 1;
 info.bVisible = a;
 SetConsoleCursorInfo(hConsole, &info);
}

bool swap (int row1, int col1, int row2, int col2, int n, char **board, HANDLE hStdOut)
{bool b1 = check_range (row1, col1, n) && check_range (row2, col2, n);
 bool b2 = check_adjacent (row1, col1, row2, col2);
 if (b1 && b2) {
	 int tmp = board [row1][col1];
	 board [row1][col1] = board [row2][col2];
	 board [row2][col2] = tmp;
 }
 else {
	 move_cursor (hStdOut, 0, n + 19);
	 printf("Oh no! Failed swap! Please try again!                      \n         ");
	 move_cursor (hStdOut, 38, n + 19);
	 cursor (hStdOut, 1);
 }
 bool b3 = overall_check_seq (row1, col1, n, board) || overall_check_seq (row2, col2, n, board);
 if (b3) {
	 move_cursor (hStdOut, 0, n + 19);
	 printf("Great!! Successful swap! Swap again!                     \n           ");
 }
 else {
	 int temp = board [row1][col1];
	 board [row1][col1] = board [row2][col2];
	 board [row2][col2] = temp;
	 move_cursor (hStdOut, 0, n + 19);
	 printf("Oh no! Failed swap! Please try again!                        \n          ");
	 move_cursor (hStdOut, 38, n + 19);
	 cursor (hStdOut, 1);
 }
 return b3;
}

bool null_symbol (int n, char **board, HANDLE hStdOut)
{int coord_row [n * n], coord_col [n * n];
 int k = 0;
 bool b1, b2;
 for (int i = n; i > 0; i--) {
	for (int j = 1; j <= n; j++) {
		if (row_repeat_symbols (i, j, board)) {
			b1 = row_repeat_symbols (i, j, board);
			char s = board [i][j];
			for (int r = j; board [i][r] == s; r++) {
				coord_row [k] = i;
				coord_col [k] =r;
				k++;
			}
		}
		if (col_repeat_symbols_bottom (i, j, board)) {
			b2 = col_repeat_symbols_bottom (i, j, board);
			char s = board [i][j];
			for (int c = i; board [c][j] == s; c--) {
				coord_row [k] = c;
				coord_col [k] = j;
				k++;
			}
		}
	}
 }
 for (int i = 0; i < k; i++) {
	 int row = coord_row [i];
	 int col = coord_col [i];
	 board [row][col] = ' ';
 }
 move_cursor (hStdOut, 0, 13);
 print_board (board, n);
 return b1 || b2;
}

void fall_down_effect (int n, char **board, char *symbols, int n_symbols, HANDLE hStdOut)
{for (int i = 1; i <= n; i++) {
	for (int j = 1; j <= n; j++) {
		if (board [i][j] == ' ') {
			for (int k = i; k > 0; k--) {
				board [k][j] = board [k - 1][j];
			}
			board [1][j] = symbols [rand () % n_symbols];
		}
		move_cursor (hStdOut, 0, 13);
		print_board (board, n);
	}
 }
}

int score_per_move (int n, char **board)
{int count = 0;
 for (int i = 1; i <= n; i ++) {
	for (int j = 1; j <= n; j++) {
		if (board [i][j] == ' ') {
			count += 10;
		}
	}
 }
 return count;
}

int saved_game (FILE *saved_file, int n, char **board, int total_score, clock_t time_left_p, int n_symbols)
{if ((saved_file = fopen("saved_game.txt", "w")) == NULL ) {
	 printf("Unable to open file.\n");
	 exit (0);
 }
 if ((fprintf(saved_file, "%d %d %d %ld", n, n_symbols, total_score, time_left_p) <= 0)) {
	 printf("Error writing to file.\n");
	 exit (0);
 }
 for (int i = 1; i <= n; i++) {
	for (int j = 1; j <= n; j++) {
		if (fprintf(saved_file, "%c", board [i][j]) <= 0) {
			printf("Error writing to file.\n");
			exit (0);
		}
	}
 }
 fclose (saved_file);
 return 1;
}

char** resume (FILE *resume_game, int *n, int *n_symbols, int *total_score, clock_t *time_left_pg)
{char **board;
 if ((resume_game = fopen ("saved_game.txt", "r")) == NULL) {
	printf("Unable to open file.\n");
	exit(0);
 }
 if ((fscanf(resume_game, "%d %d %d %ld", n, n_symbols, total_score, time_left_pg)) <= 0) {
	 printf("Error reading file.\n");
	 exit (0);
 }
 board = (char **)malloc((*n + 1) * sizeof(char *));
 for (int i = 1; i <= *n; i++) {
	 board [i] = (char *)malloc((*n + 1) * sizeof(char));
 }
 for (int i = 1; i <= *n; i++) {
	 for (int j = 1; j <= *n; j++) {
		board [i][j] = fgetc(resume_game);
	}
 }
 return board;
}

int pause_game (int n, HANDLE hStdOut, FILE *saved_file, clock_t time_left_p, clock_t *time_limit_ptr, char **board, int total_score, int n_symbols)
{char next_step;
 move_cursor (hStdOut, 0, n + 19);
 printf("GAME PAUSED                                            \nr - Resume   q - Quit   s - Save and quit\n");
 while (scanf("%c", &next_step) == 1) {
	if (next_step == 'r') {
		for (int i = 19; i <= 22; i++) {
			move_cursor (hStdOut, 0, n + i);
			printf("                                                                                           ");
		}
		move_cursor (hStdOut, 0, n + 19);
		printf("Game resumed! Swap the symbols now! ");
		*time_limit_ptr = time_left_p + clock();
		return 1;
	}
	else if (next_step == 'q') {
		free (board);
		move_cursor (hStdOut, 0, n + 19);
		printf("You're exiting the game...\n                                                                    \n      ");
		move_cursor (hStdOut, 0, n + 20);
		Sleep(2000);
		exit (0);
	}
	else if (next_step == 's') {
		if (saved_game (saved_file, n, board, total_score, time_left_p, n_symbols)) {
			free (board);
			move_cursor (hStdOut, 0, n + 19);
			printf("Game saved. You're exiting the game...\n                                                                    \n      ");
			move_cursor (hStdOut, 0, n + 20);
			Sleep(2000);
			exit(0);
		}
	}
 }
 return 1;
}

int main(int argc, char **argv)
{int n, n_symbols, score, total_score = 0, seconds = 90;
 double final_score;
 char start, pause, end;
 char **board;
 char symbols [ ] = "@#$%&+~<?0=>!^/";
 clock_t time_limit, time_left, time_left_pg;
 HANDLE hStdOut = GetStdHandle (STD_OUTPUT_HANDLE);

 FILE *saved_file = NULL;
 FILE *resume_game = NULL;

 system ("cls");
 printf("WELCOME TO THE GAME!\nDo you want to start a new game or resume your previous game?\n");
 printf("n - New game     r - Resume\n\n");
 while(scanf("%c", &start) == 1) { 
	 if (start == 'n') {
		system ("cls");
		printf("Welcome to the game!\nYou will have 90 seconds to play the game.\nEnter your preferred board size and number of symbols to start the game!\n\n");
		printf("Board size : ");
		scanf("%d", &n);
		printf("Number of symbols (Max 15) : ");
		scanf("%d", &n_symbols);
		printf("\nGAME START!\nENJOY THE GAME!\n::Press p to pause the game::\n\n");
		board = create_board (n, n_symbols, symbols);
		printf("  Total Score : 0   \n\n");
		time_limit = clock() + (seconds * CLOCKS_PER_SEC);
		print_board (board, n);
		break;
	 }
	 else if (start == 'r') {
		 board = resume(resume_game, &n, &n_symbols, &total_score, &time_left_pg);
		 system ("cls");
		 printf("Welcome back!\nContinue your game now!\nYour board size and number of symbols are shown below.\n\n");
		 printf("Board size : %d\n", n);
		 printf("Number of symbols : %d\n", n_symbols);
		 printf("\nCONTINUE PLAYING!\nENJOY THE GAME!\n::Press p to pause the game::\n\n");
		 printf("  Total Score : %d   \n\n", total_score);
		 time_limit = clock() + time_left_pg;
		 print_board (board, n);
		 break;
	 }
	 else {
		 move_cursor (hStdOut, 0, 4);
		 printf("     ");
		 move_cursor (hStdOut, 0, 4);
	 }
 }
 
 int row1, row2, col1, col2;
 printf("\n\nSwap the symbols now! ");
 while (clock() < time_limit) {
	 if (scanf("%d %d %d %d", &row1, &col1, &row2, &col2) == 4 && clock() < time_limit) {
		cursor(hStdOut, 0);
		if (swap (row1, col1, row2, col2, n, board, hStdOut)) {
			score = 0;
			while (null_symbol (n, board, hStdOut)) {
				score += score_per_move (n, board);
				fall_down_effect (n, board, symbols, n_symbols, hStdOut);
			}
			total_score += score;
			move_cursor(hStdOut, 16, 11);
			printf("%d", total_score);
			move_cursor(hStdOut, 0, n + 16);
			printf("    You have scored %d points in this swap!  \n", score);
		}
		time_left = time_limit - clock();
		move_cursor (hStdOut, 0, n + 17);
		printf("    You still have %d seconds left.  ", (int)(time_left / CLOCKS_PER_SEC));
		move_cursor (hStdOut, 38, n + 19);
		cursor (hStdOut, 1);
	 }
	 else if (scanf("%c", &pause) == 1 && clock() < time_limit) {
		if (pause != 'p') {
			move_cursor (hStdOut, 0, n + 19);
			printf("Invalid input. Please try again.               ");
			move_cursor (hStdOut, 33, n + 19);
		} 
		else {
			clock_t time_left_p = time_limit - clock();
			pause_game (n, hStdOut, saved_file, time_left_p, &time_limit, board, total_score, n_symbols);
		}
	 }
 }
 
 final_score = (double)total_score / seconds;
 move_cursor(hStdOut, 0, n + 19);
 printf("    TIME'S UP!!                                \n");
 Sleep(1000);
 printf("    Your final score is %.2lf.\n", final_score);
 Sleep(2000);
 printf("\nDo you want to start a new game or quit the game?\nh - Return to homepage    q - Quit game\n");
 while (scanf("%c", &end) == 1 ) {
	if (end == 'h') {
		main(argc, argv);
	}
	else if (end == 'q') {
		return 0;
	}
	else {
		move_cursor(hStdOut, 0, n + 25);
		printf("         \r");
	}
 }
}
