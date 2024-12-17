#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INVENTORY 5
#define MAX_DESCRIPTION 256
#define MAX_ROOMS 10
#define MAX_ITEMS 5
#define MAX_COMMAND_LEN 100

typedef struct Item {
    char name[MAX_DESCRIPTION];
} Item;

typedef struct Creature {
    char name[MAX_DESCRIPTION];
    int health;
    int attack_power;
} Creature;

typedef struct Room {
    char description[MAX_DESCRIPTION];
    struct Room *connections[4]; // 0 = up, 1 = down, 2 = left, 3 = right
    Item items[MAX_ITEMS];
    int item_count;
    Creature *creature;
} Room;

typedef struct Player {
    int health;
    int strength;
    Item inventory[MAX_INVENTORY];
    int inventory_count;
    Room *current_room;
} Player;

// Function Prototypes
void initialize_game(Player *player, Room rooms[]);
void process_command(char *command, Player *player, Room rooms[]);
void display_room(Room *room);
void move_player(Player *player, char *direction);
void pickup_item(Player *player, char *item_name);
void list_inventory(Player *player);
void attack_creature(Player *player);
void save_game(const char *filename, Player *player, Room rooms[]);
void load_game(const char *filename, Player *player, Room rooms[]);
void list_saved_games();

#endif
