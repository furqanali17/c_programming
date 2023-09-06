#include "asteroids.h"
#include <stdlib.h>
//depth of the dynamic table
#define DEPTH 9

//a dynamic table which has a 3d array which stores the scores for each position
//upto a certain depth
int dynamicTable[FIELD_HEIGHT][FIELD_WIDTH][DEPTH + 1];

//method to check if the given (row, col) position is safe to move into
int checkSafety(int row, int col, int field[][FIELD_WIDTH]) {
    //if to check are the row and col are inside bounds
    if (row < 0 || row >= FIELD_HEIGHT || col < 0 || col >= FIELD_WIDTH) {
        //return 0 if the position is out of the field bounds
        return 0;
    }
    //if to check does the row and col have an asteroid
    if (field[row][col] == ASTEROID_VAL) {
        //return 0 if there is an asteroid at the position
        return 0;
    }

    //return 1 if the position is safe
    return 1;
}

//method to calculate the score for the given (row, col) position up to the specified depth
int calculateScore(int row, int col, int depth, int field[][FIELD_WIDTH]) {
    //if to check the safety of the row and col
    if (!checkSafety(row, col, field)) {
        //return 0 if the position is not safe
        return 0;
    }

    //if we have reached the maximum depth
    if (depth == 0) {
        //return 1 as the base score
        return 1;
    }

    //if the score for this position and depth has been calculated before, return it
    if (dynamicTable[row][col][depth] != -1) {
        return dynamicTable[row][col][depth];
    }

    //calculate the score for the current position by checking the scores for the next positions
                //calculates the score for moving straight
    int score = 1 + calculateScore(row, col + 1, depth - 1, field) +
                //calculates the score for moving up
                calculateScore(row - 1, col + 1, depth - 1, field) +
                //calculates the score for moving down
                calculateScore(row + 1, col + 1, depth - 1, field);
    //save the calculated score in the dynamic table
    dynamicTable[row][col][depth] = score;

    return score;
}

//reset the dynamic programming table so scores are only recalculated when needed
void resetDynamicTable() {
    for (int i = 0; i < FIELD_HEIGHT; i++) {
        for (int j = 0; j < FIELD_WIDTH; j++) {
            for (int d = 0; d <= DEPTH; d++) {
                dynamicTable[i][j][d] = -1;
            }
        }
    }
}

struct ship_action move_ship(int field[][FIELD_WIDTH], void *ship_state) {
    //initialize variable to store current position of the ship
    int shipRow;
    //initialize variable to store direction in which ship should move
    int moveDirection = MOVE_NO;
    //initialize variable to store maximum score found
    int maxScore = -1;
    //initialize struct to store the chosen move
    struct ship_action action;

    //if to initialize ship state and assign it memory
    if (ship_state == NULL) {
        ship_state = malloc(sizeof(int));
        *((int *) ship_state) = FIELD_HEIGHT / 2;
    }

    //get the current row position of the ship
    shipRow = *((int *) ship_state);

    //call the method in main
    resetDynamicTable();

    //for loop to go through all the possible directions (-1, 0, 1)
    for (int i = -1; i <= 1; i++) {
        //find row position of the ship
        int row = shipRow + i;
        //check if the new row is safe, if not, skip to the next iteration
        if (!checkSafety(row, 1, field)) {
            continue;
        }

        //calculate the score for the new row
        int score = calculateScore(row, 1, DEPTH, field);

        //update the direction if the new score is better
        //if equal choose randomly
        if (score > maxScore || (score == maxScore && rand() % 2 == 0)) {
            maxScore = score;
            moveDirection = i;
        }
    }

    //if the current row is not safe, try moving up or down to find a safe position
    if (!checkSafety(shipRow, 1, field)) {
        if (checkSafety(shipRow - 1, 1, field)) {
            moveDirection = MOVE_UP;
        } else if (checkSafety(shipRow + 1, 1, field)) {
            moveDirection = MOVE_DOWN;
        }
    }

    //update the ship's row position based on the selected move direction
    if (moveDirection < 0 && checkSafety(shipRow - 1, 1, field)) {
        shipRow--;
    } else if (moveDirection > 0 && checkSafety(shipRow + 1, 1, field)) {
        shipRow++;
    }

    //update the ship state with the new row position
    *((int *) ship_state) = shipRow;

    //prepare the action with the selected move direction and updated ship state
    action.move = moveDirection;
    action.state = ship_state;

    return action;
}