#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the order struct
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

// Define the function pointer for the metrics
typedef char* (*metric)(int *size, struct order *orders);

// Define the metric functions
char* most_sold_pizza(int size, struct order* orders);
char* least_sold_pizza(int size, struct order* orders);
char* date_with_most_sales(int *size, struct order *orders);
char* date_with_least_sales(int size, struct order* orders);
char* date_most_sold_pizzas(int *size, struct order *orders);
char* date_least_sales(int *size, struct order *orders);
char* average_pizzas_per_order(int size, struct order *orders);
char* average_pizzas_per_day(int size, struct order *orders);
char* ingredient_most_sold(int *size, struct order *orders);
char* pizzas_per_category_sold(int size, struct order* orders);

int main(int argc, char *argv[]) {
    // Check if the correct number of arguments are provided
    if (argc < 3) {
        printf("Usage: %s <csv file> <metric1> <metric2> ...\n", argv[0]);
        return 1;
    }

    // TODO: Read the CSV file and store the data in an array of order structs
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Could not open file %s\n", argv[1]);
        return 1;
    }

    struct order orders[1000]; // Assuming there are at most 1000 orders
    char line[256];
    int i = 0;

    // Skip the header line
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        char *token;

        token = strtok(line, ",");
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

        // Remove the newline character from pizza_name
        size_t len = strlen(orders[i].pizza_name);
        if (orders[i].pizza_name[len - 1] == '\n') {
        orders[i].pizza_name[len - 1] = '\0';
    }
        i++;
    }
    
    fclose(file);

    for (int j = 2; j < argc; j++) {
        char* result;
        if (strcmp(argv[j], "pms") == 0) {
            result = most_sold_pizza(i, orders);
            printf("%s\n", result);
        } else if (strcmp(argv[j], "pls") == 0) {
            result = least_sold_pizza(i, orders);
            printf("%s\n", result);
        } else if (strcmp(argv[j], "dms") == 0) {
            result = date_with_most_sales(&i, orders);
            printf("%s\n", result);
        } else if (strcmp(argv[j], "dls") == 0) {
            result = date_with_least_sales(i, orders);
            printf("%s\n", result);
        } else if (strcmp(argv[j], "dmsp") == 0) {
            result = date_most_sold_pizzas(&i, orders);
            printf("%s\n", result);
            free(result);
        } else if (strcmp(argv[j], "dlsp") == 0) {
            result = date_least_sales(&i, orders);
            printf("Fecha con la menor venta: %s\n", result);
        } else if (strcmp(argv[j], "apo") == 0) {
            result = average_pizzas_per_order(i, orders);
            printf("%s\n", result);
        } else if (strcmp(argv[j], "apd") == 0) {
            result = average_pizzas_per_day(i, orders);
            printf("%s\n", result);
        } else if (strcmp(argv[j], "ims") == 0) {
            result = ingredient_most_sold(&i, orders);
            printf("%s\n", result);
        } else if (strcmp(argv[j], "hp") == 0) {
            result = pizzas_per_category_sold(i, orders);
            printf("%s\n", result);
        } else {
            printf("Invalid metric: %s\n", argv[j]);
            return 1;
        }
    }
    return 0;
}

char* most_sold_pizza(int size, struct order* orders) {
    // Create a map to store the total quantity sold for each pizza name ID
    struct {
        char pizza_name_id[50];
        float total_quantity;
    } pizza_sales[1000]; // Assuming there are at most 1000 different pizzas

    int pizza_count = 0;

    // Iterate over the orders to calculate the total quantity sold for each pizza
    for (int i = 0; i < size; i++) {
        // Check if the pizza name ID is already in the map
        int found = 0;
        for (int j = 0; j < pizza_count; j++) {
            if (strcmp(pizza_sales[j].pizza_name_id, orders[i].pizza_name_id) == 0) {
                // If the pizza name ID is already in the map, add the quantity sold
                pizza_sales[j].total_quantity += orders[i].quantity;
                found = 1;
                break;
            }
        }

        // If the pizza name ID is not in the map, add it
        if (!found) {
            strcpy(pizza_sales[pizza_count].pizza_name_id, orders[i].pizza_name_id);
            pizza_sales[pizza_count].total_quantity = orders[i].quantity;
            pizza_count++;
        }
    }

    // Find the pizza with the highest quantity sold
    float max_quantity = 0;
    char* most_sold_pizza_name_id = NULL;
    for (int i = 0; i < pizza_count; i++) {
        if (pizza_sales[i].total_quantity > max_quantity) {
            max_quantity = pizza_sales[i].total_quantity;
            most_sold_pizza_name_id = pizza_sales[i].pizza_name_id;
        }
    }

    // Create a string to store the result
    char* result = (char*)malloc(100 * sizeof(char));
    sprintf(result, "Pizza más vendida: %s (Cantidad vendida: %.2f)", most_sold_pizza_name_id, max_quantity);

    return result;
}

char* least_sold_pizza(int size, struct order* orders) {
    // Create a map to store the total quantity sold for each pizza name ID
    struct {
        char pizza_name_id[50];
        float total_quantity;
    } pizza_sales[1000]; // Assuming there are at most 1000 different pizzas

    int pizza_count = 0;

    // Iterate over the orders to calculate the total quantity sold for each pizza
    for (int i = 0; i < size; i++) {
        // Check if the pizza name ID is already in the map
        int found = 0;
        for (int j = 0; j < pizza_count; j++) {
            if (strcmp(pizza_sales[j].pizza_name_id, orders[i].pizza_name_id) == 0) {
                // If the pizza name ID is already in the map, add the quantity sold
                pizza_sales[j].total_quantity += orders[i].quantity;
                found = 1;
                break;
            }
        }

        // If the pizza name ID is not in the map, add it
        if (!found) {
            strcpy(pizza_sales[pizza_count].pizza_name_id, orders[i].pizza_name_id);
            pizza_sales[pizza_count].total_quantity = orders[i].quantity;
            pizza_count++;
        }
    }

    // Find the pizza with the lowest quantity sold
    float min_quantity = pizza_sales[0].total_quantity;
    char* least_sold_pizza_name_id = pizza_sales[0].pizza_name_id;
    for (int i = 1; i < pizza_count; i++) {
        if (pizza_sales[i].total_quantity < min_quantity) {
            min_quantity = pizza_sales[i].total_quantity;
            least_sold_pizza_name_id = pizza_sales[i].pizza_name_id;
        }
    }

    // Create a string to store the result
    char* result = (char*)malloc(100 * sizeof(char));
    sprintf(result, "Pizza menos vendida: %s (Cantidad vendida: %.2f)", least_sold_pizza_name_id, min_quantity);

    return result;
}

char* date_with_most_sales(int *size, struct order *orders) {
    // Create a map to store the total sales for each date
    struct {
        char order_date[10];
        float total_sales;
    } date_sales[1000]; // Assuming there are at most 1000 different dates

    int date_count = 0;

    // Iterate over the orders
    for (int i = 0; i < *size; i++) {
        // Check if the date is already in the map
        int found = 0;
        for (int j = 0; j < date_count; j++) {
            if (strcmp(date_sales[j].order_date, orders[i].order_date) == 0) {
                // If the date is already in the map, increment its total sales
                date_sales[j].total_sales += orders[i].total_price;
                found = 1;
                break;
            }
        }

        // If the date is not in the map, add it
        if (!found) {
            strcpy(date_sales[date_count].order_date, orders[i].order_date);
            date_sales[date_count].total_sales = orders[i].total_price;
            date_count++;
        }
    }

    // Find the date with the highest total sales
    float max_sales = 0;
    char *date_most_sales = NULL;
    for (int i = 0; i < date_count; i++) {
        if (date_sales[i].total_sales > max_sales) {
            max_sales = date_sales[i].total_sales;
            date_most_sales = date_sales[i].order_date;
        }
    }
    char *result = (char*)malloc(100 * sizeof(char));
    sprintf(result, "Fecha con más ventas en terminos de dinero: %s (Dinero: %.2f)", date_most_sales, max_sales);

    return result;
}

char* date_with_least_sales(int size, struct order* orders) {
    // Create a map to store the total sales for each date
    struct {
        char order_date[10];
        float total_sales;
    } date_sales[1000]; // Assuming there are at most 1000 different dates

    int date_count = 0;

    // Iterate over the orders
    for (int i = 0; i < size; i++) {
        // Check if the date is already in the map
        int found = 0;
        for (int j = 0; j < date_count; j++) {
            if (strcmp(date_sales[j].order_date, orders[i].order_date) == 0) {
                // If the date is already in the map, increment its total sales
                date_sales[j].total_sales += orders[i].total_price;
                found = 1;
                break;
            }
        }

        // If the date is not in the map, add it
        if (!found) {
            strcpy(date_sales[date_count].order_date, orders[i].order_date);
            date_sales[date_count].total_sales = orders[i].total_price;
            date_count++;
        }
    }

    // Find the date with the least total sales
    float min_sales = 1000000.0f;
    char* date_least_sales = NULL;
    for (int i = 0; i < date_count; i++) {
        if (date_sales[i].total_sales < min_sales) {
            min_sales = date_sales[i].total_sales;
            date_least_sales = date_sales[i].order_date;
        }
    }
    char* result = (char*)malloc(100 * sizeof(char));
    sprintf(result, "Fecha con menos ventas en terminos de dinero: %s (Dinero: %.2f)", date_least_sales, min_sales);

    return result;
}

char* date_most_sold_pizzas(int *size, struct order *orders) {
    // Crear un mapa para almacenar la cantidad de pizzas vendidas en cada fecha
    struct {
        char order_date[10];
        int total_pizzas;
    } date_pizza_sales[1000]; // Suponiendo que hay como máximo 1000 fechas diferentes

    int date_count = 0;

    // Iterar sobre las órdenes para calcular la cantidad de pizzas vendidas en cada fecha
    for (int i = 0; i < *size; i++) {
        // Comprobar si la fecha ya está en el mapa
        int found = 0;
        for (int j = 0; j < date_count; j++) {
            if (strcmp(date_pizza_sales[j].order_date, orders[i].order_date) == 0) {
                // Si la fecha ya está en el mapa, sumar la cantidad de pizzas vendidas
                date_pizza_sales[j].total_pizzas += orders[i].quantity;
                found = 1;
                break;
            }
        }

        // Si la fecha no está en el mapa, añadirla
        if (!found) {
            strcpy(date_pizza_sales[date_count].order_date, orders[i].order_date);
            date_pizza_sales[date_count].total_pizzas = orders[i].quantity;
            date_count++;
        }
    }

    // Encontrar la fecha con más ventas de pizzas
    int max_pizzas = 0;
    char *date_most_sold = NULL;
    for (int i = 0; i < date_count; i++) {
        if (date_pizza_sales[i].total_pizzas > max_pizzas) {
            max_pizzas = date_pizza_sales[i].total_pizzas;
            date_most_sold = date_pizza_sales[i].order_date;
        }
    }

    // Crear una cadena para almacenar el resultado
    char *result = (char*)malloc(100 * sizeof(char));
    sprintf(result, "Fecha con más ventas de pizzas: %s (Cantidad de pizzas: %d)", date_most_sold, max_pizzas);

    return result;
}

char* date_least_sales(int *size, struct order *orders) {
    struct date_sales {
        char date[10];
        int sales;
    };

    struct date_sales date_sales[1000];
    int date_count = 0;

    for (int i = 0; i < *size; i++) {
        int found = 0;
        for (int j = 0; j < date_count; j++) {
            if (strcmp(date_sales[j].date, orders[i].order_date) == 0) {
                date_sales[j].sales += orders[i].quantity;
                found = 1;
                break;
            }
        }

        if (!found) {
            strcpy(date_sales[date_count].date, orders[i].order_date);
            date_sales[date_count].sales = orders[i].quantity;
            date_count++;
        }
    }

    int min_sales = date_sales[0].sales;
    char* min_date = date_sales[0].date;

    for (int i = 1; i < date_count; i++) {
        if (date_sales[i].sales < min_sales) {
            min_sales = date_sales[i].sales;
            min_date = date_sales[i].date;
        }
    }

    char* result = malloc(100 * sizeof(char));
    sprintf(result, "Fecha: %s, (Cantidad de pizzas: %d)", min_date, min_sales);
    return result;
}

char* average_pizzas_per_order(int size, struct order *orders) {
    int total_orders = 0;
    float total_pizzas = 0;

    // Iterar sobre todas las órdenes
    for (int i = 1; i <= 7; i++) {
        float order_pizzas = 0;
        int order_count = 0;

        // Calcular la cantidad total de pizzas para la orden actual
        for (int j = 0; j < size; j++) {
            if (orders[j].order_id == i) {
                order_pizzas += orders[j].quantity;
                order_count++;
            }
        }

        // Actualizar el total de órdenes y la suma total de pizzas
        if (order_count > 0) {
            total_orders++;
            total_pizzas += order_pizzas / order_count;
        }
    }

    // Calcular el promedio global dividiendo por el número total de órdenes
    float global_average = total_pizzas / total_orders;

    // Crear una cadena para almacenar el resultado
    char* result = (char*)malloc(100 * sizeof(char)); // Puedes ajustar el tamaño según sea necesario
    snprintf(result, 100, "Promedio de pizzas por orden: %.2f", global_average);

    return result;
}

char* average_pizzas_per_day(int size, struct order *orders) {
    struct {
        char order_date[10];
        int total_pizzas;
        int count;
    } date_pizza_sales[1000];

    int date_count = 0;

    // Inicializar el array
    for (int i = 0; i < 1000; i++) {
        date_pizza_sales[i].total_pizzas = 0;
        date_pizza_sales[i].count = 0;
    }

    // Iterar sobre las órdenes para calcular el total de pizzas y el contador para cada fecha
    for (int i = 0; i < size; i++) {
        int found = 0;
        for (int j = 0; j < date_count; j++) {
            if (strcmp(date_pizza_sales[j].order_date, orders[i].order_date) == 0) {
                date_pizza_sales[j].total_pizzas += orders[i].quantity;
                date_pizza_sales[j].count++;
                found = 1;
                break;
            }
        }

        if (!found) {
            strcpy(date_pizza_sales[date_count].order_date, orders[i].order_date);
            date_pizza_sales[date_count].total_pizzas += orders[i].quantity;
            date_pizza_sales[date_count].count++;
            date_count++;
        }
    }

    float total_pizzas = 0;
    int total_days = 0;

    for (int i = 0; i < date_count; i++) {
        total_pizzas += date_pizza_sales[i].total_pizzas;
        total_days += date_pizza_sales[i].count;
    }

    if (total_days == 0) {
        return strdup("No hay datos suficientes para calcular el promedio de pizzas por día."); // Evitar división por cero
    }

    // Calcular el promedio y almacenarlo en una cadena de caracteres
    char* result = (char*)malloc(100 * sizeof(char));
    snprintf(result, 100, "Promedio de pizzas por día: %.2f", total_pizzas / total_days);
    return result;
}

char* ingredient_most_sold(int *size, struct order *orders) {
    // Create a map to store the quantity of each ingredient sold
    struct {
        char ingredient_name[50];
        float quantity;
    } ingredient_quantities[1000]; // Assuming there are at most 1000 different ingredients

    int ingredient_count = 0;

    // Iterate over the orders
    for (int i = 0; i < *size; i++) {
        // Split the list of ingredients for each order
        char ingredients_copy[200]; // Make a copy of the ingredients string because strtok modifies the original string
        strcpy(ingredients_copy, orders[i].pizza_ingredients);
        char *ingredient = strtok(ingredients_copy, ",");

        // Iterate over each ingredient in the list
        while (ingredient != NULL) {
            // Remove leading spaces
            while(ingredient[0] == ' ') {
                ingredient++;
            }

            // Check if the ingredient is already in the map
            int found = 0;
            for (int j = 0; j < ingredient_count; j++) {
                if (strcmp(ingredient_quantities[j].ingredient_name, ingredient) == 0) {
                    // If the ingredient is already in the map, increment its quantity
                    ingredient_quantities[j].quantity += orders[i].quantity;
                    found = 1;
                    break;
                }
            }

            // If the ingredient is not in the map, add it
            if (!found) {
                strcpy(ingredient_quantities[ingredient_count].ingredient_name, ingredient);
                ingredient_quantities[ingredient_count].quantity = orders[i].quantity;
                ingredient_count++;
            }

            // Move to the next ingredient
            ingredient = strtok(NULL, ",");
        }
    }

    // Find the ingredient with the highest quantity
    float max_quantity = 0;
    char *most_sold_ingredient = NULL;
    for (int i = 0; i < ingredient_count; i++) {
        if (ingredient_quantities[i].quantity > max_quantity) {
            max_quantity = ingredient_quantities[i].quantity;
            most_sold_ingredient = ingredient_quantities[i].ingredient_name;
        }
    }

    // Create a string to store the result
    char* result = (char*)malloc(100 * sizeof(char));
    sprintf(result, "Ingrediente más vendido: %s", most_sold_ingredient);

    return result;
}

char* pizzas_per_category_sold(int size, struct order* orders) {
    // Crear un mapa para almacenar la cantidad de pizzas vendidas en cada categoría
    struct {
        char category_name[50];
        int total_pizzas;
    } category_sales[1000]; // Suponiendo que hay como máximo 1000 categorías diferentes

    int category_count = 0;

    // Iterar sobre las órdenes para calcular la cantidad de pizzas vendidas en cada categoría
    for (int i = 0; i < size; i++) {
        // Comprobar si la categoría ya está en el mapa
        int found = 0;
        for (int j = 0; j < category_count; j++) {
            if (strcmp(category_sales[j].category_name, orders[i].pizza_category) == 0) {
                // Si la categoría ya está en el mapa, sumar la cantidad de pizzas vendidas
                category_sales[j].total_pizzas += orders[i].quantity;
                found = 1;
                break;
            }
        }

        // Si la categoría no está en el mapa, añadirla
        if (!found) {
            strcpy(category_sales[category_count].category_name, orders[i].pizza_category);
            category_sales[category_count].total_pizzas = orders[i].quantity;
            category_count++;
        }
    }

    // Crear una cadena para almacenar el resultado
    char* result = (char*)malloc(500 * sizeof(char)); // Incrementamos el tamaño para permitir más texto
    sprintf(result, "Cantidad de pizzas por categoría vendidas:\n");
    for (int i = 0; i < category_count; i++) {
        sprintf(result + strlen(result), "%s: %d\n", category_sales[i].category_name, category_sales[i].total_pizzas);
    }

    return result;
}