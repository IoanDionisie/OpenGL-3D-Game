////////********************************* Tema 1 - EGC ***********************************////////
////////********************** Ciuciu Ioan-Dionisie   333 CA *****************************////////
//////////////////////////////////////////////////////////////////////////////////////////////////


__ ______________________
 1| Utilizare Aplicatie:
__| ---------------------

	Aplicatia creata, Build and survive, are urmatoarele functionalitati:
		
		- Apasare click dreapta + W / A / S / D - miscarea camerei third-person, si a player-ului
		in acelasi timp, pe cele 4 directii ( stanga, dreapta, inainte, inapoi)
		- Apasare click dreapta + orientare mouse stanga / dreapta - orientarea camerei third-person
		- right-click : player-ul va lansa un proiectil, care se deplaseaza in fata acestuia.
		- buton 1: schimbarea armei in pistol
		- buton 2: schimbarea armei in rifle gun
		- buton 3: schimbarea armei in grenade launcher
		
__ ________________________
 2| Detalli de implementare:
__| -----------------------

	Pentru implementarea aplicatiei, am folosit o clasa generica GameObject, mostenita de clasele
Enemy, Player, Tower si Bullet. Aceste obiecte au fost instantiate in clasa Laborator5, mentionand
ca pentru rezolvarea temei am folosit framework-ul de laborator. 
	Pentru inamici, si proiectile, folosesc vectori, in care le introduc dupa ce le initializez
proprietatile ( cum ar fi viteza, coordonate x si y, unghi de rotatie), pe masura ce am nevoie
de un astfel de element.
	Pentru randarea obiectelor am folosit obiecte .obj descarcate de pe internet, pentru turnuri,
player, inamic, si glont.
	Pentru implementarea camerei third-person, am folosit chiar varianta din laborator, astfel 
in structura de clase am si LabCamera.h
	Un inamic este creat o data la 5 secunde, implementand acest mecanism folosind niste variabile
care retin timpul de cand a inceput jocul, si timpul la care trebuie spawnat un inamic. 
	Pentru implementarea traseului inamicilor, am folosit 3 variabile, care imi specifica momentul
cand un inamic trebuie sa isi schimbe directia si unghiul de rotatie, swap1, swap2 si swap3. Cand
inamicii ajung la finalul traseului, ei sunt distrusi, iar player-ul pierde o viata.
	Coliziunile au fost implementate aproximativ ca la tema 1, doar ca in formula de coliziune cerc-
cerc am adaugat si coordonata Z. 
	Pentru ca gloantele care au ratat inamicii sa nu ramana incarcate si sa ingreuneze jocul, am
stabilit un range al jocului din care daca un glont iese, este distrus. 
	Turnurile au o pozitie statica, nu le-am implementat rotirea si actiunea de a trage cu gloante.
	Pistolul a fost implementat pentru a trage pe o distanta mica, comparand mereu distanta dintre
pozitia unde a fost tras glontul, si pozitia curenta a glontului. Daca aceasta depaseste o anumita
valoare, este distrus. De asemenea, pentru a trage incet, am folosit doua variabile care retin 
timpul cand a fost tras ultimul glont de pistol, si timpul curent, iar daca diferenta dintre cele
doua este mai mica decat o anumita valoare, atunci actiunea click-stanga nu va avea efect. 
	Pusca, a fost implementata sa traga mai repede, si cu gloante mai mari, la distanta mai mare,
dar cu putere mai mica.
	Pentru grenade launcher, tragerea cu bolta a fost facuta crescand Y-ul pana la o anumita va-
loare, apoi scazandu-l, pana atinge planul si este distrus, sau intra in coliziune cu un inamic
si e distrus mai devreme.
	Animatia de cadere a playerului si a inamicilor, este facuta folosind variabile pe care le
incrementez cu deltaTimeSeconds, atunci cand e momentul ca inamicul / playerul sa moara. Astfel,
totul este facut folosind o rotatie pe axa OX, cu aceasta variabila, pana cand variabila atinge 
o anumita valoare, deoarece vreau ca rotatia sa fie de doar 90 de grade. Similar procedez si pen-
tru animatia de scalare spre 0 a inamicilor, cand mor.

__ __________________
 4| Probleme aparute:
__| -----------------

	Nu cred ca am intampinat probleme notabile in implementarea temei, deoarece nu am abordat 
	punctele mai grele.

__ _____________________________________
 5| Status implementare functionalitati:
__| ------------------------------------

	Din punctele temei, am implementat:
	
		- Crearea unui character 3d care este mereu orientat cu spatele catre o camera 
		third-person
		- Crearea inamicilor care parcurg traseul si sunt orientati spre directia in care merg 
		- Armele userului, cu exceptia functionalitatii pustii de a schimba camera atunci cand
		este selectata
		- Turnurile, doar statice, fara rotiri si fara a trage catre adversar
		- Mecanismul prin care proiectilele trase de userscad viata inamicilor 
		(detectia de coliziuni)
		- Animatie de distrugere a inamicilor, si a player-ului atunci cand jocul e pierdut

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////