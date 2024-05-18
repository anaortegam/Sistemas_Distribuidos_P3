#include "clave.h"
#include "claves.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mensajes.h"
#undef init
#undef set_value
#undef get_value
int init() {
    CLIENT *clnt;
    enum clnt_stat retval;
    int res;
    char *host;
    int err;
	//Funcion para obtener las variables de entorno
    err = obtenerVariablesEntorno();
    if (err == -1){
        printf("Error en las variables de entorno");
        return -1;
    }
    host = obtenerIP();
    if (host == NULL) {
        printf("Error: IP_TUPLAS no definido correctamente\n");
        return -1;
    }

    clnt = clnt_create(host, CLAVE, CLAVE_V1, "tcp");
    if (clnt == NULL) {
        clnt_pcreateerror(host);
        return -1;
    }

    retval = init_1(&res, clnt); 
    if (retval != RPC_SUCCESS) {
        clnt_perror(clnt, "call failed:");
    }
    clnt_destroy(clnt);
    return res;
	
    
}

int set_value(int key,char *value1, int N_value2, double *V_value2){
    CLIENT *clnt;
    enum clnt_stat retval;
    struct CLAVE_get_value_result pet;
    int res;
    char *host;
    int err;

    if(N_value2>32){
        perror("Vector demasiado grande");
        res = -1;
        return res;

    }
    if(N_value2<0){
        perror("Argumento N negativo");
        res = -1;
        return res;
    }
    
    //Funcion para obtener las variables de entorno
    err = obtenerVariablesEntorno();
    if (err == -1){
        printf("Error en las variables de entorno");
        return -1;
    }
    host = obtenerIP();
    if (host == NULL) {
        printf("Error: IP_TUPLAS no definido correctamente\n");
        return -1;
    }

    clnt = clnt_create(host, CLAVE, CLAVE_V1, "tcp");
    if (clnt == NULL) {
        clnt_pcreateerror(host);
        return -1;
    }
    pet.clave= key;
    strcpy(pet.valor1, value1);
    pet.N_val= N_value2;

    for(int i=0; i<N_value2;i++){
        pet.V_valor2[i] = V_value2[i];
    }
	retval = set_value_1(pet, &res, clnt);
	if (retval!= RPC_SUCCESS) {
		clnt_perror (clnt, "call failed");
	}
    clnt_destroy(clnt);
	return res;
}

int get_value(int key,char *value1, int *N_value, double *V_value2){
    CLIENT *clnt;
    enum clnt_stat retval;
    struct CLAVE_get_value_result res;
    char *host;
    int err;
	
    //Funcion para obtener las variables de entorno
    err = obtenerVariablesEntorno();
    if (err == -1){
        printf("Error en las variables de entorno");
        return -1;
    }
    host = obtenerIP();
    if (host == NULL) {
        printf("Error: IP_TUPLAS no definido correctamente\n");
        return -1;
    }

    clnt = clnt_create(host, CLAVE, CLAVE_V1, "tcp");
    if (clnt == NULL) {
        clnt_pcreateerror(host);
        return -1;
    }
	retval = get_value_1(key, &res, clnt);
	if (retval!= RPC_SUCCESS) {
		clnt_perror (clnt, "call failed");

        return -1;
	}
    clnt_destroy(clnt);
	
    strcpy(value1, res.valor1); 
    *N_value = res.N_val;
   
    for (int i = 0; i< res.N_val; i++){
        V_value2[i] = res.V_valor2[i];
    }
    return res.status;
}


int modify_value(int key, char *value1, int N_value2, double *V_value2){
    CLIENT *clnt;
    enum clnt_stat retval;
    int res;
    char *host;
    int err;
    struct CLAVE_get_value_result pet;

    if(N_value2>32){
        perror("Vector demasiado grande");
        res = -1;
        return res;

    }
    if(N_value2<0){
        perror("Argumento N negativo");
        res = -1;
        return res;
    }
    
    //Funcion para obtener las variables de entorno
    err = obtenerVariablesEntorno();
    if (err == -1){
        printf("Error en las variables de entorno");
        return -1;
    }
    host = obtenerIP();
    if (host == NULL) {
        printf("Error: IP_TUPLAS no definido correctamente\n");
        return -1;
    }

    clnt = clnt_create(host, CLAVE, CLAVE_V1, "tcp");
    if (clnt == NULL) {
        clnt_pcreateerror(host);
        return -1;
    }
    pet.clave= key;
    strcpy(pet.valor1, value1);
    pet.N_val= N_value2;
	
    for(int i=0; i<N_value2;i++){
        pet.V_valor2[i] = V_value2[i];
    }

	retval = modify_value_1(pet, &res, clnt);
	if (retval!= RPC_SUCCESS) {
		clnt_perror (clnt, "call failed");
	}
    clnt_destroy(clnt);
	return res;

}

int delete_key(int key){
    CLIENT *clnt;
    enum clnt_stat retval;
    int res;
    char *host;
    int err;
    
    //Funcion para obtener las variables de entorno
    err = obtenerVariablesEntorno();
    if (err == -1){
        printf("Error en las variables de entorno");
        return -1;
    }
    host = obtenerIP();
    if (host == NULL) {
        printf("Error: IP_TUPLAS no definido correctamente\n");
        return -1;
    }

    clnt = clnt_create(host, CLAVE, CLAVE_V1, "tcp");
    if (clnt == NULL) {
        clnt_pcreateerror(host);
        return -1;
    }
    
	retval = delete_key_1(key, &res, clnt);
	if (retval!= RPC_SUCCESS) {
		clnt_perror (clnt, "call failed");
	}
    clnt_destroy(clnt);
	return res;
}

int exist(int key){
    CLIENT *clnt;
    enum clnt_stat retval;
    int res;
    char *host;
    int err;
    
    //Funcion para obtener las variables de entorno
    err = obtenerVariablesEntorno();
    if (err == -1){
        printf("Error en las variables de entorno");
        return -1;
    }
    host = obtenerIP();
    if (host == NULL) {
        printf("Error: IP_TUPLAS no definido correctamente\n");
        return -1;
    }

    clnt = clnt_create(host, CLAVE, CLAVE_V1, "tcp");
    if (clnt == NULL) {
        clnt_pcreateerror(host);
        return -1;
    }
    
	retval = exist_1(key, &res, clnt);
	if (retval!= RPC_SUCCESS) {
		clnt_perror (clnt, "call failed");
	}
    clnt_destroy(clnt);
	return res;
}
