#include <stdio.h>
#include "data.h"
#include <string.h>
#include <stdlib.h>
#include "operations.h"

int main(int argc, char *argv[]) {
    char roomsFilePath[256];
    char itemsFilePath[256];

    if (argc > 1 && argv[1][0] != '\0') {
        snprintf(roomsFilePath, sizeof(roomsFilePath), "%s/rooms.json", argv[1]);
        snprintf(itemsFilePath, sizeof(itemsFilePath), "%s/items.json", argv[1]);
    }
    else {
        snprintf(roomsFilePath, sizeof(roomsFilePath), "./rooms.json");
        snprintf(itemsFilePath, sizeof(itemsFilePath), "./items.json");
    }
    int roomMaxId = find_max(roomsFilePath);
    int itemMaxId = find_max(itemsFilePath);

    struct Item *items_list = load_json_items(itemsFilePath, itemMaxId);
    if (!items_list) {
        fprintf(stderr, "Error loading items\n");
        return 1;
    }

    struct Room *rooms_list = load_json_rooms(roomsFilePath, roomMaxId);
    if (!rooms_list) {
        fprintf(stderr, "Error loading rooms\n");
        for (int i = 0; i <= itemMaxId; i++) {
            if(items_list[i].name) free(items_list[i].name);
            if(items_list[i].description) free(items_list[i].description);
        }
        free(items_list);
        return 1;
    }

    gameLoop(rooms_list, items_list, 1, itemMaxId);

    for (int i = 0; i <= itemMaxId; i++) {
        if(items_list[i].name) free(items_list[i].name);
        if(items_list[i].description) free(items_list[i].description);
    }
    free(items_list);

    for (int i = 0; i <= roomMaxId; i++) {
        free(rooms_list[i].name);
        free(rooms_list[i].description);
    }
    free(rooms_list);

    return 0;
}