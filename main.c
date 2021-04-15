#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

short flag_show_history = 0;

char *HISTORY_FILE_NAME = ".microshell_history";
short HISTORY_LINE_LIMIT = 20;

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

/*
Funkcja sprawdzajaca, czy podany tekst jest pusty (whitespace)

Przeglada poszczegolne znaki tekstu, jezeli trafi sie znak niebedacy
typu whitespace, to tekst traktowany jest jako niepusty.
Jezeli wszystkie znaki tekstu sa typu whitespace to tekst jest pusty
*/
short is_empty(char *str) {

    // jezeli wskaznik == NULL to jest pusty tekst
    if (str == NULL) {
        return 1;
    }

    while (*str != '\0') {

        // jezeli trafi sie znak niebedacy whitespace, to tekst nie jest pusty
        if (!isspace(*str)) {
            return 0;
        }
        str++;
    }

    return 1;
}

/*
Handler przerwania SIGQUIT
Ustawia flage [flag_show_history] na true, a samo wyswietlenie historii
dzieje sie w funkcji [main]
*/
void show_history(int signum) {
    // ustawienie flagi, zeby wyswietlic historie w funkcji main
    flag_show_history = 1;
}

int main(int argc, char *argv[]) {

    struct sigaction sa = {
        .sa_handler = show_history,
        .sa_flags = 0,
    };

    // wyczyszczenie signal set
    if (sigemptyset(&sa.sa_mask) < 0) {
        perror("Nie udalo sie ustawic signal set na empty");
        return EXIT_FAILURE;
    }

    // ustawienie obslugi sygnalu
    if (sigaction(SIGQUIT, &sa, NULL) < 0) {
        perror("Nie udalo sie ustawic obslugi sygnalu");
        return EXIT_FAILURE;
    }

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

    // wypisanie prompta (jezeli nie jest czytanie z pliku)
    if (check_file < 0) {
        printf(">> ");
    }

    // zmienna mowiaca o zakonczeniu iteracji
    short do_job = 1;

    // czytanie znakow
    do {
        one_char = fgetc(stdin);

        // sprawdzenie, czy wsytapil blad przy czytaniu z stdin
        if (ferror(stdin)) { 

            // jezeli jest ustawiono flaga, to znaczy, ze blad [fgetc] by spowodowany sygnalem
            if (flag_show_history) {

                clearerr(stdin); // wyczyszczenie flag bledu ze standardowego wejscia
                printf(" Historia\n");

                // pobranie sciezki do katalogu domowego uzytkownika
                char *home_dir = getenv("HOME");

                // sprawdzenie, czy zostala zwrocona poprawna wartosc (jezeli nie to wyswietlic komunikat)
                if (home_dir == NULL) {
                    printf("   [BRAK ZMIENNEJ HOME]\n");

                } else {
                    // otworzyc plik z historia
                    char file_path[strlen(home_dir) + strlen(HISTORY_FILE_NAME) + 1];
                    strcpy(file_path, home_dir);
                    strcat(file_path, "/");
                    strcat(file_path, HISTORY_FILE_NAME);
                    FILE *history_file = fopen(file_path, "r");

                    // jezeli pliku z historia nie ma
                    if (history_file == NULL) {
                        printf("   [Nie ma pliku z historia w: %s]\n", home_dir);
                    } 
                    
                    // jezeli plik z historia jest, to wyswietlic jego zawartosc
                    else {
                        // zmienna pomocnicza
                        char one_history_char;

                        // zmienna pomocnicza do poprawnego wyswietlania wciecia w konsoli
                        short indent = 1;

                        // czytanie znakow z pliku z historia az do EOF
                        while ((one_char = fgetc(history_file)) != EOF) {
                            if (indent) {
                                indent = 0;   
                                printf("   ");
                            }

                            printf("%c", one_char);

                            if (one_char == '\n') {
                                indent = 1;
                            }
                        }

                        fclose(history_file);
                    }
                }

                flag_show_history = 0; // wyzerowanie flagi, zeby moz czytac ewentualne bledy na stdin
                printf(">> ");
                continue; // pominiecie dalszej czesci kodu, zeby nie ustawaic do_job na 0 w warunku (on_char == EOF)

            } else {
                perror("Blad przy wczytywaniu znaku z stdin:");
                return EXIT_FAILURE;
            }
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
                
                // jezeli pierwszy znak pierwszego elementu to "#" to nalezy ignorowac linijke
                // odwolanie do pierwszego znaku pierwszego elemetu jest bezpieczne
                // bo puste elementy nie sa dodawane do listy
                if (arr[0][0] != '#') {

                    // zapisanie wykonywanego polecenia tylko jezeli program wykonywany jest w trybie interaktywnym
                    // w trybie skryptu nie ma potrzeby zapisaywania historii wykonanych polecen
                    if (check_file < 0) {

                        // pobranie sciezki do katalogu domowego uzytkownika
                        char *home_dir = getenv("HOME");

                        // zapisanie historii tylko jezeli istnieje sciezka domowa
                        if (home_dir != NULL) {

                            // czytanie pliku historii w celu sprawdzenia ilosci zapisanych polecen
                            char file_path[strlen(home_dir) + strlen(HISTORY_FILE_NAME) + 1];
                            strcpy(file_path, home_dir);
                            strcat(file_path, "/");
                            strcat(file_path, HISTORY_FILE_NAME);
                            FILE *history_file = fopen(file_path, "r");

                            // zmienne pomocnicze
                            char **copying_arr = NULL;
                            char *one_line = NULL;
                            int one_line_length = 0;
                            int current_copying_arr_length = 0;
                            int copying_arr_maximum_size = 0;

                            // jezeli istnieje plik z historia to nalezy odczytac liczbe polecen w nim zapisanych
                            if (history_file != NULL) {
                                
                                // zmienne pomocnicze
                                // short flag_was_not_empty = 0;
                                char file_char;

                                do {
                                    file_char = fgetc(history_file);

                                    if (file_char == '\n' || file_char == EOF) {

                                        // sprawdzenie, czy wczytana linia nie jest pusta
                                        if (!is_empty(one_line)) {
                                            add_element_to_array(&copying_arr, &copying_arr_maximum_size, &current_copying_arr_length, one_line);
                                        } else {
                                            if (one_line != NULL) {
                                                free(one_line);
                                            }
                                        }

                                        one_line = NULL;

                                    } else {
                                        
                                        // jezeli wczytany zostal normalny znak, to nalezy go dodac do tekstu
                                        add_char_to_string(&one_line, &one_line_length, &file_char);
                                    }

                                } while (file_char != EOF);

                                fclose(history_file);
                            }

                            // zapisanie polecen do plik z historia
                            history_file = fopen(file_path, "w");
                            if (history_file != NULL) {

                                // zmienna pomocnicza, przechowujaca offset od ktorego nalezy
                                // zaczac przepisywanie starych polecen historii z tablicy [copying_arr]
                                // do pliku
                                //
                                // jezeli liczba linii w pliku historii przekroczy limit [HISTORY_LINE_LIMIT]
                                // to offset bedzie dodatni, co spowoduje wyrzucenie najstarszych
                                // wpisow z historii i dodanie nowych
                                short line_offset = current_copying_arr_length - HISTORY_LINE_LIMIT + 1;
                                if (line_offset < 0) {
                                    line_offset = 0;
                                }

                                // przepisanie starych polecen historii z powrotem do pliku
                                short i;
                                for (i = line_offset; i < current_copying_arr_length; i++) {

                                    one_line = copying_arr[i];
                                    while (*one_line != '\0') {
                                        fputc(*one_line, history_file);
                                        one_line++;    
                                    }              
                                    fputc('\n', history_file);                      
                                }

                                // zapisanie nowego polecenia na koncu pliku
                                for (i = 0; i < current_arr_size; i++) {
                                    one_line = arr[i];
                                    while (*one_line != '\0') {
                                        fputc(*one_line, history_file);
                                        one_line++;
                                    }

                                    // wypisanie spacji rozdzielajacej elementy polecenia
                                    if (i != current_arr_size - 1) {
                                        fputc(' ', history_file);
                                    }
                                }
                                fputc('\n', history_file);

                                fclose(history_file);
                            }
                        }
                        
                    }

                    //todo: wykonywanie polecenia (parsowanie, wykonanie itp.)
                }

            }

            // wyczyszczenie tablicy (w ktorej przechowywane sa elementy obecnego polecenia)
            // po wykonaniu polecenia
            printf("clear len:%d\n", current_arr_size);
            clear_string_array(&arr, &max_arr_size, &current_arr_size);

            // wypisanie prompta jezeli byl nacisniety enter (i jezeli nie jest czytanie z pliku)
            if (one_char == '\n' && check_file < 0) {
                printf(">> ");
            }

        } else if (one_char != ' ') { // jezeli byl wczytany inny znak, to nalezy go dodac do [current_string]
            add_char_to_string(&current_string, &current_string_length, &one_char);
        }

        if (one_char == EOF) {
            do_job = 0;
        }
        
    } while (do_job);
    
    return EXIT_SUCCESS;
}