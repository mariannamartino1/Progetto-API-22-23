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
    struct valida* next; //Puntatore alla tappa successiva
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
struct node* minimo(struct node* node);
struct node* successore(struct node* root, struct node* n);
struct node* massimo(struct node* root);
struct node* predecessore(struct node* root, struct node* n);
void pianifica_percorso(struct node* root, int A, int B);

/* --- MAIN --- */
int main(){

    char input_string[MAX_LENGTH];
    struct node* root = NULL;
    int key;

    /* --- INTRO --- */
    printf("Benvenuto su CercaPercorso. Menu' dei comandi: \n");
    printf("\t- aggiungi-stazione distanza numero-auto autonomia-auto-1 ... autonomia-auto-n \n");
    printf("\t- demolisci-stazione distanza \n");
    printf("\t- aggiungi-auto distanza-stazione autonomia-auto-da-aggiungere\n");
    printf("\t- rottama-auto distanza-stazione autonomia-auto-da-rottamare\n");
    printf("\t- pianifica-percorso distanza-stazione-partenza distanza-stazione-arrivo\n");

    /* --- COMMAND EXECUTION --- */

    while(1) { //TODO: sostituire '1' con 'End Of File'
        printf("\nInserisci comando: \n");
        //Acquisition
        fgets(input_string, MAX_LENGTH, stdin);

        /* --- Input Comparison --- */
        if (strncmp(input_string, "aggiungi-stazione", 17) == 0){//OK
            int* num = string_parsing(input_string);
            //Chiamata alla funzione "aggiungi-stazione"
            root = aggiungi_stazione(root, num);
            fflush(stdin);
            //DEBUG
            inorder(root);

        }else if (strncmp(input_string, "demolisci-stazione", 18) == 0){//OK (circa)
            int* num = string_parsing(input_string);
            //Chiamata alla funzione "demolisci-stazione"
            demolisci_stazione(root, num[0]);
            fflush(stdin);
            //DEBUG
            inorder(root);

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
            pianifica_percorso(root, num[0], num[1]);
            fflush(stdin);
        }
    }

    inorder(root);

    return 0;
}

/* --- FUNCTIONS' IMPLEMENTATION ---*/

/* --- INPUT PARSING --- */

//TODO: aumentare (con discrezione) la dimensione dell'array per accogliere più dati in ingresso

//Funzione che estrae i dati passati tramite input e li inserisce in un array
int* string_parsing(const char* input_string) {
    int total_n = 0;
    int n, i, j=0;
    int* num = (int*)malloc(20 * sizeof(int));
    while (sscanf(input_string + total_n, "%*[^0123456789]%d%n", &i, &n) == 1){
        total_n += n;
        num[j] = i;
        j++;
        i++;
    }
    return num;
}

/* --- AGGIUNGI-STAZIONE --- */

//TODO: gestire caso 'non aggiunta'
//TODO: eliminare print di debug

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
    printf("\nautonomie ordinate");

    //Aggiunta effettiva stazione
    if (root == NULL){
        root = inserisci_nodo(root, dist_stazione); //Inserimento del primo elemento come radice
    }else{
        inserisci_nodo(root, dist_stazione);
    }

    //DEBUG
    printf("\nNodo inserito: %d", dist_stazione);

    //Collegamento max-heap delle macchine alla struttura dati che rappresenta la stazione
    struct node* nodo_inserito = search(root, dist_stazione);
    nodo_inserito -> autonomie = autonomie;
    nodo_inserito -> dim_parco = n_macchine;
    nodo_inserito -> capacita = n_macchine;

    //DEBUG
    printf("\nNumero macchine presenti: %d", nodo_inserito->dim_parco);
    printf("\nAutonomie della stazione inserita:\t");
    for(int i=0; i<n_macchine; i++){
        printf("%d\t", nodo_inserito->autonomie[i]);
    }

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
    printf("\nCreato parco ordinato");

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
        printf("\nCaso base");
        return crea_nodo(key);
    }else{
        //Albero non vuoto
        printf("\nAlbero non vuoto");
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
    printf("\n%d verra' eliminato", root->key);

    if (root->left == NULL) {
        //DEBUG
        printf("\nFiglio sinistro vuoto");
        struct node* temp = root->right;
        free(root);
        return temp;
    }
    else if (root->right == NULL) {
        //DEBUG
        printf("\nFiglio destro vuoto");
        struct node* temp = root->left;
        free(root);
        return temp;
    }
    else {
        //DEBUG
        printf("\nEsistono entrambi i figli");
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

//TODO: gestire caso 'non aggiunta'
//TODO: eliminare print di debug

//Aggiunge l'autonomia data alla stazione richiesta
struct node* aggiungi_auto(struct node* root, int dist, int autonomia){
    struct node* stazione;
    stazione = search(root, dist);

    //DEBUG
    printf("\nNumero di macchine presenti inizialmente: %d", stazione->dim_parco);
    printf("\nLunghezza iniziale array delle autonomie: %d", stazione->capacita);
    printf("\nAutonomie presenti inizialmente:\t");
    for(int i=0; i<stazione->dim_parco; i++){
        printf("%d\t", stazione->autonomie[i]);
    }

    if(stazione->dim_parco == 512){
        //printf("non aggiunta");
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
            //printf("aggiunta");
        }else if (stazione->dim_parco < stazione->capacita){
            stazione->autonomie[stazione->dim_parco] = autonomia;
            stazione->dim_parco = stazione->dim_parco + 1;
            //printf("aggiunta");
        }
    }

    //DEBUG
    printf("\nNumero di macchine presenti dopo aggiunta: %d", stazione->dim_parco);
    printf("\nLunghezza attuale array delle autonomie: %d", stazione->capacita);
    printf("\nAutonomie presenti prima di heapify:\t");
    for(int i=0; i<stazione->dim_parco; i++) {
        printf("%d\t", stazione->autonomie[i]);
    }

    //Riorganizzo l'array in un max-heap
    int start = ((stazione->dim_parco)/2) - 1;
    for (int i=start; i>=0; i--){
        heapify(stazione->autonomie, i, stazione->dim_parco);
    }

    //DEBUG
    printf("\nAutonomie presenti dopo heapify:\t");
    for (int i=0; i<stazione->capacita; i++){
        printf("%d\t", stazione->autonomie[i]);
    }

    return root;
}

/* --- ROTTAMA-AUTO --- */

//TODO: gestire caso 'non rottamata'
//TODO: eliminare print di debug

//Pone a zero l'autonomia dell'auto da rottamare e riorganizza il vettore max-heap della stazione passata in ingresso
struct node* rottama_auto(struct node* root, int dist, int da_rottamare){
    struct node* stazione;
    stazione = search(root, dist);

    //DEBUG
    printf("\nNumero di macchine presenti inizialmente: %d", stazione->dim_parco);
    printf("\nAutonomie presenti inizialmente:\t");
    for(int i=0; i<stazione->dim_parco; i++){
        printf("%d\t", stazione->autonomie[i]);
    }

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

    printf("\nAutonomie presenti dopo rottamazione prima di heapify:\t");
    for(int i=0; i<stazione->dim_parco; i++){
        printf("%d\t", stazione->autonomie[i]);
    }

    //Riorganizzo l'array in un max-heap
    int start = (stazione->dim_parco/2)-1;
    for (int i=start; i>=0; i--){
        heapify(stazione->autonomie, i, stazione->dim_parco);
    }

    //DEBUG
    printf("\nAutonomie presenti dopo rottamazione dopo heapify: ");
    for (int i=0; i<stazione->capacita; i++){
        printf("\t%d", stazione->autonomie[i]);
    }

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


//Algoritmo di pianificazione del percorso considerando il numero minimo di tappe possibili e più vicine all'
//inizio della stazione
void pianifica_percorso(struct node* root, int A, int B){

    struct node* stazioneA = search(root, A);
    struct node* stazioneB = search(root, B);

}
