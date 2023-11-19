/*Jeu de la battaile naval:  https://www.regles-de-jeux.com/regle-de-la-bataille-navale/ */
/*By PSR_J0740*/

#include "battle_ship.h"



int main() {
	/*Setup ...*/
	srand(time(NULL));
	rules();
	int difficulty = 0;

	difficulty_menu(&difficulty);

	GameBoard player;
	GameBoard ai;
	GameBoard place_board;
	GameBoard empty_board;

	grid_gen(&player);
	grid_gen(&ai);

	BoatInfo* boat_info = (BoatInfo*)malloc(sizeof(BoatInfo));
	boat_list_gen(boat_info);
	boat_info->bpn = 0;

	Boat_Coordinate player_boat[boat_info->bn + 1];
	Boat_Coordinate ai_boat[boat_info->bn + 1];


	if (!tester("Place boat randomly ?")) {


		Coordinate* coordinate_1 = (Coordinate*)malloc(sizeof(Coordinate));
		Coordinate* coordinate_2 = (Coordinate*)malloc(sizeof(Coordinate));

		/*Loop for placement*/
		while ((boat_info->bpn) < (boat_info->bn)) {
			clear_coordinate(coordinate_1);
			clear_coordinate(coordinate_2);
			char boat_select = 0; /*Contain the input from the scanf*/

			/*Loop for the selection of the boat to be palced*/
			while (true) {

				grid_draw_container(&player, false);

				/*List boat*/
				printf("Choose a boat:\n");
				for (int i = 0; i < boat_info->bn; i++) {
					if (!boat_info->bss[i]) {
						printf("\033[0;31m %d. %s\033[0m\n", i, boat_info->bl[i]);
					}
					else {
						printf("%d. %s [Lengh : %d]\n", i, boat_info->bl[i], boat_info->bll[i]);
					}
					printf("\n");
				}

				printf("\nInput: ");
				scanf_s("%2d", &boat_select);
				while (getchar() != '\n');
				terminal_clear();


				/*Placement verifier*/

				if (boat_select >= 0 && (int)boat_select < boat_info->bn && boat_info->bss[(int)boat_select]) {
					printf("You selected %s \n\n", boat_info->bl[(int)boat_select]);
					break;
				}
				else {
					printf("Invalid number or boat already placed\n");
				}
			}


			bool second_coordinate = false;
			char coordinate_select[10];
			/*Loop that place the boat and perform all verification (AKA Placement Handler)*/
			while (true) {
				if (!second_coordinate) {
					while (true) {
						grid_draw_container(&player, false);
						printf("Enter the first coordinate like 'a.1': \nInput: ");
						scanf_s("%s", coordinate_select, 5);
						if (placement_coordinate_verifier(boat_select, coordinate_select, &second_coordinate, boat_info, &player, &place_board, coordinate_1, coordinate_2)) {
							terminal_clear();
							break;
						}
						terminal_clear();
					}
				}
				else {
					while (true) {
						terminal_clear();
						memcpy(place_board.grid, player.grid, sizeof(char) * default_square_size * default_square_size);
						placement_valid_coordinate(boat_select, boat_info, coordinate_1, coordinate_2, &player, &place_board);

						printf("Enter the second coordinate like 'a.1': \nInput: ");
						scanf_s("%s", coordinate_select, 5);
						if (placement_coordinate_verifier(boat_select, coordinate_select, &second_coordinate, boat_info, &player, &place_board, coordinate_1, coordinate_2)) {
							terminal_clear();
							break;
						}
						terminal_clear();
					}
					if (tester(boat_info->bl[(int)boat_select])) {
						break;
					}
					else {
						second_coordinate = false;
					}
					terminal_clear();
				}

			}

			placement_boat(boat_select, boat_info, coordinate_1, coordinate_2, &player);
			terminal_clear();
			(boat_info->bpn)++;
			printf("\nBoat Placed!!\n\n");
			boat_info->bss[(int)boat_select] = false;

		}
		free(coordinate_1);
		free(coordinate_2);

	}
	else {
		placement_random(boat_info, &player, &player_boat[0]);
	}


	placement_random(boat_info, &ai, &ai_boat[0]);

	grid_gen(&place_board);


	/*GAME ...*/


	/*Set the number of point a boat is placed: Used to find a winner*/
	int number_elements = 0;
	for (int i = 0; i < boat_info->bn; i++) {
		number_elements += boat_info->bll[i];
	}

	Stats player_stat = { 0,0,0 };
	Stats ai_stat = { 0,0,0 };

	Coordinate target_coordinate = { 0,0 };
	Coordinate ai_coordinate = { 0,0 };

	terminal_clear();

	/*Game difficulty*/
	if (difficulty == 1) {
		/*Easy*/
		while ((player_stat.numHits < number_elements) && (ai_stat.numHits < number_elements)) {
			/*Player Turn*/
			terminal_clear();
			if (target(&target_coordinate, &ai, &place_board, &player_stat, true)) {
				scan_map(boat_info, &target_coordinate, &ai, &ai_boat[0]);
			}

			printf("\nTarget %d  |  Hitted %d  |  Missed %d\n", number_elements, player_stat.numHits, player_stat.numMisses);

			printf("Player Grid:\n");
			grid_draw_container(&player, false);
			printf("Targeted:\n");
			grid_draw_container(&place_board, false);
			get_coordinate(&target_coordinate, "Enter the target coordinate like 'a.1':");

			/*AI Turn*/
			ai_coordinate = ai_shoot(true);
			target(&ai_coordinate, &player, &empty_board, &ai_stat, false);
		}
	}
	if (difficulty == 2) {
		/*Hard*/
		while ((player_stat.numHits < number_elements) && (ai_stat.numHits < number_elements)) {
			/*Player Turn*/
			terminal_clear();
			printf("\nTarget %d  |  Hitted %d  |  Missed %d\n", number_elements, player_stat.numHits, player_stat.numMisses);
			if (target(&target_coordinate, &ai, &place_board, &player_stat, true)) {
				scan_map(boat_info, &target_coordinate, &ai, &ai_boat[0]);
			}
			printf("Player Grid:\n");
			grid_draw_container(&player, false);
			get_coordinate(&target_coordinate, "Enter the target coordinate like 'a.1':");

			/*AI Turn*/
			ai_coordinate = ai_shoot(true);
			target(&ai_coordinate, &player, &empty_board, &ai_stat, false);
		}

	}
	if (difficulty == 3) {
		/*Hardcore*/
		while ((player_stat.numHits < number_elements) && (ai_stat.numHits < number_elements)) {
			/*Player Turn*/
			terminal_clear();
			printf("\nTarget %d  |  Hitted %d  |  Missed %d\n", number_elements, player_stat.numHits, player_stat.numMisses);
			printf("Player Grid:\n");
			grid_draw_container(&player, false);
			get_coordinate(&target_coordinate, "Enter the target coordinate like 'a.1':");
			target(&target_coordinate, &ai, &place_board, &player_stat, true);

			/*AI Turn*/
			ai_coordinate = ai_shoot(true);
			target(&ai_coordinate, &player, &empty_board, &ai_stat, false);
		}
	}


	free(boat_info);
	/*Post game messages*/

	if (player_stat.numHits < number_elements) {
		printf("You've lost you LOOOOOOOOOOSER :P\n");
	}
	else {
		printf("You won the game YEAH!!\n");
	}


	printf("Player stats:\n");
	print_stats(&player_stat);
	printf("\n\n\nAI stats:\n");
	print_stats(&ai_stat);

	/*Pause the game to not close the windows !!DEBUG!!*/
	char end;
	printf("end");
	scanf_s("%2d", &end);
}
