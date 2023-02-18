/* GIUGLAN Luiza-Viorica 311CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

typedef struct _Node
{
    void *value;
    struct _Node *next;
} Node;

typedef struct _List
{
    struct _Node *head;
    unsigned int size;
} List;

typedef struct _Cuvant
{
    char *cuvant;
    int nr;
} Cuvant;

typedef struct _Lungime
{
    int lungime;
    struct _List *lista_cuvinte;
} Lungime;

typedef struct _HashTable
{
    int size;
    int (*hashCode)(void *);
    List **vector;
} HashTable;

List *CreateList()
{
    List *list = (List *)calloc(1, sizeof(List)); // aloca o lista
    if (list == NULL)
        return NULL; // daca alocarea a esuat
    list->head = NULL;
    list->size = 0;
    return list;
}

HashTable *CreateTable(int (*hashCode)(void *))
{
    HashTable *table = calloc(1, sizeof(HashTable)); // aloca o tabela
    if (table == NULL)
        return NULL;  // daca alocarea a esuat
    table->size = 26; // vectorul are 26 de elemente, corespunzator alfabetului
    table->hashCode = hashCode;

    table->vector = calloc(26, sizeof(List *)); // aloca vectorul
    if (table->vector == NULL)
    {
        free(table);
        return NULL;
    }

    for (int i = 0; i < 26; i++)
    {
        table->vector[i] = CreateList(); // creeaza lista pentru fiecare litera
        if (table->vector[i] == NULL)    // daca alocarea a esuat
        {
            for (int j = 0; j < i; j++)
                free(table->vector[j]);
            free(table->vector);
            free(table);
            return NULL;
        }
    }
    return table;
}

int fhash(void *x)
{
    char *cuv = (char *)x;
    if ((*cuv) >= 65 && (*cuv) <= 90)
        return (*cuv) - 'A';
    if ((*cuv) >= 97 && (*cuv) <= 122)
        return (*cuv) - 'a';
    return -1;
}

int comparareAlfabetica(void *x, void *y)
{
    Cuvant *a = (Cuvant *)x;
    Cuvant *b = (Cuvant *)y;
    return strcmp(a->cuvant, b->cuvant);
}

int comparareNumerica(void *x, void *y)
{
    Cuvant *a = (Cuvant *)x;
    Cuvant *b = (Cuvant *)y;
    if (a->nr > b->nr)
        return 1;
    if (a->nr == b->nr)
        return 0;
    return -1;
}

int comparareLungimi(void *x, void *y)
{
    Lungime *a = (Lungime *)x;
    Lungime *b = (Lungime *)y;
    if (a->lungime > b->lungime)
        return 1;
    else if (a->lungime == b->lungime)
        return 0;
    return -1;
}

Lungime *convertireLungime(void *lungime, void *lista)
{
    int *x = (int *)lungime;
    List *y = (List *)lista;
    Lungime *elem = calloc(1, sizeof(Lungime));
    elem->lungime = (*x);
    elem->lista_cuvinte = y;
    return elem;
}

Cuvant *convertireCuvant(void *cuvant, void *nr)
{
    int *x = (int *)nr;
    char *y = (char *)cuvant;
    Cuvant *cuv = calloc(1, sizeof(Cuvant));
    cuv->cuvant = calloc(100, sizeof(char));
    cuv->nr = (*x);
    strcpy(cuv->cuvant, y);
    return cuv;
}

int findCuvant(void *cuvant, List *list, int *pozitie,
  int (*comparareAlfabetica)(void *, void *))
{
    Node *n = list->head;
    
    // daca cuvantul se afla in head si nu exista next
    if (comparareAlfabetica(cuvant, n->value) == 0
      && list->head->next == NULL)
    {
        Cuvant *c = (Cuvant *)n->value;
        int nr = c->nr + 1;
        (*pozitie)++;
        list->head = NULL;
        return nr;
    }

    // daca cuvantul se afla in head si exista next
    if (comparareAlfabetica(list->head->value, cuvant) == 0)
    {
        Cuvant *c = (Cuvant *)list->head->value;
        int nr = c->nr + 1;
        list->head = list->head->next;
        return nr;
    }

    // parcurge lista si daca gaseste cuvantul il sterge si
    // intoarce nr de aparitii + 1 pentru a putea sa il adauge inapoi in lista
    while (n->next != NULL)
    {
        if (comparareAlfabetica(cuvant, n->next->value) == 0)
        {
            Cuvant *c = (Cuvant *)n->next->value;
            int nr = c->nr + 1;
            n->next = n->next->next;
            return nr;
        }
        n = n->next;
        (*pozitie)++;
    }
    return 0;
}

void addValueInOrder(void *value, List *list, 
  int (*comparareNumerica)(void *, void *),
  int (*comparareAlfabetica)(void *, void *))
{
    if (list->head == NULL) // daca lista e goala
    {
        list->head = calloc(1, sizeof(struct _Node));
        if (list->head == NULL)
            return;
        list->head->value = value;
        list->head->next = NULL;
        return;
    }
    else
    {
        // daca trebuie adaugat inaintea headului
        // daca cuvantul dat are nr de aparitii mai mare decat headul sau
        // daca cuvantul si headul au nr de aparitii egale, dar cuvantul dat
        // vine inainte lexicografic
        if (comparareNumerica(list->head->value, value) < 0 ||
          (comparareNumerica(list->head->value, value) == 0 &&
          comparareAlfabetica(list->head->value, value) > 0))
        {
            Node *aux = calloc(1, sizeof(struct _Node));
            aux->next = list->head;
            aux->value = value;
            list->head = aux;
            return;
        }
        // parcurg lista cat timp nr de aparitii al celulei e mai mare
        // decat cel al cuvantului sau nr de aparitii este egal, dar
        // cuvantul vine dupa lexicografic
        else
        {
            Node *n = list->head;
            while (n->next != NULL && 
              (comparareNumerica(n->next->value, value) > 0 ||
              (comparareNumerica(n->next->value, value) == 0 &&
              comparareAlfabetica(n->next->value, value) < 0)))
                n = n->next;
            Node *aux = calloc(1, sizeof(struct _Node));
            aux->next = n->next;
            aux->value = value;
            n->next = aux;
            return;
        }
    }
}

void inserare(HashTable *table, void *string,
  int (*comparareLungimi)(void *, void *),
  int (*comparareAlfabetica)(void *, void *),
  int (*comparareNumerica)(void *, void *))
{
    char *str = (char *)string;
    int lungime = strlen(str);
    if (lungime > 2)
    {
        int poz = table->hashCode(string);
        if (poz >= 0)
        {
            Node *n = table->vector[poz]->head;
            List *l = CreateList();
            Lungime *element = convertireLungime((void *)&lungime, (void *)l);
            // un element cu lungimea cuvantului primit si lista goala

            // LISTA GOALA
            
            // daca lista corespunzatoare literei e nula
            if (table->vector[poz]->head == NULL) 
            {
                table->vector[poz]->head = (Node *)calloc(1, sizeof(Node));
                table->vector[poz]->head->next = NULL;
                int nr = 1; // adauga cuvantul cu aparitia 1 in lista mica
                Cuvant *cuv = convertireCuvant(string, (void *)&nr);
                addValueInOrder((void *)cuv, l, comparareNumerica, 
                  comparareAlfabetica); // adauga in lista
                table->vector[poz]->head->value =
                  (void *)convertireLungime((void *)&lungime, (void *)l);
                  // salveaza elementul de tip Lungime in lista mare
                return;
            }

            // CAZURILE HEADULUI LISTEI MARI
            
            // trebuie adaugata o lungime mai mica decat cea a headului
            if (comparareLungimi(n->value, element) > 0) 
            {
                Node *aux = (Node *)calloc(1, sizeof(Node));
                if (aux == NULL)
                    return;
                int nr = 1;
                Cuvant *cuv = convertireCuvant(string, (void *)&nr);
                addValueInOrder((void *)cuv, l, comparareNumerica,
                  comparareAlfabetica);
                aux->value = (void *)convertireLungime((void *)&lungime,
                  (void *)l);
                aux->next = table->vector[poz]->head; // devine noul head
                table->vector[poz]->head = aux;
                return;
            }
            else
            {
                // trebuie adaugat in head
                if (comparareLungimi(n->value, element) == 0)
                {
                    Lungime *elem = (Lungime *)table->vector[poz]->head->value;
                    int nr_aux = 0;
                    Cuvant *c = convertireCuvant(string, (void *)&nr_aux);
                    int pozitie = 0;
                    int nr = findCuvant((void *)c, elem->lista_cuvinte,
                      &pozitie, comparareAlfabetica); 
                    // cauta cuvantul sa vada daca a mai fost adaugat
                    if (nr == 0) // nu exista cuvantul
                    {
                        nr = 1; // adauga cuvantul cu nr de aparitii 1
                        Cuvant *cuv = convertireCuvant(string, (void *)&nr);
                        addValueInOrder((void *)cuv, elem->lista_cuvinte,
                          comparareNumerica, comparareAlfabetica);
                        table->vector[poz]->head->value =
                          (void *)convertireLungime((void *)&lungime,
                          elem->lista_cuvinte);
                    }
                    else // exista cuvantul
                    {
                        Cuvant *cuv = convertireCuvant(string, (void *)&nr);
                        // adauga cuvantul cu nr de aparitii
                        // intors de functia findCuvant
                        addValueInOrder((void *)cuv, elem->lista_cuvinte,
                          comparareNumerica, comparareAlfabetica);
                        table->vector[poz]->head->value =
                          (void *)convertireLungime((void *)&lungime,
                          elem->lista_cuvinte);
                    }
                    return;
                }
                else if (comparareLungimi(n->value, element) < 0
                  && n->next == NULL) // trebuie dupa head
                {
                    table->vector[poz]->head->next =
                      calloc(1, sizeof(struct _Node));
                    table->vector[poz]->head->next->next = NULL;
                    int nr = 1;
                    Cuvant *cuv = convertireCuvant(string, (void *)&nr);
                    addValueInOrder((void *)cuv, l, comparareNumerica,
                      comparareAlfabetica);
                    table->vector[poz]->head->next->value =
                      convertireLungime((void *)&lungime, l);
                    return;
                }
            }

            // PARCURGE LISTA MARE PANA GASESTE LOCUL POTRIVIT
            Lungime *element_aux;
            element_aux = (Lungime *)table->vector[poz]->head->next->value;
            n = table->vector[poz]->head;
            // verifica daca elementul urmator are lungimea
            // mai mare sau egala cu elementul dat
            while (n->next != NULL && (comparareLungimi((void *)element_aux,
              (void *)element) < 0))
            {
                n = n->next;
                if (n->next != NULL) element_aux = (Lungime *)n->next->value;
                else break;
            }

            Lungime *elem;
            if (n->next != NULL)
            {
                elem = (Lungime *)n->next->value;
                if (comparareLungimi(elem, element) == 0) // lungime existenta
                {
                    int pozitie = 0;
                    int nr_aux = 0;
                    Cuvant *cuv = convertireCuvant(string, (void *)&nr_aux);
                    int nr = findCuvant((void *)cuv, elem->lista_cuvinte,
                      &pozitie, comparareAlfabetica);
                    if (nr == 0) // nu exista cuvantul
                    {
                        nr = 1;
                        Cuvant *cuv = convertireCuvant(string, (void *)&nr);
                        addValueInOrder((void *)cuv, elem->lista_cuvinte,
                          comparareNumerica, comparareAlfabetica);
                        n->next->value = (void *)convertireLungime(
                          (void *)&lungime, elem->lista_cuvinte);
                        return;
                    }
                    else // exista cuvantul
                    {
                        Cuvant *cuv = convertireCuvant(string, (void *)&nr);
                        addValueInOrder((void *)cuv, elem->lista_cuvinte,
                          comparareNumerica, comparareAlfabetica);
                        n->next->value = (void *)convertireLungime(
                          (void *)&lungime, elem->lista_cuvinte);
                    }
                    return;
                }
            }

            // lungime inexistenta
            if (n->next == NULL || comparareLungimi(elem, element) != 0)
            {
                Node *aux = (Node *)calloc(1, sizeof(Node));
                aux->next = n->next;
                n->next = aux;
                int nr = 1;
                Cuvant *cuv = convertireCuvant(string, (void *)&nr);
                addValueInOrder((void *)cuv, l, comparareNumerica,
                  comparareAlfabetica);
                aux->value = (void *)convertireLungime(
                  (void *)&lungime, (void *)l);
                return;
            }
        }
    }
}

void printCuvant(void *cuv)
{
    Cuvant *c = (Cuvant *)cuv;
    printf("%s/", c->cuvant);
    printf("%d", c->nr);
}

void printList(List *list, int lungime, void (*printElement)(void *))
{
    Node *m = list->head;
    printf("(");
    printf("%d:", lungime);
    while (m != NULL)
    {
        printCuvant(m->value);
        if (m->next != NULL)
            printf(", ");
        m = m->next;
    }
    printf(")");
}

void printLiteraLungime(HashTable *table, char *c, int *n,
  void (*printList)(List *, int, void (*printCuvant)(void *)),
  void (*printCuvant)(void *))
{
    int poz = table->hashCode(c);
    Node *nod = table->vector[poz]->head;
    Lungime *elem;
    int ok = 0;
    while (nod != NULL) // parcurge lista corespunzatoare literei date
    {
        elem = (Lungime *)nod->value;
        if (elem->lungime == (*n))
        {
            ok = 1;
            printList(elem->lista_cuvinte, elem->lungime, printCuvant);
            break;
        }
        else
            nod = nod->next;
    }
    if (ok)
        printf("\n");
}

void printAparitie(HashTable *table, int nr)
{
    int i, ok = 0, ok1 = 0, ok2 = 0;
    char *buffer;
    char *buffer1 = calloc(100, sizeof(char));
    char *buffer2 = calloc(100, sizeof(char));
    char *str;
    for (i = 0; i < table->size; i++)
    {
        ok = 0;
        buffer = calloc(3000, sizeof(char));
        Node *n = table->vector[i]->head;
        while (n != NULL)
        {
            Lungime *elem = (Lungime *)n->value;
            Node *m = elem->lista_cuvinte->head;
            str = calloc(3000, sizeof(char));
            while (m != NULL)
            {
                ok1 = 0;
                Cuvant *cuv = (Cuvant *)m->value;
                if (cuv->nr <= nr)
                {
                    ok = 1;
                    ok1 = 1;
                    strcat(str, cuv->cuvant);
                    strcat(str, "/");
                    sprintf(buffer1, "%d", cuv->nr);
                    // converteste nr de aparitii in string
                    strcat(str, buffer1); // in str sunt salvate cuvintele cu
                                          // aparitile cu aparitile cu virgula
                                          // intre fiecare structura
                    Node *aux;
                    if (m->next != NULL)
                    {
                        aux = m->next; // cauta in lista daca mai trebuie 
                                       // sa adauge vreun cuvant sau nu 
                                       // si pune virgula
                        while (aux != NULL)
                        {
                            Cuvant *cuv1 = (Cuvant *)aux->value;
                            if (cuv1->nr <= nr)
                            {
                                ok2 = 1;
                                strcat(str, ", ");
                                break;
                            }
                            aux = aux->next;
                        }
                    }
                }
                m = m->next;
            }
            if (ok1)
            {
                strcat(buffer, "(");
                sprintf(buffer2, "%d", elem->lungime);
                strcat(buffer, buffer2);
                strcat(buffer, ": ");
                strcat(buffer, str);
                strcat(buffer, ")");
                // in buffer salveaza lungimea si 
                //lista de cuvinte care indeplineste conditia
            }
            n = n->next;
        }
        if (ok)
        {
            printf("pos%d: ", i);
            printf("%s", buffer);
            printf("\n");
        }
    }
    free(str);
    free(buffer);
    free(buffer1);
    free(buffer2);
}

void printTabela(HashTable *table)
{
    int i, j;
    Node *n, *m;
    for (i = 0; i < table->size; i++)
    {
        if (table->vector[i]->head != NULL)
        {
            printf("pos %d: ", i);
            n = table->vector[i]->head;
            while (n != NULL)
            {
                Lungime *element = (Lungime *)n->value;
                printf("(");
                printf("%d:", element->lungime);
                m = element->lista_cuvinte->head;
                while (m != NULL)
                {
                    Cuvant *cuv = (Cuvant *)m->value;
                    printf("%s/%d", cuv->cuvant, cuv->nr);
                    if (m->next != NULL)
                        printf(", ");
                    m = m->next;
                }
                printf(")");
                n = n->next;
            }
            printf("\n");
        }
    }
}

void freeStringuri(void *x)
{
    Cuvant *cuv = (Cuvant *)x;
    free(cuv->cuvant);
    free(cuv);
}

void freeList(List **list, void (*freeValue)(void *))
{
    Node *current = (*list)->head;
    Node *next;

    while (current != NULL)
    {
        next = current->next;
        freeValue(current->value);
        free(current);
        current = next;
    }

    free(*list);
    *list = NULL;
}

void freeLungime(void *x)
{
    Lungime *elem = (Lungime *)x;
    freeList(&(elem->lista_cuvinte), freeStringuri);
    free(elem);
}

void freeTable(HashTable **table)
{
    for (int i = 0; i < (*table)->size; i++)
    {
        Node *n = (*table)->vector[i]->head;
        while (n != NULL)
        {
            Lungime *x = (Lungime *)n->value;
            freeLungime(x);
            n = n->next;
        }
        free((*table)->vector[i]);
    }
    free((*table)->vector);

    free(*table);
    *table = NULL;
}

void stergeCuvant(List **list, int pozitie)
{
    int p = 1;
    if (p == pozitie)
    {
        Node *aux = (*list)->head->next;
        freeStringuri((*list)->head->value);
        free((*list)->head);
        (*list)->head = aux;
        return;
    }
    Node *n = (*list)->head;
    while (n->next != NULL)
    {
        if (p == pozitie)
        {
            Node *aux = n->next->next;
            freeStringuri(n->next->value);
            free(n->next);
            n->next = aux;
        }
        else
        {
            n = n->next;
            p++;
        }
    }
}

int main(int argc, char *argv[])
{
    HashTable *table = CreateTable(fhash);

    // CITIREA
    FILE *f = fopen(argv[1], "r");
    char *linie = calloc(1000, sizeof(char));
    char *task_copie = calloc(1000, sizeof(char));
    char *task;
    char sep[20] = " .!?,1234567890";
    char sep1[20] = " .!?,";

    while (fgets(linie, 1000, f) != NULL)
    {
        linie[strcspn(linie, "\r")] = 0;
        linie[strcspn(linie, "\n")] = 0;
        task = strtok(linie, sep);
        if (task != NULL && strcmp(task, "insert") == 0)
        {
            while (task != NULL)
            {
                task = strtok(NULL, sep);
                if (task != NULL)
                {
                    void *cuv = (void *)task;
                    inserare(table, cuv, comparareLungimi,
                      comparareAlfabetica, comparareNumerica);
                }
            }
        }
        if (task != NULL && strcmp(task, "print") == 0)
        {
            task = strtok(NULL, sep1);
            if (task == NULL)
                printTabela(table);
            else
            {
                strcpy(task_copie, task);
                if (*task_copie >= 65 && *task_copie <= 90 ||
                  *task_copie >= 97 && *task_copie <= 122)
                {
                    task = strtok(NULL, sep1);
                    int a = atoi(task);
                    printLiteraLungime(table, task_copie, &a,
                      printList, printCuvant);
                }
                else
                {
                    printAparitie(table, atoi(task));
                }
            }
        }
    }
    fclose(f);

    // ELIBERAREA
    freeTable(&table);
    free(linie);
    free(task_copie);
}
