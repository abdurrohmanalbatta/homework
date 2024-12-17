#include "game.h"

void clear_input() {
    while (getchar() != '\n');
}


void initialize_game(Player *player, Room rooms[]) {
    player->health = 100;
    player->strength = 20;
    player->inventory_count = 0;

    for (int i = 0; i < MAX_ROOMS; i++) {
        sprintf(rooms[i].description, "This is room %d.", i + 1);
        rooms[i].item_count = 0;
        rooms[i].creature = NULL;
        for (int j = 0; j < 4; j++) {
            rooms[i].connections[j] = NULL;
        }
    }

    rooms[0].connections[1] = &rooms[1];
    rooms[1].connections[0] = &rooms[0];

    strcpy(rooms[0].items[0].name, "Sword");
    rooms[0].item_count = 1;

    rooms[1].creature = malloc(sizeof(Creature));
    strcpy(rooms[1].creature->name, "Goblin");
    rooms[1].creature->health = 50;
    rooms[1].creature->attack_power = 10;

    player->current_room = &rooms[0];
}


void display_room(Room *room) {
    printf("\n%s\n", room->description);
    if (room->item_count > 0) {
        printf("Items in this room: ");
        for (int i = 0; i < room->item_count; i++) {
            printf("%s ", room->items[i].name);
        }
        printf("\n");
    }
    if (room->creature) {
        printf("You see a %s here!\n", room->creature->name);
    }
}

void move_player(Player *player, char *direction) {
    int dir = -1;
    if (strcmp(direction, "up") == 0) dir = 0;
    else if (strcmp(direction, "down") == 0) dir = 1;
    else if (strcmp(direction, "left") == 0) dir = 2;
    else if (strcmp(direction, "right") == 0) dir = 3;

    if (dir != -1 && player->current_room->connections[dir]) {
        player->current_room = player->current_room->connections[dir];
        printf("You moved %s.\n", direction);
        display_room(player->current_room);
    } else {
        printf("You can't move in that direction!\n");
    }
}

void pickup_item(Player *player, char *item_name) {
    if (player->inventory_count >= MAX_INVENTORY) {
        printf("Your inventory is full!\n");
        return;
    }
    for (int i = 0; i < player->current_room->item_count; i++) {
        if (strcmp(player->current_room->items[i].name, item_name) == 0) {
            strcpy(player->inventory[player->inventory_count].name, item_name);
            player->inventory_count++;
            printf("Picked up %s.\n", item_name);
            for (int j = i; j < player->current_room->item_count - 1; j++) {
                player->current_room->items[j] = player->current_room->items[j + 1];
            }
            player->current_room->item_count--;
            return;
        }
    }
    printf("Item not found in this room.\n");
}


void list_inventory(Player *player) {
    printf("Inventory: ");
    for (int i = 0; i < player->inventory_count; i++) {
        printf("%s ", player->inventory[i].name);
    }
    printf("\n");
}


void attack_creature(Player *player) {
    if (player->current_room->creature) {
        Creature *creature = player->current_room->creature;
        while (player->health > 0 && creature->health > 0) {
            creature->health -= player->strength;
            printf("You attacked the %s. Its health is now %d.\n", creature->name, creature->health);
            if (creature->health <= 0) {
                printf("You defeated the %s!\n", creature->name);
                free(creature);
                player->current_room->creature = NULL;
                return;
            }
            player->health -= creature->attack_power;
            printf("The %s attacked you. Your health is now %d.\n", creature->name, player->health);
        }
        if (player->health <= 0) {
            printf("You died! Game over.\n");
            exit(0);
        }
    } else {
        printf("There is nothing to attack here.\n");
    }
}

void process_command(char *command, Player *player, Room rooms[]) {
    char cmd[MAX_COMMAND_LEN], arg[MAX_COMMAND_LEN];
    if (sscanf(command, "%s %s", cmd, arg) == 2) {
        if (strcmp(cmd, "move") == 0) move_player(player, arg);
        else if (strcmp(cmd, "pickup") == 0) pickup_item(player, arg);
        else printf("Unknown command: %s\n", cmd);
    } else if (sscanf(command, "%s", cmd) == 1) {
        if (strcmp(cmd, "look") == 0) display_room(player->current_room);
        else if (strcmp(cmd, "inventory") == 0) list_inventory(player);
        else if (strcmp(cmd, "attack") == 0) attack_creature(player);
        else if (strcmp(cmd, "exit") == 0) exit(0);
        else printf("Unknown command: %s\n", cmd);
    }
}

// Main function
int main() {
    Player player;
    Room rooms[MAX_ROOMS];
    char command[MAX_COMMAND_LEN];

    initialize_game(&player, rooms);
    printf("Welcome to the Dungeon Adventure Game!\n");
    display_room(player.current_room);

    while (1) {
        printf("\n> ");
        fgets(command, MAX_COMMAND_LEN, stdin);
        command[strcspn(command, "\n")] = '\0';
        process_command(command, &player, rooms);
    }

    return 0;
}

