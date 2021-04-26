
# Tresc zadania:

### Prosta powłoka tekstowa (shell):

- `(12p)` Powłoka pobiera ze standardowego wejścia pojedynczy wiersz. Następnie dokonuje prostej analizy wiersza dzieląc go na słowa separowane spacjami. Pierwsze słowo jest nazwa programu który należy uruchomić (wykorzystując zmienną PATH), a pozostałe sa argumentami. UWAGA: procesy uruchamiać należy poprzez wywołania systemowe z rodziny exec (execl, execlp, execvp itd.) - ~~inne rozwiązania (np. przekierowanie odpowiedzialności do systemowej powłoki) nie będą uznawane~~. Shell uruchamia program i standardowo czeka na zakończenie jego pracy, chyba ze ostatnim słowem jest znak & co powoduje uruchomienie programu w tle, jak w normalnym shellu bash. Shell kończy prace gdy otrzyma znak końca pliku. Dzięki temu możliwe jest przygotowanie prostych skryptów, które można uruchamiać z wiersza poleceń bash-a, jeżeli pierwsza linia skryptu ma postać #!/home/student/moj shell (gdzie po ! podaje się ścieżkę do programu shella) (12p). Dodatkowe opcje to:

- `(6p)` możliwość przekierowania standardowego wyjścia polecenia przy pomocy » 
- `(8p)` możliwość tworzenia potoków o dowolnej długości przy pomocy znaku | 
- `(8p)` historia poleceń - shell przechowuje (w zewnętrznym pliku w katalogu domowym użytkownika - tak ze historia powinna ”przetrwać” zakończenie shella) dokładna treść 20 poleceń, a wysłanie sygnału SIGQUIT powoduje wyświetlenie historii na standardowym wyjściu.

# Podsumowanie wymagan:
- Mozliwosc pisania prostych `skryptow`
- Uruchomienie polecenia w tle przy pomocy znaku `&`
- Zakonczenie czytania polecen przy pomocy znaku `konca pliku`
- Wykonywanie polecen przy pomocy `exec`
- Tworzenie `pipe` przy pomocy znaku `|` (prawdopodobnie wyzszy priorytet niz przekierowanie wyjscia)
- Przekierowanie standardowego wejscia przy pomocy operatora `>>`
- Historia ostatnich `20` polecen przy pomocy przerwania `SIGQUIT` (prawdopodobnie trzeba pisac do jakiegos dodatkowego pliku)
- Uruchomienie shella bez argumentow powoduje wczytywanie ze standardowego wejscia

# Uruchomienie programu:
1. Upewnic sie, czy ma sie zainstalowanego `make`-a (mozna tez skorzystac z `gcc`)
1. Upewnic sie, czy w tym samym katalogu sa obecne pliki `makefile` i `main.c`
1. W tym katalogu wykonac z `konsoli` polecenie `make` (utworzony zostanie wariant domyslny programu)
1. Utworzony zostanie plik `a.out`
1. Wykonac program `a.out`

# Co udalo sie zrobic:
- Polecenia sa parsowane, a pozniej wykonywane przy pomocy `execvp`
- Zrobione jest tworzenie potokow przy pomocy znaku `pipe |` (niemniej z tego co przeczytalem, to w normalnym bash-u wszystkie komendy z danego ciagu (`pipe`) sa wykonywane jednoczesnie - w przypadku mojego programu podpolecenia sa wykonywane sekwencyjnie)
- Czysto teoretycznie zrobione jest wykonywanie polecen "w tle" poprzez podanie znaku `&` na koncu linii polecenia (zrobilem to jednak w taki sposob, ze jezeli polecenia wykonywane "w tle" cos wypisuja, to widac to w konsoli - mam nadzieje, ze ta funkcja dziala poprawnie)
- Jest dzialajaca historia polecen (przy pomocy skrotu klawiszowego `ctrl + \` - przerwanie `SIGQUIT`), ktora przechowuje tresc 20 ostatnich polecen. 
- Jest dzialajace wykonywanie prostych skryptow (dla nich historia nie jest zapisywana - bo ma to sens)
- Jest zakonczenie czytania z pliku jezeli jest znak `EOF` (w przypadku czytania z linii polecen zdarza sie, ze trzeba wiecej niz raz podac EOF, zeby wykonywanie programu sie zakonczylo)

# Czego nie zrobilem:
- Przekierowywania wyjscia z polecenia do pliku przy pomocy `>>`

# Co dodatkowo (ciekawego) udalo sie zrobic:
- Funkcja pomocnicza, do dodawania kolejnego znaku do zniennej `char*` - czyli powstala funkcjonalnosc `dynamicznej` zmiennej `char*`
- Funkcja pomocnicza, do dodawania kolejnego elementu do tablicy `char**`. Jezeli tablica `char**` jest pelna, to jest reallokowana na rozmiar `2 razy` wiekszy i nowy element jest dodawany na pierwszym wolnym miejscu
- Zrobilem testy programu pod wzgledem `wyciekow pamieci` i po naprawieniu usterek nie ma problemow z `wyciekami pamieci` (przynajmniej podczas testowania nie bylo takich oznak)
- Sprawdzana jest zmienna `HOME` i w tej lokalizacji w pliku o nazwie `.microshell_history` (nazwe te mozna zmienic w pliku `main.c` na gorze) jest zapisywana historia polecen
- Dodalem tez obsluge `cudzyslowow "` w poleceniach. Ponadto cudzyslowy sa zapisywane w historii, ale juz do wykonywania polecenia pozbywam sie ich. Wiec polecenie np. wykonanie polecenia `echo "test"` spowoduje wypisanie w konsoli `test` (a nie "test"), a z kolei w historii bedzie widoczny wpis `echo "test"`; Dodatkowo obsluga `cudzyslowow "` pozwala na przekazywanie do polecen agrumentow "wielo-slowowych", np. `cat script | grep "| gr"` i argumentow zawierajacych znak `pipe |`. Niemniej nie mozna w takich tekstach dodawac cudzyslowow (nie ma obslugi `escape-owania` znakow `"` przy pomocy `\"`)

# Co mozna by bylo jeszcze dodac, zeby projekt mial jeszcze wiecej funkcjonalnosci:
- Mozliwosc przegladania historii przy pomocy strzalek
- Mozliwosc przesuwania kursora w ramach pisanego polecenia (obecnie trzeba zkasowac to co sie napisalo, zeby wrocic do miejsca, w ktorym trzeba dokonac pooprawy)
- Mozliwosc `esape-owania` znaku `cudzyslowu "`