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
void heapify(int arr[], int i, int heapSize);
struct node* crea_nodo(int key);
struct node* inserisci_nodo(struct node* node, int key);
void inorder(struct node* root);
struct node* search(struct node* root, int key);
struct node* demolisci_stazione(struct node* root, int k);
struct node* aggiungi_auto(struct node* root, int dist, int autonomia);
struct node* rottama_auto(struct node* root, int dist, int da_rottamare);
struct node* minimo(struct node* root);
struct node* successore(struct node* root, struct node* n);
struct node* massimo(struct node* root);
struct node* predecessore(struct node* root, struct node* n);
struct valida* inserisci_in_testa(struct valida* head, int dist);
struct valida* rimuovi_in_testa(struct valida* head);
void stampa_lista(struct valida* head);
void stampa_lista_contrario(struct valida *head);
void pianifica_percorso_a(struct node* root, int A, int B);
void pianifica_percorso_r(struct node* root, int A, int B);


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

    while(1) { //TODO: sostituire '1' con 'End Of File'
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
            root = aggiungi_stazione(root, num);
            fflush(stdin);
            //DEBUG
            //inorder(root);

        }else if (strncmp(input_string, "demolisci-stazione", 18) == 0){//OK (circa)
            int* num = string_parsing(input_string);
            //Chiamata alla funzione "demolisci-stazione"
            demolisci_stazione(root, num[0]);
            fflush(stdin);
            //DEBUG
            //inorder(root);

        }else if (strncmp(input_string, "aggiungi-auto", 13) == 0) {//OK
            int* num = string_parsing(input_string);
            //Chiamata alla funzione "aggiungi-auto"
            aggiungi_auto(root, num[0], num[1]);
            fflush(stdin);

        }else if (strncmp(input_string, "rottama-auto", 12) == 0){ //OK
            int* num = string_parsing(input_string);
            //Chiamata alla funzione "rottama-auto"
            rottama_auto(root, num[0], num[1]);
            fflush(stdin);

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

        }
    }

    //inorder(root);

    return 0;
}

/* --- FUNCTIONS' IMPLEMENTATION ---*/

/* --- INPUT PARSING --- */

//Funzione che estrae i dati passati tramite input e li inserisce in un array
int* string_parsing(const char* input_string) {
    int total_n = 0;
    int n, i, j=0;
    int* num = (int*)malloc(1000 * sizeof(int));
    while (sscanf(input_string + total_n, "%*[^0123456789]%d%n", &i, &n) == 1){
        total_n += n;
        num[j] = i;
        j++;
        i++;
    }
    return num;
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
    autonomie = aggiungi_parco(n_macchine, num); //Viene restituito un array (puntatore) di autonomie organizzato in max-heap

    //DEBUG
    //printf("\nautonomie ordinate");

    //Aggiunta effettiva stazione
    struct node* stazione = NULL;
    stazione = search(root, dist_stazione);
    if (stazione != NULL && dist_stazione != root->key) {
        printf("non aggiunta\n");
        return root;
    }else{
        root = inserisci_nodo(root, dist_stazione);
    }

    //DEBUG
    //printf("\nNodo inserito: %d", dist_stazione);

    //Collegamento max-heap delle macchine alla struttura dati che rappresenta la stazione
    struct node* nodo_inserito = search(root, dist_stazione);
    nodo_inserito -> autonomie = autonomie;
    nodo_inserito -> dim_parco = n_macchine;
    nodo_inserito -> capacita = n_macchine;

    //DEBUG
    //printf("\nNumero macchine presenti: %d", nodo_inserito->dim_parco);
    //printf("\nAutonomie della stazione inserita:\t");
    //for(int i=0; i<n_macchine; i++){
    //    printf("%d\t", nodo_inserito->autonomie[i]);
    //}
    printf("aggiunta\n");
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

    //Riorganizzazione dell'array in un max-heap
    int start = (dim/2) - 1;
    for (int i=start; i>=0; i--){
        heapify(autonomie, i, dim);
    }

    //DEBUG
    //printf("\nCreato parco ordinato");

    return autonomie;
}

//Funzione che ordina un array come se fosse un max-heap
void heapify(int arr[], int i, int heapSize) {
    int leftChild = (2*i)+1, rightChild = (2*i)+2, largest = i;
    int temp;

    //Controlla se il figlio destro è più grande
    if(rightChild < heapSize && arr[rightChild] > arr[largest]){
        largest = rightChild;
    }

    //Controlla se il figlio sinistro è più grande
    if(leftChild < heapSize && arr[leftChild] > arr[largest]){
        largest = leftChild;
    }

    //Swap
    if(largest != i){
        temp = arr[largest];
        arr[largest] = arr[i];
        arr[i] = temp;

        heapify(arr, largest, heapSize);
    }
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

    //Albero vuoto
    if (node == NULL){
        //printf("\nCaso base");
        return crea_nodo(key);
    }else{
        //Albero non vuoto
        //printf("\nAlbero non vuoto");
        if(key < node->key){
            node->left = inserisci_nodo(node->left, key);
        }else if(key > node->key){
            node->right = inserisci_nodo(node->right, key);
        }
    }

    return node;
}

//Stampa in ordine crescente le chiavi del BST
void inorder(struct node* root){
    if(root != NULL){
        inorder(root->left);
        printf("\n\t%d", root->key);
        inorder(root->right);
    }
}

//Ricerca di un elemento nel BST tramite la chiave
struct node* search(struct node *root, int key){
    if(root==NULL || root->key==key)
        return root;
    else if(key > root->key)
        return search(root->right, key);
    else
        return search(root->left, key);
}



/* --- DEMOLISCI-STAZIONE --- */

//TODO: gestire caso 'non demolita'
//TODO: eliminare print di debug

//Rimozione di un elemento dal BST
struct node* demolisci_stazione(struct node* root, int k){

    if (root == NULL)
        return root;

    if (root->key > k) {
        root->left = demolisci_stazione(root->left, k);
        return root;
    }
    else if (root->key < k) {
        root->right = demolisci_stazione(root->right, k);
        return root;
    }

    //DEBUG
    //printf("\n%d verra' eliminato", root->key);

    if (root->left == NULL) {
        //DEBUG
        //printf("\nFiglio sinistro vuoto");
        struct node* temp = root->right;
        free(root);
        return temp;
    }
    else if (root->right == NULL) {
        //DEBUG
        //printf("\nFiglio destro vuoto");
        struct node* temp = root->left;
        free(root);
        return temp;
    }
    else {
        //DEBUG
        //printf("\nEsistono entrambi i figli");
        struct node* succParent = root;
        struct node* succ = root->right;
        while (succ->left != NULL) {
            succParent = succ;
            succ = succ->left;
        }

        if (succParent != root)
            succParent->left = succ->right;
        else
            succParent->right = succ->right;

        root->key = succ->key;
        free(succ);
        return root;
    }
}


/* --- AGGIUNGI-AUTO --- */

//Aggiunge l'autonomia data alla stazione richiesta
struct node* aggiungi_auto(struct node* root, int dist, int autonomia){
    struct node* stazione;
    stazione = search(root, dist);
    if (stazione == NULL && dist != root->key){
        printf("non aggiunta\n");
        return root;
    }
    //DEBUG
    //printf("\nNumero di macchine presenti inizialmente: %d", stazione->dim_parco);
    //printf("\nLunghezza iniziale array delle autonomie: %d", stazione->capacita);
    //printf("\nAutonomie presenti inizialmente:\t");
    //for(int i=0; i<stazione->dim_parco; i++){
    //    printf("%d\t", stazione->autonomie[i]);
    //}


    if(stazione->dim_parco == 512){
        printf("non aggiunta\n");
    }else{
        if (stazione->dim_parco == stazione->capacita && stazione->dim_parco == 0){
            stazione->dim_parco = stazione->dim_parco + 1;
            stazione->capacita = 2 * stazione->dim_parco;
            stazione->autonomie = (int*)malloc(stazione->capacita * sizeof(int));
            stazione->autonomie[0] = autonomia;
            stazione->autonomie[1] = 0;
            //printf("aggiunta");
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

    //DEBUG
    //printf("\nNumero di macchine presenti dopo aggiunta: %d", stazione->dim_parco);
    //printf("\nLunghezza attuale array delle autonomie: %d", stazione->capacita);
    //printf("\nAutonomie presenti prima di heapify:\t");
    //for(int i=0; i<stazione->dim_parco; i++) {
    //    printf("%d\t", stazione->autonomie[i]);
    //}

    //Riorganizzo l'array in un max-heap
    int start = ((stazione->dim_parco)/2) - 1;
    for (int i=start; i>=0; i--){
        heapify(stazione->autonomie, i, stazione->dim_parco);
    }

    //DEBUG
    //printf("\nAutonomie presenti dopo heapify:\t");
    //for (int i=0; i<stazione->capacita; i++){
    //    printf("%d\t", stazione->autonomie[i]);
    //}

    return root;
}

/* --- ROTTAMA-AUTO --- */

//Pone a zero l'autonomia dell'auto da rottamare e riorganizza il vettore max-heap della stazione passata in ingresso
struct node* rottama_auto(struct node* root, int dist, int da_rottamare){
    struct node* stazione;
    stazione = search(root, dist);
    if (stazione == NULL && dist != root->key){
        printf("non rottamata\n");
        return root;
    }
    //DEBUG
    //printf("\nNumero di macchine presenti inizialmente: %d", stazione->dim_parco);
    //printf("\nAutonomie presenti inizialmente:\t");
    //for(int i=0; i<stazione->dim_parco; i++){
    //    printf("%d\t", stazione->autonomie[i]);
    //}

    //Scorro il max-heap finchè non trovo l'auto da rottamare, le assegno il valore dell'ultima autonomia presente nell'array,
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

    //printf("\nAutonomie presenti dopo rottamazione prima di heapify:\t");
    //for(int i=0; i<stazione->dim_parco; i++){
    //    printf("%d\t", stazione->autonomie[i]);
    //}

    //Riorganizzo l'array in un max-heap
    int start = (stazione->dim_parco/2)-1;
    for (int i=start; i>=0; i--){
        heapify(stazione->autonomie, i, stazione->dim_parco);
    }

    //DEBUG
    //printf("\nAutonomie presenti dopo rottamazione dopo heapify: ");
    //for (int i=0; i<stazione->capacita; i++){
    //    printf("\t%d", stazione->autonomie[i]);
    //}

    printf("rottamata\n");
    return root;
}


/* --- PIANIFICA-PERCORSO --- */

//TODO: gestire casi percorso inesistente ritorno
//TODO: cancellare print di debug

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
        //printf("\nAggiungendo nodo alla lista");
        struct valida* newNode = (struct valida*)malloc(sizeof(struct valida));
        newNode -> dist = dist;
        newNode -> next = head;
        head = newNode;
    }
    else{
        //DEBUG
        //printf("\nCreando la lista: ");
        struct valida* newNode = (struct valida*)malloc(sizeof(struct valida));
        newNode -> dist = dist;
        newNode -> next = NULL;
        head = newNode;
        //printf("\t%d", head->dist);
    }
    return head;
}

//Funzione che rimuove la testa della lista
struct valida* rimuovi_in_testa(struct valida* head){
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
        printf("%d ", head->dist);
        head = head->next;
    }
    printf("\n");
}

//
void stampa_lista_contrario(struct valida *head){
    if(head == NULL){
        return;
    }
    else{
        stampa_lista_contrario(head->next);
        printf("%d ", head->dist);
    }

}

//Algoritmo di pianificazione del percorso considerando il numero minimo di tappe possibili e più vicine all'
//inizio della stazione

void pianifica_percorso_a(struct node* root, int A, int B){
    struct node* stazioneA = search(root, A);
    struct node* cur = predecessore(root, search(root, B));
    struct valida* head = NULL;
    head = inserisci_in_testa(head, B);
    while(cur != stazioneA){
        if(cur->key + cur->autonomie[0] >= B){
            if(head->dist != B){
                while(head->dist != B){
                    head = rimuovi_in_testa(head);
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
                head = rimuovi_in_testa(head);
            }
            head = inserisci_in_testa(head, k);
            head = inserisci_in_testa(head, cur->key);
            cur = predecessore(root, cur);
        }

    }

    if (stazioneA->key + stazioneA->autonomie[0] >= head->dist){
        head = inserisci_in_testa(head, A);
        stampa_lista(head);
    }else{
        printf("nessun percorso\n");
    }
}

struct node* stazione_end(struct node* root, int chiave){
    //Restituire la stazione più lontana raggiungibile
    struct node *stazione = search(root, chiave);
    struct node *cur = stazione;
    int end = stazione->key - stazione->autonomie[0];

    while (cur != NULL && cur->key >= end){
        cur = predecessore(root, cur);
    }
    cur = successore(root, cur);
    return cur;
}

void pianifica_percorso_r(struct node* root, int A, int B){

    //Stazione di partenza
    struct node* stazioneA = search(root, A);
    //Stazione di arrivo
    struct node* stazioneB = search(root, B);

    struct node* prec = stazioneA; //Nodo che scorrerà fino alla fine fermandosi alle tappe valide (per conoscerne la chiave)
    prec = stazioneA;

    struct valida* head = NULL;
    head = inserisci_in_testa(head, A);

    int max_autonomia;
    int start = A;
    int end = A - stazioneA->autonomie[0];
    int flag = 1;

    struct node* curr = predecessore(root, prec);
    max_autonomia = curr->autonomie[0]; //Salvo la massima autonomia del nodo appena prima di prec

    while(flag == 1 && prec != stazioneB) {
/*
        //DEBUG
        printf("\n\nCiclo esterno.");
        printf("\nCurr iniziale vale %d", curr->key);
        printf("\nPrec vale %d", prec->key);
*/

        //'curr' scorre le tappe raggiungibili dal nodo prec
        while (curr->key < start && curr->key >= end) {
/*
            //DEBUG
            printf("\n\nCiclo interno.");
            printf("\nCurr vale %d", curr->key);
            printf("\nPrec vale %d", prec->key);
*/
            if (curr->autonomie[0] >= max_autonomia) {

                //DEBUG
                //printf("\nCurr->autonomie = %d", curr->autonomie[0]);
                //printf("\nmax_autonomia = %d", max_autonomia);

                if (head->dist !=
                    start) {//Se ho trovato un'autonomia migliore di quella presa prima cancello l'ultima tappa e riscrivo quella nuova
                    head = rimuovi_in_testa(head);
                    //printf("\nTesta rimossa, nuova testa %d", head->dist);
                }
                max_autonomia = curr->autonomie[0]; //Riassegno l'autonomia migliore
                head = inserisci_in_testa(head, curr->key);
                //printf("\nTesta migliore trovata %d", head->dist);

                prec = curr;

                //printf("\nNuovo valore prec %d", prec->key);

                curr = predecessore(root, curr);
            } else {
                curr = predecessore(root, curr);
            }

/*
            printf("\nLista attuale: ");
            stampa_lista(head);
            printf("\nA fine iterazione interna: ");
            printf("\ncurr->autonomia %d", curr->autonomie[0]);
            printf("\ncurr vale %d", curr->key);
            printf("\nprec vale %d", prec->key);
            printf("\nhead->dist: %d", head->dist);
            printf("\nstart %d", start);
            printf("\nend %d", end);
            printf("\nmax autonomia %d", max_autonomia);
*/
        }

        if (curr!=prec){
            struct node *lastprec = stazione_end(root, prec->key);//nodo più lontano raggiungibile da prec(stazione con attualmente autonomia massima tra quelle visitate)
            struct node *lastsucc = NULL;
            //printf("\nlastprec %d", lastprec->key);
            //printf("\nlastsucc %d", lastsucc->key);
            struct node *succ = successore(root, curr);
            //printf("\nsucc %d", succ->key);
            while (succ->key<=prec->key){
                //printf("\ndentro il while");
                lastsucc = stazione_end(root, succ->key);
                //printf("\nlastsucc %d", lastsucc->key);
                if (lastsucc->key <= lastprec->key){
                    //printf("\nsono entrato nel if con lastsucc %d per il nodo %d", lastsucc->key, succ->key);
                    prec = succ;
                    max_autonomia = succ->autonomie[0];
                    head = rimuovi_in_testa(head);
                    head = inserisci_in_testa(head, succ->key);
                    break;
                }
                succ = successore(root, succ);
            }
        }
/*
        printf("\n\nFuori dal ciclo interno, terminate tappe raggiungibili, nel ciclo esterno: ");
        printf("\nhead->dist: %d", head->dist);
        printf("\nstart %d", start);
        printf("\nend %d", end);
        printf("\nmax autonomia %d", max_autonomia);
*/

        if (head->dist == start) {
            head = inserisci_in_testa(head, curr->key);
            prec = curr;
            curr = predecessore(root, curr);
        }

        //Ferma a B se end risulta maggiore di B
        if (prec->key - max_autonomia <= B) {
            prec = stazioneB;
            head = inserisci_in_testa(head, B);
            //printf("\nOra è raggiungibile il nodo B.");
        }

        start = head->dist;
        end = start - max_autonomia;
        max_autonomia = curr->autonomie[0];

        if (curr->key < end) {
            flag = 0;
            //printf("curr %d, end %d", curr->key, end);
            printf("nessun percorso\n");
            prec = NULL;
            curr = NULL;
        } else {
/*
            //DEBUG
            printf("\n\nValori alla fine ciclo esterno: ");
            printf("\nStart vale: %d", start);
            printf("\nEnd vale: %d", end);
            printf("\nhead->dist: %d", head->dist);
            printf("\nCurr vale %d", curr->key);
            printf("\nPrec vale %d", prec->key);;
            printf("\nMax auton %d", max_autonomia);*/
        }

        if (head->dist == B) {
            //printf("\nLista finale: ");
            stampa_lista_contrario(head);
        }
    }
    //inorder(root);
    /*struct node *prova = stazione_end(root, A);
    printf("ciao %d", prova->key);*/
}



