# Image - Compression

Imaginea citita din fisier este memorata cu ajutorul unei matrice, a caror
elemente reprezinta un tip de date de tip struct pixel, ce retine valori pentru
fiecare dintre culorile: rosu, verde si albastru. Construirea arborelui de
compresie se realizeaza in functia insertNode care parcurge recursiv aceasta
matrice, prin impartirea in cate 4 blocuri egale pana cand pixelii dintr-un
asemenea bloc au o culoare similara. In acest caz, blocul respectiv este
reprezentat in arborele cuaternar drept nod frunza, iar apelul recursiv
se opreste. In caz contrar, blocul este reprezentat drept nod intern, urmand
sa fie divizat in continuare. In implementare am presupus ca fiecare matrice se
va diviza cel putin o data, prin urmare am apelat aceasta functie cu un arbore
initializat cu un nod intern, asadar acest caz este tratat separat, iar atat
tipul, cat si culorile nodului initial sunt actualizate corespunzator in cazul
in care compresia imaginii se poate realiza cu ajutorul unui singur nod de tip
frunza. Calculul factorului de similaritate este efectuat in concordanta cu
formulele furnizate in enuntul temei, iar verificarea sa se realizeaza prin
compararea cu factorul primit drept argument. Tot in aceasta functie se
calculeaza dimensiunea laturii patratului pentru cea mai mare zona din imagine
care a ramas nedivizata, prin actualizare, dupa caz, cu dimensiunea blocului
curent.
	Avand arborele format, pentru aflarea numarului de niveluri se utilizeaza
functia height care realizeaza parcurgerea in adancime a fiecarui subarbore si
returneaza, in final, adancimea maxima. Folosind aceeasi logica, cum numarul de
blocuri pentru care scorul similaritatii pixelilor este mai mic sau egal decat
factorul furnizat corespunde cu numarul de frunze din arbore, acesta este
calculat tot prin parcurgerea fiecarui subarbore si adunarea fiecarei frunze
intalnite, lucru care se realizeaza prin apelarea functiei countLeaves.

Generarea fisierului de compresie se realizeaza prin parcurgerea pe
nivel a arborelui, invatata la curs. In functia printLevelOrder se adauga
intr-o coada (implementata ca o lista simplu inlantuita) radacina arborelui,
ca mai apoi, dupa extragerea ei, sa fie adaugati toti copiii acesteia. La
extragerea radicinii, se afiseaza in fisierul de compresie informatiile
necesare despre aceasta, conform celei de a doua cerinte, in functie de tip.
Se adauga apoi in coada toti copiii, iar acest algoritm se repeta pana la
ultimul nod din arbore, etragerea si prelucrarea acestuia ducand la golirea
cozii.

La final, se elibereaza memoria alocata.
