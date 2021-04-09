#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
Funkcja dodajaca element do tablicy i ewentualnie 
zwiekszajaca rozmiar tablicy na 2 razy wiekszy 

W przypadku kiedy funkcja wykryje, ze obecny rozmiar tablicy nie jest
w stanie pomiescic dodatkowego elementu, zajmowana przez tablice pamiec
jest reallokowana na rozmiar 2 razy wiekszy, a nowy element jest 
dodawany na pierwszym wolnym miejscu
*/
int add_element_to_array(char ***arr, int *max_arr_size, int *current_arr_size, char *new_element)  {
    
    // sprawdzenie, czy jest potrzeba reallokacji
    if ((*current_arr_size) >= (*max_arr_size)) {

        // zmienna pomocnicza
        int new_size = (*max_arr_size) * 2;

        // sprawdzenie, czy przypadkiem rozmiar tablicy
        // byl == 0; w takim przypadku zmienna pomoocnicza
        // [new_size] bedzie == 0, wiec trzeba recznie zmienic
        // jej wartosc
        if (new_size == 0) new_size = 1;

        // proba reallokacji pamieci
        char **new_arr = realloc((*arr), new_size* sizeof(char*));

        if (new_arr == NULL) { // jezeli nie udalo sie rallokowac
            return -1; 
        }

        // zmiana wartosci zmiennej [max_arr_size] na nowa wielkosc tablicy
        (*max_arr_size) = new_size;

        // przypisanie nowego wskaznika na tablice w miejsce starego wskaznika
        (*arr) = new_arr;
    }

    // dodanie nowego elementu na odpowiednie miejsce
    (*arr)[*current_arr_size] = new_element;
    (*current_arr_size) += 1;
    return 0;
}

/*
Funkcja dodajaca znak do danego char*

Funkcja reallokuje zajmowana przez char* pamiec tak, aby pomiescila
ona dodatkowy jeden znak
*/
//todo: moze trzeba przerobic, zeby nie trzeba bylo podawac parametru [length]
int add_char_to_string(char **str, int *length, char *new_char) {

    // sprawdzenie, czy obecnie string jest pusty
    if (*str == NULL || length == 0) {

        // reallokacja pamieci tak, zeby mogla pomiescic jeden znak (char) '\0'
        char *new_str = realloc((*str), sizeof(char));

        // jezeli reallokacja nie powiodla sie
        if (new_str == NULL) {
            return -1;
        }

        // przepisanie wskaznika na nowy
        (*str) = new_str;

        // zapisanie znaku konca string
        (*str)[0] = '\0';

        // zapisanie nowej dlugosci string
        *length = 1;
    }

    // zmienna pomocnicza
    int new_size = (*length) + 1;

    // reallokacja pamiesci zmiennej [str]
    char *new_str = realloc((*str), sizeof(char) * new_size);

    // jezeli byl blad podczas reallokacji
    if (new_str == NULL) {
        return -1;
    }

    // zapisanie nowego wskaznika w miejsce starego wskaznika
    (*str) = new_str;

    // dodanie znaku na koniec
    (*str)[*length-1] = *new_char;
    (*str)[*length] = '\0';

    // zapisanie nowej dlugosci [str] w zmiennej [length]
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

    //! prawdopodobnie trzeba tylko sprawdzic, czy podany argument jest nazwa pliku
    //! jezeli tak, to odpowiednio przekierowac stdin, zeby wczytywalo z pliku
    //! mozliwe, ze trzeba bedzie w jakis sopsob przekierowac pozostale argumenty
    //! jako standardowe wejscie, ale to trzeba jeszcze zbadac
    // wlaczenie odpowiedniej wersji programu na bazie ilosci przekazanych argumentow
    if (argc <= 1) { 
        //todo: wlaczenie programu w trybie interaktywnym (standardowe wejscie jest z konsoli, czyli pewnie nie trzeba nic zmieniac)
    } else if ( argc <= 2) {
        //todo: sprawdzic, czy podany argument jest plikiem, czy poleceniem
        // if (plik) {
        //     //todo: przekierowac standardowe wejscie, zeby bylo z pliku
        // } else {
        //     //todo: sprobowac wykonac polecenie
        // }
    } else {
        //todo: sprobowac uruchomic przekazane polecenie
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
            //todo: wykonywanie polecenia
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