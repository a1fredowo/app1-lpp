#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct order {
    int pizza_id;
    int order_id;
    char pizza_name_id[50];
    float quantity;
    char order_date[10];
    char order_time[10];
    float unit_price;
    float total_price;
    char pizza_size[2];
    char pizza_category[50];
    char pizza_ingredients[200];
    char pizza_name[50];
};

// se define la función de tipo de puntero a función para las métricas
typedef char* (*metric)(int *size, struct order *orders);

// se definen las metricas
char* pizzas_mas_vendidas(int *size, struct order *orders);
char* pizzas_menos_vendidas(int *size, struct order *orders);
char* fecha_con_mas_ventas(int *size, struct order *orders);
char* fecha_con_menos_ventas(int *size, struct order *orders);
char* fecha_pizzas_mas_vendidas(int *size, struct order *orders);
char* fecha_pizzas_menos_ventas(int *size, struct order *orders);
char* promedio_pizzas_por_orden(int *size, struct order *orders);
char* promedio_pizzas_por_dia(int *size, struct order *orders);
char* ingrediente_mas_vendido(int *size, struct order *orders);
char* pizzas_por_categoria_vendidas(int *size, struct order *orders);


int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Error, escriba los archivos como: %s <archivo csv> <metrica1> <metrica2> ...\n", argv[0]);
        return 1;
    }

    FILE *archivo = fopen(argv[1], "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo %s\n", argv[1]);
        return 1;
    }

    struct order orders[1000]; // asumiendo que hay como máximo 1000 órdenes
    char linea[256];
    int i = 0;

    // skipear la primera línea
    fgets(linea, sizeof(linea), archivo);

    while (fgets(linea, sizeof(linea), archivo)) {
        char *token;
        // se crean los tokens para separar los datos del archivo csv y asi poder almacenarlos en el struct
        token = strtok(linea, ",");
        orders[i].pizza_id = atoi(token);

        token = strtok(NULL, ",");
        orders[i].order_id = atoi(token);

        token = strtok(NULL, ",");
        strcpy(orders[i].pizza_name_id, token);

        token = strtok(NULL, ",");
        orders[i].quantity = atof(token);

        token = strtok(NULL, ",");
        strcpy(orders[i].order_date, token);

        token = strtok(NULL, ",");
        strcpy(orders[i].order_time, token);

        token = strtok(NULL, ",");
        orders[i].unit_price = atof(token);

        token = strtok(NULL, ",");
        orders[i].total_price = atof(token);

        token = strtok(NULL, ",");
        strcpy(orders[i].pizza_size, token);

        token = strtok(NULL, ",");
        strcpy(orders[i].pizza_category, token);

        token = strtok(NULL, "\"");
        strcpy(orders[i].pizza_ingredients, token);

        token = strtok(NULL, ",");
        strcpy(orders[i].pizza_name, token);

        // se elimina el salto de línea al final del nombre de la pizza
        size_t len = strlen(orders[i].pizza_name);
        if (orders[i].pizza_name[len - 1] == '\n') {
            orders[i].pizza_name[len - 1] = '\0';
        }
        i++;
    }
    
    fclose(archivo);

    // se define un arreglo de punteros a funciones para las métricas
    metric metricas[] = {pizzas_mas_vendidas, pizzas_menos_vendidas, fecha_con_mas_ventas, 
                        fecha_con_menos_ventas, fecha_pizzas_mas_vendidas, fecha_pizzas_menos_ventas, 
                        promedio_pizzas_por_orden, promedio_pizzas_por_dia, 
                        ingrediente_mas_vendido, pizzas_por_categoria_vendidas};

    // se obtiene el número de métricas
    int num_metricas = sizeof(metricas) / sizeof(metricas[0]);

    // se definen los nombres de las métricas
    const char* nom_metricas[] = {"pms", "pls", "dms", "dls", "dmsp", "dlsp", "apo", "apd", "ims", "hp"};

    // se itera sobre las métricas proporcionadas como argumentos de la línea de comandos
    for (int j = 2; j < argc; j++) {
        // busca la métrica en el arreglo de nombres de métricas
        int index_metrica = -1;
        for (int k = 0; k < num_metricas; k++) {
            if (strcmp(argv[j], nom_metricas[k]) == 0) {
                index_metrica = k;
                break;
            }
        }

        // verifica si se encontró la métrica y la llama
        if (index_metrica != -1) {
            char* result = metricas[index_metrica](&i, orders);
            printf("%s\n", result);
            free(result);
        } else {
            printf("Metrica invalida: %s\n", argv[j]);
            return 1;
        }
    }
    return 0;
}

char* pizzas_mas_vendidas(int *size, struct order *orders) {
    // se crea una estructura para almacenar las cantidades de cada pizza
    struct {
        char nombre_id_pizza[50];
        char nombre_pizza[50];
        float cantidad;
    } cantidades_pizzas[1000];

    int cantidad_pizzas = 0;

    for (int i = 0; i < *size; i++) {
        int encontrado = 0;
        for (int j = 0; j < cantidad_pizzas; j++) { // se itera sobre las pizzas únicas
            if (strcmp(cantidades_pizzas[j].nombre_id_pizza, orders[i].pizza_name_id) == 0) {
                cantidades_pizzas[j].cantidad += orders[i].quantity;
                encontrado = 1;
                break;
            }
        }

        if (!encontrado) { // si la pizza no está en el arreglo, agregarla
            strcpy(cantidades_pizzas[cantidad_pizzas].nombre_id_pizza, orders[i].pizza_name_id);
            strcpy(cantidades_pizzas[cantidad_pizzas].nombre_pizza, orders[i].pizza_name);
            cantidades_pizzas[cantidad_pizzas].cantidad = orders[i].quantity;
            cantidad_pizzas++;
        }
    }

    float cantidad_maxima = 0.0;
    char *pizza_mas_vendida = NULL;
    for (int i = 0; i < cantidad_pizzas; i++) { // se busca la pizza con la cantidad más alta
        if (cantidades_pizzas[i].cantidad > cantidad_maxima) {
            cantidad_maxima = cantidades_pizzas[i].cantidad;
            pizza_mas_vendida = cantidades_pizzas[i].nombre_pizza; 
        }
    }

    char* result = (char*)malloc(100 * sizeof(char));
    snprintf(result, 100, "Pizza más vendida: %s", pizza_mas_vendida);

    return result;
}

char* pizzas_menos_vendidas(int *size, struct order *orders) {
    struct {
        char nombre_id_pizza[50];
        char nombre_pizza[50];
        float cantidad;
    } cantidades_pizzas[1000]; // arreglo para almacenar las cantidades de cada pizza

    int contador_pizzas = 0; // contador para el número de pizzas únicas

    for (int i = 0; i < *size; i++) {
        int encontrado = 0; // Bandera para verificar si la pizza ya está en el arreglo
        // Iterar sobre las pizzas únicas
        for (int j = 0; j < contador_pizzas; j++) {
            // Si la pizza ya está en el arreglo, incrementar su cantidad
            if (strcmp(cantidades_pizzas[j].nombre_id_pizza, orders[i].pizza_name_id) == 0) {
                cantidades_pizzas[j].cantidad += orders[i].quantity;
                encontrado = 1;
                break;
            }
        }

        // si la pizza no está en el arreglo, agregarla
        if (!encontrado) {
            strcpy(cantidades_pizzas[contador_pizzas].nombre_id_pizza, orders[i].pizza_name_id);
            strcpy(cantidades_pizzas[contador_pizzas].nombre_pizza, orders[i].pizza_name);
            cantidades_pizzas[contador_pizzas].cantidad = orders[i].quantity;
            contador_pizzas++;
        }
    }

    // se encuentra la pizza con la cantidad más baja
    float cantidad_minima = 10000;
    char *pizza_menos_vendida = NULL;
    for (int i = 0; i < contador_pizzas; i++) {
        if (cantidades_pizzas[i].cantidad < cantidad_minima) {
            cantidad_minima = cantidades_pizzas[i].cantidad;
            pizza_menos_vendida = cantidades_pizzas[i].nombre_pizza;
        }
    }

    char* result = (char*)malloc(100 * sizeof(char));
    snprintf(result, 100, "Pizza menos vendida: %s", pizza_menos_vendida);

    return result;
}

// y asi son todas las métricas, se crean mapas para almacenar los datos y luego se buscan los valores pedidos dependiendo de la métrica

char* fecha_con_mas_ventas(int *size, struct order *orders) {
    struct {
        char fecha_pedido[10];
        float ventas_totales;
    } ventas_por_fecha[1000];

    int cantidad_fechas = 0;

    for (int i = 0; i < *size; i++) {
        int encontrado = 0;
        for (int j = 0; j < cantidad_fechas; j++) {
            if (strcmp(ventas_por_fecha[j].fecha_pedido, orders[i].order_date) == 0) {
                ventas_por_fecha[j].ventas_totales += orders[i].total_price;
                encontrado = 1;
                break;
            }
        }

        if (!encontrado) {
            strcpy(ventas_por_fecha[cantidad_fechas].fecha_pedido, orders[i].order_date);
            ventas_por_fecha[cantidad_fechas].ventas_totales = orders[i].total_price;
            cantidad_fechas++;
        }
    }

    float ventas_maximas = 0;
    char *fecha_mas_ventas = NULL;
    for (int i = 0; i < cantidad_fechas; i++) {
        if (ventas_por_fecha[i].ventas_totales > ventas_maximas) {
            ventas_maximas = ventas_por_fecha[i].ventas_totales;
            fecha_mas_ventas = ventas_por_fecha[i].fecha_pedido;
        }
    }
    char *result = (char*)malloc(100 * sizeof(char));
    sprintf(result, "Fecha con más ventas en términos de dinero: %s (Dinero: %.2f)", fecha_mas_ventas, ventas_maximas);
    return result;
}

char* fecha_con_menos_ventas(int *size, struct order *orders) {
    struct {
        char fecha_pedido[10];
        float ventas_totales;
    } ventas_por_fecha[1000];

    int cantidad_fechas = 0;

    for (int i = 0; i < *size; i++) {
        int encontrado = 0;
        for (int j = 0; j < cantidad_fechas; j++) {
            if (strcmp(ventas_por_fecha[j].fecha_pedido, orders[i].order_date) == 0) {
                ventas_por_fecha[j].ventas_totales += orders[i].total_price;
                encontrado = 1;
                break;
            }
        }

        if (!encontrado) {
            strcpy(ventas_por_fecha[cantidad_fechas].fecha_pedido, orders[i].order_date);
            ventas_por_fecha[cantidad_fechas].ventas_totales = orders[i].total_price;
            cantidad_fechas++;
        }
    }

    float ventas_minimas = 1000000.0f;
    char* fecha_menos_ventas = NULL;
    for (int i = 0; i < cantidad_fechas; i++) {
        if (ventas_por_fecha[i].ventas_totales < ventas_minimas) {
            ventas_minimas = ventas_por_fecha[i].ventas_totales;
            fecha_menos_ventas = ventas_por_fecha[i].fecha_pedido;
        }
    }
    char* result = (char*)malloc(100 * sizeof(char));
    sprintf(result, "Fecha con menos ventas en términos de dinero: %s (Dinero: %.2f)", fecha_menos_ventas, ventas_minimas);
    return result;
}

char* fecha_pizzas_mas_vendidas(int *size, struct order *orders) {
    struct {
        char fecha_pedido[10];
        int total_pizzas;
    } ventas_por_fecha[1000];

    int cantidad_fechas = 0;

    for (int i = 0; i < *size; i++) {
        int encontrado = 0;
        for (int j = 0; j < cantidad_fechas; j++) {
            if (strcmp(ventas_por_fecha[j].fecha_pedido, orders[i].order_date) == 0) {
                ventas_por_fecha[j].total_pizzas += orders[i].quantity;
                encontrado = 1;
                break;
            }
        }

        if (!encontrado) {
            strcpy(ventas_por_fecha[cantidad_fechas].fecha_pedido, orders[i].order_date);
            ventas_por_fecha[cantidad_fechas].total_pizzas = orders[i].quantity;
            cantidad_fechas++;
        }
    }

    int max_pizzas = 0;
    char *fecha_mas_vendida = NULL;
    for (int i = 0; i < cantidad_fechas; i++) {
        if (ventas_por_fecha[i].total_pizzas > max_pizzas) {
            max_pizzas = ventas_por_fecha[i].total_pizzas;
            fecha_mas_vendida = ventas_por_fecha[i].fecha_pedido;
        }
    }

    char *result = (char*)malloc(100 * sizeof(char));
    sprintf(result, "Fecha con más ventas de pizzas: %s (Cantidad de pizzas: %d)", fecha_mas_vendida, max_pizzas);
    return result;
}

char* fecha_pizzas_menos_ventas(int *size, struct order *orders) {
    struct ventas_fecha {
        char fecha[10];
        int ventas;
    };

    struct ventas_fecha ventas_por_fecha[1000];
    int cantidad_fechas = 0;

    for (int i = 0; i < *size; i++) {
        int encontrado = 0;
        for (int j = 0; j < cantidad_fechas; j++) {
            if (strcmp(ventas_por_fecha[j].fecha, orders[i].order_date) == 0) {
                ventas_por_fecha[j].ventas += orders[i].quantity;
                encontrado = 1;
                break;
            }
        }

        if (!encontrado) {
            strcpy(ventas_por_fecha[cantidad_fechas].fecha, orders[i].order_date);
            ventas_por_fecha[cantidad_fechas].ventas = orders[i].quantity;
            cantidad_fechas++;
        }
    }

    int ventas_minimas = ventas_por_fecha[0].ventas;
    char* fecha_menos_vendida = ventas_por_fecha[0].fecha;

    for (int i = 1; i < cantidad_fechas; i++) {
        if (ventas_por_fecha[i].ventas < ventas_minimas) {
            ventas_minimas = ventas_por_fecha[i].ventas;
            fecha_menos_vendida = ventas_por_fecha[i].fecha;
        }
    }

    char* result = malloc(100 * sizeof(char));
    sprintf(result, "Fecha con menos ventas de pizzas: %s, (Cantidad de pizzas: %d)", fecha_menos_vendida, ventas_minimas);
    return result;
}

char* promedio_pizzas_por_orden(int *size, struct order *orders) {
    int total_ordenes = 0;
    float total_pizzas = 0;

    // se encuentra el numero máximo de ordenes
    int maximo_id_orden = 0;
    for (int i = 0; i < *size; i++) {
        if (orders[i].order_id > maximo_id_orden) {
            maximo_id_orden = orders[i].order_id;
        }
    }

    for (int i = 1; i <= maximo_id_orden; i++) {
        float pizzas_orden = 0;
        int conteo_ordenes = 0;

        for (int j = 0; j < *size; j++) {
            if (orders[j].order_id == i) {
                pizzas_orden += orders[j].quantity;
                conteo_ordenes++;
            }
        }

        if (conteo_ordenes > 0) {
            total_ordenes++;
            total_pizzas += pizzas_orden;
        }
    }

    float promedio_global = total_pizzas / total_ordenes;

    char* result = (char*)malloc(100 * sizeof(char));
    snprintf(result, 100, "Promedio de pizzas por orden: %.2f", promedio_global);
    return result;
}

char* promedio_pizzas_por_dia(int *size, struct order *orders) {
    struct {
        char fecha_pedido[10];
        int total_pizzas;
        int conteo;
    } ventas_pizzas_fecha[1000];

    int conteo_fechas = 0;

    // se inicializa el arreglo
    for (int i = 0; i < 1000; i++) {
        ventas_pizzas_fecha[i].total_pizzas = 0;
        ventas_pizzas_fecha[i].conteo = 0;
    }

    for (int i = 0; i < *size; i++) {
        int encontrado = 0;
        for (int j = 0; j < conteo_fechas; j++) {
            if (strcmp(ventas_pizzas_fecha[j].fecha_pedido, orders[i].order_date) == 0) {
                ventas_pizzas_fecha[j].total_pizzas += orders[i].quantity;
                ventas_pizzas_fecha[j].conteo++;
                encontrado = 1;
                break;
            }
        }

        if (!encontrado) {
            strcpy(ventas_pizzas_fecha[conteo_fechas].fecha_pedido, orders[i].order_date);
            ventas_pizzas_fecha[conteo_fechas].total_pizzas += orders[i].quantity;
            ventas_pizzas_fecha[conteo_fechas].conteo++;
            conteo_fechas++;
        }
    }

    float total_pizzas = 0;
    int total_dias = conteo_fechas;

    for (int i = 0; i < conteo_fechas; i++) {
        total_pizzas += ventas_pizzas_fecha[i].total_pizzas;
    }

    if (total_dias == 0) {
        return strdup("No hay suficientes datos para calcular el promedio de pizzas por día."); // para evitar división por cero
    }

    char* result = (char*)malloc(100 * sizeof(char));
    snprintf(result, 100, "Promedio de pizzas por día: %.2f", total_pizzas / total_dias);
    return result;
}

char* ingrediente_mas_vendido(int *size, struct order *orders) {
    struct {
        char nombre_ingrediente[50];
        float cantidad;
    } cantidades_ingredientes[1000];

    int conteo_ingredientes = 0;

    for (int i = 0; i < *size; i++) {
        // se divide la lista de ingredientes para cada orden
        char copia_ingredientes[200]; // se hace una copia de la cadena de ingredientes porque strtok modifica la cadena original
        strcpy(copia_ingredientes, orders[i].pizza_ingredients);
        char *ingrediente = strtok(copia_ingredientes, ",");

        while (ingrediente != NULL) {
            // Eliminar espacios iniciales
            while(ingrediente[0] == ' ') {
                ingrediente++;
            }

            int encontrado = 0;
            for (int j = 0; j < conteo_ingredientes; j++) {
                if (strcmp(cantidades_ingredientes[j].nombre_ingrediente, ingrediente) == 0) {
                    // si el ingrediente ya está en el mapa, incrementar su cantidad
                    cantidades_ingredientes[j].cantidad += orders[i].quantity;
                    encontrado = 1;
                    break;
                }
            }

            if (!encontrado) {
                strcpy(cantidades_ingredientes[conteo_ingredientes].nombre_ingrediente, ingrediente);
                cantidades_ingredientes[conteo_ingredientes].cantidad = orders[i].quantity;
                conteo_ingredientes++;
            }

            // se va al siguiente ingrediente
            ingrediente = strtok(NULL, ",");
        }
    }

    float cantidad_maxima = 0;
    char *ingrediente_mas_vendido = NULL;
    for (int i = 0; i < conteo_ingredientes; i++) {
        if (cantidades_ingredientes[i].cantidad > cantidad_maxima) {
            cantidad_maxima = cantidades_ingredientes[i].cantidad;
            ingrediente_mas_vendido = cantidades_ingredientes[i].nombre_ingrediente;
        }
    }

    char* result = (char*)malloc(100 * sizeof(char));
    sprintf(result, "Ingrediente más vendido: %s", ingrediente_mas_vendido);
    return result;
}

char* pizzas_por_categoria_vendidas(int *size, struct order *orders) {
    struct {
        char nombre_categoria[50];
        int total_pizzas;
    } ventas_por_categoria[1000];

    int conteo_categorias = 0;

    for (int i = 0; i < *size; i++) {
        int encontrado = 0;
        for (int j = 0; j < conteo_categorias; j++) {
            if (strcmp(ventas_por_categoria[j].nombre_categoria, orders[i].pizza_category) == 0) {
                // si la categoría ya está en el mapa, sumar la cantidad de pizzas vendidas
                ventas_por_categoria[j].total_pizzas += orders[i].quantity;
                encontrado = 1;
                break;
            }
        }

        if (!encontrado) {
            strcpy(ventas_por_categoria[conteo_categorias].nombre_categoria, orders[i].pizza_category);
            ventas_por_categoria[conteo_categorias].total_pizzas = orders[i].quantity;
            conteo_categorias++;
        }
    }

    char* result = (char*)malloc(500 * sizeof(char)); // se incrementa el tamaño para permitir más texto
    sprintf(result, "Cantidad de pizzas por categoría vendidas:\n");
    for (int i = 0; i < conteo_categorias; i++) {
        sprintf(result + strlen(result), "%s: %d\n", ventas_por_categoria[i].nombre_categoria, ventas_por_categoria[i].total_pizzas);
    }
    return result;
}