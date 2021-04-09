#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>

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
Funkcja czyszczaca tablice string

Zwalnia pamiec zajmowana przez poszczegolne elementy tablicy
a potem zwalnia pamiec zajmowana przez sama tablice. Odpowiednie 
wskazniki sa ustawiane na NULL
*/
void clear_string_array(char ***arr, int *max_arr_size, int *current_arr_size) {

    // zmienna pomocnicza do iterowania
    int i;

    // zwolnienie pamieci zajmowanej przez poszczegolne elementy string
    for (i = 0; i < *current_arr_size; i++) {
        // printf("free: %s\n", (*arr)[i]);
        free((*arr)[i]);
        (*arr)[i] = NULL;
    }

    // zwolnienie pamieci zajmowanej przez tablice string
    free(*arr);
    *arr = NULL;

    // wyzerowanie zmiennych przekazanych przez parametr do funkcji
    *max_arr_size = 0;
    *current_arr_size = 0;
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

    // zmienna pomocnicza, przechowujaca deskryptor pliku skryptu
    // wstepnie ustawiona na -1, zaby w dalszej czesi programu
    // mozna bylo sprawdzic, czy
    int check_file = -1;

    // czytanie stdin z pliku jezeli podana zostala sciezka do pliku
    if (argc >= 2) {

        // otwarcie deskryptora w celu przekirowania stdin na plik, jezeli
        // przy wykonywaniu porogramu podana zostala sciezka na plik skryptu
        check_file = open(argv[1], O_RDONLY);

        // jezeli blad przy otwarciu pliku skryptu
        if (check_file < 0) {
            perror("Nie udalo sie otworzyc pliku skryptu");
            return EXIT_FAILURE;
        }

        // jezeli udalo sie otworzyc poprawnie deskryptor, to
        // nalezy przekierowac stdin na plik
        dup2(check_file, STDIN_FILENO);
    } 

    // zmienne potrzebne do przechowywania elementow polecenia (do parsowania)
    char **arr = NULL;
    int max_arr_size = 0, current_arr_size = 0;

    // zmienne potrzebne do czytania poszczegolnych znakow i elementow polecenia
    char *current_string = NULL;
    int current_string_length = 0;
    char one_char;

    // czytanie znakow
    do {
        one_char = fgetc(stdin);

        // sprawdzenie, czy wsytapil blad przy czytaniu z stdin
        if (ferror(stdin)) { 
            perror("Blad przy wczytywaniu znaku z stdin:");
            return EXIT_FAILURE;
        }

        // sprawdzenie, czy podany zostal znak oddzielajacy poszczegolne elementy polecenia
        if (one_char == ' ' || one_char == '\n' || one_char == EOF) { // jezeli wczytano odpowiedni znak, to nalezy uznac, ze jest to koniec elementu
            if (current_string != NULL && current_string != "") {
                int result = add_element_to_array(&arr, &max_arr_size, &current_arr_size, current_string);
                if (result == -1) { // byl blad przy probie dodania elementu do tablicy
                    printf("Byl blad przy dodawaniu elementu do tablicy (main loop)");
                    return EXIT_FAILURE;
                }
            }
            current_string = NULL;
        }

        // sprawdzenie, czy podany zostal znak powodujacy wykonanie polecenia
        if (one_char == '\n' || one_char == EOF) { 
            
            // zabezpieczenie przed pustymi tablicami
            if (current_arr_size > 0) {
                //todo: wykonywanie polecenia (parsowanie, wykonanie itp.)
            }
            
            // wyczyszczenie tablicy (w ktorej przechowywane sa elementy obecnego polecenia)
            printf("clear len:%d\n", current_arr_size);
            clear_string_array(&arr, &max_arr_size, &current_arr_size);

        } else if (one_char != ' ') { // jezeli byl wczytany inny znak, to nalezy go dodac do [current_string]
            add_char_to_string(&current_string, &current_string_length, &one_char);
        }
        
    } while (one_char != EOF);
    
    return EXIT_SUCCESS;
}