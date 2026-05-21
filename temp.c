char current;
char buffer[256];
int current_size = 0;
bool spazio = true;
int temp_size = INITIAL_TENSOR_SIZE;

Tensor* tensor = malloc(sizeof(Tensor));    // inizializzo sullo stack cosi poi con la free lo posso liberare nella free_tensor
tensor->size = 0;

tensor->data = malloc(sizeof(float) * INITIAL_TENSOR_SIZE);
current = fgetc(file);
do
    {
        if(spazio == true) {
            if(current_size > 0) {
                tensor->data[tensor->size] = atof(buffer);
                tensor->size = tensor->size + 1;
                check_tensor_size(tensor, &temp_size);
                current_size = 0;
                memset(buffer, 0, sizeof(buffer));  // svuoto il buffer, per usarla importo memset
            }
            if(isspace(current) == 0) {     // se è diverso da " "
                printf("char error: %c\n", current);
                printf("Errore nella forma del tensore.\n");
                exit(EXIT_FAILURE);
            } else {
                spazio = false;
                current = fgetc(file);
            }
        } else {
            if(isspace(current) != 0) {     // se è " "
                printf("Errore nella forma del tensore (doppio spazio!).\n");
                exit(EXIT_FAILURE);
            } else if (isdigit(current) || current == '.') {
                buffer[current_size] = current;
                current_size++;
                //
                current = fgetc(file);
                if(!isdigit(current) && current != '.') {
                    spazio = true;
                }
            }
        }
    } while (current != ']');


//controllare che ci sia almeno uno spazio
//saltare eventuali più spazi
//prendere i . e i -
//errore per tutto il resto

char current;
char buffer[256];
int current_size = 0;
bool spazio = true;
int temp_size = INITIAL_TENSOR_SIZE;

Tensor* tensor = malloc(sizeof(Tensor));    // inizializzo sullo stack cosi poi con la free lo posso liberare nella free_tensor
tensor->size = 0;

tensor->data = malloc(sizeof(float) * INITIAL_TENSOR_SIZE);
current = fgetc(file);
do
{
    if(spazio == true) {
        if(current_size > 0) {
            tensor->data[tensor->size] = atof(buffer);
            tensor->size++;
            check_tensor_size(tensor, &temp_size);
            current_size = 0;
            memset(buffer, 0, sizeof(buffer));  // svuoto il buffer se ho scritto il numero corrente
        }
        if(isspace(current) == 0) {  // se è diverso da " "
            printf("Errore nella forma del tensore.\n");
            exit(EXIT_FAILURE);
        } else {
            spazio = false;
        }
    } else {
        while(isspace(current) != 0) {  // se è " "
            current = fgetc(file);
        }
        if(isdigit(current) || current == '.' || current == '-') {
            buffer[current_size] = current;
            current_size++;
            current = fgetc(file);
            if(!isdigit(current) && current != '.' && current != '-') {
                spazio = true;
            }
        }
    }
} while(current != ']');