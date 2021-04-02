#include <stdlib.h>

/*
Funkcja dodajaca element do tablicy i ewentualnie 
zwiekszajaca rozmiar tablicy na 2 razy wiekszy 
*/
int add_element_to_array(char ***arr, int *max_arr_size, int *current_arr_size, char *new_element)  {
    // sprawdzenie, czy jest potrzeba reallokacji
    if (current_arr_size >= max_arr_size) {
        // trzeba zwiekszyc rozmiar tablicy
        char **new_arr = realloc((*arr), (*max_arr_size) * 2 * sizeof(char*));
        if (new_arr != NULL) { // jezeli udalo sie rallokowac
            (*max_arr_size) *= 2;
            (*arr) = new_arr;
        } else {
            return -1; // nie udalo sie reallokowac tablicy
        }
    }
    // dodanie nowego elementu na odpowiednie miejsce
    (*arr)[*current_arr_size * sizeof(char*)] = new_element; //TODO: naprawic
    (*current_arr_size) += 1;
    return 0;
}

int main(int argc, char *argv[]) {

    // zmienne potrzebne do przechowywania elementow polecenia (do parsowania)
    char **arr;
    int max_arr_size = 0, current_arr_size = 0;

    return 0;
}