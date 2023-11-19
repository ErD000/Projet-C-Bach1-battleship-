/*By PSR_J0740*/
#include "battle_ship.h"


/*Define Function*/

/*Visual and basic*/
void terminal_clear() {
#ifdef _WIN32
	system("cls");  // for Windows
#else
	system("clear");  // for Unix-based systems
#endif
}

bool tester(char* value) {
	char choice[1];
	printf("\nYou selected '%s'\n", value);
	printf("Confirm 'Y' | Deny 'N' ");
	scanf_s("\n %c", choice, 1);
	while (getchar() != '\n');
	if (strlen(choice) > 0 && (choice[0] == 'y' || choice[0] == 'Y')) {
		printf("\n");
		return true;
	}
	else {
		printf("\n");
		return false;
	}
}

void grid_gen(GameBoard* grid) {
	grid->grid[0][0] = 0;
	for (int i = 1; i < 11; i++) {
		grid->grid[0][i] = grid_alpha[i - 1];
		grid->grid[i][0] = i;
	}
	for (int i = 1; i < 11; i++) {
		for (int j = 1; j < 11; j++) {
			grid->grid[i][j] = 'O';
		}
	}
}

void boat_list_gen(BoatInfo* boat_info) {
	boat_info->bn = 0;
	for (int i = 0; i < number_of_boat; i++) {
		for (int j = 0; j < boat_number[i]; j++) {
			strcpy_s(boat_info->bl[boat_info->bn], sizeof(boat_info->bl[boat_info->bn]), boat_name[i]);
			boat_info->bsl[boat_info->bn] = boat_symbol[i];
			boat_info->bll[boat_info->bn] = boat_lengh[i];
			boat_info->bn++;
		}
	}

	/*Create an array of boolean that is set to false when a boat is already placed*/
	for (int i = 0; i < boat_info->bn; i++) {
		boat_info->bss[i] = true;
	}
}

void clear_coordinate(Coordinate* coordinate) {
	coordinate->x = 0;
	coordinate->y = 0;
}

int  coordinate_x_converter(char x, char grid_alpha[default_square_size]) {
	for (int i = 0; i < default_square_size; i++) {
		if (grid_alpha[i] == toupper(x)) {
			return i + 1;
		}
	}
	return -1;
}

int  random_number(int max) {
	return rand() % max;
}

bool target(Coordinate* coordinate, GameBoard* target_grid, GameBoard* font, Stats* stat, bool attacking) {
	if (coordinate->y > 0 && coordinate->x > 0) {
		if (target_grid->grid[coordinate->y][coordinate->x] != 'O' && target_grid->grid[coordinate->y][coordinate->x] != 'X') {
			if (attacking) {
				printf("\033[31mYou hitted the target\033[0m\n");
			}
			else {
				printf("\033[31mYour ship in {x%d | y%d} was shot\033[0m\n", coordinate->x, coordinate->y);
			}

			stat->numHits++;
			stat->totalShots++;
			font->grid[coordinate->y][coordinate->x] = 'X';
			target_grid->grid[coordinate->y][coordinate->x] = 'X';
			return true;
		}
		else {
			if (attacking) {
				printf("\033[34mYou missed the target\033[0m\n");
			}

			stat->numMisses++;
			stat->totalShots++;
			if (target_grid->grid[coordinate->y][coordinate->x] != 'X') {
				font->grid[coordinate->y][coordinate->x] = 'D';
			}
			return false;
		}
	}
}



void rules() {
	printf("This is the Battleship game's rule:\n");
	printf("You start with:\n");
	printf("  1 - %s      : Lengh 5 : \033[32m%c\033[0m\n", boat_name[0], boat_symbol[0]);
	printf("  1 - %s               : Lengh 4 : \033[33m%c\033[0m\n", boat_name[1], boat_symbol[1]);
	printf("  2 - %s  : Lengh 3 : \033[35m%c\033[0m\n", boat_name[2], boat_symbol[2]);
	printf("  2 - %s             : Lengh 3 : \033[34m%c\033[0m\n", boat_name[3], boat_symbol[3]);
	printf("  1 - %s          : Lengh 2 : \033[36m%c\033[0m\n\n", boat_name[4], boat_symbol[4]);
	printf("You can place them any where on a 10x10 grid:\n");
	printf("  To place a ship, enter coordinate as follow: 'x.y' then 'x2.y2' \n");
	printf("  Ex: Aircraft carrier 'c.4' then 'i.4' \n\n");
	printf("To win you must destroy all your enemy's battle ship\n");
	printf("  To shoot enter coordinate as follow: 'x.y' \n");
	printf("  Ex: 'e.7' \n\n\n\n");
}
void difficulty_menu(int* difficulty) {
	printf("Choose your dificulty: \n");
	printf("  1. Normal \n  2. Hard \n  3. Hardcore \n");
	printf("\nInput: ");
	scanf_s("\n %1d", difficulty);
	while (getchar() != '\n');
	switch (*difficulty) {
	case 2:
		terminal_clear();
		*difficulty = 2;
		printf("You selected Hard:\n");
		printf("  - You don't have a map wich indicate missed and hitted target\n");
		printf("  - You have a message when you hitted or not\n");
		printf("  - You have a message when you sinked a ship\n\n");
		if (tester("Hard")) {
			terminal_clear();
			break;
		}
		else {
			terminal_clear();
			difficulty_menu(difficulty);
		}
	case 3:
		terminal_clear();
		*difficulty = 3;
		printf("You selected Hardcore:\n");
		printf("  - You don't have a map wich indicate missed and hitted target\n");
		printf("  - You have a message when you hitted or not\n");
		printf("  - You don't have a message when you sinked a ship\n\n");
		if (tester("Hardcore")) {
			terminal_clear();
			break;
		}
		else {
			terminal_clear();
			difficulty_menu(difficulty);
		}
	default:
		terminal_clear();
		*difficulty = 1;
		printf("You selected Normal (default):\n");
		printf("  - You have a map wich indicate missed and hitted target\n");
		printf("  - You have a message when you hitted or not\n");
		printf("  - You have a message when you sinked a ship\n\n");
		if (tester("Normal")) {
			terminal_clear();
			break;
		}
		else {
			terminal_clear();
			difficulty_menu(difficulty);
		}
	}
}
void get_coordinate(Coordinate* coordinate, char* value) {
	char target_select[10];
	while (true) {
		printf("%s \nInput: ", value);
		scanf_s("%s", target_select, 5);
		if (coordinate_verifier(target_select, coordinate)) {
			break;
		}
	}
}
void grid_draw_container(GameBoard* grid, bool outer_call) {
	for (int i = 0; i < default_square_size; i++) {
		for (int j = 0; j < default_square_size; j++) {
			/*Alligne x & y axis (because 10 is two digit)*/
			if (grid->grid[i][j] >= 0 && grid->grid[i][j] <= 9) {
				printf("%d  ", grid->grid[i][j]);
			}
			else if (grid->grid[i][j] > 9 && grid->grid[i][j] <= 10) {
				printf("%d ", grid->grid[i][j]);
			}
			/*Write with color depending on the input*/
			else if (grid->grid[i][j] == 'H' && i > 0) {
				printf("\033[32m%c\033[0m ", grid->grid[i][j]);
			}
			else if (grid->grid[i][j] == 'I' && i > 0) {
				printf("\033[33m%c\033[0m ", grid->grid[i][j]);
			}
			else if (grid->grid[i][j] == 'U' && i > 0) {
				printf("\033[35m%c\033[0m ", grid->grid[i][j]);
			}
			else if (grid->grid[i][j] == 'L' && i > 0) {
				printf("\033[34m%c\033[0m ", grid->grid[i][j]);
			}
			else if (grid->grid[i][j] == 'T' && i > 0) {
				printf("\033[36m%c\033[0m ", grid->grid[i][j]);
			}
			else if (grid->grid[i][j] == 'D' && i > 0) {
				printf("\033[44m \033[0m ");
			}
			else if (grid->grid[i][j] == 'X' && i > 0) {
				printf("\033[41m \033[0m ");
			}
			else if (grid->grid[i][j] == 'Y' && j > 0) {
				printf("\033[0;31m%c\033[0m ", grid->grid[i][j]);
			}
			else {
				printf("\033[37m%c\033[0m ", grid->grid[i][j]);
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
		printf("%s : \033[32m%c\033[0m | %s : \033[33m%c\033[0m | %s : \033[35m%c\033[0m | %s : \033[34m%c\033[0m | %s : \033[36m%c\033[0m | Missed : \033[44m \033[0m | Hitted/Destroyed : \033[41m \033[0m \n",
			boat_name[0], boat_symbol[0],
			boat_name[1], boat_symbol[1],
			boat_name[2], boat_symbol[2],
			boat_name[3], boat_symbol[3],
			boat_name[4], boat_symbol[4]);
	}
}
void print_stats(Stats* stats) {
	printf("Number of hits: %d\n", stats->numHits);
	printf("Number of misses: %d\n", stats->numMisses);
	printf("Total number of shots: %d\n", stats->totalShots);
	if (stats->totalShots > 0) {
		double hit_rate = (double)stats->numHits / stats->totalShots * 100;
		printf("Hit rate: %.2f%%\n", hit_rate);
	}
}


/*Advanced*/
bool coordinate_verifier(char coordinate_select[10], Coordinate* temporary_coordinate) {
	char char_coordinate_x = 0;


	sscanf_s(coordinate_select, "%c.%d", &char_coordinate_x, 1, &temporary_coordinate->y);
	temporary_coordinate->x = coordinate_x_converter(char_coordinate_x, grid_alpha);
	printf("\nX: %d\nY: %d\n", temporary_coordinate->x, temporary_coordinate->y);

	/*Verify that x & y are inside the grid*/
	if (temporary_coordinate->x < 1) {
		printf("\nError: Invalide x value \n\n");
		return false;
	}
	if (temporary_coordinate->x >= 1 && temporary_coordinate->x <= default_square_size - 1 && temporary_coordinate->y >= 1 && temporary_coordinate->y <= default_square_size - 1) {
		printf("\nX: %d\nY: %d\n", temporary_coordinate->x, temporary_coordinate->y);
	}
	else {
		printf("\nError: Invalide coordinate (Too big or less than 0) \n\n");
		return false;
	}

	return true;
}
bool placement_coordinate_verifier(char boat_select, char coordinate_select[10], bool* second_coordinate, BoatInfo* boat_info, GameBoard* player, GameBoard* place_board, Coordinate* coordinate_1, Coordinate* coordinate_2) {
	Coordinate temporary_coordinate = { 0,0 };

	if (!coordinate_verifier(coordinate_select, &temporary_coordinate)) {
		return false;
	}

	if (*second_coordinate) {

		grid_draw_container(place_board, true);
		if (place_board->grid[temporary_coordinate.y][temporary_coordinate.x] != 'Y') {
			terminal_clear();
			printf("\nError: Invalide coordinate \n\n");
			return false;
		}
		if (placement_colision(boat_select, boat_info, coordinate_1, &temporary_coordinate, player)) {
			coordinate_2->x = temporary_coordinate.x;
			coordinate_2->y = temporary_coordinate.y;
		}
		else {
			terminal_clear();
			printf("\nError: A boat is in the way \n\n");
			return false;
		}

	}
	else {

		if (player->grid[temporary_coordinate.y][temporary_coordinate.x] != 'O') {
			terminal_clear();
			printf("\nError: A boat is already here \n\n");
			return false;
		}
		else {
			coordinate_1->x = temporary_coordinate.x;
			coordinate_1->y = temporary_coordinate.y;
			*second_coordinate = true;
		}
	}
}

void placement_valid_coordinate(char boat_select, BoatInfo* boat_info, Coordinate* coordinate_1, Coordinate* coordinate_2, GameBoard* grid1, GameBoard* grid2) {
	char lengh = boat_info->bll[(int)boat_select] - 1;
	int new_coordinate_x_1 = 0; int new_coordinate_x_2 = 0;
	int new_coordinate_y_1 = 0; int new_coordinate_y_2 = 0;


	printf("lengh: %d \n", boat_info->bll[(int)boat_select]);
	/*Verify coordinate for placement is in board*/
	if ((coordinate_1->x + lengh) <= 10) {
		new_coordinate_x_1 = coordinate_1->x + lengh;
	}
	if ((coordinate_1->x - lengh) >= 1) {
		new_coordinate_x_2 = coordinate_1->x - lengh;
	}
	if ((coordinate_1->y + lengh) <= 10) {
		new_coordinate_y_1 = coordinate_1->y + lengh;
	}
	if ((coordinate_1->y - lengh) >= 1) {
		new_coordinate_y_2 = coordinate_1->y - lengh;
	}

	/*Set new_coordinate to the coordinate that need to be tested*/
	Coordinate new_coordinate = { new_coordinate_x_1,coordinate_1->y };

	/*Verify if a boat is already placed and show valide location for boat placement*/
	if (grid2->grid[coordinate_1->y][new_coordinate_x_1] == 'O' && placement_colision(boat_select, boat_info, coordinate_1, &new_coordinate, grid1)) {
		grid2->grid[coordinate_1->y][new_coordinate_x_1] = 'Y';
	}

	/*Update for test*/
	new_coordinate.x = new_coordinate_x_2;
	new_coordinate.y = coordinate_1->y;

	if (grid2->grid[coordinate_1->y][new_coordinate_x_2] == 'O' && placement_colision(boat_select, boat_info, coordinate_1, &new_coordinate, grid1)) {
		grid2->grid[coordinate_1->y][new_coordinate_x_2] = 'Y';
	}

	/*Update for test*/
	new_coordinate.x = coordinate_1->x;
	new_coordinate.y = new_coordinate_y_1;

	if (grid2->grid[new_coordinate_y_1][coordinate_1->x] == 'O' && placement_colision(boat_select, boat_info, coordinate_1, &new_coordinate, grid1)) {
		grid2->grid[new_coordinate_y_1][coordinate_1->x] = 'Y';
	}

	/*Update for test*/
	new_coordinate.x = coordinate_1->x;
	new_coordinate.y = new_coordinate_y_2;

	if (grid2->grid[new_coordinate_y_2][coordinate_1->x] == 'O' && placement_colision(boat_select, boat_info, coordinate_1, &new_coordinate, grid1)) {
		grid2->grid[new_coordinate_y_2][coordinate_1->x] = 'Y';
	}
	grid_draw_container(grid2, true);
}

bool placement_colision(char boat_select, BoatInfo* boat_info, Coordinate* coordinate_1, Coordinate* coordinate_2, GameBoard* grid) {
	/*Verify that no boat is in the way for placement*/
	if (coordinate_2->x == coordinate_1->x && coordinate_2->y < coordinate_1->y) { /*Above orgin*/
		for (int i = 0; i <= boat_info->bll[(int)boat_select]; i++) {
			if (!(grid->grid[coordinate_2->y + i][coordinate_1->x] == 'O')) {
				return false;
			}
		}
	}
	if (coordinate_2->x == coordinate_1->x && coordinate_2->y > coordinate_1->y) { /*Below orgin*/
		for (int i = 0; i <= boat_info->bll[(int)boat_select]; i++) {
			if (!(grid->grid[coordinate_2->y - i][coordinate_1->x] == 'O')) {
				return false;
			}
		}
	}
	if (coordinate_2->y == coordinate_1->y && coordinate_2->x < coordinate_1->x) { /*Left from orgin*/
		for (int i = 0; i <= boat_info->bll[(int)boat_select]; i++) {
			if (!(grid->grid[coordinate_1->y][coordinate_2->x + i] == 'O')) {
				return false;
			}
		}
	}
	if (coordinate_2->y == coordinate_1->y && coordinate_2->x > coordinate_1->x) { /*Right from orgin*/
		for (int i = 0; i <= boat_info->bll[(int)boat_select]; i++) {
			if (!(grid->grid[coordinate_1->y][coordinate_2->x - i] == 'O')) {
				return false;
			}
		}
	}
	return true;
}

void placement_boat(char boat_select, BoatInfo* boat_info, Coordinate* coordinate_1, Coordinate* coordinate_2, GameBoard* grid) {
	if (coordinate_2->y < coordinate_1->y) { /*Above orgin*/
		for (int i = 0; i < boat_info->bll[(int)boat_select]; i++) {
			grid->grid[coordinate_1->y - i][coordinate_1->x] = boat_info->bsl[(int)boat_select];
		}
	}
	if (coordinate_2->y > coordinate_1->y) { /*Bellow orgin*/
		for (int i = 0; i < boat_info->bll[(int)boat_select]; i++) {
			grid->grid[coordinate_1->y + i][coordinate_1->x] = boat_info->bsl[(int)boat_select];
		}
	}
	if (coordinate_2->x < coordinate_1->x) { /*Left from orgin*/
		for (int i = 0; i < boat_info->bll[(int)boat_select]; i++) {
			grid->grid[coordinate_1->y][coordinate_1->x - i] = boat_info->bsl[(int)boat_select];
		}
	}
	if (coordinate_2->x > coordinate_1->x) { /*Right from orgin*/
		for (int i = 0; i < boat_info->bll[(int)boat_select]; i++) {
			grid->grid[coordinate_1->y][coordinate_1->x + i] = boat_info->bsl[(int)boat_select];
		}
	}
}

void placement_random(BoatInfo* boat_info, GameBoard* grid, Boat_Coordinate* bc) {
	char boat_placed = 0;
	while (boat_placed < boat_info->bn) {
		int lengh = boat_info->bll[(int)boat_placed] - 1;
		while (true) {
			Coordinate new_coordinate_1 = { random_number(9) + 1, random_number(9) + 1 };
			Coordinate new_coordinate_2;
			switch (random_number(3)) {
			case 3:
				new_coordinate_2.x = (new_coordinate_1.x + lengh);
				new_coordinate_2.y = new_coordinate_1.y;
				break;
			case 2:
				new_coordinate_2.x = (new_coordinate_1.x - lengh);
				new_coordinate_2.y = new_coordinate_1.y;
				break;
			case 1:
				new_coordinate_2.x = new_coordinate_1.x;
				new_coordinate_2.y = (new_coordinate_1.y + lengh);
				break;
			default:
				new_coordinate_2.x = new_coordinate_1.x;
				new_coordinate_2.y = (new_coordinate_1.y - lengh);
				break;
			}
			if (placement_colision(boat_placed, boat_info, &new_coordinate_1, &new_coordinate_2, grid)) {
				placement_boat(boat_placed, boat_info, &new_coordinate_1, &new_coordinate_2, grid);
				bc[boat_placed + 1].start = new_coordinate_1;
				bc[boat_placed + 1].stop = new_coordinate_2;
				boat_placed++;
				break;
			}
		}
	}
}

void scan_map(BoatInfo* boat_info, Coordinate* coordinate, GameBoard* grid, Boat_Coordinate* bc) {
	int match = 0;
	int destoyed;
	/*Loop over each boat*/
	for (int i = 1; i < boat_info->bn + 1; i++) {
		destoyed = 0;
		/*Verify the orientation of the boat since Boat_Coordinate bc save two coordinate for each boat*/
		if (bc[i].start.x > bc[i].stop.x && bc[i].start.y == bc[i].stop.y) {
			/*Loop for each valid coordinate of the boat*/
			for (int j = 0; j <= (bc[i].start.x - bc[i].stop.x); j++) {
				/*If the given coordinate is inside the coordinate range of a boat, and since we know that the target() function will only return true if a boat was not already targeted at that coordinate,
				then it means that this boat was just destroyed at that location.*/
				if (coordinate->x == (bc[i].start.x - j) && coordinate->y == bc[i].start.y) {
					match++;
					destoyed++;
				}
				/*Count if the boat is destroyed at this location*/
				else if (grid->grid[bc[i].start.y][(bc[i].start.x - j)] == 'X') {
					destoyed++;
				}
			}
			/*If all the coordinates that belong to a boat are destroyed, and a new hit is detected, then it can be concluded that the boat has just been sunk*/
			if (match != 0 && destoyed == ((bc[i].start.x - bc[i].stop.x) + 1)) {
				printf("You've destroyed a %s", boat_info->bl[(i - 1)]);
			}
		}
		if (bc[i].start.x < bc[i].stop.x && bc[i].start.y == bc[i].stop.y) {
			for (int j = 0; j <= (bc[i].stop.x - bc[i].start.x); j++) {
				if (coordinate->x == (bc[i].stop.x - j) && coordinate->y == bc[i].start.y) {
					match++;
					destoyed++;
				}
				else if (grid->grid[bc[i].start.y][(bc[i].stop.x - j)] == 'X') {
					destoyed++;
				}
			}

			if (match != 0 && destoyed == ((bc[i].stop.x - bc[i].start.x) + 1)) {
				printf("You've destroyed a %s", boat_info->bl[(i - 1)]);
			}
		}
		if (bc[i].start.x == bc[i].stop.x && bc[i].start.y > bc[i].stop.y) {
			for (int j = 0; j <= (bc[i].start.y - bc[i].stop.y); j++) {
				if (coordinate->y == (bc[i].start.y - j) && coordinate->x == bc[i].start.x) {
					match++;
					destoyed++;
				}
				else if (grid->grid[(bc[i].start.y - j)][bc[i].start.x] == 'X') {
					destoyed++;
				}
			}

			if (match != 0 && destoyed == ((bc[i].start.y - bc[i].stop.y) + 1)) {
				printf("You've destroyed a %s", boat_info->bl[(i - 1)]);
			}
		}
		if (bc[i].start.x == bc[i].stop.x && bc[i].start.y < bc[i].stop.y) {
			for (int j = 0; j <= (bc[i].stop.y - bc[i].start.y); j++) {
				if (coordinate->y == (bc[i].stop.y - j) && coordinate->x == bc[i].start.x) {
					match++;
					destoyed++;
				}
				else if (grid->grid[(bc[i].stop.y - j)][bc[i].start.x] == 'X') {
					destoyed++;
				}
			}

			if (match != 0 && destoyed == ((bc[i].stop.y - bc[i].start.y) + 1)) {
				printf("You've destroyed a %s", boat_info->bl[(i - 1)]);
			}
		}
	}
}

void print_boat_coordinates(Boat_Coordinate* bc, int size) {
	for (int i = 1; i < size + 1; i++) {
		printf("Boat %d: (%d, %d) to (%d, %d)\n", i, bc[i].start.x, bc[i].start.y, bc[i].stop.x, bc[i].stop.y);
	}
	printf("\n");
}

Coordinate ai_shoot(bool memory) {
	static bool shot[10][10] = { false };
	Coordinate coordinate;

	do {
		coordinate.x = random_number(10) + 1;
		coordinate.y = random_number(10) + 1;
	} while (memory && shot[coordinate.x][coordinate.y]);

	shot[coordinate.x][coordinate.y] = true;

	return coordinate;
}
