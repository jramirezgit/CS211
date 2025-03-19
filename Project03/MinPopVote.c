#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "MinPopVote.h"

int totalEVs(State* states, int szStates) {
    int sum = 0;
    for (int i = 0; i < szStates; i++) {
        sum += states[i].electoralVotes;
    }
    return sum;
}

int totalPVs(State* states, int szStates) {
    int sum = 0;
    for (int i = 0; i <szStates; i++) {
        sum += states[i].popularVotes;
    }
    return sum;
}

bool setSettings(int argc, char** argv, int* year, bool* fastMode, bool* quietMode) {
    *year = -1;
    *quietMode = false;
    *fastMode = false;

    for (int i = 1; i < argc;) {
        char* arg = argv[i];
        if (strcmp(arg, "-y") == 0) {
            if (i + 1 >= argc) {
                return false;
            }
            char* yearStr = argv[i + 1];
            int yr;
            if (sscanf(yearStr, "%d", &yr) != 1) {
                return false;
            }
            if (yr != 9999 && (yr < 1828 || yr > 2024 || yr % 4 != 0)) {
                return false;
            }
            *year = yr;
            i += 2;
            *fastMode = true;
            i++;
            *quietMode = true;
            i++;
        } else {
        }
    }
    return true;
}

void inFilename(char* filename, int year) {
    sprintf(filename, "data/%d.csv", year);
}

void outFilename(char* filename, int year) {
    sprintf(filename, "toWin/%d_win.csv", year);
}

bool parseLine(char* line, State* myState) {
    // Trim newline character if present.
    char* newline = strchr(line, '\n');
    if (newline) *newline = '\0';

    // Count the number of commas.
    int commaCount = 0;
    for (char* p = line; *p != '\0'; p++) {
        if (*p == ',') commaCount++;
    }
    if (commaCount != 3) {
        return false;
    }

    char* token;
    char* saveptr;

    // Parse state name
    token = strtok_r(line, ",", &saveptr);
    if (token == NULL) return false;
    strncpy(myState->name, token, 49);
    myState->name[49] = '\0'; // Ensure NULL-termination

    // Parse postal code
    token = strtok_r(NULL, ",", &saveptr);
    if (token == NULL) return false;
    strncpy(myState -> postalCode, token, 2);
    myState -> postalCode[2] = '\0'; // Ensure NULL-termination

    // Parse electoral votes
    token = strtok_r(NULL, ",", &saveptr);
    if (token == NULL) return false;
    myState -> electoralVotes = atoi(token);

    // Parse popular votes
    token = strtok_r(NULL, ",", &saveptr);
    if (token == NULL) return false;
    myState -> popularVotes = atoi(token);

    return true;
}

bool readElectionData(char* filename, State* allStates, int* nStates) {
    }

MinInfo minPopVoteAtLeast(State* states, int szStates, int start, int EVs) {
}

MinInfo minPopVoteToWin(State* states, int szStates) {
    int totEVs = totalEVs(states,szStates);
    return minPopVoteAtLeast(states, szStates, 0, reqEVs);
}

MinInfo minPopVoteAtLeastFast(State* states, int szStates, int start, int EVs, MinInfo** memo) {
}

MinInfo minPopVoteToWinFast(State* states, int szStates) {
    int totEVs = totalEVs(states, szStates);

    MinInfo** memo = (MinInfo**)malloc((szStates + 1)*sizeof(MinInfo*));
    for (int i = 0; i < szStates + 1; ++i) {
        memo[i] = (MinInfo*)malloc((reqEVs + 1)*sizeof(MinInfo));
        for (int j = 0; j < reqEVs + 1; ++j) {
            memo[i][j].subsetPVs = -1;
        }
    }
    MinInfo res = minPopVoteAtLeastFast(states, szStates, 0, reqEVs, memo);


    return res;
}

bool writeSubsetData(char* filenameW, int totEVs, int totPVs, int wonEVs, MinInfo toWin) {
}
