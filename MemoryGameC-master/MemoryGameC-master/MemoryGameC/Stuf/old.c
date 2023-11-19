/*Jeu de la battaile naval:  https://www.regles-de-jeux.com/regle-de-la-bataille-navale/ */


/*Jeu de la battaile naval:  https://www.regles-de-jeux.com/regle-de-la-bataille-navale/ */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "interface.h"
/*#include "gameengine.h"*/

#define max_boat 15
#define default_square_size 11
#define _CRT_SECURE_NO_WARNINGS

/*Variable Declare*/
static bool game_state = false; /*Define whether or nat the game started*/
static char difficulty = 0;
static char grid_player[default_square_size][default_square_size];
static char grid_placement[default_square_size][default_square_size];
static char grid_ai[default_square_size][default_square_size];
static char grid_alpha[11] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J' };
static char boat_name[5][25] = { "Aircraft Carrier","Cruiser","Against torpedo boat","Submarine","Torpedo boat" };
static char boat_symbol[9] = { 'H', 'I', 'U', 'L', 'T' , 'O', 'X', 'Y', 'D' };
static char boat_lengh[5] = { 5,4,3,3,2 };
static char boat_number[5] = { 1,1,2,2,1 };
static char boat_select;
static char boat_list[max_boat][25];
static char boat_symbol_list[max_boat];
static char boat_lengh_list[max_boat];
static char boat_placed;
static char coordinate_select[10];
static int  coordinate_x1; static int  coordinate_x2;
static int  coordinate_y1; static int  coordinate_y2;


/*Function Declare*/
int  boat_count(char* value);
int  coordinate_x_converter(char x);
void clear_coordinate();

void rules(bool started);
void difficulty_menu();

void grid_gen();
void grid_draw(bool player_tag);
void grid_draw_container(char value[11][11], bool outer_call);

void placement();
void placement_select(int number_of_boat, bool* boat_select_state);
void placement_verifier(char boat_select, int number_of_boat, bool* boat_select_state);
void placement_handler(char boat_select, int number_of_boat, bool* boat_select_state, bool second_coordinate);
void placement_coordinate_verifier(char boat_select, int number_of_boat, bool* boat_select_state, bool second_coordinate);
void placement_valid_coordinate(char boat_select);
bool placement_colision(int coordinate_x, int coordinate_y, char boat_select);
void placement_boat(char boat_select);




/*General Function*/
void stats(int number) {/*Give the remaining ships, and the number of destroyed target (MAP) based on what was played*/

	printf("yolo\n");
}
int  boat_count(char* value) { /*Might be discarded (unused)*/
	int lengh_array = sizeof(value) / sizeof(value[0]);
	int sum = 0;
	for (int i = 0; i < lengh_array; i++) {
		sum += value[i];
	}
	return sum;
}
int  coordinate_x_converter(char x) {
	for (int i = 0; i < default_square_size; i++) {
		if (grid_alpha[i] == toupper(x)) {
			return i + 1;
		}
	}
	return -1;
}
void clear_coordinate() {
	coordinate_x1 = 0;
	coordinate_x2 = 0;
	coordinate_y1 = 0;
	coordinate_y2 = 0;
}


/*Game grid generator and function to display them*/
void grid_gen() {
	for (char i = 1; i < 11; i++) {
		grid_player[0][i] = grid_alpha[i - 1];
		grid_player[i][0] = i;
	}
	for (char i = 1; i < 11; i++) {
		for (char j = 1; j < 11; j++) {
			grid_player[i][j] = 'O';
		}
	}
	memcpy(grid_ai, grid_player, sizeof(char) * default_square_size * default_square_size); /*Copy the same grid for the ai*/
}
void grid_draw(bool player_tag) {/*Draw a grid of 11x11 (subject to change) and mark the player's ship location and the missed, hitted and sinked ship of the enemy depending on the selected dificulty*/
	if (player_tag) { /*True: Human (You), False (AI)*/
		printf("Player grid: \n");
		grid_draw_container(grid_player, false);
	}
	else {
		printf("AI grid: \n");
		grid_draw_container(grid_ai, false);
	}
}
void grid_draw_container(char value[11][11], bool outer_call) {/*Contain the fonction that realy write the grif*/
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 11; j++) {
			/*Alligne x & y axis (because 10 is two digit)*/
			if (value[i][j] >= 0 && value[i][j] <= 9) {
				printf("%d  ", value[i][j]);
			}
			else if (value[i][j] > 9 && value[i][j] <= 10) {
				printf("%d ", value[i][j]);
			}
			/*Write with color depending on the input*/
			else if (value[i][j] == 'H' && i > 0) {
				printf("\033[32m%c\033[0m ", value[i][j]);
			}
			else if (value[i][j] == 'I' && i > 0) {
				printf("\033[33m%c\033[0m ", value[i][j]);
			}
			else if (value[i][j] == 'U' && i > 0) {
				printf("\033[35m%c\033[0m ", value[i][j]);
			}
			else if (value[i][j] == 'L' && i > 0) {
				printf("\033[34m%c\033[0m ", value[i][j]);
			}
			else if (value[i][j] == 'T' && i > 0) {
				printf("\033[36m%c\033[0m ", value[i][j]);
			}
			else if (value[i][j] == 'X' || value[i][j] == 'Y' && j > 0) {
				printf("\033[0;31m%c\033[0m ", value[i][j]);
			}
			else {
				printf("\033[37m%c\033[0m ", value[i][j]);
			}
		}
		printf("\n");
	}
	if (outer_call) {
		printf("\n");
		printf("%s : \033[32m%c\033[0m | %s : \033[33m%c\033[0m | %s : \033[35m%c\033[0m | %s : \033[34m%c\033[0m | %s : \033[36m%c\033[0m | Impossible placement : \033[37m%c\033[0m | Possible placement : \033[0;31m%c\033[0m \n",
			boat_name[0], boat_symbol[0],
			boat_name[1], boat_symbol[1],
			boat_name[2], boat_symbol[2],
			boat_name[3], boat_symbol[3],
			boat_name[4], boat_symbol[4],
			boat_symbol[5], boat_symbol[7]);
	}
	else {
		printf("\n");
		printf("%s : \033[32m%c\033[0m | %s : \033[33m%c\033[0m | %s : \033[35m%c\033[0m | %s : \033[34m%c\033[0m | %s : \033[36m%c\033[0m | Missed : \033[37m%c\033[0m | Hitted : \033[0;31m%c\033[0m \n",
			boat_name[0], boat_symbol[0],
			boat_name[1], boat_symbol[1],
			boat_name[2], boat_symbol[2],
			boat_name[3], boat_symbol[3],
			boat_name[4], boat_symbol[4],
			boat_symbol[8], boat_symbol[6]);
	}
}

/*Handle the placement of boat from player and AI at the start*/
void placement() {/*Allow player to place it's ship at the start*/
	int counter = 0;
	for (int i = 0; i < 5; i++) { /*Generate a new list of all boat*/
		for (int j = 0; j < boat_number[i]; j++) {
			strcpy_s(boat_list[counter], sizeof(boat_list[counter]), boat_name[i]);
			boat_symbol_list[counter] = boat_symbol[i];
			boat_lengh_list[counter] = boat_lengh[i];
			counter++;
		}
	}
	bool boat_select_state[max_boat]; /*Use to know what boat is already placed*/
	for (int i = 0; i < counter; i++) { boat_select_state[i] = true; } /*Set boat_select_state to true*/


	placement_select(counter, boat_select_state);



	/*grid_draw(true);*/
}
void placement_select(int number_of_boat, bool* boat_select_state) { /*Print the list of ships that can be placed*/
	grid_draw(true);
	boat_select = 0; /*Contain the input from the scanf*/
	printf("Choose a boat:\n");
	for (int i = 0; i < number_of_boat; i++) {
		if (!boat_select_state[i]) {
			printf("\033[9m %d. %s\033[0m\n", i, boat_list[i]);
		}
		else {
			printf("%d. %s [Lengh : %d]\n", i, boat_list[i], boat_lengh_list[i]);
		}
		printf("\n");
	}
	printf("\nInput: ");
	scanf_s("%2d", &boat_select);
	while (getchar() != '\n');
	terminal_clear();
	placement_verifier(boat_select, number_of_boat, boat_select_state);
}
void placement_verifier(char boat_select, int number_of_boat, bool* boat_select_state) { /*Verify the input from placement_boat and loop if incorrect*/
	if (boat_select >= 0 && boat_select < number_of_boat && boat_select_state[boat_select]) {
		printf("You selected %s \n\n", boat_list[boat_select]);
		placement_handler(boat_select, number_of_boat, boat_select_state, false);
	}
	else {
		printf("Invalid number or boat already placed\n");
		placement_select(number_of_boat, boat_select_state);
	}
}
void placement_handler(char boat_select, int number_of_boat, bool* boat_select_state, bool second_coordinate) {
	if (!second_coordinate) {
		clear_coordinate();
		grid_draw(true);
		printf("Enter the first coordinate like 'a.1': \nInput: ");
		scanf_s("%s", coordinate_select, 5);
		placement_coordinate_verifier(boat_select, number_of_boat, boat_select_state, second_coordinate);
		terminal_clear();
		placement_handler(boat_select, number_of_boat, boat_select_state, true);
	}
	else {
		terminal_clear();
		placement_valid_coordinate(boat_select); /*Draw a grid that show where the boat can be placed*/

		printf("Enter the second coordinate like 'a.1': \nInput: ");
		scanf_s("%s", coordinate_select, 5);
		placement_coordinate_verifier(boat_select, number_of_boat, boat_select_state, true);
	}
	terminal_clear();
	if (!tester(boat_list[boat_select])) {
		placement_handler(boat_select, number_of_boat, boat_select_state, false);
	}
	while (boat_placed < number_of_boat) {
		placement_boat(boat_select);
		terminal_clear();
		boat_placed++;
		printf("\nBoat Placed!!\n\n");
		boat_select_state[boat_select] = false;
		placement_select(number_of_boat, boat_select_state);
	}


}
/*Required function for the good work of 'placement_handler'*/
void placement_coordinate_verifier(char boat_select, int number_of_boat, bool* boat_select_state, bool second_coordinate) {
	char char_coordinate_x = 0;
	int coordinate_x = 0;
	int coordinate_y = 0;

	sscanf_s(coordinate_select, "%c.%d", &char_coordinate_x, 1, &coordinate_y);
	coordinate_x = coordinate_x_converter(char_coordinate_x);
	printf("\nX: %d\nY: %d\n", coordinate_x, coordinate_y);
	if (coordinate_x < 1) {
		terminal_clear();
		printf("\nError: Invalide x value \n\n");
		placement_handler(boat_select, number_of_boat, boat_select_state, second_coordinate);
	}
	if (coordinate_x >= 1 && coordinate_x <= 10 && coordinate_y >= 1 && coordinate_y <= 10) {
		printf("\nX: %d\nY: %d\n", coordinate_x, coordinate_y);
	}
	else {
		terminal_clear();
		printf("\nError: Invalide coordinate (Too big or less than 0) \n\n");
		placement_handler(boat_select, number_of_boat, boat_select_state, second_coordinate);
	}

	if (second_coordinate) {
		grid_draw_container(grid_placement, true);
		if (grid_placement[coordinate_y][coordinate_x] != 'Y') {
			terminal_clear();
			printf("\nError: Invalide coordinate \n\n");
			placement_handler(boat_select, number_of_boat, boat_select_state, second_coordinate);
		}
		if (placement_colision(coordinate_x, coordinate_y, boat_select)) {
			coordinate_x2 = coordinate_x;
			coordinate_y2 = coordinate_y;
		}
		else {
			terminal_clear();
			printf("\nError: A boat is in the way \n\n");
			placement_handler(boat_select, number_of_boat, boat_select_state, second_coordinate);
		}

	}
	else {
		if (grid_player[coordinate_y][coordinate_x] != 'O') {
			terminal_clear();
			printf("\nError: A boat is already here \n\n");
			placement_handler(boat_select, number_of_boat, boat_select_state, second_coordinate);
		}
		else {
			coordinate_x1 = coordinate_x;
			coordinate_y1 = coordinate_y;
		}
	}
}
void placement_valid_coordinate(char boat_select) { /*Draw a grid that show where the boat can be placed (write 'Y' for valid coordinate)*/
	memcpy(grid_placement, grid_player, sizeof(char) * default_square_size * default_square_size);
	char lengh = boat_lengh_list[boat_select] - 1;
	int new_coordinate_x_1 = 0; int new_coordinate_x_2 = 0;
	int new_coordinate_y_1 = 0; int new_coordinate_y_2 = 0;

	printf("lengh: %d \n", boat_lengh_list[boat_select]);
	/*Verify coordinate for placement is in board*/
	if ((coordinate_x1 + lengh) <= 10) {
		new_coordinate_x_1 = coordinate_x1 + lengh;
	}
	if ((coordinate_x1 - lengh) >= 1) {
		new_coordinate_x_2 = coordinate_x1 - lengh;
	}
	if ((coordinate_y1 + lengh) <= 10) {
		new_coordinate_y_1 = coordinate_y1 + lengh;
	}
	if ((coordinate_y1 - lengh) >= 1) {
		new_coordinate_y_2 = coordinate_y1 - lengh;
	}

	/*Verify if a boat is already placed and show valide location for boat placement*/
	if (grid_placement[coordinate_y1][new_coordinate_x_1] == 'O' && placement_colision(new_coordinate_x_1, coordinate_y1, boat_select)) {
		grid_placement[coordinate_y1][new_coordinate_x_1] = 'Y';
	}
	if (grid_placement[coordinate_y1][new_coordinate_x_2] == 'O' && placement_colision(new_coordinate_x_2, coordinate_y1, boat_select)) {
		grid_placement[coordinate_y1][new_coordinate_x_2] = 'Y';
	}
	if (grid_placement[new_coordinate_y_1][coordinate_x1] == 'O' && placement_colision(coordinate_x1, new_coordinate_y_1, boat_select)) {
		grid_placement[new_coordinate_y_1][coordinate_x1] = 'Y';
	}
	if (grid_placement[new_coordinate_y_2][coordinate_x1] == 'O' && placement_colision(coordinate_x1, new_coordinate_y_2, boat_select)) {
		grid_placement[new_coordinate_y_2][coordinate_x1] = 'Y';
	}
	grid_draw_container(grid_placement, true);
}
bool placement_colision(int coordinate_x, int coordinate_y, char boat_select) {
	/*Verify that no boat is in the way for placement*/
	if (coordinate_x == coordinate_x1 && coordinate_y < coordinate_y1) { /*Above orgin*/
		printf("\nAbove orgin: %d", boat_lengh_list[boat_select]);
		for (int i = 0; i < boat_lengh_list[boat_select]; i++) {
			if (!(grid_player[coordinate_y + i][coordinate_x1] == 'O')) {
				printf("False \n");
				return false;
			}
		}
	}
	if (coordinate_x == coordinate_x1 && coordinate_y > coordinate_y1) { /*Below orgin*/
		printf("\nBelow orgin: %d", boat_lengh_list[boat_select]);
		for (int i = 0; i < boat_lengh_list[boat_select]; i++) {
			if (!(grid_player[coordinate_y - i][coordinate_x1] == 'O')) {
				printf("False \n");
				return false;
			}
		}
	}
	if (coordinate_y == coordinate_y1 && coordinate_x < coordinate_x1) { /*Left from orgin*/
		printf("\nLeft from orgin: %d", boat_lengh_list[boat_select]);
		for (int i = 0; i < boat_lengh_list[boat_select]; i++) {
			if (!(grid_player[coordinate_y1][coordinate_x + i] == 'O')) {
				return false;
			}
		}
	}
	if (coordinate_y == coordinate_y1 && coordinate_x > coordinate_x1) { /*Right from orgin*/
		printf("\nRight from orgin: %d", boat_lengh_list[boat_select]);
		for (int i = 0; i < boat_lengh_list[boat_select]; i++) {
			if (!(grid_player[coordinate_y1][coordinate_x - i] == 'O')) {
				return false;
			}
		}
	}
}
void placement_boat(char boat_select) { /*Place on the grid the boat based on the Two coordinate*/
	terminal_clear();
	/*Place the boat*/
	if (coordinate_y2 < coordinate_y1) { /*Above orgin*/
		printf("\nAbove orgin Placer \n");
		for (int i = 0; i < boat_lengh_list[boat_select]; i++) {
			grid_player[coordinate_y1 - i][coordinate_x1] = boat_symbol_list[boat_select];
		}
	}
	if (coordinate_y2 > coordinate_y1) { /*Bellow orgin*/
		printf("\nBellow orgin Placer \n");
		for (int i = 0; i < boat_lengh_list[boat_select]; i++) {
			grid_player[coordinate_y1 + i][coordinate_x1] = boat_symbol_list[boat_select];
		}
	}
	if (coordinate_x2 < coordinate_x1) { /*Left from orgin*/
		printf("\nLeft from orgin Placer \n");
		for (int i = 0; i < boat_lengh_list[boat_select]; i++) {
			grid_player[coordinate_y1][coordinate_x1 - i] = boat_symbol_list[boat_select];
		}
	}
	if (coordinate_x2 > coordinate_x1) { /*Right from orgin*/
		printf("\nRight from orgin Placer \n");
		for (int i = 0; i < boat_lengh_list[boat_select]; i++) {
			grid_player[coordinate_y1][coordinate_x1 + i] = boat_symbol_list[boat_select];
		}
	}
}





void target() {/*Let the player input the coordinate of it's target (Verify if it was already shot or not depending on the selected difficulty)*/

}


int main() {
	grid_gen();
	rules(game_state);
	placement();

	/*After that point it's just test*/

	int i;
	printf("\nValeur un: ");
	scanf_s("\n %d", &i, 10);
	printf("%d", i);
	printf("%d", &i);
}