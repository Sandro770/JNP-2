# TODO
1) Implementacja
2) Testowanie poprawnościowe
3) Kompilacja C i C++ testowanie
4) Code review
5) zwrócić uwagę na kolejność tworzeń np. stałych globalnych, czy mają być static czy coś ?
6) dokumentacja
7) format kodu 
8) przetestować na studentsie testy 


# Pytania:
1) a)jak ustalić typ hash_function_t, jako typedef, using czy template function<> 
   b) pytanie, kiedy będzie przy okazji sprawdzany typ, czy np. pierwszy argument nie jest stringiem
   ad b) o jaką sytuację Ci chodzi? W której funkcji?
[ODP [1]]: użyłem "using hash_function_t = uint64_t(*)(uint64_t*, size_t);" to rozwiązuje ten podpunkt

2) co to w ogóle jest obiekt haszujący
[ODP [2]]: obiekt klasy Hash 

3) Jak dokładnie działa przechowywanie elementów w kontenerach? Jak dokładnie działa set?

4) Format testów rocznikowych jeśli chodzi o logi nie zgadza się z formatem zadań przykładowych, przykładowe są bardziej spójne. Trzeba zapytać prof. o co chodzi.

5) co z kopiowaniem całego vectora dla funkcji haszującej

6) co jeśli hash_function = NULL? co jeśli jest tryb ndebug a co jeśli nie, czy mam zatrzymać program, czy muszę używać assercji?

# Informacje z konsultacji:
1) format diagnostycznych nie musi mieć jakiegoś ścisłego formatu
2) Jak hash_create dostanie NULL'a to powinien zrobić coś najmniej złego

# TODO:
1) nieczytelny kod - niejasne funkcje - nie widać podziału na diagnostykę i program
   a) Ma być podział pod kątem przestrzeni nazw czy co?
3) oddzielne dać typedef dla kompilacji c, oddzielnie using dla kompilacji c++
4) dane diagnostyczne źle wypisywane, powinny być możliwe do zoptymalizowania jakieś consty, czy coś i też nie ma referencji więc tym bardziej źle 
   b) referencje tylko do danych diagnostycznych trzeba dodać? Co to znaczy jakieś consty?
6) niepotrzebnie zdublowany namespace jnp1 w pliku hash.cc
7) nie może być kopii vectora, musi być przez referencję 
8) inicjowanie structa przez Hash(): () {}, czyli poprzez listę inicjalizacyjną
   c) ??
10) zdublowany typedef w .h i .cc // chyba poprawione
11) stddef.h i inne nie muszą być koniecznie w iostream, więc trzeba je includować w kompilacji cpp też // chyba poprawione
12) ogólne poprawki stylistyczne
13) używać makr do danych diagnostycznych
   d) Nie do końca wiem jak to rozumieć
15) hash_function_id_t dokumentować, numberOfCreatedHashes dokumentować
