// Verevkin Aleksandr
// Passwords check program


//declaring needed libraries and constants
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define password_max_length 103


//check for switches in the arguments
int prem_arg_check(int argc, char* argv[], char sw) {
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if(argv[i][1] == sw) {
                if(argv[i][2] == '\0') {
                    if (argv[i + 1] == NULL) {
                        return -1;
                    }
                    return i + 1;
                }
            }
        }
    }
    return 0;
}

//funtion that check if password have right amount of symbols
bool password_length_check(char buff[password_max_length]) {
    for (int i = 0; buff[i] != '\0'; i++) {
        if (i >= 101) {
            return true;
        }
    }
    return false;
}

//function that check arguments
//error in case of not supported argument
int param_check(int level_arg, int param_arg, char* argv[]) {
    if (level_arg != 0 && ((atoi(argv[level_arg]) < 1 || atoi(argv[level_arg]) > 4) || (atoi(argv[level_arg]) != atof(argv[level_arg])))) {
        fprintf(stderr, "Error: LEVEL argument must be whole number in [1, 4] interval\n");
        return 1;
    }
    if (param_arg != 0 && ((atoi(argv[param_arg]) <= 0) || (atoi(argv[param_arg]) != atof(argv[param_arg])))) {
        fprintf(stderr, "Error: PARAM argument must be positive, whole number\n");
        return 1;
    }
    //return success code in case of no errors
    return 0;
}

//function that finds lenght of given password
int length(char buff[password_max_length]) {
    int password_len = 0;
    for (int i = 0; buff[i] != '\n'; i++, password_len++);
    return password_len;
}

//function that finds, if password have same substrings
bool substrings_in_row(char buff[password_max_length], int param) {
    //call lenght of password function
    //and saves it in the variable
    int str_len = length(buff);
    //pass the password through the loops too find out
    //if password has same substrings
    for (int i = 0; i + param + 1 <= str_len; i++) {
        for (int j = i + 1; j + param <= str_len; j++) {
            if (buff[i] == buff[j]) {
                int sameness = 1;
                for (int s = i + 1, l = j + 1; sameness < param; s++, l++) {
                    if (buff[s] == buff[l]) {
                        sameness++;
                        //return false in case of same substings in the password
                        if (sameness == param) {
                            return false;
                        }
                    } else {
                        break;
                    }
                }
            }
        }
    }
    //return true in case that password dont have same substrings
    return true;
}

//function that finds, if password have same characters in the row
bool same_in_row(char buff[password_max_length], int param, int level) {
    char same_char = '\0';
    int counter = 1;
    for (int i = 0; buff[i] != '\0'; i++) {
        if (buff[i] == same_char) {
            counter++;
            if (counter == param) {
                break;
            }
        } else { 
            same_char = buff[i];
            if (counter == param) {
                break;
            }
        }
    }

    //check if password have same characters in a row
    if (counter != param) {
        //check if its needed to go to the next level
        if (level > 3) {
            if (substrings_in_row(buff, param)) {
                return true;
            } else {
                return false;
            }
        } else {
            return true;
        }
    }
    return false;
}

//function that check if password have special symbols in it
bool symbol(char buff[password_max_length]) {
    for (int i = 0; buff[i] != '\0'; i++) {
        //use ascii table symbol counting
        if (((buff[i] >= 32) && (buff[i] <= 47)) || 
            ((buff[i] >= 58) && (buff[i] <= 64)) ||
            ((buff[i] >= 91) && (buff[i] <= 96)) ||
            ((buff[i] >= 123) && (buff[i] <= 126))) {
                return true;
        }
    }
    return false;
}

//function that check if password have number in it
bool number(char buff[password_max_length]) {
    for (int i = 0; buff[i] != '\0'; i++) {
        if ((buff[i] >= '0') && (buff[i] <= '9')) {
            return true;
        }
    }
    return false;
}

//function that check if password have small letter in it
//and pass password further if needed
bool small_letter(char buff[password_max_length], int param, int level) {
    for (int i = 0; buff[i] != '\0'; i++) {
        if ((buff[i] >= 'a') && (buff[i] <= 'z')) {
            if (level > 2 && param < 3) {
                    if (same_in_row(buff, param, level)) {
                        return true;
                    } else {
                        return false;
                    }
            } else if (level > 1 && param == 3) {
                if (number(buff) || symbol(buff)) {    
                    if (level > 2) {
                        if (same_in_row(buff, param, level)){
                            return true;
                        } else {
                            return false;
                        }
                    } else {
                        return true;
                    }
                } else {
                    return false;
                }
            } else if (level > 1 && param > 3) {
                if (number(buff) && symbol(buff)) {
                    if (level > 2) {
                        if (same_in_row(buff, param, level)) {
                            return true;
                        } else {
                            return false;
                        }
                    } else {
                        return true;
                    }
                } else {
                    return false;
                }            
            } else {
                return true;
            }
        }
    }
    return false;
}

//function that check if password have capital letter in it
bool capital_letter(char buff[password_max_length], int param, int level, bool stats) {

    //define variables for statistics counting
    //define hash table to find unique characters
    int hash_tab[128] = {0};
    int min_len = password_max_length;
    int all_characters = 0, passwords_count = 0, nchars = 0;
    //passing each password through the functions
    //and print them if they are pass the conditions
    while (fgets(buff, password_max_length, stdin) != NULL) {
        //check length of each password
        //error in case of larger password
        if (password_length_check(buff)) {
            fprintf (stderr, "Error: password max length is 100 symbols\n");
            return false;
        }

        //check if password have capital letter in it
        //pass password to the next check function
        for (int i = 0; buff[i] != '\0'; i++) {
            if ((buff[i] >= 'A') && (buff[i] <= 'Z')) {
                if (small_letter(buff, param, level)) {
                    printf("%s", buff);
                    break;
                }
            }
        }

        //counting statistics of the passwords if needed
        if (stats) {
            int len = 0;
	        for (int i = 0; buff[i] != '\0'; i++) {
		        if (buff[i] != '\n') { 
                    hash_tab[(int)buff[i]] = 1;
                    len++;
                }
	        }
            if (len < min_len) {
                min_len = len;
            } 
            all_characters += len;
            passwords_count++;
        }

    }

    //print out statistics of the passwords if needed
    if (stats) {
        for (int i = 0; i < 128; i++) {
		    nchars += hash_tab[i];
        }
        //print out statistics
        printf("Statistika:\n");
        printf("Ruznych znaku: %d\n", nchars);
        printf("Minimalni delka: %d\n", min_len);
        printf("Prumerna delka: %.1lf\n", (double)all_characters / (double)passwords_count);
    }
    return true;
}

//function that check if program have '--stats' in the arguments
bool stats_check(int argc, char* argv[]) {
    char str[8] = "--stats";
    bool accept = false;
    for (int i = 1; i < argc; i++) {
        int j = 0;
        accept = true;
        while (argv[i][j] != '\0' || str[j] != '\0') { 
            if (argv[i][j] != str[j]) {
                accept = false;
                break;
            }
            j++;
        }
        if (accept) break;
    }

    if (accept) {
        return true;
    } else {
        return false;
    }
}

//main function
int main(int argc, char* argv[]){
    
    char buff[password_max_length];
    int level, param;
    int level_arg = prem_arg_check(argc, argv, 'l');
    int param_arg = prem_arg_check(argc, argv, 'p');

    //error handling
    //not existing argument after switch
    if (level_arg < 0 || param_arg < 0) {
        fprintf (stderr, "Error: '-' switch pointing on not existing argument\n");
        return 1;
    }

    //check if used argument "--stats"
    bool stats = stats_check(argc, argv);

    //check arguments for -l -p switches
    if (argc == 1 || (argc == 2 && stats)) {
        level = 1, param = 1;
    } else if (level_arg || param_arg) {
        if (param_check(level_arg, param_arg, argv)){
            return 1;
        }
        if (!level_arg) {
            level = 1;
        } else {
            level = atoi(argv[level_arg]);
        }
        if (!param_arg) {
            param = 1;
        } else {
            param = atoi(argv[param_arg]);
        }
    } else {
        if (argc != 3 && (argc != 4 || !stats)) {
            fprintf(stderr, "Error: program start form:\n   ./pwcheck LEVEL PARAM [--stats]\nor ./pwcheck [-l LEVEL] [-p PARAM] [--stats]\n");
            return 1;
        }
        //check if program have right arguments
        if (param_check(1, 2, argv)) {
            return 1;
        }
        //convert arguments to the integers
        level = atoi(argv[1]);
        param = atoi(argv[2]);
    }

    //pass paswords through the tests
    if (!(capital_letter(buff, param, level, stats))) {
        return 1;
    }
    
    return 0;
}
