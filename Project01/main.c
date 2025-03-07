// Program: Elementary Cellular Automaton
// Author: Jose Ramirez
// Last Updated: 02/07/2025 (v1.8.0)
// v1.8.0: Added to GitHub

#include <stdio.h>
#include <stdbool.h>

// Defines the number of cells in the world
const int WORLD_SIZE = 65;

// Structure to represent each cell.

typedef struct cell_struct{
    bool state[3]; //active status for [left, me, right] cells
    bool active; //current status of the cell
    int total; //running accumulated count of this cell's active status for all generations
} cell;

// Convert an 8-bit integer rule (0 - 255) to an array of bits.
// Returns true if input rule is valid (0 - 255), false otherwise.

bool setBitArray(bool bitArray[8], int rule) {
    if (rule < 0 || rule > 255) return false;
    for (int i = 0; i < 8; i++) {
        bitArray[i] = (rule & (1 << i)) != 0;
    }
    return true;
}

// Convert a 3-bit state array to its associated index of the rule bit array.

int stateToIndex(bool state[3]) {
    return (state[0] << 2) | (state[1] << 1 | state[2]);
}

// Updates the state array for each cell in the world array based on the
// current status of active for the nearby [left,me,right] cells
// The world is assumed to be periodic/cyclic.

void setStates(cell world[WORLD_SIZE]) {
    for (int i = 0; i < WORLD_SIZE; i++) {
        world[i].state[0] = world[(i - 1 + WORLD_SIZE) % WORLD_SIZE].active;
        world[i].state[1] = world[i].active;
        world[i].state[2] = world[(i + 1) % WORLD_SIZE].active;
    }
}

// Evolve each cell's active status to the next generation using its state array.
// Returns the number of active cells in the new generation.

int evolveWorld(cell world[WORLD_SIZE], bool ruleBitArray[8]) {
    int activeCount = 0;
    bool nextGeneration[WORLD_SIZE];

    for (int i = 0; i < WORLD_SIZE; i++) {
        int index = stateToIndex(world[i].state);
        nextGeneration[i] = ruleBitArray[index];
        if (nextGeneration[i]) activeCount++;
    }

    for (int i = 0; i < WORLD_SIZE; i++) {
        world[i].active = nextGeneration[i];
        world[i].total += nextGeneration[i];
    }

    setStates(world);
    return activeCount;
}

// Prints the current state of the world.

void printWorld(cell world[WORLD_SIZE], int generation, int activeCount) {
    for (int i = 0; i < WORLD_SIZE; i++) {
        printf(world[i].active ? "o" : "'");
    }
    printf(" %d\n", activeCount);
}

// Prints the total number of times each cell has been active.

void printTotals(cell world[WORLD_SIZE]) {
    printf("_________________________________________________________________\n");

    for (int line = 0; line < 2; line++) {
        for (int i = 0; i < WORLD_SIZE; i++) {
            int digit = (line == 0) ? (world[i].total / 10) : (world[i].total % 10);

            if (line == 0) {
                printf(digit != 0 ? "%d" : " ", digit);
            } else {
                printf(world[i].total != 0 ? "%d" : " ", digit);
            }
        }
        printf("\n");
    }
}

int main() {
    cell world[WORLD_SIZE]; // Initialize world array.
    int rule;
    bool ruleBitArray[8];

    printf("Welcome to the Elementary Cellular Automaton!\n");

    do {
        printf("Enter the rule # (0-255): ");
        printf("\n");
        scanf("%d", &rule);
        printf("\n");
    } while (!setBitArray(ruleBitArray, rule));

    // Display the bit array for the chosen rule

    printf("The bit array for rule #%d is ", rule);
    for (int i = 7; i >= 0; i--) {
        printf("%d", ruleBitArray[i]);
    }

    printf("\n\n");

    // Display the evolution of all possible states.

    printf("The evolution of all possible states are as follows:\n");
    for (int i = 0; i < 8; i++) {
        bool state[3] = {(i & 4) != 0, (i & 2) != 0, (i & 1) != 0};
        printf("|%c%c%c|\t", state[0] ? '\'' : 'o', state[1] ? '\'' : 'o', state[2] ? '\'' : 'o');
    }

    printf("\n");

    for (int i = 7; i >= 0; i--) {
        printf(" |%c|\t", ruleBitArray[i] ? 'o' : '\'');
    }

    printf("\n\n");

    // Initialize the world with all cells inactive.

    for (int i = 0; i < WORLD_SIZE; i++) {
        world[i].active = false;
        world[i].total = 0;
    }

    world[WORLD_SIZE / 2].active = true; // Activate the center cell.
    setStates(world);

    int generations;

    do{
        printf("Enter the number of generations (1-99): \n");
        scanf("%d", &generations);
    } while (generations < 1 || generations > 99);
    printf("\n");

    printf("Initializing world & evolving...\n");

    printWorld(world, 0, 1);

    // Iterate over the specified number of generations.
    for (int gen = 0; gen < generations; gen++) {
        int activeCount = evolveWorld(world, ruleBitArray);
        printWorld(world, gen + 1, activeCount);
    }

    printTotals(world);
    return 0;
}
