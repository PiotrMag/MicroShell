# Prawdopodobna kolejnosc krokow:
1. Wczytanie tresci polecenia do wykonania
1. Logiczne rozdzielenie elementow polecenia poprzez znak spacji
1. Wykrycie, czy ostatni element to znak `&`. W takim przypadku trzba uruchamiac polecenie jako service (`??` - tu trzeba przemyslec jak to by mialo dzialac)
1. Wykrycie wszystkich znakow `|` w celu tworzenia potokow (w celu przekierowywania wyjscie z jednego prorgramu do drugiego)
1. Wykrycie wszystkich znakow `>>` w celu tworzenia przekierowania wyjscia z programu do pliku
1. Jezeli jest taka potrzeba to nalezy utworzyc deskryptory do zrobienia `pipe`. Jezeli w poprzednim poleceniu byl poerator `|`, to nalezy przekierowac standardowe wejscie nowego polecenia na odpowiedni deskryptor pliku. Jezeli po poleceniu jest operator `>>` to nalezy przekierowac wyjscie do pliku (jezeli pliku nie istnieje, to wyswietlic komunikat o bledzie i **zakonczyc wykonywanie**). Jezeli po poleceniu jest operator `|` to nalezy przekierowac standardowe wyjscie do deskryptora, ktory bedzie mozna uzyc w nastepnym poleceniu
1. Zrobic `fork`
1. Proba uruchomienia polecenia przy pomocy `exec`, jezeli jest blad, to wyswietlana jest o tym informacja i **dalsze wykonywanie jest przerywane**

# Pytania, na ktore warto znalezc odpowiedz:
- Jaka jest kolejnosc wykonywania operatorow `>>` oraz `|`
- Czy w C jest biblioteka do latwego parsowania `string`
    - Mozna skorzystac z `strtok` w celu rozdzielenia polecenia a poszczegolne skladowe
- Jak zrobic dynamiczna liste `string`
    - `realloc`
