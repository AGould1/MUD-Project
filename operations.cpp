#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "operations.h"
#include "data.h"
#include "shop.h"
#include "ui.h"
#include "gym.h"
#define INVENTORY_SIZE 100

int player_inventory[INVENTORY_SIZE];
int inventory_count = 0;

void display_inventory(Item items[]) {
    if (inventory_count == 0) {
        printf("You currently have nothing in your backpack\n");
    } else {
        printf("Your backpack of infinite holding contains:\n");
        for (int i = 0; i < inventory_count; i++) {
            int item_id = player_inventory[i];
            printf("  + [%d] %s\n", item_id, items[item_id].name);
        }
    }
}


char* name_to_ucase(const char* name) {
    if (name == NULL) {
        return NULL;
    }
    char* upper = (char*) malloc(strlen(name) + 1);
    if (upper == NULL) {
        return NULL;
    }
    for (int i = 0; name[i]; i++) {
        upper[i] = toupper(name[i]);
    }
    upper[strlen(name)] = '\0';
    return upper;
}

void gameLoop(Room rooms[], Item items[], int startingRoomId, int itemMaxId) {
    int currentRoomId = startingRoomId;

    while (1) {
        Room* currentRoom = &rooms[currentRoomId];

        char* upperName = name_to_ucase(currentRoom->name);
        printf("#%d : %s\n", currentRoom->id, upperName);
        free(upperName);

        printf("%s\n", currentRoom->description);

        if (currentRoom->item > -1 && items[currentRoom->item].name != NULL) {
            printf("you see a %s\n", items[currentRoom->item].name);
        }

        printf("[ ");
        if (currentRoom->north != -1) {
            printf("[n]orth, ");
        }
        if (currentRoom->south != -1) {
            printf("[s]outh, ");
        }
        if (currentRoom->east != -1) {
            printf("[e]ast, ");
        }
        if (currentRoom->west != -1) {
            printf("[w]est, ");
        }
        printf("[l]ook, [g]et, [i]nventory, [d]rop, [q]uit ]\n");

        printf("> ");

        char input[10];
        if (fgets(input, sizeof(input), stdin)) {
            input[strcspn(input, "\n")] = 0;

            if (input[0] == 'q') {
                printf("Exiting game\n");
                break;
            }
            else if (input[0] == 'p' || strcmp(input, "pokemon") == 0) {
                pokemonMenu();
            }
            else if (input[0] == 'c' || strcmp(input, "challenge") == 0) {
                int trainerToChallenge = -1;
                if (currentRoomId == 324) {
                    trainerToChallenge = 0;
                }
                else if (currentRoomId == 344) {
                    trainerToChallenge = 1;
                }
                else if (currentRoomId == 343) {
                    trainerToChallenge = 2;
                }
                else if (currentRoomId == 328) {
                    trainerToChallenge = 3;
                }
                else if (currentRoomId == 325) {
                    trainerToChallenge = 4;
                }
                else if (currentRoomId == 326) {
                    trainerToChallenge = 5;
                }
                else if (currentRoomId == 322) {
                    trainerToChallenge = 6;
                }
                else if (currentRoomId == 331) {
                    trainerToChallenge = 7;
                }
                else if (currentRoomId == 329) {
                    trainerToChallenge = 8;
                }
                else if (currentRoomId == 327) {
                    trainerToChallenge = 9;
                }
                else if (currentRoomId == 323) {
                    trainerToChallenge = 10;
                }
                else if (currentRoomId == 358) {
                    trainerToChallenge = 11;
                }

                if (trainerToChallenge != -1) {
                    challengeTrainer(trainerToChallenge);
                }
                else {
                    printf("There is no trainer to challenge in this room.\n");
                }
            }
            else if (strcmp(input, "sh") == 0 || strcmp(input, "shop") == 0) {
                Shop weaponShop(items, ITEM_TYPE_WEAPON);
                NonWeaponShop generalShop(items, ITEM_TYPE_GENERAL);
                NonWeaponShop armorShop(items, ITEM_TYPE_ARMOR);
                NonWeaponShop potionShop(items, ITEM_TYPE_POTION);

                if (currentRoomId == 10) {
                    generalShop.enter(player_inventory, &inventory_count);
                }
                else if (currentRoomId == 11) {
                    weaponShop.enter(player_inventory, &inventory_count);
                }
                else if (currentRoomId == 20) {
                    armorShop.enter(player_inventory, &inventory_count);
                }
                else if (currentRoomId == 33) {
                    potionShop.enter(player_inventory, &inventory_count);
                }
                else {
                    weaponShop.enter(player_inventory, &inventory_count);
                }           
            }
            else if (input[0] == 'n' && currentRoom->north != -1) {
                currentRoomId = currentRoom->north;
            } 
            else if (input[0] == 's' && currentRoom->south != -1) {
                currentRoomId = currentRoom->south;
            } 
            else if (input[0] == 'e' && currentRoom->east != -1) {
                currentRoomId = currentRoom->east;
            } 
            else if (input[0] == 'w' && currentRoom->west != -1) {
                currentRoomId = currentRoom->west;
            }
            else if (input[0] == 'g') {
                if (currentRoom->item > -1) {
                    if (inventory_count < INVENTORY_SIZE) {
                        int item_id = currentRoom->item;
                        player_inventory[inventory_count] = item_id;
                        char *item_name = items[item_id].name;

                        inventory_count++;
                        printf("You picked up a %s and put it into your backpack\n", item_name);
                        currentRoom->item = -1;
                    }
                    else {
                        printf("Your backpack is full!\n");
                    }
                }
                else {
                    printf("There is nothing here to pick up.\n");
                }
            }
            else if (input[0] == 'i') {
                display_inventory(items);
            }
            else if (input[0] == 'd') {
                if (inventory_count == 0) {
                    printf("You have nothing in your backpack to drop.\n");
                }
                else {
                    display_inventory(items);
                    printf("Enter the item ID to drop: ");

                    char drop_input[20];
                    if (fgets(drop_input, sizeof(drop_input), stdin)) {
                        drop_input[strcspn(drop_input, "\n")] = 0;
                        int item_to_drop = atoi(drop_input);
                        int found_index = -1;
                        for (int i = 0; i < inventory_count; i++) {
                            if (player_inventory[i] == item_to_drop) {
                                found_index = i;
                                break;
                            }
                        }
                        if (found_index == -1) {
                            printf("Item does not exist in backpack\n");
                        }
                        else {
                            for (int i = found_index; i < inventory_count - 1; i++) {
                                player_inventory[i] = player_inventory[i + 1];
                            }
                            inventory_count--;
                            currentRoom->item = item_to_drop;
                            printf("You dropped a %s\n", items[item_to_drop].name);
                        }
                    }
                }
            }
            else if (input[0] == 'l') {
                if (currentRoom->item != -1) {
                    printf("%s\n", items[currentRoom->item].description);
                }
                else {
                    printf("There is nothing here to look at.\n");
                }
            }
        }
    }
}