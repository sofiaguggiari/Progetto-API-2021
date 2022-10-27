#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <limits.h>

struct node{
    long lunghezza;
    int numGrafo;
    struct node *next;
    struct node *prev;
};

struct node *testa;
struct node *coda;

int lunghezzaLista;

long myAtoiSpazio(char const *str) {
    long num = 0;
    for(int i = 0; str[i] != ' '; ++i)
        num = num * 10 + str[i] - '0';
    return num;
}

long myAtoiVirgola(char const *str) {
    long num = 0;
    for(int i = 0; str[i] != ','; ++i)
        num = num * 10 + str[i] - '0';
    return num;
}

long myAtoiFineRiga(char const *str) {
    long num = 0;
    for(int i = 0; str[i] != '\n'; ++i)
        num = num * 10 + str[i] - '0';
    return num;
}

void aggiungiTesta(long lun, int num) {
    struct node* nuovoNodo = malloc(sizeof(struct node));

    nuovoNodo->numGrafo = num;
    nuovoNodo->lunghezza = lun;
    nuovoNodo->prev = NULL;

    if(testa == NULL)
        nuovoNodo->next = NULL;
    else {
        nuovoNodo->next = testa;
        testa->prev = nuovoNodo;
    }

    if(coda == NULL)
        coda = nuovoNodo;

    testa = nuovoNodo;
}

void aggiungiCoda(long lun, int num) {
    struct node* nuovoNodo = malloc(sizeof(struct node));

    nuovoNodo->numGrafo = num;
    nuovoNodo->lunghezza = lun;
    nuovoNodo->prev = coda;
    coda->next = nuovoNodo;
    nuovoNodo->next = NULL;

    coda = nuovoNodo;
}

struct node* trovaPosizione(long lun) {
    struct node* current = testa;

    if(lun < testa->lunghezza)
        return current;

    while(current->next != NULL) {
        if(lun >= current->next->lunghezza)
            current = current->next;
        else
            break;
    }

    if(current->lunghezza == lun) {
        while(current->next != NULL) {
            if(current->next->lunghezza == lun)
                current = current->next;
            else
                break;
        }
    }

    return current;
}

void rimuoviCoda() {
    struct node* current = coda->prev;
    current->next = NULL;
    free(coda);
    coda = NULL;
    coda = current;
}

void aggiungi(long lun, int num, int k) {
    if(k == 0)          //top k = 0
        return;

    if(lunghezzaLista == 0) {               //non ho elementi nella top
        aggiungiTesta(lun, num);
        lunghezzaLista++;
        return;
    }

    if(coda != NULL) {
        if(lun >= coda->lunghezza && lunghezzaLista == k){    //ho già abbastanza elementi nella top
            return;
        } else if(lun >= coda->lunghezza) {
            aggiungiCoda(lun, num);
            lunghezzaLista++;
            return;
        }
    }

    if(testa != NULL) {
        if(lun < testa->lunghezza) {      //elemento più piccolo di quelli salvati nella top
            aggiungiTesta(lun, num);
            lunghezzaLista++;
            if(lunghezzaLista > k) {
                rimuoviCoda();
                lunghezzaLista--;
            }
            return;
        }
    }

    struct node* nodoPrecedente = trovaPosizione(lun);

    struct node* nuovoNodo = malloc(sizeof(struct node));
    nuovoNodo->numGrafo = num;
    nuovoNodo->lunghezza = lun;
    nuovoNodo->next = nodoPrecedente->next;
    nuovoNodo->prev = nodoPrecedente;
    nodoPrecedente->next->prev = nuovoNodo;
    nodoPrecedente->next = nuovoNodo;

    lunghezzaLista++;

    if(lunghezzaLista > k) {
        rimuoviCoda();
        lunghezzaLista--;
    }
}

void printList(long lunghezzaTop) {
    if(lunghezzaLista == 0 || lunghezzaTop ==  0) {
        printf("\n");
        return;
    }

    struct node *tmp = testa;
    while(tmp != coda) {
        printf("%d ", tmp->numGrafo);
        tmp = tmp->next;
    }
    printf("%d\n", tmp->numGrafo);
}

int distanzaMinima(const long dist[], long const array[], long nNodi) {
    long min = LONG_MAX;
    long indiceMinimo = 0;

    for (int i = 0; i < nNodi; i++) {
        if (array[i] == 0 && dist[i] <= min) {
            min = dist[i];
            indiceMinimo = i;
        }
    }

    return indiceMinimo;
}

int lunghezzaNumero(long num) {
    if (num >= 1000000000)
        return 10;
    if (num >= 100000000)
        return 9;
    if (num >= 10000000)
        return 8;
    if (num >= 1000000)
        return 7;
    if (num >= 100000)
        return 6;
    if (num >= 10000)
        return 5;
    if (num >= 1000)
        return 4;
    if (num >= 100)
        return 3;
    if (num >= 10)
        return 2;
    return 1;
}

int dijkstra(long numero, long *array, char *input, long *distanza) {
    long lunghezza = 0;
    int contatore = 0;
    int max = 9000;
    int fixer = 0;

    if(fgets(input, max, stdin) == NULL)            //leggo la prima riga della matrice del grafo
        return -1;

    for(int i = 0; i<numero; i++) {
        if(i != numero-1)
            array[i+numero] = myAtoiVirgola(input);     //converto i numeri e li salvo in un array
        else
            array[i+numero] = myAtoiFineRiga(input);
        if(i != numero-1) {         //vado avanti col puntatore per tutti gli elementi tranne l'ultimo
            input = input + 1 + lunghezzaNumero(array[i+numero]);
            fixer = fixer + 1 + lunghezzaNumero(array[i+numero]);
        }
        if(array[i+numero] == 0 && i!=0)       //se ho 0 lo salvo in un contatore
            contatore++;
        array[i] = 0;
        distanza[i] = LONG_MAX;
    }

    if(contatore == numero-1) {           //se la prima riga della matrice di incidenza contiene tutti 0, il nodo 0 è staccato dagli altri --> lunghezza = 0
        for(int i = 1; i < numero; i++) {
            if(fgets(input, max, stdin) == NULL)
                return -1;
        }
        return 0;
    }

    for(int i=numero; i<numero*numero; i++) {       //leggo le altre righe e salvo
        if(i%numero == 0) {
            input = input - fixer;
            fixer = 0;
            if(fgets(input, max, stdin) == NULL)
                return -1;
        }
        if((i+1)%numero != 0)
            array[i+numero] = myAtoiVirgola(input);     //converto i numeri e li salvo in un arra
        else
            array[i+numero] = myAtoiFineRiga(input);
        if((i+1)%numero != 0) {
            input = input + 1 + lunghezzaNumero(array[i+numero]);
            fixer = fixer + 1 + lunghezzaNumero(array[i+numero]);
        }
    }

    distanza[0] = 0;

    for(int i = 0; i <numero-1; i++) {          //applico dijkstra
        int var = distanzaMinima(distanza, array, numero);
        if(var < numero) {
            array[var] = 1;
        }

        if(distanza[var] == LONG_MAX)
            break;

        for (int j = 0; j < numero; j++) {
            long alt = distanza[var] + array[var * numero + j + numero];
            //controllo di non aver già visitato il nodo, che esista un arco che colleghi i nodi var e j e che
            //la distanza tra var e j sia minore di quella finora considerata
            if (array[j] == 0 && array[var * numero + j + numero] != 0 && alt < distanza[j]) {
                distanza[j] = alt;
            }
        }
    }

    for(int k = 0; k<numero; k++) {         //sommo i cammini minimi
        if(distanza[k] == LONG_MAX)         //convenzione data
            distanza[k] = 0;
        lunghezza = lunghezza + distanza[k];
    }

    return lunghezza;
}

int main() {
    char *input = malloc(100);
    int contatore = 0;          //tiene conto del numero dei grafi aggiunti finora
    if(fgets(input, 100, stdin) == NULL)            //leggo la prima riga: n (numero nodi), k (elementi classifica)
        return 0;

    long numero = myAtoiSpazio(input);
    input = input + 1 + lunghezzaNumero(numero);
    long k = myAtoiFineRiga(input);

    input = input - 1 - lunghezzaNumero(numero);
    free(input);

    char *string= malloc(5000);

    long *matrix = malloc(sizeof(long)*numero*numero + sizeof(long)*numero);
    char *lines = malloc(9000);
    long *distanza = malloc(sizeof(long)*numero);
    testa = NULL;
    coda = NULL;
    lunghezzaLista = 0;

    while(fgets(string, 5000, stdin) != NULL) {       //continuo a leggere righe finchè ce ne sono
        if(strchr(string, 'A') != NULL) {
            long lunghezza = dijkstra(numero, matrix, lines, distanza);
            aggiungi(lunghezza, contatore, k);
            contatore++;
        } else if(strchr(string, 'T') != NULL) {
            printList(k);
        }
    }
    return 0;
}
