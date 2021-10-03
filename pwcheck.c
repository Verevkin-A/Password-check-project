#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define password_max_length 101


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

bool substrings_in_row(char buff[password_max_length], int param) {
    
    if (param > 0) {

    while (fgets(buff, password_max_length, stdin) != NULL) {

        printf("%s", buff);
    }
    }
    return false;
}

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

    if (counter != param) {

        if (level > 3) {

            if (substrings_in_row(buff, param)) {

                return true;

            } else { return false; }
        
        } else { return true; }
    }
    return false;
}

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

bool number(char buff[password_max_length]) {

    for (int i = 0; buff[i] != '\0'; i++) {

        if ((buff[i] >= '0') && (buff[i] <= '9')) {

            return true;
        }
    }
    return false;
}

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

void capital_letter(char buff[password_max_length], int param, int level) {

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

void stat_check(char buff[password_max_length], char* argv[]) {

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

int main(int argc, char* argv[]){
    
    if (param_check(argc, argv)) return 1;

    char buff[password_max_length];

    int level = atoi(argv[1]);
    int param = atoi(argv[2]);

    capital_letter(buff, param, level);
    
    if (argc > 3) {
        
        stat_check(buff, argv);
    }

    char str[20] = "abcabc";
    
    for (int substr_len = 2; substr_len <= 6; substr_len++) {

        for (int start = 0; start <= 6 - substr_len; start++) {

            int end = start + substr_len - 1;

            for (int substr_index = start; substr_index <= end; substr_index++) {
                
                printf("%c", str[substr_index]);
            }
            printf("\n");
        }
    }

    return 0;
}