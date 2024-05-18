struct CLAVE_get_value_result {
    int status;
    int clave;
    char valor1[256]; 
    int N_val;     
    double V_valor2[32]; 
};

program CLAVE {
    version CLAVE_V1 {
        int INIT(void) = 1;
        int SET_VALUE(struct CLAVE_get_value_result) = 2;
        struct CLAVE_get_value_result GET_VALUE(int clave) = 3;
        int MODIFY_VALUE(struct CLAVE_get_value_result) = 4;
        int DELETE_KEY(int) = 5;
        int EXIST(int) = 6;
    } = 1; 
} = 99; 
