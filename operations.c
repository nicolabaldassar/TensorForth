// Nicola Baldassar SM3201596
#include "operations.h"
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <omp.h>


// non sarà commentato lo scopo di tutte le funzioni riguardanti le operazioni in quanto già ben definite
// nella consegna

// consuma i due tensori in cima allo stack e ne crea un terzo che è la loro somma
// prende in input lo stack
void somma(Stack* stack)
{
    Tensor* t1 = pop(stack);
    Tensor* t2 = pop(stack);
    Tensor* t3 = malloc(sizeof(Tensor));
    // controllo dimensioni
    if(t1->shape[0] != t2->shape[0] || t1->shape[1] != t2->shape[1]) {
        printf("Errore! Stai provando a fare una somma con tensori di dimensione diversa.\n");
        exit(EXIT_FAILURE);
    }
    // copio nel t3 appena creato la struttura di t1, ma non il contenuto di data
    tensor_structure_copy(t3, t1);
    #pragma omp parallel for
    for(int i = 0; i < t1->size; i++)
    {
        t3->data[i] = t1->data[i] + t2->data[i];
    }
    push(stack, t3);
    free_tensor(&t1);
    free_tensor(&t2);
}

// consuma i due tensori in cima allo stack e ne crea un terzo che è la loro differenza
// prende in input lo stack
void differenza(Stack* stack)
{
    Tensor* t1 = pop(stack);
    Tensor* t2 = pop(stack);
    Tensor* t3 = malloc(sizeof(Tensor));
    if(t1->shape[0] != t2->shape[0] || t1->shape[1] != t2->shape[1]) {
        printf("Errore! Stai provando a fare una differenza con tensori di dimensione diversa.\n");
        exit(EXIT_FAILURE);
    }
    tensor_structure_copy(t3, t1);
    #pragma omp parallel for
    for(int i = 0; i < t1->size; i++)
    {
        t3->data[i] = t1->data[i] - t2->data[i];
    }
    push(stack, t3);
    free_tensor(&t1);
    free_tensor(&t2);
    return;
}

// consuma i due tensori in cima allo stack e ne crea un terzo che è il loro prodotto elemento per elemento
// prende in input lo stack
void prodotto(Stack* stack)
{
    Tensor* t1 = pop(stack);
    Tensor* t2 = pop(stack);
    Tensor* t3 = malloc(sizeof(Tensor));
    if(t1->shape[0] != t2->shape[0] || t1->shape[1] != t2->shape[1]) {
        printf("Errore! Stai provando a fare un prodotto con tensori di dimensione diversa.\n");
        exit(EXIT_FAILURE);
    }
    tensor_structure_copy(t3, t1);
    #pragma omp parallel for
    for(int i = 0; i < t1->size; i++)
    {
        t3->data[i] = t1->data[i] * t2->data[i];
    }
    push(stack, t3);
    free_tensor(&t1);
    free_tensor(&t2);
    return;
}

void minore(Stack* stack)
{
    Tensor* t1 = pop(stack);
    Tensor* t2 = pop(stack);
    Tensor* t3 = malloc(sizeof(Tensor));
    if(t1->shape[0] != t2->shape[0] || t1->shape[1] != t2->shape[1]) {
        printf("Errore! Stai provando a fare una comparazione tra tensori di dimensione diversa.\n");
        exit(EXIT_FAILURE);
    }
    tensor_structure_copy(t3, t1);
    // parallelizzato e branchless
    #pragma omp parallel for
    for(int i = 0; i < t1->size; i++)
    {
        t3->data[i] = (t1->data[i] < t2->data[i]);
    }
    push(stack, t3);
    free_tensor(&t1);
    free_tensor(&t2);
    return;   
}

void maggiore(Stack* stack)
{
    Tensor* t1 = pop(stack);
    Tensor* t2 = pop(stack);
    Tensor* t3 = malloc(sizeof(Tensor));
    if(t1->shape[0] != t2->shape[0] || t1->shape[1] != t2->shape[1]) {
        printf("Errore! Stai provando a fare una comparazione tra tensori di dimensione diversa.\n");
        exit(EXIT_FAILURE);
    }
    tensor_structure_copy(t3, t1);
    #pragma omp parallel for
    for(int i = 0; i < t1->size; i++)
    {
        t3->data[i] = (t1->data[i] > t2->data[i]);
    }
    push(stack, t3);
    free_tensor(&t1);
    free_tensor(&t2);
    return;   
}

void uguale(Stack* stack)
{
    Tensor* t1 = pop(stack);
    Tensor* t2 = pop(stack);
    Tensor* t3 = malloc(sizeof(Tensor));
    if(t1->shape[0] != t2->shape[0] || t1->shape[1] != t2->shape[1]) {
        printf("Errore! Stai provando a fare una comparazione tra tensori di dimensione diversa.\n");
        exit(EXIT_FAILURE);
    }
    tensor_structure_copy(t3, t1);
    #pragma omp parallel for
    for(int i = 0; i < t1->size; i++)
    {
        t3->data[i] = (t1->data[i] == t2->data[i]);
    }
    push(stack, t3);
    free_tensor(&t1);
    free_tensor(&t2);
    return;   
}

void and_logico(Stack* stack)
{
    Tensor* a = pop(stack);
    Tensor* b = pop(stack);
    Tensor* t = malloc(sizeof(Tensor));
    tensor_structure_copy(t, a);
    // controllo dimensioni
    if(a->shape[0] != b->shape[0] || a->shape[1] != b->shape[1]) {
        printf("I tensori di un'operazione di comparazione devono essere della stessa dimensione.\n");
        exit(EXIT_FAILURE);
    }
    // controllo che entrambi i tensori siano binari
    if(!is_binary_tensor(a) || !is_binary_tensor(b)) {
        printf("I tensori di un'operazione di comparazione devono essere composti solo da 0 e 1.\n");
        exit(EXIT_FAILURE);
    }
    #pragma omp parallel for
    for(int i = 0; i < t->size; i++) {
        t->data[i] = a->data[i] && b->data[i];
    }
    push(stack, t);
    free_tensor(&a);
    free_tensor(&b);
    return;
}

void or_logico(Stack* stack)
{
    Tensor* a = pop(stack);
    Tensor* b = pop(stack);
    Tensor* t = malloc(sizeof(Tensor));
    tensor_structure_copy(t, a);
    if(a->shape[0] != b->shape[0] || a->shape[1] != b->shape[1]) {
        printf("I tensori di un'operazione di comparazione devono essere della stessa dimensione.\n");
        exit(EXIT_FAILURE);
    }
    if(!is_binary_tensor(a) || !is_binary_tensor(b)) {
        printf("I tensori di un'operazione di comparazione devono essere composti solo da 0 e 1.\n");
        exit(EXIT_FAILURE);
    }
    #pragma omp parallel for
    for(int i = 0; i < t->size; i++) {
        t->data[i] = a->data[i] || b->data[i];
    }
    push(stack, t);
    free_tensor(&a);
    free_tensor(&b);
    return;
}

void negazione(Stack* stack)
{
    Tensor* a = pop(stack);
    // controllo che il tensore sia binario
    if(!is_binary_tensor(a)) {
        printf("I tensori di un'operazione di comparazione devono essere composti solo da 0 e 1.\n");
        exit(EXIT_FAILURE);
    }
    Tensor* t = malloc(sizeof(Tensor));
    tensor_structure_copy(t, a);
    #pragma omp parallel for
    for(int i = 0; i < t->size; i++) {
        t->data[i] = 1.0f - a->data[i];
    }
    push(stack, t);
    free_tensor(&a);
}

void maschera(Stack* stack)
{
    Tensor* m = pop(stack);
    Tensor* a = pop(stack);
    Tensor* b = pop(stack);
    if(a->size != b->size || b->size != m->size) {
        printf("I tensori devono essere della stessa dimensione.\n");
        exit(EXIT_FAILURE);
    }
    Tensor* t = malloc(sizeof(Tensor));
    tensor_structure_copy(t, m);
    // parallelizzato e branchless
    #pragma omp parallel for
    for(int i = 0; i < t->size; i++) {
        t->data[i] = m->data[i] * a->data[i] + (1.0f - m->data[i]) * b->data[i];
    }
    push(stack, t);
    free_tensor(&m);
    free_tensor(&a);
    free_tensor(&b);
}

void matrix_prod(Stack* stack)
{
    Tensor* a = pop(stack);
    Tensor* b = pop(stack);
    // controllo che siano matrici
    if(a->ndim != 2 || b->ndim != 2) {
        printf("Per fare un prodotto tra matrici sono richieste delle matrici.\n");
        exit(EXIT_FAILURE);
    }
    // controllo dimensioni per prodotto matriciale
    if(a->shape[1] != b->shape[0]) {
        printf("Le matrici non hanno la giusta dimensione per fare un prodotto matriciale.\n");
        exit(EXIT_FAILURE);
    }
    // inizializzazione nuovo tensore a matrice con le dimensioni corrette
    Tensor* t = malloc(sizeof(Tensor));
    t->ndim = 2;
    t->shape[0] = a->shape[0];
    t->shape[1] = b->shape[1];
    t->size = t->shape[0] * t->shape[1];
    t->ref_count = 1;
    // queste seguenti tre sono inizializzazioni da fare ogni volta che si inizializza un tensore
    // con lo scopo standard di contenere numeri.
    // sono 3 informazioni necessarie nel caso limite in cui il tensore conterrà il nome di un file
    // da salvare in memoria, ma in quel caso i tensori "filename" saranno gestiti da funzioni apposite
    t->isFilename = false;
    t->map_pointer = NULL;
    t->map_size = 0;
    // assegno la memoria e usando calloc inizializzo a zero tutti i vvalori
    t->data = calloc(t->size, sizeof(float));
    #pragma omp parallel for collapse(2)
    for(int i = 0; i < t->shape[0]; i++) {
        for(int j = 0; j < t->shape[1]; j++) {
            for(int k = 0; k < a->shape[1]; k++) {
                t->data[i * t->shape[1] + j] += a->data[i * a->shape[1] + k] * b->data[k * b->shape[1] + j];
            }
        }
    }
    push(stack, t);
    free_tensor(&a);
    free_tensor(&b);
}

void dot_prod(Stack* stack)
{
    Tensor* a = pop(stack);
    Tensor* b = pop(stack);
    if(a->ndim != 1 || b->ndim != 1) {
        printf("Per il prodotto interno i tensori devono essere monodimensionali.\n");
        exit(EXIT_FAILURE);
    }
    if(a->size != b->size) {
        printf("Per il prodotto interno i tensori devono essere della stessa dimensione.\n");
        exit(EXIT_FAILURE);
    }
    // inizializziamo il tensore a tornare uno scalare singolo, quindi tensore di 1 dimensione
    // con 1 riga e 1 colonna e size 1
    Tensor* t = malloc(sizeof(Tensor));
    t->ndim = 1;
    t->shape[0] = 1;
    t->shape[1] = 1;
    t->size = 1;
    t->data = malloc(sizeof(float) * t->size);
    t->ref_count = 1;
    t->isFilename = false;
    t->map_pointer = NULL;
    t->map_size = 0;
    float sum = 0.0f;
    // usiamo la reduction per una scrittura controllata nella variabile sum in modo da evitare race conditions
    #pragma omp parallel for reduction(+:sum)
    for(int i = 0; i < a->size; i++) {
        sum += a->data[i] * b->data[i];
    }
    t->data[0] = sum;
    push(stack, t);
    free_tensor(&a);
    free_tensor(&b);
}

void convoluzione(Stack* stack)
{
    Tensor* k = pop(stack);
    Tensor* a = pop(stack);
    if(k->ndim != 2 || a->ndim != 2) {
        printf("I tensori per la convoluzione devono essere in due dimensioni.\n");
        exit(EXIT_FAILURE);
    }
    Tensor* t = malloc(sizeof(Tensor));
    tensor_structure_copy(t, a);
    // da progetto si suppone che il kernel debba essere dispari
    if(k->shape[0] % 2 == 0 || k->shape[1] % 2 == 0) {
        printf("Il kernel per una convoluzione deve avere dimensioni dispari.\n");
        exit(EXIT_FAILURE);
    }
    // calcoliamo il apdding da aggiungere sui due assi
    int x_padding_to_add = (k->shape[1] - 1) / 2;
    int y_padding_to_add = (k->shape[0] - 1) / 2;
    // creo e inizializzo il tensore col padding e con il tensore a dentro
    Tensor* a_padding = malloc(sizeof(Tensor));
    a_padding->ndim = 2;
    a_padding->shape[0] = a->shape[0] + y_padding_to_add * 2;
    a_padding->shape[1] = a->shape[1] + x_padding_to_add * 2;
    a_padding->size = a_padding->shape[0] * a_padding->shape[1];
    a_padding->ref_count = 1;
    // inizializzato tutto a zero con calloc
    a_padding->data = calloc(a_padding->size, sizeof(float));
    //
    a_padding->map_pointer = NULL;
    a_padding->map_size = 0;
    a_padding->isFilename = false;
    // riempio la matrice con a in posizione corretta
    #pragma omp parallel for collapse(2)
    for(int i = 0; i < a->shape[0]; i++) {
        for(int j = 0; j < a->shape[1]; j++) {
            int i_ind = i + y_padding_to_add;
            int j_ind = j + x_padding_to_add;
            a_padding->data[i_ind * a_padding->shape[1] + j_ind] = a->data[i * a->shape[1] + j];
        }
    }
    // riempio anche la matrice t di zeri
    // lo devo fare io perche t è stata inizializzata con la "tensor_copy" quindi con malloc
    #pragma omp parallel for
    for(int i = 0; i < t->size; i++) {
        t->data[i] = 0;
    }
    // faccio il calcolo, parallelizando i due cicli esterni
    #pragma omp parallel for collapse(2)
    for(int i = 0; i < t->shape[0]; i++) {
        for(int j = 0; j < t->shape[1]; j++) {
            for(int x = 0; x < k->shape[0]; x++) {
                for(int y = 0; y < k->shape[1]; y++) {
                    t->data[i * t->shape[1] + j] += a_padding->data[(i+x) * a_padding->shape[1] + (j+y)] * k->data[x * k->shape[1] + y];
                }
            }
        }
    }
    push(stack, t);
    free_tensor(&a_padding);
    free_tensor(&k);
    free_tensor(&a);
}

void reshape(Stack* stack)
{
    Tensor* s = pop(stack);
    Tensor* a = pop(stack);
    // il tensore con le dimensioni non può essere una matrice
    if(s->ndim == 2) {
        printf("Il tensore con le nuove dimensioni deve essere monodimensionale.\n");
        exit(EXIT_FAILURE);
    }
    if(s->size > 2) {
        printf("Il tensore con le nuove dimensioni deve essere di due elementi.\n");
        exit(EXIT_FAILURE);
    }
    // gestisco il caso in cui il tensore passato alla reshape sia di 1 elemento e lo porto alla mia convenzione che
    // i tensori 1D hanno shape[0] = 1 e shape[1] = size
    if(s->size == 1) {
        int col_n = s->data[0];
        s->size = 2;
        s->shape[0] = 1;
        s->shape[1] = 2;
        s->data = realloc(s->data, sizeof(float) * 2);
        s->data[0] = 1;
        s->data[1] = col_n;
    }
    if(a->size != s->data[0] * s->data[1]) {
        printf("Le nuove dimensioni devono essere compatibili con la dimensione del tensore da modificare.\n");
        exit(EXIT_FAILURE);
    }
    if(s->data[0] <= 1) {
        a->ndim = 1;
    } else {
        a->ndim = 2;
    }
    a->shape[0] = s->data[0];
    a->shape[1] = s->data[1];
    push(stack, a);
    free_tensor(&s);
}

void ravel(Stack* stack)
{
    Tensor* a = pop(stack);
    int new_size = a->shape[0] * a->shape[1];
    a->ndim = 1;
    a->shape[0] = 1;
    a->shape[1] = new_size;
    push(stack, a);
}

void get_dim(Stack* stack)
{
    Tensor* a = pop(stack);
    Tensor* t = malloc(sizeof(Tensor));
    // inizializzo il tensore t a essere 1D e tornare 2 valori
    t->ndim = 1;
    t->shape[0] = 1;
    t->shape[1] = 2;
    t->size = 2;
    t->ref_count = 1;
    //
    t->isFilename = false;
    t->map_pointer = NULL;
    t->map_size = 0;
    t->data = malloc(sizeof(float) * t->size);
    t->data[0] = a->shape[0];
    t->data[1] = a->shape[1];
    push(stack, t);
    free_tensor(&a);
}

void rand_tens(Stack* stack)
{
    Tensor* s = pop(stack);
    if(s->ndim != 1 || s->size > 2) {
        printf("Il tensore che deve essere 1D e al massimo di 2 elementi.\n");
        exit(EXIT_FAILURE);
    }
    Tensor* t = malloc(sizeof(Tensor));
    // gestisco i casi in cui potrebbe essermi passato il tensore dimensione
    if(s->size == 1) {
        t->ndim = 1;
        t->shape[0] = 1;
        t->shape[1] = s->data[0];
    } else if(s->size == 2 && s->data[0] == 1) {
        t->ndim = 1;
        t->shape[0] = 1;
        t->shape[1] = s->data[1];
    } else if (s->size == 2 && s->data[0] != 1) {
        t->ndim = 2;
        t->shape[0] = s->data[0];
        t->shape[1] = s->data[1];
    }
    t->size = t->shape[0] * t->shape[1];
    t->data = malloc(sizeof(float) * t->size);
    t->ref_count = 1;
    //
    t->isFilename = false;
    t->map_pointer = NULL;
    t->map_size = 0;
    // non parallelizzo come da istruzioni
    for(int i = 0; i < t->size; i++) {
        t->data[i] = (float)rand() / (float)RAND_MAX ;
    }
    push(stack, t);
    free_tensor(&s);
}

void relu(Stack* stack)
{
    Tensor* a = pop(stack);
    #pragma omp parallel for
    for(int i = 0; i < a->size; i++) {
        a->data[i] *= (a->data[i] > 0);
    }
    push(stack, a);
}

void min(Stack* stack)
{
    Tensor* a = pop(stack);
    Tensor* b = pop(stack);
    if(a->size != b->size) {
        printf("I tensori devono essere della stessa dimensione.\n");
        exit(EXIT_FAILURE);
    }
    Tensor* t = malloc(sizeof(Tensor));
    tensor_structure_copy(t, a);
    #pragma omp parallel for
    for(int i = 0; i < t->size; i++) {
        t->data[i] = a->data[i] * (a->data[i] < b->data[i]) + b->data[i] * (1 - (a->data[i] < b->data[i]));
    }
    push(stack, t);
    free_tensor(&a);
    free_tensor(&b);
}

void max(Stack* stack)
{
    Tensor* a = pop(stack);
    Tensor* b = pop(stack);
    if(a->size != b->size) {
        printf("I tensori devono essere della stessa dimensione.\n");
        exit(EXIT_FAILURE);
    }
    Tensor* t = malloc(sizeof(Tensor));
    tensor_structure_copy(t, a);
    #pragma omp parallel for
    for(int i = 0; i < t->size; i++) {
        t->data[i] = a->data[i] * (a->data[i] > b->data[i]) + b->data[i] * (1 - (a->data[i] > b->data[i]));
    }
    push(stack, t);
    free_tensor(&a);
    free_tensor(&b);
}

void somma_tens(Stack* stack)
{
    Tensor* a = pop(stack);
    Tensor* t = malloc(sizeof(Tensor));
    t->ndim = 1;
    t->size = 1;
    t->shape[0] = 1;
    t->shape[1] = 1;
    t->ref_count = 1;
    //
    t->isFilename = false;
    t->map_pointer = NULL;
    t->map_size = 0;
    t->data = malloc(sizeof(float) * t->size);
    float sum = 0;
    // parallelizzo con la reduction per evitare race conditions
    #pragma omp parallel for reduction(+:sum)
    for(int i = 0; i < a->size; i++) {
        sum += a->data[i];
    }
    t->data[0] = sum;
    push(stack, t);
    free_tensor(&a);
}

void fill(Stack* stack)
{
    Tensor* v = pop(stack);
    Tensor* s = pop(stack);
    if(s->size > 2) {
        printf("Errore! Impossibile inizializzare un tensore di più di 2 dimensioni.\n");
        exit(EXIT_FAILURE);
    }
    Tensor* t = malloc(sizeof(Tensor));
    if(s->size == 1) {
        t->ndim = 1;
        t->shape[0] = 1;
        t->shape[1] = s->data[0];
        t->size = s->data[0];
    } else {
        t->ndim = 2;
        t->shape[0] = s->data[0];
        t->shape[1] = s->data[1];
        t->size = s->data[0] * s->data[1];
    }
    t->data = malloc(sizeof(float) * t->size);
    //
    t->ref_count = 1;
    t->isFilename = false;
    t->map_pointer = NULL;
    t->map_size = 0;
    #pragma omp parallel for
    for(int i = 0; i < t->size; i++) {
        t->data[i] = v->data[i % v->size];
    }
    push(stack, t);
    free_tensor(&v);
    free_tensor(&s);
}

// stampa il tensore in cima allo stack e lo consuma
// prende in input lo stack
void print_tens(Stack* stack)
{
    Tensor* t = pop(stack);
    printf("Tensor(shape=[%d, %d], data=[ ", t->shape[0], t->shape[1]);
    for(int i = 0; i < t->size; i++)
    {
        printf("%f ", t->data[i]);
    }
    printf("])\n");
    free_tensor(&t);
    return;
}

void dup(Stack* stack)
{
    if(is_empty(stack)) {
        printf("Impossibile duplicare un elemento se lo stack è vuoto.\n");
        exit(EXIT_FAILURE);
    }
    Tensor* t = peek(stack);
    increment_ref_count(t);
    push(stack, t);
}

void swap(Stack* stack)
{
    if(stack->top < 1) {    // ricordo che lo stack ha il primo elemento in indice 0
        printf("Lo stack non ha sufficienti elementi per eseguire la swap.\n");
        exit(EXIT_FAILURE);
    }
    // lo faccio senza usare la pop che nel caso di tensori con ref count > 1 mi torna una copia del tensore e non l'originale
    Tensor* a = stack->tensors_pointer[stack->top];
    Tensor* b = stack->tensors_pointer[stack->top-1];
    stack->tensors_pointer[stack->top] = b;
    stack->tensors_pointer[stack->top-1] = a;
    return;
}

void over(Stack* stack)
{
    if(stack->top < 1) {
        printf("Lo stack non ha sufficienti elementi per eseguire la over.\n");
        exit(EXIT_FAILURE);
    }
    Tensor* t = stack->tensors_pointer[stack->top - 1];
    stack->tensors_pointer[stack->top - 1]->ref_count++;
    push(stack, t);
    return;
}

void drop(Stack* stack)
{
    Tensor* t = pop(stack);
    free_tensor(&t);
    return;
}

void read_pgm(Stack* stack)
{
    // uso la pop per estrarre un tensore in cui nei dati c'è il filename
    char* filename = pop_filename(stack);
    // apro in lettura binaria
    FILE* file = fopen(filename, "rb");
    if(file == NULL)
    {
        printf("Errore nell'apertura del file.\n");
        exit(EXIT_FAILURE);
    }
    // uso stat per ottenere informazioni sul file che mi servono per la mmap
    struct stat buffer;
	if(stat(filename, &buffer) == -1) {
        printf("Errore nel salvataggio della size\n");
		fclose(file);
		exit(EXIT_FAILURE);
    }
    int width;
    int height;
    int offset;
    off_t size;
    char* data;
    int valori_convertiti = fscanf(file, "%*s %i %i %*i\n", &width, &height);
    if(valori_convertiti != 2) {
		printf("Errore! EOF\n");
		fclose(file);
	    exit(EXIT_FAILURE);
	}
    offset = ftell(file);
    if(offset == -1) {
        printf("Errore nel salvataggio dell'offset\n");
		fclose(file);
		exit(EXIT_FAILURE);
    }
    size = buffer.st_size;
    data = mmap((void*)0, size, PROT_READ, MAP_PRIVATE, fileno(file), 0);
    if(data == MAP_FAILED) {
		printf("Errore nel mappare in memoria il file.\n");
		fclose(file);
		exit(EXIT_FAILURE);
	}
    //
    Tensor* t = malloc(sizeof(Tensor));
    if(height <= 1) {
        t->ndim = 1;
    } else {
        t->ndim = 2;
    }
    t->shape[0] = height;
    t->shape[1] = width;
    t->size = t->shape[0] * t->shape[1];
    t->ref_count = 1;
    t->isFilename = false;
    t->map_pointer = NULL;
    t->map_size = 0;
    t->data = malloc(sizeof(float) * t->size);
    unsigned char* pixels = (unsigned char*)data + offset;
    #pragma omp parallel for
    for(int i = 0; i < t->size; i++) {
        // divido per 255 per portare i valori da 0-255 a 0-1
        t->data[i] = (float)pixels[i] / 255;
    }
    push(stack, t);
    munmap(data, size);
    free(filename);
    fclose(file);
    return;
}

void write_pgm(Stack* stack)
{
    char* filename = pop_filename(stack);
    FILE* file = fopen(filename, "wb");
    if(file == NULL)
    {
        printf("Errore nell'apertura del file.\n");
        exit(EXIT_FAILURE);
    }
    Tensor* t = pop(stack);
    fprintf(file, "P5\n%d %d\n255\n", t->shape[1], t->shape[0]);
    unsigned char* data = malloc(sizeof(unsigned char) * t->size);
    #pragma omp parallel for
    for(int i = 0; i < t->size; i++) {
        float current = t->data[i];
        if(current < 0) {
            current = 0.0f;
        }
        if(current > 1) {
            current = 1.0f;
        }
        data[i] = (unsigned char)(current * 255.0f);
    }
    if(fwrite(data, sizeof(unsigned char), t->size, file) != t->size) {
        printf("Errore nella scrittura dell'immagine.\n");
        free(data);
        fclose(file);
        exit(EXIT_FAILURE);
    }
    free(data);
    free(filename);
    fclose(file);
    free_tensor(&t);
}

void read_file(Stack* stack)
{
    char* filename = pop_filename(stack);
    FILE* file = fopen(filename, "rb");
    if(file == NULL)
    {
        printf("Errore nell'apertura del file.\n");
        exit(EXIT_FAILURE);
    }
    // uso stat per ottenere la size necessaria per la mmap 
    struct stat buffer;
    if(stat(filename, &buffer) == -1) {
        printf("Errore nel salvataggio della size\n");
		fclose(file);
		exit(EXIT_FAILURE);
    }
    // mappatura del file
    void* mapped_tensor = mmap((void*)0, buffer.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fileno(file), 0);
    if(mapped_tensor == MAP_FAILED) {
        printf("Errore nella mappatura del file.\n");
        fclose(file);
        free(filename);
        exit(EXIT_FAILURE);
    }
    on_disk_tensor* odt = (on_disk_tensor*) mapped_tensor;
    // riempiamo il tensore coi dati mappati
    Tensor* t = malloc(sizeof(Tensor));
    t->shape[0] = odt->shape[0];
    t->shape[1] = odt->shape[1];
    t->size = t->shape[0] * t->shape[1];
    t->ndim = odt->ndim;
    t->data = (float*)((uint8_t*)mapped_tensor + odt->data_offset);
    t->map_pointer = mapped_tensor;
    t->map_size = buffer.st_size;
    t->ref_count = 1;
    t->isFilename = false;
    free(filename);
    fclose(file);
    push(stack, t);
    return;
}

void write_file(Stack* stack)
{
    char* filename = pop_filename(stack);
    FILE* file = fopen(filename, "wb");
    if(file == NULL)
    {
        printf("Errore nell'apertura del file.\n");
        exit(EXIT_FAILURE);
    }
    Tensor* t = pop(stack);
    // riempiamo la struct che andremo a salvare su disco
    on_disk_tensor odt;
    odt.shape[0] = t->shape[0];
    odt.shape[1] = t->shape[1];
    odt.ndim = t->ndim;
    odt.data_offset = 64;
    // scrittura della struct
    fwrite(&odt, sizeof(odt), 1, file);
    // scrittura del padding
    uint8_t padding[64 - sizeof(odt)] = {0};    // uso uint8_t perché ognuno pesa 8 bit (1 byte)
    fwrite(padding, 1, sizeof(padding), file);
    // scrittura dei dati
    fwrite(t->data, sizeof(float), t->size, file);
    free_tensor(&t);
    free(filename);
    fclose(file);
    return;
}