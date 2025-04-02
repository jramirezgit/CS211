// Program: 14.2.1 Food Web Analysis
// Author: Jose Ramirez
// Last Updated: 02/20/2025

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Org_struct {
    char name[20];
    int* prey; //dynamic array of indices
    int numPrey;
} Org;

void addOrgToWeb(Org** pWeb, int* pNumOrgs, char* newOrgName) {

    // Check if the food web is currently empty.
    if (*pWeb == NULL) {
        // Allocate memory for one organism's name.
        *pWeb = (Org*)malloc(sizeof(Org));
        // Copy the new organism's name.
        strcpy((*pWeb)[0].name, newOrgName);
        // Initialize the prey pointer to NULL and number of preys to 0.
        (*pWeb)[0].prey = NULL;
        (*pWeb)[0].numPrey = 0;
        // Update the number of organisms.
        *pNumOrgs = 1;
    } else {
        // Calculate the new size (one more than current).
        int oldCount = *pNumOrgs;
        int newCount = oldCount + 1;

        // Allocate new memory for the updated food web.
        Org* newWeb = (Org*)malloc(newCount * sizeof(Org));

        // Copy all existing organisms to the new allocation.
        for (int i = 0; i < oldCount; i++) {
            newWeb[i] = (*pWeb)[i];
        }

        // Free the old memory allocation.
        free(*pWeb);

        // Update the pointer to point to the new allocation.
        *pWeb = newWeb;

        // Append the new organism at the end.
        strcpy((*pWeb)[oldCount].name, newOrgName);
        (*pWeb)[oldCount].prey = NULL;
        (*pWeb)[oldCount].numPrey = 0;

        // Update the number of organisms.
        *pNumOrgs = newCount;
    }
}

bool addRelationToWeb(Org* web, int numOrgs, int predInd, int preyInd) {

    // Check if indices are valid.
    if (predInd < 0 || predInd >= numOrgs || preyInd < 0  || preyInd >= numOrgs || predInd == preyInd) {
        printf("Invalid predator and/or prey index. No relation added to the food web.\n");
        return false;
    }

    Org* predator = &web[predInd];

    // Check for duplicate relation.
    for (int i =0; i < predator -> numPrey; i++) {
        if (predator -> prey[i] == preyInd) {
            printf("Duplicate predator/prey relation. No relation added to the food web.\n");
            return false;
        }
    }

    // Handle memory allocation.
    int newSize = predator -> numPrey + 1;
    int* newPrey = NULL;

    if (predator -> numPrey == 0) {
        newPrey = (int*)malloc(sizeof(int));
    } else {
        newPrey = (int*)malloc(newSize * sizeof(int));
        if (newPrey != NULL) {
            for (int i = 0; i < predator -> numPrey; i++) {
                newPrey[i] = predator -> prey[i];
            }
            free(predator -> prey);
        }
    }

    if (newPrey == NULL) {
        return false; // memory allocation failed
    }

    newPrey[newSize -1] = preyInd;
    predator -> prey = newPrey;
    predator -> numPrey = newSize;

    return true;
}

bool removeOrgFromWeb(Org** pWeb, int* pNumOrgs, int index) {

    // Check if the index is valid.
    if (index < 0 || index >= *pNumOrgs) {
        printf("Invalid extinction index. No organism removed from the food web.\n");
        return false;
    }

    Org* oldWeb = *pWeb;
    int oldNumOrgs = *pNumOrgs;
    int newNumOrgs = oldNumOrgs - 1;
    Org* newWeb = NULL;

    // (1) Create the new web array without an organism at index
    if (newNumOrgs > 0) {
        newWeb = (Org*)malloc(newNumOrgs * sizeof(Org));
        if (newWeb == NULL) {
            return false; // Memory allocation failed.
        }

        // Copy organisms before the index.
        for (int i = 0; i < index; i++) {
            newWeb[i] = oldWeb[i];
        }

        // Copy organisms after the index.
        for (int i = index; i < newNumOrgs; i++) {
            newWeb[i] = oldWeb[i + 1];
        }
    }

    // Free the prey array of the extinct organism.
    free(oldWeb[index].prey);

    // Free the old web array.
    free(oldWeb);

    // Update the web pointer and number of organisms.
    *pWeb = newWeb;
    *pNumOrgs = newNumOrgs;

    // If there are no organisms left, return immediately.
    if (newNumOrgs == 0) {
        return true;
    }

    // (2) Update all prey arrays.
    for (int i = 0; i < newNumOrgs; i++) {
        Org* org = &((*pWeb)[i]);

        // Skip organisms with no prey
        if (org -> numPrey == 0) {
            continue;
        }

        // Check if the prey array contains the extinct index.
        bool found = false;
        for (int j = 0; j < org -> numPrey; j++) {
            if (org -> prey[j] == index) {
                found = true;
                break;
            }
        }

        if (found) {
            // Count elements not equal to the extinct index.
            int oldPreyCount = org -> numPrey;
            int newPreyCount = 0;
            for (int j = 0; j < oldPreyCount; j++) {
                if (org -> prey[j] != index) {
                    newPreyCount++;
                }
            }

            // Handle the new prey array creation.
            if (newPreyCount == 0) {
                free(org -> prey);
                org -> prey = NULL;
                org -> numPrey = 0;
            } else {
                int* newPrey = (int*)malloc(newPreyCount * sizeof(int));
                if (newPrey == NULL) {
                    return false; // Memory allocation failed.
                }

                int k = 0;
                for (int j = 0; j < oldPreyCount; j++) {
                    if (org -> prey[j] != index) {
                        newPrey[k++] = org -> prey[j];
                    }
                }

                free(org -> prey);
                org -> prey = newPrey;
                org -> numPrey = newPreyCount;
            }
        }

        // Adjust prey indices greater than the extinct index.
        for (int j = 0; j < org -> numPrey; j++) {
            if (org -> prey[j] > index) {
                org -> prey[j]--;
            }
        }
    }

    return true;
}


void freeWeb(Org* web, int numOrgs) {

    // Check if web is NULL (nothing to free).
    if (web == NULL) {
        return;
    }

    // Free each organism's prey array.
    for (int i = 0; i < numOrgs; i++) {
        if (web[i].prey != NULL) {
            free(web[i].prey);
        }
    }

    // Free the web array itself.
    free(web);
}

void printWeb(Org* web, int numOrgs) {
    for (int i = 0; i < numOrgs; i++) {
        printf("  (%d) %s", i, web[i].name);
        if (web[i].numPrey > 0) {
            printf(" eats ");
            for (int j = 0; j < web[i].numPrey; j++) {
                if (j > 0) {
                    printf(", ");
                }
                printf("%s", web[web[i].prey[j]].name);
            }
        }
        printf("\n");
    }
}


void displayAll(Org* web, int numOrgs, bool modified) {

    if (modified) printf("UPDATED ");
    printf("Food Web Predators & Prey:\n");
    printWeb(web,numOrgs);
    printf("\n");

    // Apex Predators: not eaten by any other.
    if (modified) printf("UPDATED ");
    printf("Apex Predators:\n");
    if (numOrgs > 0) {
        bool* isPrey = (bool*)calloc(numOrgs, sizeof(bool));
        for (int i = 0; i < numOrgs; i++) {
            for (int j = 0; j < web[i].numPrey; j++) {
                int preyIndex = web[i].prey[j];
                isPrey[preyIndex] = true;
            }
        }
        for (int i = 0; i < numOrgs; i++) {
            if (!isPrey[i]) {
                printf("  %s\n", web[i].name);
            }
        }
        free(isPrey);
    }
    printf("\n");

    // Producers: eat nothing.
    if (modified) printf("UPDATED ");
    printf("Producers:\n");
    for (int i = 0; i < numOrgs; i++) {
        if (web[i].numPrey == 0) {
            printf("  %s\n", web[i].name);
        }
    }
    printf("\n");

    // Most Flexible Eaters: most prey.
    if (modified) printf("UPDATED ");
    printf("Most Flexible Eaters:\n");
    if (numOrgs > 0) {
        int maxPrey = 0;
        for (int i = 0; i < numOrgs; i++) {
            if (web[i].numPrey > maxPrey) {
                maxPrey = web[i].numPrey;
            }
        }
        if (maxPrey > 0) {
            for (int i = 0 ; i < numOrgs; i++) {
                if (web[i].numPrey == maxPrey) {
                    printf("  %s\n", web[i].name);
                }
            }
        }
    }
    printf("\n");

    // Tastiest Food: most eaten.
    if (modified) printf("UPDATED ");
    printf("Tastiest Food:\n");
    if (numOrgs > 0) {
        int* eatenCount = (int*)calloc(numOrgs, sizeof(int));
        for (int i = 0; i < numOrgs; i++) {
            for (int j = 0;j < web[i].numPrey; j++) {
                int preyIndex = web[i].prey[j];
                eatenCount[preyIndex]++;
            }
        }
        int maxEaten = 0;
        for (int i = 0; i < numOrgs; i++) {
            if (eatenCount[i] > maxEaten) {
                maxEaten = eatenCount[i];
            }
        }
        if (maxEaten > 0) {
            for (int i = 0; i < numOrgs; i++) {
                if (eatenCount[i] == maxEaten) {
                    printf("  %s\n", web[i].name);
                }
            }
        }
        free(eatenCount);
    }
    printf("\n");

    // Food Web Height.
    if (modified) printf("UPDATED ");
    printf("Food Web Heights:\n");
    if (numOrgs > 0) {
        int* heights = (int*)malloc(numOrgs * sizeof(int));
        for (int i = 0; i < numOrgs; i++) {
            heights[i] = 0;
        }
        bool changed;
        do {
            changed = false;
            for (int i = 0; i < numOrgs; i++) {
                if (web[i].numPrey == 0) continue;
                int maxHeight = -1;
                for (int j = 0; j < web[i].numPrey; j++) {
                    int preyIndex = web[i].prey[j];
                    if (heights[preyIndex] > maxHeight) {
                        maxHeight = heights[preyIndex];
                    }
                }
                int newHeight = maxHeight + 1;
                if (newHeight > heights[i]) {
                    heights[i] = newHeight;
                    changed = true;
                }
            }
        } while (changed);
        for (int i = 0; i < numOrgs; i++) {
            printf("  %s: %d\n", web[i].name, heights[i]);
        }
        free(heights);
    }
    printf("\n");

    // Vore Types
    if (modified) printf("UPDATED ");
    printf("Vore Types:\n");
    if (numOrgs > 0) {
        bool* isProducer = (bool*)malloc(numOrgs * sizeof(bool));
        for (int i = 0; i < numOrgs; i++) {
            isProducer[i] = (web[i].numPrey == 0);
        }

        int* categories = (int*)malloc(numOrgs * sizeof(int));

        for (int i = 0; i < numOrgs; i++) {
            if (isProducer[i]) {
                categories[i] = 0;
            } else {
                int preyProducers = 0, preyNonProducers = 0;
                for (int j = 0; j < web[i].numPrey; j++) {
                    int preyIndex = web[i].prey[j];
                    if (isProducer[preyIndex]) {
                        preyProducers++;
                    } else {
                        preyNonProducers++;
                    }
                }
                if (preyProducers > 0 && preyNonProducers == 0) {
                    categories[i] = 1;
                } else if (preyNonProducers > 0 && preyProducers == 0) {
                    categories[i] = 3;
                } else {
                    categories[i] = 2;
                }
            }
        }
        printf("  Producers:\n");
        for (int i = 0; i < numOrgs; i++) {
            if (categories[i] == 0) {
                printf("    %s\n", web[i].name);
            }
        }

        printf("  Herbivores:\n");
        for (int i = 0; i < numOrgs; i++) {
            if (categories[i] == 1) {
                printf("    %s\n", web[i].name);
            }
        }

        printf("  Omnivores:\n");
        for (int i = 0; i < numOrgs; i++) {
            if (categories[i] == 2) {
                printf("    %s\n", web[i].name);
            }
        }

        printf("  Carnivores:\n");
        for (int i = 0; i < numOrgs; i++) {
            if (categories[i] == 3) {
                printf("    %s\n", web[i].name);
            }
        }
        free(isProducer);
        free(categories);
    }
    printf("\n");
}



bool setModes(int argc, char* argv[], bool* pBasicMode, bool* pDebugMode, bool* pQuietMode) {
    // Initialize all modes to false.
    *pBasicMode = false;
    *pDebugMode = false;
    *pQuietMode = false;

    // Track if each node has been set to detect duplicates.
    bool basicSet = false;
    bool debugSet = false;
    bool quietSet = false;

    for (int i = 1; i < argc; i++) {
        char* arg = argv[i];

        // Check if the argument has at least two characters and starts with '-'.
        if (strlen(arg) < 2 || arg[0] != '-') {
            return false;
        }

        // Determine which mode the argument is for based on the second character.
        char modeChar = arg[1];
        if (modeChar == 'b') {
            if (basicSet) {
                return false; // Duplicate basic mode argument.
            }
            *pBasicMode = true;
            basicSet = true;
        } else if (modeChar == 'd') {
            if (debugSet) {
                return false; // Duplicate debug mode argument.
            }
            *pDebugMode = true;
            debugSet = true;
        } else if (modeChar == 'q') {
            if (quietSet) {
                return false; // Duplicate quiet mode argument.
            }
            *pQuietMode = true;
            quietSet = true;
        } else {
            return false; // Invalid mode character
        }
    }

    return true; // All Arguments Processed Successfully.
}


void printONorOFF(bool mode) {
    if (mode) {
        printf("ON\n");
    } else {
        printf("OFF\n");
    }
}


int main(int argc, char* argv[]) {
    // Initialize program modes from command-line arguments.
    bool basicMode = false; // Basic Mode Flag (No Modifications)
    bool debugMode = false; // Debug Mode Flag (Show Intermediate Steps)
    bool quietMode = false; // Quiet Mode Flag (Suppress Prompts)

    // Process Command-line Arguments
    if (!setModes(argc, argv, &basicMode, &debugMode, &quietMode)) {
        printf("Invalid command-line argument. Terminating program...\n");
        return 1;
    }

    // Print Program Settings
    printf("Program Settings:\n");
    printf("  basic mode = ");
    printONorOFF(basicMode);
    printf("  debug mode = ");
    printONorOFF(debugMode);
    printf("  quiet mode = ");
    printONorOFF(quietMode);
    printf("\n");

    // Initialize Food Web Data Structure
    int numOrgs = 0; // Number of Organisms in the Web.

    // Phase 1: Build Initial Food Web
    printf("Welcome to the Food Web Application\n\n");
    printf("--------------------------------\n\n");

    Org* web = NULL; // Dynamic Array of Organisms.

    // Get Organisms Names Until User Enters "Done".
    printf("Building the initial food web...\n");
    if (!quietMode) printf("Enter the name for an organism in the web (or enter DONE): ");
    char tempName[20] = "";
    scanf("%s",tempName);
    if (!quietMode) printf("\n");
    while (strcmp(tempName,"DONE") != 0) {
        addOrgToWeb(&web,&numOrgs,tempName); // Add Organism to Web.
        if (debugMode) {    // Debug Output if Enabled.
            printf("DEBUG MODE - added an organism:\n");
            printWeb(web,numOrgs);
            printf("\n");
        }
        if (!quietMode) printf("Enter the name for an organism in the web (or enter DONE): ");
        scanf("%s",tempName);
        if (!quietMode) printf("\n");
    }
    if (!quietMode) printf("\n");

    // Phase 2: Establish Predator-Prey Relationships.
    if (!quietMode) printf("Enter the pair of indices for a predator/prey relation.\n");
    if (!quietMode) printf("Enter any invalid index when done (-1 2, 0 -9, 3 3, etc.).\n");
    if (!quietMode) printf("The format is <predator index> <prey index>: ");

    int predInd, preyInd;
    scanf("%d %d",&predInd, &preyInd);
    if (!quietMode) printf("\n");

    // Process Relationships Until Invalid Input.
    while (predInd >= 0 && preyInd >= 0 && predInd < numOrgs &&  preyInd < numOrgs && predInd != preyInd) {
        addRelationToWeb(web,numOrgs,predInd,preyInd); // Add Relationship.
        if (debugMode) {
            printf("DEBUG MODE - added a relation:\n");
            printWeb(web,numOrgs);
            printf("\n");
        }
        if (!quietMode) printf("Enter the pair of indices for a predator/prey relation.\n");
        if (!quietMode) printf("Enter any invalid index when done (-1 2, 0 -9, 3 3, etc.).\n");
        if (!quietMode) printf("The format is <predator index> <prey index>: ");

        scanf("%d %d",&predInd, &preyInd);
        if (!quietMode) printf("\n");
    }
    printf("\n");

    printf("--------------------------------\n\n");

    // Phase 3: Initial Analysis.
    printf("Initial food web complete.\n");
    printf("Displaying characteristics for the initial food web...\n");

    displayAll(web,numOrgs,false); // Show Initial Analysis.

    // Phase 4: Interactive Modification (If not in Basic Mode).
    if (!basicMode) {
        printf("--------------------------------\n\n");
        printf("Modifying the food web...\n\n");
        char opt = '?';

        while (opt != 'q') { // Loop Until User Quits.
            if (!quietMode) { // Present Menu Options
                printf("Web modification options:\n");
                printf("   o = add a new organism (expansion)\n");
                printf("   r = add a new predator/prey relation (supplementation)\n");
                printf("   x = remove an organism (extinction)\n");
                printf("   p = print the updated food web\n");
                printf("   d = display ALL characteristics for the updated food web\n");
                printf("   q = quit\n");
                printf("Enter a character (o, r, x, p, d, or q): ");
            }
            scanf(" %c", &opt);
            if (!quietMode) printf("\n\n");

            // Handle User Selection.
            if (opt == 'o') { // Add Organism.
                char newName[20];
                if (!quietMode) printf("EXPANSION - enter the name for the new organism: ");
                scanf("%s",newName);
                if (!quietMode) printf("\n");
                printf("Species Expansion: %s\n", newName);
                addOrgToWeb(&web,&numOrgs,newName);
                printf("\n");

                if (debugMode) {
                    printf("DEBUG MODE - added an organism:\n");
                    printWeb(web,numOrgs);
                    printf("\n");
                }

            } else if (opt == 'x') { // Remove Organism
                int extInd;
                if (!quietMode) printf("EXTINCTION - enter the index for the extinct organism: ");
                scanf("%d",&extInd);
                if (!quietMode) printf("\n");
                if (extInd >= 0 && extInd < numOrgs) {
                    printf("Species Extinction: %s\n", web[extInd].name);
                    removeOrgFromWeb(&web,&numOrgs,extInd);
                } else {
                    printf("Invalid index for species extinction\n");
                }
                printf("\n");

                if (debugMode) {
                    printf("DEBUG MODE - removed an organism:\n");
                    printWeb(web,numOrgs);
                    printf("\n");
                }

            } else if (opt == 'r') { // Add Relation
                if (!quietMode) printf("SUPPLEMENTATION - enter the pair of indices for the new predator/prey relation.\n");
                if (!quietMode) printf("The format is <predator index> <prey index>: ");
                scanf("%d %d",&predInd, &preyInd);
                if (!quietMode) printf("\n");

                if (addRelationToWeb(web,numOrgs,predInd,preyInd)) {
                    printf("New Food Source: %s eats %s\n", web[predInd].name, web[preyInd].name);
                }
                printf("\n");
                if (debugMode) {
                    printf("DEBUG MODE - added a relation:\n");
                    printWeb(web,numOrgs);
                    printf("\n");
                }

            } else if (opt == 'p') { // Print Current Web
                printf("UPDATED Food Web Predators & Prey:\n");
                printWeb(web,numOrgs);
                printf("\n");

            } else if (opt == 'd') { // Full Analysis
                printf("Displaying characteristics for the UPDATED food web...\n\n");
                displayAll(web,numOrgs,true);

            }
            printf("--------------------------------\n\n");

        }

    }

    // Cleanup
    freeWeb(web,numOrgs); // Free All Allocated Memory.

    //printf("\n\n");

    return 0;
}

