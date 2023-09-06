#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "utils.c"

#define DICT_SIZE 5757
#define WORD_SIZE 5
#define MAX_GUESSES 6
#define ALPHABET_SIZE 26
#define MAX_GAMES 100

//method to play the game multiple times
int playAgain() {
    //taking input form user
    char input;
    printf("\n");
    printf("Do you want to play again? (enter y for yes, anything else for no): ");
    scanf(" %c", &input);
    getchar(); //consumes the newline character
    if (input == 'y' || input == 'Y') {
        return 1;
    } else {
        return 0;
    }
}

//method used to calculate the percentages of wins
int calculate_percent_wins(int games_won, char word_list[DICT_SIZE][WORD_SIZE + 1], int total_games) {
    //for loop goes through all games played
    for (int i = 0; i < total_games; i++) {
        if (word_list[i][0] == '\0') {
            continue;  //skips the game where player failed to guess the word
        } else if (word_list[i][0] == '+' && word_list[i][1] == '\0') {
            games_won++;  //only counts games where played guessed the word
        }
    }
    return games_won * 100 / total_games;
}

//method used to calculate the average percentage of alphabets used
float calculate_average_percentage(int letters_used, int alphabet_size, int total_games) {
    float total_percentage = 0;

    total_percentage += (float) letters_used / (float) alphabet_size;

    return total_percentage / (float) total_games;
}


//method used to print histogram of guesses it took to win games
void printHistogram(int guesses) {
    //2d int array that checks number of guesses used in each game
    static int guess_counts[MAX_GAMES][MAX_GUESSES] = {0}; //initialized to zero
    static int games_played = 0;
    int i, j, count;

    //updates the guess count for current game
    guess_counts[games_played][guesses]++;

    printf("Histogram of Guesses:\n");

    //loop to go through all guesses made
    for (i = 1; i <= MAX_GUESSES; i++) {
        count = 0;
        //loops through and adds no.of guesses made into j
        for (j = 0; j <= games_played; j++) {
            count += guess_counts[j][i - 1];
        }
        //prints the number of guesses made
        printf("%2d: ", i);
        //loop to print aes-trick on the value of j
        for (j = 0; j < count; j++) {
            printf("*");
        }
        printf("\n");
    }
    //increments static variable so method is not overwritten when playing multiple games
    games_played++;
}

//method to check if word exists in wordlist
int is_word_in_list(const char *word, char word_list[DICT_SIZE][WORD_SIZE + 1]) {
    int i, j;

    //loop through each word in wordlist
    for (i = 0; i < DICT_SIZE; i++) {
        int is_match = 1;
        //loop through each character of that word
        for (j = 0; j < WORD_SIZE; j++) {
            //checks the characters of word with characters of word in wordlist
            if (word_list[i][j] != word[j]) {
                is_match = 0;
                break;
            }
        }
        if (is_match) {
            return 1;
        }
    }
    return 0;
}

//method to print word with dashes
void print_word(const char *word, const char *guessed_letters, const char *guessed_word) {

    //loops through each character until it reaches word size
    for (int i = 0; i < WORD_SIZE; i++) {
        //stored the first character in c
        char c = word[i];
        int is_guessed = 0;
        //loops through each character in the guessed letters
        for (int j = 0; guessed_letters[j] != '\0'; j++) {
            if (c == guessed_letters[j]) {
                printf("-");
                is_guessed = 1;
                break;
            }
        }
        if (!is_guessed && guessed_word[i] == c) {
            //if it matches print the word instead of "-"
            printf("%c", c);
        } else {
            printf("-");
        }
    }
    printf("\n");
}

//method to print word with dashes for hard_mode
void print_word_hard_mode(const char *word, const char *letters_guessed, const char *guess) {

    //loops through each character until it reaches word size
    for (int i = 0; i < WORD_SIZE; i++) {
        //if the letter is not null and guessed
        if (letters_guessed[i] != '\0') {
            printf("%c", letters_guessed[i]);
        } else {
            int is_present = 0;
            //second loop to check if it has been guessed
            for (int j = 0; j < WORD_SIZE; j++) {
                if (guess[j] == word[i]) {
                    is_present = 1;
                    break;
                }
            }
            if (is_present) {
                printf("-");
            } else {
                printf("-");
            }
        }
    }
}

//main method
int main() {
    //2d array to call wordlist
    char word_list[DICT_SIZE][WORD_SIZE + 1];
    //character array to store true word
    char true_word[WORD_SIZE + 1];
    char letters_guessed_in_correct_position[ALPHABET_SIZE] = {'\0'};
    int games_won = 0;
    int total_games = 0;
    int count_guess[ALPHABET_SIZE] = {0};
    int letters_used;

    //restart loop
    int play = 1;
    while (play) {
        //resets all elements in count_guess
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            count_guess[i] = 0;
        }
        letters_used = 0;

        //loads the wordlist
        load_word_list(word_list);

        srand(time(NULL));

        int random_index = rand() % DICT_SIZE;
        for (int i = 0; i < WORD_SIZE; i++) {
            true_word[i] = word_list[random_index][i];
        }

        //ensures last character is deleted
        true_word[WORD_SIZE] = '\0';

        //printf("%s\n", true_word);

        printf("Welcome to the word matching game! You have 6 guesses to find a randomly selected 5 letter word.\n");

        char input;
        printf("\nEnter x to play in hard-mode, anything else to play in normal mode: ");
        scanf(" %c", &input);
        getchar(); // consume the newline character
        //if condition to get into hard-mode
        if (input == 'X' || input == 'x') {
            printf("\n Welcome to the hard-mode! You have 6 guesses to find a randomly selected 5 letter word.\n");

            int guesses = 0;
            //to keep track of letters used in correct positions
            int used_letters[WORD_SIZE] = {0};
            //to store letters guessed in correct positions
            char letters_guessed_in_correct_positions[WORD_SIZE + 1] = {'\0'};
            //to keep track of unique letters entered
            int count_guess_hard_mode[ALPHABET_SIZE] = {0};
            for (int i = 0; i < MAX_GUESSES;) {
                printf("\nGuess %d: ", i + 1);
                char guess[WORD_SIZE + 1];
                int c;
                int count = 0;
                //while user's input c is not new line or end of file
                //if it is an alphabet set it to lower
                //and store it into count guess
                while ((c = getchar()) != '\n' && c != EOF) {
                    if (isalpha(c)) {
                        guess[count++] = (char) tolower(c);
                        count_guess_hard_mode[c - 'a']++;
                    }
                }

                //ensures last character is deleted
                guess[count] = '\0';

                //if condition for when user's input is less than 5
                if (count != WORD_SIZE) {
                    printf("The word must be exactly %d letters long and only alphabets.\n", WORD_SIZE);
                    continue;
                }

                //to check if user's input matches true word
                int is_match = 1;
                for (int j = 0; j < WORD_SIZE; j++) {
                    if (guess[j] != true_word[j]) {
                        is_match = 0;
                        break;
                    }
                }
                if (is_match == 1) {
                    printf("Congratulations! You guessed the word %s.\n", true_word);
                    games_won++;
                    break;
                }

                //loop to check if letters in user's guess have been used before and rule them out if they have
                for (int j = 0; j < WORD_SIZE; j++) {
                    if (used_letters[j] && guess[j] != letters_guessed_in_correct_positions[j]) {
                        printf("You cannot use the letter '%c' in that position.\n", guess[j]);
                        continue;
                    }
                    //if for when correct position is found and user is forced to use it in that position
                    if (letters_guessed_in_correct_positions[j] != '\0' &&
                        guess[j] != letters_guessed_in_correct_positions[j]) {
                        printf("You must use the letter '%c' in that position.\n",
                               letters_guessed_in_correct_positions[j]);
                        continue;
                    }
                }

                //if condition for when user's input is not in wordlist
                if (!is_word_in_list(guess, word_list)) {
                    printf("That word is not in the list.\n");
                    continue;
                }

                // update letters guessed in correct position and used letters array
                for (int j = 0; j < WORD_SIZE; j++) {
                    if (guess[j] == true_word[j]) {
                        letters_guessed_in_correct_positions[j] = guess[j];
                        used_letters[j] = 1;
                    }
                }

                // print the guess with letters guessed in correct position
                printf("Your guess: ");
                print_word_hard_mode(true_word, letters_guessed_in_correct_positions, (const char *) used_letters);

                char letters_guessed_in_wrong_position[WORD_SIZE + 1] = {'\0'};

                int used_positions[WORD_SIZE] = {0};
                //loop through each character from user's input and compare it to true word
                //if it matches then it is in correct position
                //if it does not set used position to 1
                for (int j = 0; j < WORD_SIZE; j++) {
                    if (guess[j] == true_word[j]) {
                        letters_guessed_in_correct_positions[guess[j] - 'a'] = guess[j];
                        used_positions[j] = 1;
                    }
                        //if it is correct position ensure it stays in the right array if guessed again
                    else if (letters_guessed_in_correct_positions[guess[j] - 'a'] != '\0') {
                        int idx = 0;
                        while (true_word[idx] != '\0') {
                            if (true_word[idx] == letters_guessed_in_correct_positions[guess[j] - 'a']) {
                                break;
                            }
                            idx++;
                        }
                        used_positions[idx] = 1;
                    }
                        //else for when letter has not been guessed before and is not in correct position
                    else {
                        int is_present = 0;
                        for (int k = 0; k < WORD_SIZE; k++) {
                            if (guess[j] == true_word[k]) {
                                is_present = 1;
                                break;
                            }
                        }
                        if (is_present) {
                            int idx = 0;
                            while (idx < WORD_SIZE) {
                                if (used_positions[idx] == 0) {
                                    letters_guessed_in_wrong_position[idx] = guess[j];
                                    used_positions[idx] = 1;
                                    break;
                                }
                                idx++;
                            }
                        }
                    }
                }
                //show correct letters
                printf("\nCorrect letters: ");
                printf("%s", letters_guessed_in_wrong_position);
            }

            total_games++;

            //for loop to check how many letters out of alphabet were used while making guesses
            for (int k = 0; k < ALPHABET_SIZE; k++) {
                if (count_guess[k] > 0) {
                    letters_used++;
                }
            }

            int percent_wins = calculate_percent_wins(games_won, word_list, total_games);
            printf("You won %d out of %d games (%.2d%%).\n", games_won, total_games, percent_wins);
            printf("Total games played: %d\n", total_games);


            float average_percentage = calculate_average_percentage(letters_used, ALPHABET_SIZE, total_games);
            printf("Average percentage of characters used: %.2f%%\n", average_percentage * 100);

            printHistogram(guesses);

        }
            //else to go into normal mode
        else {
            printf("\n Welcome to the normal-mode! You have 6 guesses to find a randomly selected 5 letter word.\n");

            //static variable used to increment guesses
            int guesses = 0;
            for (int i = 0; i < MAX_GUESSES;) {
                printf("\nGuess %d: ", i + 1);
                char guess[WORD_SIZE + 1];
                int c;
                int count = 0;
                while ((c = getchar()) != '\n' && c != EOF) {
                    if (isalpha(c)) {
                        guess[count++] = (char) tolower(c);
                        count_guess[c - 'a']++;
                    }
                }

                //ensures last character is deleted
                guess[count] = '\0';

                //if condition for when user's input is less than 5
                if (count != WORD_SIZE) {
                    printf("The word must be exactly %d letters long and only alphabets.\n", WORD_SIZE);
                    continue;
                }

                //if condition for when user's input is not in wordlist
                if (!is_word_in_list(guess, word_list)) {
                    printf("That word is not in the list.\n");
                    continue;
                }

                //to check if user's input matches true word
                int is_match = 1;
                for (int j = 0; j < WORD_SIZE; j++) {
                    //if not breaks
                    if (guess[j] != true_word[j]) {
                        is_match = 0;
                        break;
                    }
                }
                //if it matches
                if (is_match == 1) {
                    printf("Congratulations! You guessed the word %s.\n", true_word);
                    //increments local variable to keep track of games won
                    games_won++;
                    break;
                }

                //char array to store letters guessed in wrong position
                char letters_guessed_in_wrong_position[WORD_SIZE + 1] = {'\0'};

                //loop to check if user's input has a character from true word
                //and if it is correct or not
                for (int j = 0; j < WORD_SIZE; j++) {
                    if (guess[j] == true_word[j]) {
                        letters_guessed_in_correct_position[guess[j] - 'a'] = guess[j];
                    } else {
                        int is_present = 0;
                        for (int k = 0; k < WORD_SIZE; k++) {
                            if (guess[j] == true_word[k]) {
                                is_present = 1;
                                break;
                            }
                        }
                        if (is_present) {
                            int idx = 0;
                            while (letters_guessed_in_wrong_position[idx] != '\0') {
                                idx++;
                            }
                            letters_guessed_in_wrong_position[idx] = guess[j];
                        }
                    }
                }
                printf("Your guess: ");
                print_word(true_word, letters_guessed_in_correct_position, guess);

                //loop to check which letters are guessed correctly
                //and which incorrectly
                for (int j = 0; j < WORD_SIZE; j++) {
                    if (guess[j] == true_word[j]) {
                        letters_guessed_in_correct_position[guess[j] - 'a'] = guess[j];
                    } else {
                        int is_correct = 0;
                        for (int k = 0; k < WORD_SIZE; k++) {
                            if (guess[j] == true_word[k]) {
                                is_correct = 1;
                                break;
                            }
                        }
                        if (is_correct) {
                            letters_guessed_in_wrong_position[count] = guess[j];
                            count++;
                        }
                    }
                }
                //showing the correct letter but in wrong position
                //correct letters in right position are shown in the guess
                if (count > 0) {
                    printf("\nCorrect letters: ");
                    printf("%s", letters_guessed_in_wrong_position);

                }
                i++;
                guesses++;
            }

            //if condition for when user was unable to guess the word
            if (guesses >= MAX_GUESSES) {
                printf("Sorry, you did not guess the word %s.\n", true_word);
            }

            total_games++;

            //for loop to check how many letters out of alphabet were used while making guesses
            for (int k = 0; k < ALPHABET_SIZE; k++) {
                if (count_guess[k] > 0) {
                    letters_used++;
                }
            }

            int percent_wins = calculate_percent_wins(games_won, word_list, total_games);
            printf("You won %d out of %d games (%.2d%%).\n", games_won, total_games, percent_wins);
            printf("Total games played: %d\n", total_games);


            float average_percentage = calculate_average_percentage(letters_used, ALPHABET_SIZE, total_games);
            printf("Average percentage of characters used: %.2f%%\n", average_percentage * 100);

            printHistogram(guesses);

        }
        play = playAgain();
    }
    printf("Thanks for playing!\n");
    return 0;
}