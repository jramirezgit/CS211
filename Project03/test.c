#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "MinPopVote.h"

bool test_totalEVs() {
    State aStates[10];
    int res;

    aStates[0].electoralVotes = 5;
    aStates[1].electoralVotes = 8;
    aStates[2].electoralVotes = 12;
    aStates[3].electoralVotes = 6;
    aStates[4].electoralVotes = 7;
    aStates[5].electoralVotes = 10;

    printf(" Checking totalEVs() for 5 States:\n");
    res = totalEVs(aStates,5);
    if (res != 38) {
        printf("  individual state EVs are 5, 8, 12, 6, 7\n");
        printf("  expected total EVs = 38, actual total EVs = %d\n",res);
        return false;
    }

    printf(" Checking totalEVs() for 6 States:\n");
    res = totalEVs(aStates,6);
    if (res != 48) {
        printf("  individual state EVs are 5, 8, 12, 6, 7, 10\n");
        printf("  expected total EVs = 48, actual total EVs = %d\n",res);
        return false;
    }

    printf(" Checking totalEVs() for 0 states:\n");
    res = totalEVs(aStates, 0);
    if (res != 0) {
        printf(" individual state EVs are 5\n");
        printf(" expected total EVs = 5, actual total EVs = %d\n", res);
        return false;
    }

    return true;
}

bool test_totalPVs() {
    State aStates[5];
    int res;

    aStates[0].popularVotes = 100;
    aStates[1].popularVotes = 200;
    aStates[2].popularVotes = 300;
    aStates[3].popularVotes = 400;
    aStates[4].popularVotes = 500;

    printf(" Checking totalPVs() for 3 states:\n");
    res = totalPVs(aStates, 3);
    if (res != 600) {
        printf(" Expected 600, actual %d\n", res);
        return false;
    }

    printf(" Checking totalPVs() for 5 states:\n");
    res = totalPVs(aStates, 5);
    if (res != 1500) {
        printf(" Expected 1500, actual %d\n", res);
        return false;
    }

    printf(" Checking totalPVs() for 0 States:\n");
    res = totalPVs(aStates, 0);
    if (res != 0) {
        printf(" Expected 0, actual %d\n", res);
        return false;
    }

    return true;
}

bool test_setSettings() {
    int year;
    bool fast, quiet;
    bool result;

    { // Test Case 1: All Valid Flags
        char* argv[] = {"app", "-f", "-q", "-y", "2020"};
        int argc = 5;
        year = -1;
        fast = false;
        quiet = false;
        result = setSettings(argc, argv, &year, &fast, &quiet);
        if (!result || year != 2020 || !fast || !quiet) {
            printf("Test Case 1 Failed: Valid flags (-f -q -y 2020)\n");
            return false;
        }
    }

    { // Test Case 2: Missing Argument After -y
        char* argv[] = {"app", "-y"};
        int argc = 2;
        year = -1;
        fast = false;
        quiet = false;
        result = setSettings(argc, argv, &year, &fast, &quiet);
        if (result) {
            printf("Test Case 2 Failed: Missing Year Argument\n");
            return false;
        }
    }

    { // Test Case 3: Invalid Year
        char* argv[] = {"app", "-y", "1995"};
        int argc = 3;
        year = -1;
        fast = false;
        quiet = false;
        result = setSettings(argc, argv, &year, &fast, &quiet);
        if (result) {
            printf("Test Case 3 Failed: Invalid Year 1955\n");
            return false;
        }
    }

    { // Test Case 4: Valid Year 9999
        char* argv[] = {"app", "-y", "9999"};
        int argc = 3;
        year = -1;
        fast = false;
        quiet = false;
        result  = setSettings(argc, argv, &year, &fast, &quiet);
        if (!result || year != 9999) {
            printf("Test Case 4 Failed: Valid Year 9999\n");
            return false;
        }
    }

    { // Test Case 5: Invalid Flag (-v)
        char* argv[] ={"app", "-v"};
        int argc = 2;
        year = -1;
        fast = false;
        quiet = false;
        result = setSettings(argc, argv, &year, &fast, &quiet);
        if (result) {
            printf("Test Case 5 Failed: Invalid Flag -v\n");
            return false;
        }
    }

    { // Test Case 6: Multiple -y Flags (Valid)
        char* argv[] = {"app", "-y", "2016", "-y", "2020"};
        int argc = 5;
        year = -1;
        fast = false;
        quiet = false;
        result = setSettings(argc , argv, &year, &fast, &quiet);
        if (!result || year != 2020) {
            printf("Test Case 6 Failed: Multiple -y Flags\n");
            return false;
        }
    }

    { // Test Case 7: Non-integer Year
        char* argv[] = {"app", "-y", "abc"};
        int argc = 3;
        year = -1;
        fast = false;
        quiet = false;
        result = setSettings(argc, argv, &year, &fast, &quiet);
        if (result) {
            printf("Test Case 7 Failed: Non-integer Year\n");
            return false;
        }
    }

    { // Test Case 8: Flags in Different Order
        char* argv[] = {"app", "-q", "-y", "2000", "-f"};
        int argc = 5;
        year = -1;
        fast = false;
        quiet = false;
        result = setSettings(argc, argv, &year, &fast, &quiet);
        if (!result || year != 2000 || !fast || !quiet) {
            printf("Test Case 8 Failed: Flags in Different Order\n");
            return false;
        }
    }

    { // Test Case 9: Valid Year 1828
        char* argv[] = {"app", "-y", "1828"};
        int argc = 3;
        year = -1;
        fast = false;
        quiet = false;
        result = setSettings(argc, argv, &year, &fast, &quiet);
        if (!result || year != 1828) {
            printf("Test Case 9 Failed: Valid Year 1828\n");
            return false;
        }
    }

    { // Test Case 10: Valid Year 2024
        char* argv[] = {"app", "-y", "2024"};
        int argc = 3;
        year = -1;
        fast = false;
        quiet = false;
        result = setSettings(argc, argv, &year, &fast, &quiet);
        if (!result || year != 2024) {
            printf("Test Case 10 Failed: Valid Year 2024\n");
            return false;
        }
    }

    { // Test Case 11: Invalid Year 1827
        char* argv[] = {"app", "-y", "1827"};
        int argc = 3;
        year = -1;
        fast = false;
        quiet = false;
        result = setSettings(argc, argv, &year, &fast, &quiet);
        if (result) {
            printf("Test Case 11 Failed: Invalid Year 1827\n");
            return false;
        }
    }

    { // Test Case 12: Invalid Year 2025
        char* argv[] = {"app", "-y", "2025"};
        int argc = 3;
        year = -1;
        fast = false;
        quiet = false;
        result = setSettings(argc, argv, &year, &fast, &quiet);
        if (result) {
            printf("Test Case 12 Failed: Invalid Year 2025\n");
            return false;
        }
    }

    { // Test Case 13: Invalid Year 1800
        char* argv[] = {"app", "-y", "1800"};
        int argc = 3;
        year = -1;
        fast = false;
        quiet = false;
        result = setSettings(argc, argv, &year, &fast, &quiet);
        if (result) {
            printf("Test Case 13 Failed: Invalid Year 1800\n");
            return false;
        }
    }

    return true;
}

bool test_inFilename() {
    char filename[100];
    bool allPassed = true;

    // Test Case 1: Regular Year
    inFilename(filename, 2020);
    if (strcmp(filename, "data/2020.csv") != 0) {
        printf(" Test Failed for 2020. Expected 'data/2020.csv', got '%s'\n", filename);
        allPassed = false;
    }

    // Test Case 2: Year 9999
    inFilename(filename, 9999);
    if (strcmp(filename, "data/9999.csv") != 0) {
        printf(" Test Failed for 9999. Expected 'data/9999.csv', got '%s'\n", filename);
        allPassed = false;
    }

    // Test Case 3: Minimum Valid Year
    inFilename(filename, 1828);
    if (strcmp(filename, "data/1828.csv") != 0) {
        printf(" Test Failed for 1828. Expected 'data/1828.csv', got '%s'\n", filename);
        allPassed = false;
    }

    // Test Case 4: Maximum Valid Year
    inFilename(filename, 2024);
    if (strcmp(filename, "data/2024.csv") != 0) {
        printf(" Test Failed for 2024. Expected 'data/2024.csv', got '%s'\n", filename);
        allPassed = false;
    }

    return allPassed;
}

bool test_outFilename() {
    char filename[100];
    bool allPassed = true;

    // Test Case 1: Regular Year
    outFilename(filename, 2020);
    if (strcmp(filename, "toWin/2020_win.csv") != 0) {
        printf(" Test Failed for 2020. Expected 'toWin/2020_win.csv', got '%s'\n", filename);
        allPassed = false;
    }

    // Test Case 2: Year 9999
    outFilename(filename, 9999);
    if (strcmp(filename, "toWin/9999_win.csv") != 0) {
        printf(" Test Failed for 9999. Expected 'toWin/9999_win.csv', got '%s'\n", filename);
        allPassed = false;
    }

    // Test Case 3: Minimum Valid Year
    outFilename(filename, 1828);
    if (strcmp(filename, "toWin/1828_win.csv") != 0) {
        printf(" Test Failed for 1828. Expected 'toWin/1828_win.csv', got '%s'\n", filename);
        allPassed = false;
    }

    // Test Case 4: Maximum Valid Year
    outFilename(filename, 2024);
    if (strcmp(filename, "toWin/2024_win.csv") != 0) {
        printf(" Test Failed for 2024. Expected 'toWin/2024_win.csv', got '%s'\n", filename);
        allPassed = false;
    }

    return allPassed;
}

bool test_parseLine() {
    State myState;
    bool result;
    char line[100];

    // Test Case 1: Valid Line
    strcpy(line, "Illinois,IL,20,6033744");
    result = parseLine(line, &myState);
    if (!result || strcmp(myState.name, "Illinois") != 0 ||
        strcmp(myState.postalCode, "IL") != 0 ||
        myState.electoralVotes != 20 ||
        myState.popularVotes != 6033744) {
        printf("Test Case 1 Failed: Valid Line\n");
        return false;
    }

    // Test Case 2: Line with newline
    strcpy(line, "New York,NY,29,7500000\n");
    result = parseLine(line, &myState);
    if (!result || strcmp(myState.name, "New York") != 0 ||
        strcmp(myState.postalCode, "NY") != 0 ||
        myState.electoralVotes != 29 ||
        myState.popularVotes != 7500000) {
        printf("Test Case 2 Failed: Line with newline\n");
        return false;
    }

    // Test Case 3: Line with two commas (invalid)
    strcpy(line, "InvalidLine,two,parts");
    result = parseLine(line, &myState);
    if(result) {
        printf("Test Case 3 Failed: Line with two commas\n");
        return false;
    }

    // Test Case 4: Non-integer electoral votes
    strcpy(line, "BadElectoral,ST,abc,1000");
    result = parseLine(line, &myState);
    if (!result || myState.electoralVotes != 0) {
        printf("Test Case 4 Failed: Non-integer electoral votes\n");
        return false;
    }

    // Test Case 5: Four commas (invalid)
    strcpy(line, ",,,,");
    result = parseLine(line, &myState);
    if (result) {
        printf("Test Case 5 Failed: Four commas\n");
        return false;
    }
    return true;
}

bool test_readElectionData() {
    State states[51];
    int nStates = 0;
    bool result;

    // Test Case 1: Valid file with correct data.
    FILE* testFile = fopen("test_valid.csv", "w");
    if (!testFile) return false;
    fprintf(testFile, "State1,ST,5,1000\n");
    fprintf(testFile, "State2,AB,10,2000\n");
    fprintf(testFile, "State3,CD,15,3000\n");
    fclose(testFile);

    result = readElectionData("test_valid.csv", states, &nStates);
    if (!result || nStates != 3) {
        printf("Test Case 1 Failed: Valid file not read correctly\n");
        remove("test_valid.csv");
        return false;
    }
    if (strcmp(states[0].name, "State1") != 0 || strcmp(states[0].postalCode, "ST") != 0 ||
        states[0].electoralVotes != 5 || states[0].popularVotes != 1000) {
        printf("Test Case 1 Failed: Incorrect data for State1\n");
        remove("test_valid.csv");
        return false;
    }
    if (strcmp(states[1].name, "State2") != 0 || strcmp(states[1].postalCode, "AB") != 0 ||
        states[1].electoralVotes != 10 || states[1].popularVotes != 2000) {
        printf("Test Case 2 Failed: Incorrect data for State2\n");
        remove("test_valid.csv");
        return false;
    }
    if (strcmp(states[2].name, "State3") != 0 || strcmp(states[2].postalCode, "CD") != 0 ||
        states[2].electoralVotes != 15 || states[2].popularVotes != 3000) {
        printf("Test Case 3 Failed: Incorrect data for State3\n");
        remove("test_valid.csv");
        return false;
    }
    remove("test_valid.csv");

    // Test Case 2: File with invalid lines.
    testFile = fopen("test_invalid.csv", "w");
    if (!testFile) return false;
    fprintf(testFile, "BadLine,OnlyTwo\n");
    fprintf(testFile, "GoodLine,AB,10,2000\n");
    fclose(testFile);

    nStates = 0;
    result = readElectionData("test_invalid.csv", states, &nStates);
    if (!result || nStates != 1) {
        printf("Test Case 2 Failed: Expected 1 valid state, got %d\n", nStates);
        remove("test_invalid.csv");
        return false;
    }
    if (strcmp(states[0].name, "GoodLine") != 0 || strcmp(states[0].postalCode, "AB") != 0 ||
        states [0].electoralVotes != 10 || states[0].popularVotes != 2000) {
        printf("Test Case 2 Failed: Valid states data incorrect\n");
        remove("test_invalid.csv");
        return false;
    }
    remove("test_invalid.csv");

    // Test Case 3: Empty file.
    testFile = fopen("test_empty.csv", "w");
    if (!testFile) return false;
    fclose(testFile);
    nStates = 0;
    result = readElectionData("test_empty.csv", states, &nStates);
    if (!result || nStates != 0) {
        printf("Test Case 3 Failed: Empty file should have nStates = 0\n");
        remove("test_empty.csv");
        return false;
    }
    remove("test_empty.csv");

    // Test Case 4: Non-existent file.
    result = readElectionData("nonexistent.csv", states, &nStates);
    if (result) {
        printf("Test Case 4 Failed: Non-existent file should return false\n");
        return false;
    }

    // Test Case 5: File exceeding 51 states.
    testFile = fopen("test_full.csv", "w");
    if (!testFile) return false;
    for (int i = 0; i < 52; i++) {
        fprintf(testFile, "States%d,ST%d,%d,%d\n", i, i, i*5, i*1000);
    }
    fclose(testFile);

    nStates = 0;
    result = readElectionData("test_full.csv", states, &nStates);
    if (!result || nStates != 51) {
        printf("Test Case 5 Failed: Expected 51 states, got %d\n", nStates);
        remove("test_full.csv");
        return false;
    }
    remove("test_full.csv");

    return true;
}


bool test_minPVsSlow() {
    int size = 5;
    State states[4];

    states[0].electoralVotes = 5;
    states[0].popularVotes = 60;
    strcpy(states[0].name, "A");

    states[1].electoralVotes = 2;
    states[1].popularVotes = 20;
    strcpy(states[1].name, "B");

    states[2].electoralVotes = 8;
    states[2].popularVotes = 70;
    strcpy(states[2].name, "C");

    states[3].electoralVotes = 3;
    states[3].popularVotes = 30;
    strcpy(states[3].name, "D");

    MinInfo res = minPopVoteToWin(states, 4);
    bool expected = (res.subsetPVs == 47);
    return expected;
}

bool test_minPVsFast() {
    int size = 5;
    State states[4];

    states[0].electoralVotes = 5;
    states[0].popularVotes = 60;
    strcpy(states[0].name, "A");

    states[1].electoralVotes = 2;
    states[1].popularVotes = 20;
    strcpy(states[1].name, "B");

    states[2].electoralVotes = 8;
    states[2].popularVotes = 70;
    strcpy(states[2].name, "C");

    states[3].electoralVotes = 3;
    states[3].popularVotes = 30;
    strcpy(states[3].name, "D");

    MinInfo res = minPopVoteToWinFast(states, 4);
    bool expected = (res.subsetPVs == 47);
    return expected;
}

int main() {
    printf("Welcome to the Popular Vote Minimizer Testing Suite!\n\n");

    printf("Testing totalEVs()...\n");
    if (test_totalEVs()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing totalPVs()...\n");
    if (test_totalPVs()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing setSettings()...\n");
    if (test_setSettings()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing inFilename()...\n");
    if (test_inFilename()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing outFilename()...\n");
    if (test_outFilename()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing parseLine()...\n");
    if (test_parseLine()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing readElectionData()...\n");
    if (test_readElectionData()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing minPopVoteToWin()...\n");
    if (test_minPVsSlow()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing minPopVoteToWinFast()...\n");
    if (test_minPVsFast()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    return 0;
}
