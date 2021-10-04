// IZP 2021/2020
// 1. Project - Práce s textem
// Verevkin Aleksandr (xverev00)


//declaring needed libraries and constants
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define password_max_length 101

//function that check arguments
//error in case of not supported argument
int param_check(int argc, char* argv[]) {

    if (argc < 3) {

        printf("Error: program start form:\n./pwcheck LEVEL PARAM [--stats]\n");
        return 1;
    }

    if ((atoi(argv[1]) < 1 || atoi(argv[1]) > 4) || (atoi(argv[1]) != atof(argv[1]))) {

        printf("Error: LEVEL argument must be whole number in [1, 4] interval\n");
        return 1;
    }

    if ((atoi(argv[2]) <= 0) ||(atoi(argv[2]) != atof(argv[2]))) {

        printf("Error: PARAM argument must be positive, whole number\n");
        return 1;
    }
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

                        if (sameness == param) {
                            
                            //return false in case of same substings in the password
                            return false;
                        }

                    } else { break; }
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
            if (counter == param) break;

        } else {

        same_char = buff[i];
        if (counter == param) break;
            
        }
    }

    //check if password have same characters in a row
    if (counter != param) {

        //check if its needed to go to the next level
        if (level > 3) {

            if (substrings_in_row(buff, param)) {

                return true;

            } else { return false; }
        
        } else { return true; }
    }

    return false;
}

//function that check if password have special symbols in it
bool symbol(char buff[password_max_length]) {

    for (int i = 0; buff[i] != '\0'; i++) {
    
        if (((buff[i] >= 33) && (buff[i] <= 47)) || 
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

                    } else { return false; }

            } else if (level > 1 && param == 3) {

                if (number(buff) || symbol(buff)) {
                    
                    if (level > 2) {

                        if (same_in_row(buff, param, level)){

                            return true;

                        } else { return false; }

                    } else { return true; }

                } else { return false; }

            } else if (level > 1 && param > 3) {

                if (number(buff) && symbol(buff)) {

                    if (level > 2) {

                        if (same_in_row(buff, param, level)) {

                            return true;

                        } else { return false; }
                    
                    } else { return true; }
                
                } else { return false; }
            
            } else { return true; }
        }
    }
    return false;
}

//function that check if password have capital letter in it
void capital_letter(char buff[password_max_length], int param, int level) {

    //passing each password through the functions
    //and print them if they are pass the conditions
    while (fgets(buff, password_max_length, stdin) != NULL) {
        
        for (int i = 0; buff[i] != '\0'; i++) {

            if ((buff[i] >= 'A') && (buff[i] <= 'Z')) {
                
                if (small_letter(buff, param, level)) {

                    printf("%s", buff);
                    break;
                }
            }
        }
    }
}

//function that calculate and print out stats of the given passwords
void stat(char buff[password_max_length]) {

    int hash_tab[128] = {0};

    int nchars = 0;
    int min_len = password_max_length;
    
    int all_characters = 0;
    int passwords_count = 0;

    rewind(stdin);

    while (fgets(buff, password_max_length, stdin) != NULL) {

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

    for (int i = 0; i < 128; i++) {

		nchars += hash_tab[i];
	}

    printf("Statistika:\n");
    printf("Ruznych znaku: %d\n", nchars);
    printf("Minimalni delka: %d\n", min_len);
    printf("Prumerna delka: %.1lf\n", (double)all_characters / (double)passwords_count);
}

//function that check if program have '--stats' in the arguments
//and pass passwords to the statistics function if find out
void stats_check(char buff[password_max_length], char* argv[]) {

    int i = 0;
    char str[8] = "--stats";
    bool accept = true;

    while (argv[3][i] != '\0' || str[i] != '\0'){

        if (argv[3][i] != str[i]) {

            accept = false;
            break;
        }
        i++;
    }

    if (accept) stat(buff);
}

//main function
int main(int argc, char* argv[]){
    
    //check if program have right arguments
    if (param_check(argc, argv)) return 1;

    char buff[password_max_length];

    //convert arguments to the integers
    int level = atoi(argv[1]);
    int param = atoi(argv[2]);

    //pass paswords through the tests
    capital_letter(buff, param, level);
    
    if (argc > 3) {
        
        stats_check(buff, argv);
    }

    return 0;
}