#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX_LENGTH 10000

//PROGETTO DI ALGORITMI E STRUTTURE DATI 2022/2023
//CercaPercorso

/* --- STRUCTS --- */

//Stazione di Servizio (Binary Search Tree)
struct node{
    int key; //distanza della stazione dall'inizio dell'autostrada
    int dim_parco; //numero autonomie presenti (dato sempre aggiornato)
    int capacita; //spazio occupabile da autonomie (cioè dimensione dell'array delle autonomie)
    int *autonomie; //puntatore all'array contenente le autonomie presenti nel parco
    struct node *left, *right; //figli
};

//Tappa considerata necessaria per il percorso (Linked List)
struct valida{
    int dist; //Chiave della stazione salvata
    struct valida* next; //Puntatore alla tappa successiva/precedente
};


/* --- FUNCTIONS' DECLARATION --- */

int* string_parsing(const char* input_string);
struct node* aggiungi_stazione(struct node* root, int num[]);
int* aggiungi_parco(int dim, int num[]);
int* sort(int autonomie[], int n_auto);
struct node* crea_nodo(int key);
struct node* inserisci_nodo(struct node* node, int key);
struct node* ricerca(struct node* root, int key);
struct node* demolisci_stazione(struct node* root, int k);
struct node* aggiungi_auto(struct node* root, int dist, int autonomia);
struct node* rottama_auto(struct node* root, int dist, int da_rottamare);
struct node* minimo(struct node* root);
struct node* successore(struct node* root, struct node* n);
struct node* massimo(struct node* root);
struct node* predecessore(struct node* root, struct node* n);
struct valida* inserisci_in_testa(struct valida* head, int dist);
struct valida* rimuovi_testa(struct valida* head);
void stampa_lista(struct valida* head);
void pianifica_percorso_a(struct node* root, int A, int B);
struct node* stazione_start(struct node* root, int chiave);
struct node* stazione_end(struct node* root, int chiave);
void pianifica_percorso_r(struct node* root, int A, int B);
struct valida* aggiusta_lista(struct node* root, struct valida* head, int n_tappe, int A, int B);
void freeTree(struct node* root);


/* --- MAIN --- */
int main(){

    char input_string[MAX_LENGTH];
    struct node* root = NULL;

    /* --- INTRO --- */
    /*
    printf("Benvenuto su CercaPercorso. Menu' dei comandi: \n");
    printf("\t- aggiungi-stazione distanza numero-auto autonomia-auto-1 ... autonomia-auto-n \n");
    printf("\t- demolisci-stazione distanza \n");
    printf("\t- aggiungi-auto distanza-stazione autonomia-auto-da-aggiungere\n");
    printf("\t- rottama-auto distanza-stazione autonomia-auto-da-rottamare\n");
    printf("\t- pianifica-percorso distanza-stazione-partenza distanza-stazione-arrivo\n");
    */
    /* --- COMMAND EXECUTION --- */

    while(1) {
        //printf("\nInserisci comando: \n");
        //Acquisition
        fflush(stdin);
        if(fgets(input_string, sizeof(input_string), stdin)==NULL || feof(stdin)){
            break;
        }

        /* --- Input Comparison --- */
        if (strncmp(input_string, "aggiungi-stazione", 17) == 0){//OK
            int* num = string_parsing(input_string);
            //Chiamata alla funzione "aggiungi-stazione"
            struct node* stazione = NULL;
            stazione = ricerca(root, num[0]);
            if (stazione != NULL && num[0] != root->key) {
                printf("non aggiunta\n");
            }else if(stazione != NULL && num[0] == stazione->key && num[0] == root->key){
                printf("non aggiunta\n");
            }else{
                root = aggiungi_stazione(root, num);
                printf("aggiunta\n");
            }
            fflush(stdin);
            free(num);


        }else if (strncmp(input_string, "demolisci-stazione", 18) == 0){//OK (circa)
            int* num = string_parsing(input_string);
            //Chiamata alla funzione "demolisci-stazione"
            struct node* stazione = NULL;
            stazione = ricerca(root, num[0]);
            if (stazione != NULL && num[0] != root->key) {
                free(stazione->autonomie);
                root = demolisci_stazione(root, num[0]);
                printf("demolita\n");
            }else if(stazione != NULL && num[0] == stazione->key && num[0] == root->key){
                free(stazione->autonomie);
                root = demolisci_stazione(root, num[0]);
                printf("demolita\n");
            }else{
                printf("non demolita\n");
            }
            fflush(stdin);
            free(num);

        }else if (strncmp(input_string, "aggiungi-auto", 13) == 0) {//OK
            int* num = string_parsing(input_string);
            //Chiamata alla funzione "aggiungi-auto"
            root = aggiungi_auto(root, num[0], num[1]);
            fflush(stdin);
            free(num);

        }else if (strncmp(input_string, "rottama-auto", 12) == 0){ //OK
            int* num = string_parsing(input_string);
            //Chiamata alla funzione "rottama-auto"
            rottama_auto(root, num[0], num[1]);
            fflush(stdin);
            free(num);

        }else if (strncmp(input_string, "pianifica-percorso", 18) == 0){//WIP
            int* num = string_parsing(input_string);
            //Chiamata alla funzione "pianifica-percorso"
            if(num[0] > num[1]) {
                pianifica_percorso_r(root, num[0], num[1]);
            }
            else{
                pianifica_percorso_a(root, num[0], num[1]);
            }
            fflush(stdin);
            free(num);

        }
    }
    freeTree(root);
    return 0;
}

/* --- FUNCTIONS' IMPLEMENTATION ---*/
void freeTree(struct node* root) {
    if (root == NULL) {
        return;
    }

    // Liberare i sottoalberi (ricorsivamente)
    freeTree(root->left);
    freeTree(root->right);

    free(root->autonomie);
    // Liberare il nodo corrente
    free(root);
}
/* --- INPUT PARSING --- */

//Funzione che estrae i dati passati tramite input e li inserisce in un array
int* string_parsing(const char* input_string) {
    int tot=0, j=0;
    int i, n;
    int* array = (int*)malloc(1000 * sizeof(int));
    while (sscanf(input_string + tot, "%*[^0123456789]%d%n", &i, &n) == 1){
        tot += n;
        array[j] = i;
        i++;
        j++;
    }
    return array;
}

/* --- AGGIUNGI-STAZIONE --- */

//Inserimento nuova stazione nel BST
struct node* aggiungi_stazione(struct node* root, int num[]){
    int n_macchine;
    int dist_stazione;
    int* autonomie;

    dist_stazione = num[0]; //chiave struttura dati;

    //Inserimento delle macchine
    n_macchine = num[1];
    autonomie = aggiungi_parco(n_macchine, num); //Viene restituito un array (puntatore) di autonomie in ordine decrescente

    //Aggiunta effettiva stazione
    root = inserisci_nodo(root, dist_stazione);

    //Collegamento max-heap delle macchine alla struttura dati che rappresenta la stazione
    struct node* nodo_inserito = ricerca(root, dist_stazione);
    nodo_inserito -> autonomie = autonomie;
    nodo_inserito -> dim_parco = n_macchine;
    nodo_inserito -> capacita = n_macchine;

    return root;
}

//Aggiunta ordinata delle autonomie delle auto date nella stazione da aggiungere
int* aggiungi_parco(int dim, int num[]){

    //Creazione array delle autonomie
    int* autonomie = (int*)malloc(dim * sizeof(int));
    int j=0;
    for (int i=2; i<dim+2 ; i++) {
        autonomie[j] = num[i];
        j++;
    }

    //Ordinamento decrescente delle autonomie
    autonomie = sort(autonomie, dim);

    return autonomie;
}

//Riordina un array decrescente
int* sort(int autonomie[], int n_auto) {

    int i,j, swap;
    do{
        swap = 0;
        for (i=0; i<n_auto-1; i++) {
            if (autonomie[i]<autonomie[i+1]) {
                j=autonomie[i+1];
                autonomie[i+1]=autonomie[i];
                autonomie[i]=j;
                swap = 1;
            }
        }
    } while(swap==1);

    return autonomie;
}

//Creazione nuovo nodo del BST
struct node* crea_nodo(int key){
    struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp->key = key;
    temp->left = temp->right = NULL;
    temp->autonomie = NULL;
    temp->dim_parco = 0;
    temp->capacita = 0;
    return temp;
}

//Inserimento del nodo creato nel BST
struct node* inserisci_nodo(struct node* node, int key){

    //Albero vuoto, caso base
    if (node == NULL){
        return crea_nodo(key);
    }else{
        //Albero non vuoto
        if(key < node->key){
            node->left = inserisci_nodo(node->left, key);
        }else if(key > node->key){
            node->right = inserisci_nodo(node->right, key);
        }
    }

    return node;
}

//Ricerca di un elemento nel BST tramite la chiave
struct node* ricerca(struct node *root, int key){
    if(root==NULL || root->key==key) {
        return root;
    }else if(key > root->key) {
        return ricerca(root->right, key);
    }else {
        return ricerca(root->left, key);
    }
}



/* --- DEMOLISCI-STAZIONE --- */

//Rimozione di un elemento dal BST
struct node* demolisci_stazione(struct node* root, int k){

    //Caso base
    if (root == NULL) {
        return root;
    }

    //Ricerca del nodo da eliminare
    if (k < root->key) {
        root->left = demolisci_stazione(root->left, k);
    } else if (k > root->key) {
        root->right = demolisci_stazione(root->right, k);
    } else {
        if (root->left == NULL) {
            struct node *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct node *temp = root->left;
            free(root);
            return temp;
        }

        //Copia del contenuto da spostare
        struct node* temp = minimo(root->right);
        root->key = temp->key;
        root->autonomie = temp->autonomie;
        root->dim_parco = temp->dim_parco;
        root->capacita = temp->capacita;

        root->right = demolisci_stazione(root->right, temp->key);
    }
    return root;
}


/* --- AGGIUNGI-AUTO --- */

//Aggiunge l'autonomia data alla stazione richiesta
struct node* aggiungi_auto(struct node* root, int dist, int autonomia){
    struct node* stazione;
    stazione = ricerca(root, dist);
    if (stazione == NULL && dist != root->key){
        printf("non aggiunta\n");
        return root;
    }

    if(stazione->dim_parco == 512){
        printf("non aggiunta\n");
    }else{
        if (stazione->dim_parco == stazione->capacita && stazione->dim_parco == 0){
            stazione->dim_parco = stazione->dim_parco + 1;
            stazione->capacita = 2 * stazione->dim_parco;
            stazione->autonomie = (int*)malloc(stazione->capacita * sizeof(int));
            stazione->autonomie[0] = autonomia;
            stazione->autonomie[1] = 0;
            printf("aggiunta\n");
        }else if (stazione->dim_parco == stazione->capacita && stazione->dim_parco != 0){
            stazione->dim_parco = stazione->dim_parco + 1;
            stazione->capacita = 2 * stazione->dim_parco;
            stazione->autonomie = (int*)realloc(stazione->autonomie, stazione->capacita * sizeof(int));
            stazione->autonomie[stazione->dim_parco-1] = autonomia;
            //Poni a zero la parte dell'array non contenente auto
            for(int i=stazione->dim_parco; i<stazione->capacita; i++){
                stazione->autonomie[i]=0;
            }
            printf("aggiunta\n");
        }else if (stazione->dim_parco < stazione->capacita){
            stazione->autonomie[stazione->dim_parco] = autonomia;
            stazione->dim_parco = stazione->dim_parco + 1;
            printf("aggiunta\n");
        }
    }

    //Riordino l'array decrescente
    stazione->autonomie = sort(stazione->autonomie, stazione->dim_parco);

    return root;
}

/* --- ROTTAMA-AUTO --- */

//Pone a zero l'autonomia dell'auto da rottamare e riorganizza il vettore max-heap della stazione passata in ingresso
struct node* rottama_auto(struct node* root, int dist, int da_rottamare){
    struct node* stazione = NULL;
    int count = 0;
    stazione = ricerca(root, dist);

    if (stazione == NULL && dist != root->key){
        printf("non rottamata\n");
        return root;
    }else{
        count = 0;
        for(int i=0;i<stazione->dim_parco; i++){
            if (stazione->autonomie[i]==da_rottamare){;
                break;
            }else{
                count++;
            }
        }

    }

    if (count == stazione->dim_parco){
        printf("non rottamata\n");
        return root;
    }

    //Scorro l'array finchè non trovo l'auto da rottamare, le assegno il valore dell'ultima autonomia presente nell'array,
    //pongo quella a zero
    for (int i=0; i<stazione->dim_parco; i++){
        if (stazione->autonomie[i]==da_rottamare){
            stazione->autonomie[i] = stazione->autonomie[stazione->dim_parco-1];
            stazione->autonomie[stazione->dim_parco-1] = 0;
            break;
        }
    }

    //Aggiorno i valori
    stazione->dim_parco = stazione->dim_parco - 1;

    //Riordino l'array decrescente
    stazione->autonomie = sort(stazione->autonomie, stazione->dim_parco);

    printf("rottamata\n");
    return root;
}


/* --- PIANIFICA-PERCORSO --- */

//Funzione che restituisce il successore in ordine crescente del nodo passato come parametro
struct node* successore(struct node* root, struct node* n){

    struct node* succ = NULL;

    if (n->right != NULL){
        return minimo(n->right);
    }

    while (root != NULL){
        if (n->key < root->key){
            succ = root;
            root = root->left;
        } else if (n->key > root->key){
            root = root->right;
        } else {
            break;
        }
    }

    return succ;
}

//Funzione che restituisce il valore minimo trovato nel BST
struct node* minimo(struct node* root){

    struct node* curr = root;

    //Scorre finche non si trova il nodo più a sinistra del BST
    while (curr->left != NULL){
        curr = curr->left;
    }

    return curr;
}

//Funzione che restituisce il predecessore in ordine crescente del nodo passato come parametro
struct node* predecessore(struct node* root, struct node* n){

    struct node* prec = NULL;

    if (n->left != NULL){
        return massimo(n->left);
    }

    while (root != NULL){
        if (n->key > root->key){
            prec = root;
            root = root->right;
        } else if(n->key < root->key){
            root = root->left;
        }else{
            break;
        }
    }

    return prec;

}

//Funzione che restituisce il valore massimo trovato nel BST
struct node* massimo(struct node* root){

    struct node* curr = root;

    //Scorre finche non si trova il nodo più a destra del BST
    while (curr->right != NULL){
        curr = curr->right;
    }

    return curr;
}

//Funzione di inserimento nella lista contenente le chiavi delle stazioni considerate 'valide'
struct valida* inserisci_in_testa(struct valida* head, int dist){
    if(head != NULL){
        struct valida* nodo = (struct valida*)malloc(sizeof(struct valida));
        nodo -> dist = dist;
        nodo -> next = head;
        head = nodo;
    }
    else{
        struct valida* nodo = (struct valida*)malloc(sizeof(struct valida));
        nodo -> dist = dist;
        nodo -> next = NULL;
        head = nodo;
    }
    return head;
}

//Funzione che rimuove la testa della lista
struct valida* rimuovi_testa(struct valida* head){
    if(head != NULL){
        struct valida* temp = head;
        head = head -> next;
        free(temp);
    }

    return head;
}


//Utilitario per printare la lista
void stampa_lista(struct valida* head){
    while(head != NULL) {
        if(head->next != NULL){
            printf("%d ", head->dist);
        }else{
            printf("%d", head->dist);
        }
        head = head->next;
    }
    printf("\n");
}

//Algoritmo di pianificazione del percorso considerando il numero minimo di tappe possibili e più vicine all'
//inizio della stazione - andata
void pianifica_percorso_a(struct node* root, int A, int B){
    struct node* stazioneA = ricerca(root, A);
    struct node* cur = predecessore(root, ricerca(root, B));
    struct valida* head = NULL;
    head = inserisci_in_testa(head, B);
    while(cur != stazioneA){
        if(cur->key + cur->autonomie[0] >= B){
            if(head->dist != B){
                while(head->dist != B){
                    head = rimuovi_testa(head);
                }
                head = inserisci_in_testa(head, cur->key);
            } else{
                head = inserisci_in_testa(head, cur->key);
            }
            cur = predecessore(root, cur);
        }else if(cur->key+cur->autonomie[0]<head->dist){
            cur = predecessore(root, cur);
        }
        else {
            int k = head->dist;
            while (cur->key + cur->autonomie[0] >= head->dist) {
                k = head->dist;
                head = rimuovi_testa(head);
            }
            head = inserisci_in_testa(head, k);
            head = inserisci_in_testa(head, cur->key);
            cur = predecessore(root, cur);
        }
    }

    int chiave = 0;
    if (stazioneA->key + stazioneA->autonomie[0] >= head->dist){
        struct node *stazione = stazione_start(root, A);
        while (head->dist <= stazione->key){
            chiave = head->dist;
            head = rimuovi_testa(head);
        }
        head = inserisci_in_testa(head, chiave);
        head = inserisci_in_testa(head, A);
        stampa_lista(head);
    }else{
        printf("nessun percorso\n");
    }
    struct valida* temp = NULL;
    temp = head;
    while(temp->next != NULL){
        temp = head->next;
        free(head);
        head = temp;
    }
}

//Restituisce la stazione più lontana raggiungibile per l'andata
struct node* stazione_start(struct node* root, int chiave){

    struct node *stazione = ricerca(root, chiave);
    struct node *cur = stazione;
    int start = stazione->key + stazione->autonomie[0];

    struct node *max = massimo(root);

    if (start>max->key){
        return max;
    }

    while (cur != NULL && cur->key <= start){
        cur = successore(root, cur);
    }
    cur = predecessore(root, cur);
    return cur;
}

//Restituisce la stazione più lontana raggiungibile per il ritorno
struct node* stazione_end(struct node* root, int chiave){

    struct node *stazione = ricerca(root, chiave);
    struct node *cur = stazione;
    int end = stazione->key - stazione->autonomie[0];

    struct node *min = minimo(root);

    if (end<min->key){
        return min;
    }

    while (cur != NULL && cur->key >= end){
        cur = predecessore(root, cur);
    }
    cur = successore(root, cur);
    return cur;
}

void pianifica_percorso_r(struct node* root, int A, int B){

    //Stazione di partenza
    struct node* stazioneA = ricerca(root, A);
    //Stazione di arrivo
    struct node* stazioneB = ricerca(root, B);

    struct node* prec = stazioneA; //Nodo che scorrerà fino alla fine fermandosi alle tappe valide (per conoscerne la chiave)
    prec = stazioneA;

    struct valida* head = NULL;
    head = inserisci_in_testa(head, A);
    int tappe = 1;

    int max_gittata;
    int start = A;
    int end = A - stazioneA->autonomie[0];
    int flag = 1;
    int done = 0;
    struct node* curr = predecessore(root, prec);
    max_gittata = curr->key - curr->autonomie[0]; //Salvo la massima autonomia del nodo appena prima di prec

    while(flag == 1 && prec != stazioneB) {

        //flag per capire se in questa iterazione del ciclo esterno sono entrato nel ciclo interno: se non sono entrato significa che la prossima tappa non è
        //raggiungibile
        done = 0;
        //'curr' scorre le tappe raggiungibili dal nodo prec
        while (curr->key < start && curr->key >= end) {

            if (curr->key - curr->autonomie[0] <= max_gittata) {

                if (head->dist != start) {//Se ho trovato una gittata migliore di quella presa prima cancello l'ultima tappa e riscrivo quella nuova
                    head = rimuovi_testa(head);
                    tappe--;
                }
                max_gittata = curr->key - curr->autonomie[0]; //Riassegno la gittata migliore
                head = inserisci_in_testa(head, curr->key);
                tappe++;
                prec = curr;

                curr = predecessore(root, curr);
            } else {
                curr = predecessore(root, curr);
            }

            done = 1;
        }

        if (done == 0){
            printf("nessun percorso\n");
            flag = 0;
            break;
        }

        if (head->dist == start) {
            head = inserisci_in_testa(head, curr->key);
            tappe++;
            prec = curr;
            curr = predecessore(root, curr);
        }

        //Ferma a B se end risulta maggiore di B
        if (max_gittata <= B) {
            prec = stazioneB;
            head = inserisci_in_testa(head, B);
            tappe++;
        }

        start = head->dist;
        end = max_gittata;
        max_gittata = curr->key-curr->autonomie[0];

        if (curr->key < end) {
            flag = 0;
            printf("nessun percorso\n");
            prec = NULL;
            curr = NULL;
        }

        if (head->dist == B) {
            head = aggiusta_lista(root, head, tappe, A, B);
            stampa_lista(head);
        }
    }
    struct valida* temp = NULL;
    temp = head;
    while(temp->next != NULL){
        temp = head->next;
        free(head);
        head = temp;
    }
}

struct valida* aggiusta_lista(struct node *root, struct valida* head, int n_tappe, int A, int B){

    int prec_dist;
    struct node *cur_albero = NULL; //cursore che scorre l'albero in base alle tappe
    struct node *end = NULL;
    struct valida *nuova_lista = NULL;
    nuova_lista = inserisci_in_testa(nuova_lista, B);
    int start = B;

    while (head->dist <= A){
        head = rimuovi_testa(head);//head = tappa intermedia (rimosso head iniziale/B)
        prec_dist = head->dist;//salva tappa intermedia;
        if (prec_dist == A){
            nuova_lista = inserisci_in_testa(nuova_lista, A);
            break;
        }
        head = rimuovi_testa(head);//head = tappa corrente (rimossa tappa intermedia)
        cur_albero = ricerca(root, head->dist); //trovo tappa corrente all'interno dell'albero
        end = stazione_end(root, cur_albero->key); //trovo massima gittata del nodo corrente nell'albero
        while (end->key <= prec_dist){ //controllo intervallo tra massima gittata di cur e il nodo intermedio, se posso sostituire il nodo intermedio con uno più vicino
            if(end->key - end->autonomie[0] <= start){//se trovo un nodo dell'albero da cui è raggiungibile B e che è raggiungibile dall'attuale testa della lista
                head = inserisci_in_testa(head, end->key);//inserisco quel nodo(tanto 'prec' l'avevo già rimosso)
                nuova_lista = inserisci_in_testa(nuova_lista, head->dist);
                break;
            }else{
                end = successore(root, end);
            }
        }
        start = head->dist;
    }
    return nuova_lista;
}





