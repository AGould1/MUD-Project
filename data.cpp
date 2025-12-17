#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include <ctype.h>

/** Add code for .c here **/
ItemType get_item_type(char *line) {
    char *type_str = extract_string(line);
    if (!type_str) return ITEM_TYPE_NONE;

    ItemType type = ITEM_TYPE_NONE;
    if (strcasecmp(type_str, "general") == 0)
        type = ITEM_TYPE_GENERAL;
    else if (strcasecmp(type_str, "quest") == 0)
        type = ITEM_TYPE_QUEST;
    else if (strcasecmp(type_str, "potion") == 0)
        type = ITEM_TYPE_POTION;
    else if (strcasecmp(type_str, "weapon") == 0)
        type = ITEM_TYPE_WEAPON;
    else if (strcasecmp(type_str, "armor") == 0)
        type = ITEM_TYPE_ARMOR;

    free(type_str);
    return type;
}


Room *load_json_rooms(char *filename, int roomMaxId) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening rooms file");
        return nullptr;
    }
    Room *rooms_list = (Room*) malloc(sizeof(Room) * (roomMaxId + 1));
    if (!rooms_list) {
        perror("Failed to allocate rooms_list");
        fclose(file);
        return nullptr;
    }
    Room defaultRoom = {-1, nullptr, nullptr, -1, -1, -1, -1, -1, 0};
    for (int i = 0; i <= roomMaxId; i++) {
        rooms_list[i] = defaultRoom;
    }
    char line[2048];
    Room tempRoom = defaultRoom;

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "{")) {
            tempRoom = defaultRoom;
        }
        else if (strstr(line, "\"id\"")) {
            sscanf(line, " \"%*[^:]: %d", &tempRoom.id);
        }
        else if (strstr(line, "\"name\"")) {
            char name_buffer[256];
            if (sscanf(line, " \"%*[^:]: \"%255[^\"]", name_buffer) == 1) {
                tempRoom.name = strdup(name_buffer);
            }
        }
        else if (strstr(line, "\"description\"")) {
            char desc_buffer[2048];
            if (sscanf(line, " \"%*[^:]: \"%1023[^\"]", desc_buffer) == 1) {
                tempRoom.description = strdup(desc_buffer);
            }
        }
        else if (strstr(line, "\"north\"")) {
            if (strstr(line, "null")) {
                tempRoom.north = -1;
            }
            else {
                sscanf(line, " \"%*[^:]: %d", &tempRoom.north);
            }
        }
        else if (strstr(line, "\"south\"")) {
            if (strstr(line, "null")) {
                tempRoom.south = -1;
            }
            else {
                sscanf(line, " \"%*[^:]: %d", &tempRoom.south);
            }
        }
        else if (strstr(line, "\"east\"")) {
            if (strstr(line, "null")) {
                tempRoom.east = -1;
            }
            else {
                sscanf(line, " \"%*[^:]: %d", &tempRoom.east);
            }
        }
        else if (strstr(line, "\"west\"")) {
            if (strstr(line, "null")) {
                tempRoom.west = -1;
            }
            else {
                sscanf(line, " \"%*[^:]: %d", &tempRoom.west);
            }
        }
        else if (strstr(line, "\"item\"")) {
            if (strstr(line, "null")) {
                tempRoom.item = -1;
            }
            else {
                sscanf(line, " \"%*[^:]: %d", &tempRoom.item);
            }
        }
        else if (strstr(line, "\"starting\"")) {
            if (strstr(line, "null")) {
                tempRoom.starting = 0;
            }
            else {
                sscanf(line, " \"%*[^:]: %d", &tempRoom.starting);
            }
        }
        else if (strstr(line, "}")) {
            if (tempRoom.id >= 0 && tempRoom.id <= roomMaxId) {
                rooms_list[tempRoom.id] = tempRoom;
            }
        }
    }
    fclose(file);
    return rooms_list;
}

int extract_int(char *line) {
    char *colon = strchr(line, ':');
    if (!colon) return -1;

    char *start = colon + 1;
    while (*start && isspace(*start)) start++;

    if (strstr(start, "null")) return -1;

    int value;
    if (sscanf(start, "%d", &value) == 1) {
        return value;
    }
    return -1;
}

int find_max(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == nullptr) {
        perror("Error opening file");
        return -1;
    }
    int maxId = -1;
    char line[256];

    while(fgets(line, sizeof(line), file)) {
        char *idPos = strstr(line, "\"id\"");
        if (idPos != nullptr) {
            int id;
            if (sscanf(idPos, "\"id\" : %d", &id) == 1) {
                if (id > maxId) {
                    maxId = id;
                }
            }
        }
    }
    fclose(file);
    return maxId;
}
char *extract_string(char *line) {
    char *colon = strchr(line, ':');
    if (!colon) return nullptr;

    char *start = colon + 1;
    while (*start && isspace(*start)) start++;

    if (*start != '"') return nullptr;
    start++;

    char *end_quote = strchr(start, '"');
    if (!end_quote) return nullptr;

    size_t len = end_quote - start;
    char *result = (char*) malloc(len + 1);
    if (!result) return nullptr;

    strncpy(result, start, len);
    result[len] = '\0';
    return result;
}
Item *load_json_items(char *filename, int itemMaxId) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening items.json");
        return nullptr;
    }
    Item *items_list = (Item*) malloc(sizeof(Item) * (itemMaxId + 1));
    if (!items_list) {
        perror("Memory allocation failed");
        return nullptr;
    }

    Item init_item = { .id = -1, .name = nullptr, .description = nullptr, .value = 0, .damage = 0, .type = ITEM_TYPE_NONE};
    for (int i = 0; i <= itemMaxId; i++) {
        items_list[i] = init_item;
    }
    char line[2048];
    Item current;
    int in_item = 0;
    int lastValidItemId = -1;

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "{")) {
            current = init_item;
            in_item = 1;
            continue;
        }
        if (strstr(line, "}")) {
            if (current.id >= 0 && current.id <= itemMaxId) {
                items_list[current.id] = current;
                lastValidItemId = current.id;
            }
            in_item = 0;
            continue;
        }
        if (in_item) {
            if (strstr(line, "\"id\"")) {
                current.id = extract_int(line);
            } else if (strstr(line, "\"name\"")) {
                current.name = extract_string(line);
            } else if (strstr(line, "\"description\"")) {
                current.description = extract_string(line);
            } else if (strstr(line, "\"value\"")) {
                current.value = extract_int(line);
            } else if (strstr(line, "\"damage\"")) {
                current.damage = extract_int(line);
            } else if (strstr(line, "\"type\"")) {
                current.type = get_item_type(line);
            }
        }
    }
    if (lastValidItemId >= 0 && lastValidItemId <= itemMaxId) {
            items_list[lastValidItemId].last = true;
    }
    fclose(file);
    return items_list;
}