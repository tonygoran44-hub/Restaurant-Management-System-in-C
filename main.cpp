#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 Structura comanda reprezinta un nod al unei liste simplu inlantuite.
 Fiecare nod contine datele unei comenzi si un pointer catre urmatoarea comanda.
*/
typedef struct comanda {
    int idComanda;
    int timpPreparare;
    char numeClient[50];
    char felMancare[50];
    struct comanda *next;   // pointer catre urmatoarea comanda din coada
} comanda;

/*
 front  -> primul element din coada (urmatoarea comanda de servit)
 rear   -> ultimul element din coada (ultima comanda adaugata)
*/
comanda *front = NULL, *rear = NULL;

int cerere, id;
int p = 0, u = 0;          // p = numar comenzi servite, u = numar comenzi in coada
int timpTotal = 0;

void startMenu();
comanda* citesteComanda();
void asteapta();
void adaugaComanda();
void servireComanda();
void afisareComandaCurenta();
void afisareToateComenzile();
void afisareRec(comanda* nod);
void Statistici();
void AnulareComanda(int id);

int main()
{
    startMenu();
    scanf("%d", &cerere);

    while (cerere != 0)
    {
        switch (cerere)
        {
            case 1: adaugaComanda(); break;
            case 2: servireComanda(); break;
            case 3: afisareComandaCurenta(); break;
            case 4: afisareToateComenzile(); break;
            case 5: Statistici(); break;
            case 6:
                printf("Specifica id-ul comenzii care urmeaza a fi stearsa: ");
                scanf("%d", &id);
                AnulareComanda(id);
                break;
            default:
                printf("Nu am inteles cererea!\n");
                asteapta();
        }

        system("cls");   /* pe Linux/Mac -> system("clear"); */
        startMenu();
        scanf("%d", &cerere);
    }
    return 0;
}

void startMenu()
{
    printf("1 - Adauga comanda\n");
    printf("2 - Servire urmatoarea comanda\n");
    printf("3 - Afisare comanda curenta\n");
    printf("4 - Afisarea tuturor comenzilor\n");
    printf("5 - Statistici pentru comenzi\n");
    printf("6 - Stergere comanda\n");
    printf("0 - Terminare program\n");
}

comanda* citesteComanda()
{
    /*
     Alocare dinamica pentru o noua comanda.
     Memoria va fi eliberata ulterior la servire sau anulare.
    */
    comanda *c = (comanda*)malloc(sizeof(comanda));

    printf("id comanda: ");
    scanf("%d", &c->idComanda);

    printf("timp de preparare: ");
    scanf("%d", &c->timpPreparare);

    printf("numele clientului: ");
    scanf("%s", c->numeClient);

    printf("felul de mancare: ");
    scanf("%s", c->felMancare);

    c->next = NULL;   // noua comanda va fi ultimul element din coada
    return c;
}

void asteapta()
{
    char s[10];
    do {
        printf("\nScrie \"ok\" pentru a continua:\n");
        scanf("%s", s);
    } while (strcmp(s, "ok") != 0);
}

void adaugaComanda()
{
    comanda *c = citesteComanda();

    /*
     Daca coada este vida, front si rear vor indica acelasi nod.
     Altfel, noua comanda este adaugata la finalul cozii.
    */
    if (rear == NULL)
        front = rear = c;
    else
    {
        rear->next = c;   // legam ultima comanda de cea noua
        rear = c;         // actualizam rear
    }

    timpTotal += c->timpPreparare;
    u++;
    asteapta();
}

void servireComanda()
{
    /*
     Daca front este NULL, coada este goala si nu exista comenzi de servit.
    */
    if (front == NULL)
    {
        printf("Nu exista comenzi!\n");
        asteapta();
        return;
    }

    comanda *cCurenta = front;

    printf("Comanda %d pentru %s a fost servita.\n",
           cCurenta->idComanda, cCurenta->numeClient);

    /*
     Eliminarea primului element din coada:
     front avanseaza la urmatorul nod.
    */
    front = front->next;

    /*
     Daca dupa eliminare coada devine vida,
     rear trebuie resetat la NULL.
    */
    if (front == NULL)
        rear = NULL;

    free(cCurenta);   // eliberam memoria comenzii servite
    p++;
    u--;
    asteapta();
}

void afisareComandaCurenta()
{
    if (front == NULL)
        printf("Nu exista comenzi in asteptare.\n");
    else
    {
        printf("Comanda curenta:\n");
        printf("id: %d\n", front->idComanda);
        printf("timp de preparare: %d\n", front->timpPreparare);
        printf("client: %s\n", front->numeClient);
        printf("fel de mancare: %s\n", front->felMancare);
    }
    asteapta();
}

void afisareToateComenzile()
{
    /*
     Afisarea tuturor comenzilor se face recursiv,
     pornind de la primul nod al cozii.
    */
    if (front == NULL)
        printf("Coada este goala.\n");
    else
        afisareRec(front);

    asteapta();
}

void afisareRec(comanda *nod)
{
    /*
     Cazul de oprire al recursivitatii:
     daca nodul este NULL, lista a fost parcursa complet.
    */
    if (nod == NULL)
        return;

    printf("Comanda %d: %s pentru %s\n",
           nod->idComanda, nod->felMancare, nod->numeClient);

    // apel recursiv pentru urmatorul nod din lista
    afisareRec(nod->next);
}

void Statistici()
{
    printf("Numar comenzi servite: %d\n", p);
    printf("Numar comenzi in coada: %d\n", u);
    printf("Timp total de preparare: %d\n", timpTotal);
    asteapta();
}

void AnulareComanda(int id)
{
    /*
     cur  -> nodul curent parcurs
     prev -> nodul anterior, necesar pentru refacerea legaturilor
    */
    comanda *cur = front, *prev = NULL;

    // cautam comanda cu id-ul specificat
    while (cur && cur->idComanda != id)
    {
        prev = cur;
        cur = cur->next;
    }

    if (cur == NULL)
        printf("Nu exista comanda cu id %d\n", id);
    else
    {
        /*
         Daca se sterge primul nod, mutam front.
         Altfel, sarim peste nodul curent.
        */
        if (cur == front)
            front = front->next;
        else
            prev->next = cur->next;

        /*
         Daca se sterge ultimul nod,
         trebuie actualizat pointerul rear.
        */
        if (cur == rear)
            rear = prev;

        free(cur);   // eliberam memoria comenzii anulate
        u--;
        printf("Comanda a fost anulata.\n");
    }
    asteapta();
}
