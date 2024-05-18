#include "clave.h"

typedef struct Tupla {
    int clave;
    char *valor1; // Máximo de 255 caracteres + 1 para el carácter nulo de terminación de cadena
    int N;
    double *valor2;
    struct Tupla *siguiente;
} Tupla;

typedef struct Tupla * List;

List l = NULL;

int printList() {
    List aux = l;
    while(aux != NULL){
        printf("Nueva tupla\n");
        printf("Clave=%d    value1=%s   N=%d\n", aux->clave, aux->valor1, aux->N);
        printf("Valor2:");
        for(int i = 0; i < aux->N; i++){
            printf(" %.6f", aux->valor2[i]); // Imprimir valor2[i] con 6 decimales de precisión
        }
        printf("\n\n"); // Agregar una línea en blanco después de cada tupla
        aux = aux->siguiente;
    }
    return 0;
}

bool_t

init_1_svc(int *result, struct svc_req *rqstp)
{
	bool_t retval;

	while (l != NULL) {
        List temp = l;
        l = (l)->siguiente;
        free(temp->valor2);
        free(temp->valor1);
        free(temp);
    }
    l = NULL;

    int n = 0;
    retval = TRUE;
    *result = n;
    printf("---------------------Lista inicializada--------------------\n");
    printf("\n\n"); // Agregar una línea en blanco
    return retval;
}

bool_t
set_value_1_svc(struct CLAVE_get_value_result arg1, int *result,  struct svc_req *rqstp)
{
	bool_t retval;
	List aux = l;
    while (aux != NULL) {
        if (aux->clave == arg1.clave) {
            printf("----Error: Ya existe un elemento con la clave %d. No se puede insertar----\n", arg1.clave);
            printf("\n\n"); // Agregar una línea en blanco
			arg1.status = -1;
            *result = -1; // Clave duplicada, retorna error
			retval = TRUE;
			return retval;
        }
        aux = aux->siguiente;
    }
    struct Tupla *ptr;

    ptr = (struct Tupla *) malloc(sizeof(struct Tupla));
    if (ptr == NULL){ 
		arg1.status = -1;
        *result = -1; 
		retval = FALSE;
		return retval;
	}
    ptr->clave = arg1.clave;
	ptr->valor1 = malloc(strlen(arg1.valor1) + 1); // +1 para el carácter nulo de terminación de cadena
	strcpy(ptr->valor1, arg1.valor1);

	ptr->N = arg1.N_val;
	ptr->valor2 = malloc(sizeof(double) * arg1.N_val);
	for (int i = 0; i < arg1.N_val; i++) {
		ptr->valor2[i] = arg1.V_valor2[i];
	}
    ptr->siguiente = l;
    l = ptr;
	printf("------------Tupla de clave %d insertada------------\n", arg1.clave);
    printList();
    retval = TRUE;
	arg1.status = 0;
    *result = 0;
	return retval;
}

bool_t
get_value_1_svc(int clave, struct CLAVE_get_value_result *result,  struct svc_req *rqstp)
{
	bool_t retval;
	// Verificar si la lista está vacía
	if (l == NULL) {
        printf("---------------------Get Value--------------------\n");
	    printf("\n\n"); // Agregar una línea en blanco
		retval = TRUE;
		result->status = -1;
        return retval;
    }
    // Buscar la tupla con la clave especificada
    List aux = l;
    while (aux != NULL) {
        if (aux->clave == clave) {
            
            // Se encontró la clave, copiar los valores a la estructura de respuesta
            result->clave = aux->clave;
            strcpy(result->valor1, aux->valor1);
            result->N_val = aux->N;
            for (int i = 0; i < result->N_val; i++) {
                result->V_valor2[i] = aux->valor2[i];
            }
            printf("------------Tupla de clave %d encontrada------------\n", clave);
            printf("\n\n"); // Agregar una línea en blanco
			retval = TRUE;
			result->status = 0;
			return retval;
        }
        aux = aux->siguiente;
    }
    perror("------------Se ha intentado acceder a una clave que no existe (get_value)--------\n");
    printf("\n\n"); // Agregar una línea en blanco
	result->status = -1;
	retval = TRUE;
	return retval;
}

bool_t
modify_value_1_svc(struct CLAVE_get_value_result arg1, int *result,  struct svc_req *rqstp)
{	
	bool_t retval;
	if (l == NULL) {
        perror("------------La lista está vacía. Modify_value--------------\n");
        printf("\n\n"); // Agregar una línea en blanco
		retval = TRUE;
		arg1.status = -1;
		*result=-1;
        return retval;
    }
    // Buscar la tupla con la clave especificada
    List aux = l;
    while (aux != NULL) {
        if (aux->clave == arg1.clave) {
            // Se encontró la clave, copiar los valores a la estructura de respuesta
            aux->clave = arg1.clave;
            free(aux->valor1);
            aux->valor1 = malloc(strlen(arg1.valor1) + 1);
            strcpy(aux->valor1, arg1.valor1);
            aux->N = arg1.N_val;
            free(aux->valor2);
            aux->valor2 = malloc(arg1.N_val * sizeof(double));
            for(int i = 0; i < arg1.N_val; i++){
                aux->valor2[i] = arg1.V_valor2[i];
            }
			retval = TRUE;
			arg1.status=0;
			*result = 0;
            printf("------------Tupla de clave %d modificada------------\n", aux->clave);
            printList();
            return retval;
        }
        aux = aux->siguiente;
    }
    perror("--------------Se ha intentado modificar a una clave que no existe (modify_value)----------------\n");
    printf("\n\n"); // Agregar una línea en blanco
	retval = TRUE;
    arg1.status = -1;
	*result=-1;
	return retval;
}

bool_t
delete_key_1_svc(int arg1, int *result,  struct svc_req *rqstp)
{
	bool_t retval;

	if (l == NULL) {
        perror("-------------La lista está vacía (delete_key)--------------------\n");
        printf("\n\n"); // Agregar una línea en blanco
        retval = TRUE;
		*result = -1;
		return retval;
    }

    List current = l;
    List previous = NULL;

    // Buscar el nodo con la clave key
    while (current != NULL && current->clave != arg1) {
        previous = current;
        current = current->siguiente;
    }

    // Si current es NULL, significa que no se encontró la clave
    if (current == NULL) {
        perror("--------No se ha encontrado la clave para eliminar---------\n");
        printf("\n\n"); // Agregar una línea en blanco
        retval = TRUE;
		*result = -1;
		return retval;
    }

    // Si previous es NULL, el nodo a eliminar es el primero de la lista
    if (previous == NULL) {
        l = current->siguiente;
    } else {
        // El nodo a eliminar está en el medio o al final de la lista
        previous->siguiente = current->siguiente;
    }
	printf("------------Tupla de clave %d eliminada------------\n", arg1);
    printList();
    // Liberar la memoria del nodo eliminado
    free(current->valor2); // Liberar la memoria del arreglo valor2
    free(current->valor1); // Liberar la memoria de la cadena valor1
    free(current); // Liberar la memoria del nodo
    retval = TRUE;
	*result = 0;
	return retval;
}

bool_t
exist_1_svc(int arg1, int *result,  struct svc_req *rqstp)
{
	bool_t retval;

	int encontrado = 0;
    
    if (l == NULL) {
        perror("--------------La lista está vacía. Exixts----------------\n");
        printf("\n\n"); // Agregar una línea en blanco
        retval = TRUE;
		*result = -1;
		return retval;
    }
    List aux = l;
    while (aux != NULL) {
        if (aux->clave == arg1) {
            // Se encontró la clave, copiar los valores a la estructura de respuesta
            encontrado = 1;
        }
        aux = aux->siguiente;
        
    }
    if(encontrado == 0){
       	printf("----------------La clave %d no existe----------------\n", arg1);
        printf("\n\n"); // Agregar una línea en blanco
        retval = TRUE;
		*result = 0;
		return retval;
    }
    else{
        printf("----------------La clave %d existe----------------\n", arg1);
        printf("\n\n"); // Agregar una línea en blanco
        retval = TRUE;
		*result = 1;
		return retval;
    }
}

int
clave_1_freeresult (SVCXPRT *transp, xdrproc_t xdr_result, caddr_t result)
{
	xdr_free (xdr_result, result);

	/*
	 * Insert additional freeing code here, if needed
	 */

	return 1;
}