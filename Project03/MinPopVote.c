// Program: MinPopVote.c
// Author: Jose Ramirez
// Last Updated: 03/19/2025
// Summary: Election Vote Calculator - Calculates minimum popular votes needed to win US presidency.

// Important headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include "MinPopVote.h"

// Calculate total electoral votes from all states.
int totalEVs(State* states, int szStates) {
    int sum = 0;
    for (int i = 0; i < szStates; i++) {
        sum += states[i].electoralVotes;
    }
    return sum;
}

// Calculate total popular votes from all states.
int totalPVs(State* states, int szStates) {
    int sum = 0;
    for (int i = 0; i <szStates; i++) {
        sum += states[i].popularVotes;
    }
    return sum;
}

// Parse command line arguments and set program settings.
bool setSettings(int argc, char** argv, int* year, bool* fastMode, bool* quietMode) {
    *year = -1;
    *quietMode = false;
    *fastMode = false;

    // Iterate through command line arguments.
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
        } else if (strcmp(arg, "-f") == 0) { // Fast Mode
            *fastMode = true;
            i++;
        } else if (strcmp(arg, "-q") == 0) { // Quiet Mode
            *quietMode = true;
            i++;
        } else {
            return false; // Invalid argument
        }
    }
    return true;
}

// Generate input filename based on year.
void inFilename(char* filename, int year) {
    sprintf(filename, "data/%d.csv", year);
}

// Generate output filename based on year.
void outFilename(char* filename, int year) {
    sprintf(filename, "toWin/%d_win.csv", year);
}

// Parse a CSV line into State structure
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

// Read election data from CSV file
bool readElectionData(char* filename, State* allStates, int* nStates) {
    *nStates = 0;

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return false;
    }

    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        State state;
        if (parseLine(line, &state)) {
            if (*nStates < 51) {
                allStates[*nStates] = state;
                (*nStates)++;
            }
        }
    }

    fclose(file);
    return true;
}

// Recursive brute-force solution to find minimum popular votes.
MinInfo minPopVoteAtLeast(State* states, int szStates, int start, int EVs) {

    // Base case: End of array or EVs requirement met.
    if(start == szStates || EVs <= 0) {
        MinInfo res;
        res.subsetPVs = 0;
        if (EVs <= 0) {
            res.sufficientEVs = true;
        } else {
            res.sufficientEVs = false;
        }
        res.szSomeStates = 0;
        return res;
    }

    // Recursive case: Try excluding and including current state.
    MinInfo exclude = minPopVoteAtLeast(states, szStates, start + 1, EVs);
    MinInfo include = minPopVoteAtLeast(states, szStates, start + 1, EVs - states[start].electoralVotes);

    // Add current state's required votes (majority).
    include.subsetPVs += (states[start].popularVotes/2) + 1;
    include.someStates[include.szSomeStates] = states[start];
    include.szSomeStates++;

    // Choose better option between include/exclude.
    if (include.sufficientEVs && (!exclude.sufficientEVs || include.subsetPVs <= exclude.subsetPVs)) {
        return include;
    } else {
        return exclude;
    }

}

// Wrapper function to calculate minimum votes needed to win.
MinInfo minPopVoteToWin(State* states, int szStates) {
    int totEVs = totalEVs(states,szStates);
    int reqEVs = totEVs/2 + 1; // required EVs to win election.
    return minPopVoteAtLeast(states, szStates, 0, reqEVs);
}

// Memoized version of minimum votes calculator.
MinInfo minPopVoteAtLeastFast(State* states, int szStates, int start, int EVs, MinInfo** memo) {
    // Base case: Same as brute-force version.
    if (start == szStates | EVs <= 0) {
        MinInfo res;
        res.subsetPVs = 0;
        if (EVs <= 0) {
            res.sufficientEVs = true;
        } else {
            res.sufficientEVs = false;
        }
        res.szSomeStates = 0;
        return res;
    }

    // Returned memoized result if available.
    if (memo[start][EVs].subsetPVs != -1) {
        return memo[start][EVs];
    }

    // Recursive cases same as brute-force.
    MinInfo exclude = minPopVoteAtLeastFast(states, szStates, start + 1, EVs, memo);
    MinInfo include = minPopVoteAtLeastFast(states, szStates, start + 1, EVs - states[start].electoralVotes, memo);
    include.subsetPVs += (states[start].popularVotes/2) + 1;
    include.someStates[include.szSomeStates] = states[start];
    include.szSomeStates++;

    // Store result in memo table before returning.
    if (include.sufficientEVs && (!exclude.sufficientEVs || include.subsetPVs < exclude.subsetPVs)) {
        memo[start][EVs] = include;
    } else {
        memo[start][EVs] = exclude;
    }

    return memo[start][EVs];
}

// Wrapper for memoized solution.
MinInfo minPopVoteToWinFast(State* states, int szStates) {

    int totEVs = totalEVs(states, szStates);
    int reqEVs = totEVs/2 + 1;

    // Initialize memoization table.
    MinInfo** memo = (MinInfo**)malloc((szStates + 1)*sizeof(MinInfo*));
    for (int i = 0; i < szStates + 1; ++i) {
        memo[i] = (MinInfo*)malloc((reqEVs + 1)*sizeof(MinInfo));
        for (int j = 0; j < reqEVs + 1; ++j) {
            memo[i][j].subsetPVs = -1;
        }
    }

    MinInfo res = minPopVoteAtLeastFast(states, szStates, 0, reqEVs, memo);

    // Clean up memoization memory.
    for (int i = 0; i < szStates + 1; i++) {
        free(memo[i]);
    }

    free(memo);
    return res;
}


// Write election results to output file.
bool writeSubsetData(char* filenameW, int totEVs, int totPVs, int wonEVs, MinInfo toWin) {
    FILE* file = fopen(filenameW, "w");
    if(file == NULL) {
        return false;
    }

    // Write summary line
    fprintf(file, "%d,%d,%d,%d\n", totEVs, totPVs, wonEVs, toWin.subsetPVs);

    // Write states in reserve order (recursion unwinds backwards)
    for (int i = toWin.szSomeStates - 1; i >= 0; i--) {
        fprintf(file, "%s,%s,%d,%d\n", toWin.someStates[i].name, toWin.someStates[i].postalCode, toWin.someStates[i].electoralVotes, (toWin.someStates[i].popularVotes / 2) + 1);
    }

    fclose(file);
    return true;
}
