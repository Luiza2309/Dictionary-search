INSERARE

- adauga in lista mare;
- daca lista e nula, creeaza un element cu lungimea cuvantului dat si lista mica care contine cuvantul dat si nr de aparitii 1;
- daca lungimea este mai mica decat cea a headului va crea o celula cu lungimea si lista si va schimba headul;
- daca lungimea se afla in head, va cauta daca cuvantul dat exista;
	- daca da: acesta va fi sters si reintrodus cu nr de aparitii marit cu 1;
	- daca nu: cuvantul va fi adaugat in lista cu nr de aparitii 1;
- daca lungimea trebuie adaugata dupa head se creeaza o celula si se adauga;
- daca nu s-a intrat in niciun caz de pana acum, se va parcurge lista mare pana cand elementul urmator contine o lungime mai mare sau egala cu lungimea cuvantului dat;
	- daca exista next, se uita daca elementul urmator are lungimea cautata si adauga cuvantul in functie daca exista sau nu deja in lista;
	- daca nu exista next sau urmatorul element nu are lungimea cautata, se creeaza o celula si se adauga in pozitia data de while;


ADDVALUEINORDER

- adauga in lista mica;
- daca lista e goala, se adauga cuvantul in head;
- daca trebuie adaugat inainte de head descrescator dupa aparitii si alfabetic;
- daca nu a intrat in niciun caz, se parcurge lista cat timp celula contine:
	- fie aparitii mai mari;
	- fie aparitii egale, dar un cuvant mai mic lexicografic;
- se adauga cuvantul pe pozitia indicata de while;


PRINTTABELA

- se parcurge vectorul, afisandu-se "pos i" la fiecare pas;
- se parcurge lista de pe pozitia i din vector, afisandu-se lungimea;
- se parcurge lista mica, afisandu-se cuvantul si aparitia;


PRINTLITERALUNGIME

- parcurge lista indicata de litera data si afiseaza listele de cuvinte care au lungimea data;


PRINTAPARITIE

- parcurge vectorul;
- parcurge lista mare;
- parcurge lista mica;
- daca cuvantul are nr de aparitii dorit, il salveaza in str;
- daca s-a gasit macar un cuvant de lungimea la care s-a ajuns, se salveaza in buffer lungimea si cuvintele care respecta;
- daca s-a gasit macar un cuvant in litera la care s-a ajuns din vector, se afiseaza "pos i" si bufferul;


                          = Tema 1 SD HashTable =

[in0.txt]...........................................passed  [05/85]
[VALGRIND: in0.txt].................................failed  [ 0/85]
[in1.txt]...........................................passed  [05/85]
[VALGRIND: in1.txt].................................failed  [ 0/85]
[in2.txt]...........................................passed  [05/85]
[VALGRIND: in2.txt].................................failed  [ 0/85]
[in3.txt]...........................................passed  [10/85]
[VALGRIND: in3.txt].................................failed  [ 0/85]
[in4.txt]...........................................passed  [10/85]
[VALGRIND: in4.txt].................................failed  [ 0/85]
[in5.txt]...........................................passed  [10/85]
[VALGRIND: in5.txt].................................failed  [ 0/85]
[in6.txt]...........................................passed  [10/85]
[VALGRIND: in6.txt].................................failed  [ 0/85]
[in7.txt]...........................................passed  [10/85]
[VALGRIND: in7.txt].................................failed  [ 0/85]
[in8.txt]...........................................passed  [10/85]
[VALGRIND: in8.txt].................................failed  [ 0/85]
[in9.txt]...........................................passed  [10/85]
[VALGRIND: in9.txt].................................failed  [ 0/85]

                                           Total: [85/85]