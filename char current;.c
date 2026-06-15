    char current;
    char buffer[256];
    int current_size = 0;
    bool spazio = true;     // spazio == true significa che ci aspetta uno spazio
    int temp_size = INITIAL_TENSOR_SIZE;

    Tensor* tensor = malloc(sizeof(Tensor));
    tensor->size = 0;
    
    tensor->data = malloc(sizeof(float) * INITIAL_TENSOR_SIZE);
    current = fgetc(file);
    

    while(current != ']')
    {
        if(spazio == true)
        {
            if(isspace(current) == 0) {  // se non è " "
                printf("Errore nella forma del tensore (spazio non presente)\n");
                exit(EXIT_FAILURE);
            } else {
                spazio = false;
            }
        }
        else if(spazio == false && isspace(current) == 0)  // se non è " "
        {
            if(isdigit(current) || current == '.' || current == '-')
            {
                buffer[current_size] = current;
                current_size++;
            }
        }
        current = fgetc(file);
        if(isspace(current) != 0 && current_size > 0) // se è " "
        {
            buffer[current_size] = '\0';
            tensor->data[tensor->size] = atof(buffer);
            tensor->size++;
            current_size = 0;
            memset(buffer, 0, sizeof(buffer));  // svuoto il buffer se ho scritto il numero corrente
            current = fgetc(file);
            spazio = true;
        }
    }



    truncate_tensor_size(tensor);