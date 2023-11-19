/*By PSR_J0740*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define default_square_size 11
#define max_boat            15
#define number_of_boat		5  //Can't be changed for more than 5 due to boat_symbol


/*Variable Define*/
static char grid_alpha[11] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J' };
static char boat_name[number_of_boat][25] = { "Aircraft Carrier","Cruiser","Against torpedo boat","Submarine","Torpedo boat" };
static char boat_symbol[9] = { 'H', 'I', 'U', 'L', 'T' , 'O', 'X', 'Y', 'D' };
static char boat_lengh[number_of_boat] = { 5,4,3,3,2 };
static char boat_number[number_of_boat] = { 1,1,1,1,1 };



/*Struct*/
typedef struct stats {
	int        	numHits;
	int        	numMisses;
	int        	totalShots;
} Stats;

typedef struct coordinate {
	int        	x;
	int        	y;
} Coordinate;

typedef struct boat_coordinate {
	Coordinate 	start;
	Coordinate 	stop;
} Boat_Coordinate;

typedef struct {
	char grid[default_square_size][default_square_size];
} GameBoard;

typedef struct {
	int 		bn;					/*boat_number*/
	int 		bpn;				/*boat_placed_number*/
	char 		bl[max_boat][25];  	/*boat_list*/
	char 		bsl[max_boat]; 		/*boat_symbol_list*/
	int 		bll[max_boat];  	/*boat_lengh_list*/
	bool 		bss[max_boat]; 		/*Boat_select_state*/ /*Used to know what boat is already placed*/
} BoatInfo;

/*Declare function*/

/*Clear Terminal*/
void terminal_clear();
/*Ask for confirmation*/
bool tester(char* value);
/*Place the default value of the board inside the array*/
void grid_gen();
/*Generates a list of boats based on their name and number defined in the BoatInfo struct*/
void boat_list_gen(BoatInfo* boat_info);
/*Clear coordinate of Struct Coordinate*/
void clear_coordinate(Coordinate* coordinate);
/*Take a letter as input and return a number (row) for the array*/
int  coordinate_x_converter(char x, char grid_alpha[default_square_size]);
/*Verfify if a target was hitted*/
bool target(Coordinate* coordinate, GameBoard* taget_grid, GameBoard* grid, Stats* stat, bool attacking);

/*Welcome Menu + rules*/
void rules();
/*Allow one to select dificulty for the game*/
void difficulty_menu();
/*Return valid coordinate*/
void get_coordinate(Coordinate* coordinate, char* value);
/*Print a beautiful gameboard*/
void grid_draw_container(GameBoard* grid, bool outer_call);
/*Print stats of the game*/
void print_stats(Stats* stats);

/*Perform the most basic verification on coordinate and return valid coordinate*/
bool coordinate_verifier(char coordinate_select[10], Coordinate* temporary_coordinate);
/*Verify that the coordinate inputed are valide (perform the first verification)*/
bool placement_coordinate_verifier(char boat_select, char coordinate_select[10], bool* second_coordinate, BoatInfo* boat_info, GameBoard* player, GameBoard* place_board, Coordinate* coordinate_1, Coordinate* coordinate_2);
/*Place red 'Y' on the location the player can chose to place the boat, that action is performed on any array and require a copy of the lattest map of the game to work*/
void placement_valid_coordinate(char boat_select, BoatInfo* boat_info, Coordinate* coordinate_1, Coordinate* coordinate_2, GameBoard* grid1, GameBoard* grid2); /*Draw a grid that show where the boat can be placed*/
/*Verify that there is nothing in the way of the boat based on the two coordinate*/
bool placement_colision(char boat_select, BoatInfo* boat_info, Coordinate* coordinate_1, Coordinate* coordinate_temp, GameBoard* grid);
/*Place the selected boat 'boat_select' on the map using the given coordinate (no verification is performed)*/
void placement_boat(char boat_select, BoatInfo* boat_info, Coordinate* coordinate_1, Coordinate* coordinate_2, GameBoard* grid);
/*Place boat randomly*/
void placement_random(BoatInfo* boat_info, GameBoard* grid, Boat_Coordinate* bc);
/*Scan for a new destoyed boat*/
void scan_map(BoatInfo* boat_info, Coordinate* coordinate, GameBoard* grid, Boat_Coordinate* bc);
/*Return random coordinate to shoot at. It can also remember or not previous target*/
Coordinate ai_shoot(bool memory);
