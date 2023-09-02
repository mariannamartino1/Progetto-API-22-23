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
int* sort(int autonomie[], int n_auto);
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
struct node* stazione_start(struct node* root, int chiave);
struct node* stazione_end(struct node* root, int chiave);
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
            struct node* stazione = NULL;
            stazione = search(root, num[0]);
            if (stazione != NULL && num[0] != root->key) {
                printf("non aggiunta\n");
            }else if(stazione != NULL && num[0] == stazione->key && num[0] == root->key){
                printf("non aggiunta\n");
            }else{
                root = aggiungi_stazione(root, num);
                printf("aggiunta\n");
            }
            //printf("\nalbero\n");
            //printf("\nroot %d\n", root->key);
            //inorder(root);
            //printf("\nalbero\n");
            fflush(stdin);
            //DEBUG
            //inorder(root);

        }else if (strncmp(input_string, "demolisci-stazione", 18) == 0){//OK (circa)
            int* num = string_parsing(input_string);
            //Chiamata alla funzione "demolisci-stazione"
            struct node* stazione = NULL;
            stazione = search(root, num[0]);
            if (stazione != NULL && num[0] != root->key) {
                root = demolisci_stazione(root, num[0]);
                printf("demolita\n");
            }else if(stazione != NULL && num[0] == stazione->key && num[0] == root->key){
                root = demolisci_stazione(root, num[0]);
                printf("demolita\n");
            }else{
                printf("non demolita\n");
            }
            fflush(stdin);
            //DEBUG
            //inorder(root);

        }else if (strncmp(input_string, "aggiungi-auto", 13) == 0) {//OK
            int* num = string_parsing(input_string);
            //Chiamata alla funzione "aggiungi-auto"
            root = aggiungi_auto(root, num[0], num[1]);
            fflush(stdin);

        }else if (strncmp(input_string, "rottama-auto", 12) == 0){ //OK
            int* num = string_parsing(input_string);
            //Chiamata alla funzione "rottama-auto"
            //struct node *stazione = search(root, num[0]);
            //printf("\n\n%d\n\n", stazione->key);
            //printf("\n\n%d\n\n", stazione->autonomie[0]);
            rottama_auto(root, num[0], num[1]);
            //inorder(root);
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
    root = inserisci_nodo(root, dist_stazione);


    //DEBUG
    //printf("\nNodo inserito: %d", dist_stazione);

    //Collegamento max-heap delle macchine alla struttura dati che rappresenta la stazione
    struct node* nodo_inserito = search(root, dist_stazione);
    nodo_inserito -> autonomie = autonomie;
    nodo_inserito -> dim_parco = n_macchine;
    nodo_inserito -> capacita = n_macchine;

    //DEBUG
    /*
    printf("\nNumero macchine presenti: %d", nodo_inserito->dim_parco);
    printf("\nAutonomie della stazione inserita %d:\t", nodo_inserito->key);
    for(int i=0; i<n_macchine; i++) {
        printf("%d\t", nodo_inserito->autonomie[i]);
    }*/

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

    /*
    //Riorganizzazione dell'array in un max-heap
    int start = (dim/2) - 1;
    for (int i=start; i>=0; i--){
        heapify(autonomie, i, dim);
    }*/

    //Ordinamento decrescente delle autonomie
    autonomie = sort(autonomie, dim);

    //DEBUG
    //printf("\nCreato parco ordinato");

    return autonomie;
}

//Riordina in decrescente un array
int* sort(int autonomie[], int n_auto) {

    int i,j;
    int swap;
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
/*
    for(int i=0; i<n_auto; i++) {
        printf("%d\t", autonomie[i]);
    }*/

    return autonomie;
}



//Funzione che ordina un array come se fosse un max-heap
void heapify(int arr[], int i, int heapSize) {
    int leftChild = (2*i)+1, rightChild = (2*i)+2, largest = i;
    int temp;

    printf("\nright child %d", rightChild);
    //Controlla se il figlio destro è più grande
    if(rightChild < heapSize && arr[rightChild] > arr[largest]){
        largest = rightChild;
    }

    printf("\nleft child %d", leftChild);
    //Controlla se il figlio sinistro è più grande
    if(leftChild < heapSize && arr[leftChild] > arr[largest]){
        largest = leftChild;
    }

    //Swap
    if(largest != i){
        printf("\nlargest %d", largest);
        temp = arr[largest];
        printf("\n temp");
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
        printf("\t%d", root->key);
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

//Rimozione di un elemento dal BST
struct node* demolisci_stazione(struct node* root, int k){

    // base case
    if (root == NULL)
        return root;

    // If the key to be deleted is smaller than the root's key,
    // then it lies in left subtree
    if (k < root->key)
        root->left = demolisci_stazione(root->left, k);

        // If the key to be deleted is greater than the root's key,
        // then it lies in right subtree
    else if (k > root->key)
        root->right = demolisci_stazione(root->right, k);

        // if key is same as root's key, then This is the node
        // to be deleted
    else {
        // node with only one child or no child
        if (root->left == NULL) {
            struct node *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct node *temp = root->left;
            free(root);
            return temp;
        }

        // node with two children: Get the inorder successor (smallest
        // in the right subtree)
        struct node* temp = minimo(root->right);

        // Copy the inorder successor's content to this node
        root->key = temp->key;
        root->autonomie = temp->autonomie;
        root->dim_parco = temp->dim_parco;
        root->capacita = temp->capacita;

        // Delete the inorder successor
        root->right = demolisci_stazione(root->right, temp->key);
    }
    return root;
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
 /*   //DEBUG
    printf("\nNumero di macchine presenti inizialmente STAZIONE %d: %d",stazione->key, stazione->dim_parco);
    printf("\nLunghezza iniziale array delle autonomie: %d", stazione->capacita);
    printf("\nAutonomie presenti inizialmente:\t");
    for(int i=0; i<stazione->dim_parco; i++){
        printf("%d\t", stazione->autonomie[i]);
    }*/


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
/*
    //DEBUG
    printf("\nNumero di macchine presenti dopo aggiunta: %d", stazione->dim_parco);
    printf("\nLunghezza attuale array delle autonomie: %d", stazione->capacita);
    printf("\nAutonomie presenti prima di ordinamento:\t");
    for(int i=0; i<stazione->dim_parco; i++) {
        printf("%d\t", stazione->autonomie[i]);
    }*/

    /*
    //Riorganizzo l'array in un max-heap
    int start = ((stazione->dim_parco)/2) - 1;
    for (int i=start; i>=0; i--){
        heapify(stazione->autonomie, i, stazione->dim_parco);
    }*/

    //Riordino l'array decrescente
    stazione->autonomie = sort(stazione->autonomie, stazione->dim_parco);
/*
    //DEBUG
    printf("\nAutonomie presenti dopo ordinamento:\t");
    for (int i=0; i<stazione->capacita; i++){
        printf("%d\t", stazione->autonomie[i]);
    }*/

    return root;
}

/* --- ROTTAMA-AUTO --- */

//Pone a zero l'autonomia dell'auto da rottamare e riorganizza il vettore max-heap della stazione passata in ingresso
struct node* rottama_auto(struct node* root, int dist, int da_rottamare){
    struct node* stazione = NULL;
    int count = 0;
    stazione = search(root, dist);
    //DEBUG
    /*
    printf("\nNumero di macchine presenti inizialmente: %d", stazione->dim_parco);
    printf("\nAutonomie presenti inizialmente:\t");
    for(int i=0; i<stazione->dim_parco; i++){
        printf("%d\t", stazione->autonomie[i]);
    }*/
    if (stazione == NULL && dist != root->key){
        //printf("\tentrato if");
        printf("non rottamata\n");
        return root;
    }else{
        //printf("\nstaz %d\n",stazione->key );
        //printf("\tentrato else\n");
        count = 0;
        for(int i=0;i<stazione->dim_parco; i++){
            if (stazione->autonomie[i]==da_rottamare){
                //printf("\ttrovata\n");
                break;
            }else{
                count++;
            }
        }

    }

    //DEBUG
    /*
    printf("\nNumero di macchine presenti inizialmente: %d", stazione->dim_parco);
    printf("\nAutonomie presenti inizialmente:\t");
    for(int i=0; i<stazione->dim_parco; i++){
        printf("%d\t", stazione->autonomie[i]);
    }*/
    //printf("count %d\n", count);
    //printf("dim parco%d\n", stazione->dim_parco);
    if (count == stazione->dim_parco){
        printf("non rottamata\n");
        return root;
    }

    //DEBUG
/*
    printf("\nNumero di macchine presenti inizialmente STAZIONE %d: %d",stazione->key, stazione->dim_parco);
    printf("\nAutonomie presenti inizialmente:\t");
    for(int i=0; i<stazione->dim_parco; i++){
        printf("%d\t", stazione->autonomie[i]);
    }*/

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
/*
    printf("\nAutonomie presenti dopo rottamazione prima di ordinamento:\t");
    for(int i=0; i<stazione->dim_parco; i++){
        printf("%d\t", stazione->autonomie[i]);
    }*/
/*
    //Riorganizzo l'array in un max-heap
    int start = (stazione->dim_parco/2)-1;
    for (int i=start; i>=0; i--){
        heapify(stazione->autonomie, i, stazione->dim_parco);
    }*/

    //Riordino l'array decrescente
    stazione->autonomie = sort(stazione->autonomie, stazione->dim_parco);
/*
    //DEBUG
    printf("\nAutonomie presenti dopo rottamazione dopo ordinamento: ");
    for (int i=0; i<stazione->capacita; i++){
        printf("\t%d", stazione->autonomie[i]);
    }*/
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

//Stampa lista per il ritorno
void stampa_lista_contrario(struct valida *head){

    if(head == NULL){
        return;
    }
    else{
        stampa_lista_contrario(head->next);
        if (head->next == NULL){
            printf("%d", head->dist);
        }else{
            printf(" %d", head->dist);
        }
    }

}

//Algoritmo di pianificazione del percorso considerando il numero minimo di tappe possibili e più vicine all'
//inizio della stazione

void pianifica_percorso_a(struct node* root, int A, int B){//TODO tappa intermedia dopo A da eliminare
    //inorder(root);
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
        //printf("\n%d", cur->key);
        //stampa_lista(head);
    }

    int chiave = 0;
    if (stazioneA->key + stazioneA->autonomie[0] >= head->dist){
        struct node *stazione = stazione_start(root, A);
        while (head->dist <= stazione->key){
            chiave = head->dist;
            head = rimuovi_in_testa(head);
        }
        head = inserisci_in_testa(head, chiave);
        head = inserisci_in_testa(head, A);
        stampa_lista(head);
    }else{
        printf("nessun percorso\n");
    }
}

//
struct node* stazione_start(struct node* root, int chiave){

    struct node *stazione = search(root, chiave);
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

//Restituisce la stazione più lontana raggiungibile al ritorno
struct node* stazione_end(struct node* root, int chiave){

    struct node *stazione = search(root, chiave);
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

    inorder(root);

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
    int done = 0;

    struct node* curr = predecessore(root, prec);
    max_autonomia = curr->autonomie[0]; //Salvo la massima autonomia del nodo appena prima di prec

    while(flag == 1 && prec != stazioneB) {

        //DEBUG
        printf("\n\nCiclo esterno.");
        printf("\nCurr iniziale vale %d", curr->key);
        printf("\nPrec vale %d", prec->key);
        struct node* x = prec;

        //flag per capire se in questa iterazione del ciclo esterno sono entrato nel ciclo interno: se non sono entrato significa che la prossima tappa non è
        //raggiungibile
        done = 0;
        //'curr' scorre le tappe raggiungibili dal nodo prec
        while (curr->key < start && curr->key >= end) {

           //DEBUG
            printf("\n\nCiclo interno.");
            printf("\nCurr vale %d", curr->key);
            printf("\nPrec vale %d", prec->key);

            if (curr->autonomie[0] >= max_autonomia) {

                //DEBUG
                printf("\nCurr->autonomie = %d", curr->autonomie[0]);
                printf("\nmax_autonomia = %d", max_autonomia);

                if (head->dist !=
                    start) {//Se ho trovato un'autonomia migliore di quella presa prima cancello l'ultima tappa e riscrivo quella nuova
                    head = rimuovi_in_testa(head);
                    printf("\nTesta rimossa, nuova testa %d", head->dist);
                }
                max_autonomia = curr->autonomie[0]; //Riassegno l'autonomia migliore
                head = inserisci_in_testa(head, curr->key);
                printf("\nTesta migliore trovata %d", head->dist);

                prec = curr;

                printf("\nNuovo valore prec %d", prec->key);

                curr = predecessore(root, curr);
            } else {
                curr = predecessore(root, curr);
            }


            printf("\nLista attuale: ");
            stampa_lista(head);
            printf("\nA fine iterazione interna, dentro: ");
            printf("\ncurr->autonomia %d", curr->autonomie[0]);
            printf("\ncurr vale %d", curr->key);
            printf("\nprec vale %d", prec->key);
            printf("\nhead->dist: %d", head->dist);
            printf("\nstart %d", start);
            printf("\nend %d", end);
            printf("\nmax autonomia %d", max_autonomia);

            done = 1;
        }

        if (done == 0){
            printf("nessun percorso\n");
            flag = 0;
            break;
        }

        struct node* y = curr;
        printf("\nuscito\n");
        if (curr!=prec){

            struct node *lastprec = stazione_end(root, prec->key);//nodo più lontano raggiungibile da prec(stazione con attualmente autonomia massima tra quelle visitate)
            struct node *lastsucc = NULL;
            struct node *lastcurr = stazione_end(root, curr->key);
            printf("\nlastcurr %d", lastcurr->key);
            printf("\nlastprec %d", lastprec->key);
            struct node *succ = successore(root, curr);
            struct node* y = succ;
            lastsucc = stazione_end(root, succ->key);
            int primo_succ = succ->key;
            struct node *max_lastsucc = lastsucc;
            struct node *max_succ = succ;
            printf("\nsucc %d", succ->key);

            while (succ->key<=prec->key){
                printf("\ndentro il while");
                printf("\nlastsucc %d", lastsucc->key);
                if (lastsucc->key <= lastprec->key){
                    printf("\nsono entrato nel primo if con lastsucc %d per il nodo %d", lastsucc->key, succ->key);
                    if(succ->key == primo_succ && lastsucc->key <= max_lastsucc->key){
                        printf("\nEntrato nell'if secondo con succ%d lastsucc%d max", succ->key, lastsucc->key);
                        max_lastsucc = lastsucc;
                        printf("\nmaxlastuscc %d", max_lastsucc->key);
                        max_succ = succ;
                        printf("\nmaxsuxx %d", max_succ->key);
                    }else if(lastsucc->key < max_lastsucc->key && succ->key != primo_succ){
                        printf("\nEntrato nell'if secondo con succ%d lastsucc%d max", succ->key, lastsucc->key);
                        max_lastsucc = lastsucc;
                        printf("\nmaxlastuscc %d", max_lastsucc->key);
                        max_succ = succ;
                        printf("\nmaxsuxx %d", max_succ->key);
                    }
                }
                if (max_lastsucc->key <= B){
                    break;
                }
                if(succ->key < A){
                    succ = successore(root, succ);
                    lastsucc = stazione_end(root, succ->key);
                }else{
                    break;
                }
            }
            prec = max_succ;
            max_autonomia = max_succ->autonomie[0];
            head = rimuovi_in_testa(head);
            head = inserisci_in_testa(head, max_succ->key);
        }

        printf("\n\nFuori dal ciclo interno, terminate tappe raggiungibili, nel ciclo esterno: ");
        printf("\nhead->dist: %d", head->dist);
        printf("\nstart %d", start);
        printf("\nend %d", end);
        printf("\nmax autonomia %d", max_autonomia);


        if (head->dist == start) {
            head = inserisci_in_testa(head, curr->key);
            prec = curr;
            curr = predecessore(root, curr);
        }

        //Ferma a B se end risulta maggiore di B
        if (prec->key - max_autonomia <= B) {
            prec = stazioneB;
            head = inserisci_in_testa(head, B);
            printf("\nOra è raggiungibile il nodo B.");
        }

        start = head->dist;
        end = start - max_autonomia;
        max_autonomia = curr->autonomie[0];

        if (curr->key < end) {
            flag = 0;
            printf("curr %d, end %d", curr->key, end);
            printf("nessun percorso\n");
            prec = NULL;
            curr = NULL;
        } else {

            //DEBUG
            printf("\n\nValori alla fine ciclo esterno: ");
            printf("\nStart vale: %d", start);
            printf("\nEnd vale: %d", end);
            printf("\nhead->dist: %d", head->dist);
            printf("\nCurr vale %d", curr->key);
            printf("\nPrec vale %d", prec->key);;
            printf("\nMax auton %d", max_autonomia);

            //CHECK
            printf("\ncurr (y) = %d\n", y->key);//ultimo valore di curr OK
            printf("\nlast (x) = %d\n", x->key);//tappa precedente a questa ultima inserita OK
            stampa_lista(head);

            struct node *sacho = NULL;
            sacho = y;
        }

        if (head->dist == B) {
            printf("\nLista finale: ");
            stampa_lista_contrario(head);
            printf("\n");
        }
    }
    //inorder(root);
    /*struct node *prova = stazione_end(root, A);
    printf("ciao %d", prova->key);*/
}



