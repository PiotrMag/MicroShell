#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
Funkcja dodajaca element do tablicy i ewentualnie 
zwiekszajaca rozmiar tablicy na 2 razy wiekszy 
*/
int add_element_to_array(char ***arr, int *max_arr_size, int *current_arr_size, char *new_element)  {
    // sprawdzenie, czy jest potrzeba reallokacji
    if ((*current_arr_size) >= (*max_arr_size)) {
        int new_size = (*max_arr_size) * 2;
        if (new_size == 0) new_size = 1;
        // trzeba zwiekszyc rozmiar tablicy
        char **new_arr = realloc((*arr), new_size* sizeof(char*));
        if (new_arr != NULL) { // jezeli udalo sie rallokowac
            (*max_arr_size) = new_size;
            (*arr) = new_arr;
        } else {
            return -1; // nie udalo sie reallokowac tablicy
        }
    }
    // dodanie nowego elementu na odpowiednie miejsce
    (*arr)[*current_arr_size] = new_element; //TODO: naprawic
    (*current_arr_size) += 1;
    return 0;
}

/*
Funkcja dodajaca znak do danego char*
*/
int add_char_to_string(char **str, int *length, char *new_char) {
    // sprawdzenie, czy obecnie string jest pusty
    if (*str == NULL || length == 0) {
        char *new_str = realloc((*str), sizeof(char));
        if (new_str == NULL) {
            return -1;
        }
        (*str) = new_str;
        (*str)[0] = '\0';
        *length = 1;
    }
    int new_size = (*length) + 1;
    char *new_str = realloc((*str), sizeof(char) * new_size);
    if (new_str == NULL) {
        return -1;
    }
    (*str) = new_str;
    // dodanie znaku na koniec
    (*str)[*length-1] = *new_char;
    (*str)[*length] = '\0';
    *length = new_size;
    return 0;
}

/*
Funkcja testujaca dzialanie funkcji [add_element_to_array]
Probuje dodac kilka elementow i je wypisac
*/
void test_string_list() {
    char **arr = NULL;
    int max_size = 0, current_size = 0;

    // dodanie kilku przykladowych elementow
    add_element_to_array(&arr, &max_size, &current_size, "nowy");
    add_element_to_array(&arr, &max_size, &current_size, "drugi");
    add_element_to_array(&arr, &max_size, &current_size, "trzeci");
    add_element_to_array(&arr, &max_size, &current_size, "czwarty");
    add_element_to_array(&arr, &max_size, &current_size, "piaty");

    printf("current_size: %d\n", current_size);
    printf("max_size: %d\n", max_size);

    // wypisanie wszystkich elementow
    int i;
    for (i = 0; i < current_size; i++) {
        printf("%s\n", arr[i]);
    }

    free(arr);
}

int main(int argc, char *argv[]) {

    // wypisanie argumentow
    int n;
    for (n = 0; n < argc; n++) {
        printf("%s ", argv[n]);
    }

    // zmienne potrzebne do przechowywania elementow polecenia (do parsowania)
    char **arr = NULL;
    int max_arr_size = 0, current_arr_size = 0;

    // czytanie elemento polecenia
    char *current_string = NULL;
    int current_string_length = 0;
    char one_char;
    do {
        one_char = fgetc(stdin);
        if (ferror(stdin)) { // jezeli wystapil blad przy czytaniu znaku z stdin
            perror("Blad przy wczytywaniu znaku z stdin:");
            return EXIT_FAILURE;
        }
        if (one_char == ' ' || one_char == '\n' || one_char == EOF) { // jezeli wczytano odpowiedni znak, to nalezy uznac, ze jest to koniec elementu
            if (current_string != NULL && current_string != "" /*&& !isspace(current_string)*/) {
                int result = add_element_to_array(&arr, &max_arr_size, &current_arr_size, current_string);
                if (result == -1) { // byl blad przy probie dodania elementu do tablicy
                    printf("Byl blad przy dodawaniu elementu do tablicy (main loop)");
                    return EXIT_FAILURE;
                }
            }
            current_string = NULL;
        }
        if (one_char == '\n' || one_char == EOF) { // jezeli wczytano \n lub EOF to nalezy wkonac polecenie
            //TODO: wykonywanie polecenia
        } else if (one_char != ' ') { // jezeli byl wczytany inny znak, to nalezy go dodac do [current_string]
            add_char_to_string(&current_string, &current_string_length, &one_char);
        }
    } while (one_char != EOF);

    int i; 
    for (i = 0; i < current_arr_size; i++) {
        printf("%s\n", arr[i]);
    }
    
    return EXIT_SUCCESS;
}