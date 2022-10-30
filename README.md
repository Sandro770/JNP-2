# TODO
1) Implementacja
2) Testowanie poprawnościowe
3) Kompilacja C i C++ testowanie
4) Code review
5) zwrócić uwagę na kolejność tworzeń np. stałych globalnych, czy mają być static czy coś ?
6) dokumentacja
7) format kodu 

# Pytania:
1) a)jak ustalić typ hash_function_t, jako typedef, using czy template function<> 
   b) pytanie, kiedy będzie przy okazji sprawdzany typ, czy np. pierwszy argument nie jest stringiem
   ad b) o jaką sytuację Ci chodzi? W której funkcji?
[ODP [1]]: użyłem "using hash_function_t = uint64_t(*)(uint64_t*, size_t);" to rozwiązuje ten podpunkt

2) co to w ogóle jest obiekt haszujący
[ODP [2]]: obiekt klasy Hash 

3) Jak dokładnie działa przechowywanie elementów w kontenerach? Jak dokładnie działa set?
