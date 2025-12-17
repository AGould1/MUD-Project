#ifndef DATA_H
#define DATA_H 

#include <stdbool.h>

typedef enum ItemType {
    ITEM_TYPE_NONE=-1,
    ITEM_TYPE_GENERAL,
    ITEM_TYPE_QUEST,
    ITEM_TYPE_POTION,
    ITEM_TYPE_WEAPON,
    ITEM_TYPE_ARMOR    
} ItemType;

typedef struct Room {
    /* define fields */
    int id;
    char *name;
    char *description;
    int north;
    int south;
    int east;
    int west;
    int item;
    int starting;
} Room;

typedef struct Item {
    /* define fields */
    int id;
    char *name;
    char *description;
    int value;
    int damage;
    ItemType type;
    bool last;
} Item;

int find_max(char *filename);

Room * load_json_rooms(char * filename, int roomMaxId);

Item * load_json_items(char * filename, int itemMaxId);

int extract_int(char *line);
char *extract_string(char *line);

#endif 