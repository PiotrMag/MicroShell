# Tresc zadania:

### Prosta powłoka tekstowa (shell):

- `(12p)` Powłoka pobiera ze standardowego wejścia pojedynczy wiersz. Następnie dokonuje prostej analizy wiersza dzieląc go na słowa separowane spacjami. Pierwsze słowo jest nazwa programu który należy uruchomić (wykorzystując zmienną PATH), a pozostałe sa argumentami. UWAGA: procesy uruchamiać należy poprzez wywołania systemowe z rodziny exec (execl, execlp, execvp itd.) - inne rozwiązania (np. przekierowanie odpowiedzialności do systemowej powłoki) nie będą uznawane. Shell uruchamia program i standardowo czeka na zakończenie jego pracy, chyba ze ostatnim słowem jest znak & co powoduje uruchomienie programu w tle, jak w normalnym shellu bash. Shell kończy prace gdy otrzyma znak końca pliku. Dzięki temu możliwe jest przygotowanie prostych skryptów, które można uruchamiać z wiersza poleceń bash-a, jeżeli pierwsza linia skryptu ma postać #!/home/student/moj shell (gdzie po ! podaje się ścieżkę do programu shella) (12p). Dodatkowe opcje to:

- `(6p)` możliwość przekierowania standardowego wyjścia polecenia przy pomocy » 
- `(8p)` możliwość tworzenia potoków o dowolnej długości przy pomocy znaku | 
- `(8p)` historia poleceń - shell przechowuje (w zewnętrznym pliku w katalogu domowym użytkownika - tak ze historia powinna ”przetrwać” zakończenie shella) dokładna treść 20 poleceń, a wysłanie sygnału SIGQUIT powoduje wyświetlenie historii na standardowym wyjściu.

---
# Podsumowanie wymagan:
- Mozliwosc pisania prostych `skryptow`
- Uruchomienie polecenia w tle przy pomocy znaku `&`
- Zakonczenie czytania polecen przy pomocy znaku `konca pliku`
- Wykonywanie polecen przy pomocy `exec`
- Tworzenie `pipe` przy pomocy znaku `|` (prawdopodobnie wyzszy priorytet niz przekierowanie wyjscia)
- Przekierowanie standardowego wejscia przy pomocy operatora `>>`
- Historia ostatnich `20` polecen przy pomocy przerwania `SIGQUIT` (prawdopodobnie trzeba pisac do jakiegos dodatkowego pliku)
- Uruchomienie shella bez argumentow powoduje wczytywanie ze standardowego wejscia

---
# Prawdopodobna kolejnosc krokow:
1. Wczytanie tresci polecenia do wykonania
1. Logiczne rozdzielenie elementow polecenia poprzez znak spacji
1. Wykrycie, czy ostatni element to znak `&`. W takim przypadku trzba uruchamiac polecenie jako service (`??` - tu trzeba przemyslec jak to by mialo dzialac)
1. Wykrycie wszystkich znakow `|` w celu tworzenia potokow (w celu przekierowywania wyjscie z jednego prorgramu do drugiego)
1. Wykrycie wszystkich znakow `>>` w celu tworzenia przekierowania wyjscia z programu do pliku
1. Jezeli jest taka potrzeba to nalezy utworzyc deskryptory do zrobienia `pipe`. Jezeli w poprzednim poleceniu byl poerator `|`, to nalezy przekierowac standardowe wejscie nowego polecenia na odpowiedni deskryptor pliku. Jezeli po poleceniu jest operator `>>` to nalezy przekierowac wyjscie do pliku (jezeli pliku nie istnieje, to wyswietlic komunikat o bledzie i **zakonczyc wykonywanie**). Jezeli po poleceniu jest operator `|` to nalezy przekierowac standardowe wyjscie do deskryptora, ktory bedzie mozna uzyc w nastepnym poleceniu
1. Zrobic `fork`
1. Proba uruchomienia polecenia przy pomocy `exec`, jezeli jest blad, to wyswietlana jest o tym informacja i **dalsze wykonywanie jest przerywane**

---
# Pytania, na ktore warto znalezc odpowiedz:
- Jaka jest kolejnosc wykonywania operatorow `>>` oraz `|`
- Czy w C jest biblioteka do latwego parsowania `string` 