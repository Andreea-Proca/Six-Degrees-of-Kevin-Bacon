# Six-Degrees-of-Kevin-Bacon
 
Pentru toate cele 3 taskuri am folosit aceeasi citire, iar in plus pentru al doilea task am citit si
cele doua nume de actori pentru care trebuie determinat gradul de rudenie.
Pentru sortarea alfabetica am folosit functia "qsort" pentru care am implementat functia de comparare a doua siruri de caractere, "cmp_char".

Task 1
Pentru taskul 1 am implementat si folosit o parcurgere in adancime de tip recursiv care, in plus,
retine si numarul de elemente vizitate. 
Am apelat algoritmul plecand din fiecare nod si am retinut pe parcurs maximul de elemente vizitate
si nodul din care a pornit parcurgerea pentru a obtine acel numar.
Pentru a afisa actorii din acea componenta conexa, am apelat aceeasi functie plecand din nodul retinut.
Numele au fost retinute intr-un vector si sortate alfabetic folosind functia "qsort", dupa care salvate in fisier.

Task 2
Pentru taskul 2 am implementat si folosit o parcurgere in latime de tip iterativ care pornetse de la un nod sursa 
si se opreste la cel destinatie, ambele date.
Aceasta returneaza numarul minim de muchii dintre cele doua noduri, care este salvat in fisier in functia "main".
Pentru a determina care sunt nodurile sursa si destinatie citite ca numele actorilor, le-am cautat parcurgand lista de adiacenta.
Pentru parcurgerea in adancime am utilizat o structura de timp coada pe care am implementat-o alaturi de operatiile efectuate cu aceasta in fisierul "queue.c".

Task 3
Pentru taskul 3 am implementat algoritmul Tarjan folosind o parcurgere in adancime de tip recursiv si functia "punti" 
care initializeaza si apeleaza functia de parcurgere in adancime.
Am mai implementat si o functie "min" care determina minimul dintre doua valori date si o functie "save" 
care retine numele a doi actori dati in ordine alfabetica intr-un sir de caractere, intr-un vector de siruri de caractere dat ca parametru.
Apoi, vectorul ce contine sirurile de caractere este ordonat alfabetic folosind functia "qsort".
In fisier este salvat pe prima linie numarul de elemente al vectorului, apoi fiecare element al acestuia.
