#include <stdlib.h>
#include <stdio.h>

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

    // zmienne potrzebne do przechowywania elementow polecenia (do parsowania)
    // char **arr;
    // int max_arr_size = 0, current_arr_size = 0;
    test_string_list();
    return 0;
}