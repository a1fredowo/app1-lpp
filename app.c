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

// Define the functions for each metric
char* most_sold_pizza(int *size, struct order *orders) {
    // Create a map to store the quantity of each pizza sold
    struct {
        char pizza_name_id[50];
        char pizza_name[50];
        float quantity;
    } pizza_quantities[1000]; // Assuming there are at most 1000 different pizzas

    int pizza_count = 0;

    // Iterate over the orders
    for (int i = 0; i < *size; i++) {
        // Check if the pizza is already in the map
        int found = 0;
        for (int j = 0; j < pizza_count; j++) {
            if (strcmp(pizza_quantities[j].pizza_name_id, orders[i].pizza_name_id) == 0 &&
                strcmp(pizza_quantities[j].pizza_name, orders[i].pizza_name) == 0) {
                // If the pizza is already in the map, increment its quantity
                pizza_quantities[j].quantity += orders[i].quantity;
                found = 1;
                break;
            }
        }

        // If the pizza is not in the map, add it
        if (!found) {
            strcpy(pizza_quantities[pizza_count].pizza_name_id, orders[i].pizza_name_id);
            strcpy(pizza_quantities[pizza_count].pizza_name, orders[i].pizza_name);
            pizza_quantities[pizza_count].quantity = orders[i].quantity;
            pizza_count++;
        }
    }

    // Find the pizza with the highest quantity
    int max_quantity = 0;
    char *most_sold_pizza = NULL;
    for (int i = 0; i < pizza_count; i++) {
        if (pizza_quantities[i].quantity > max_quantity) {
            max_quantity = pizza_quantities[i].quantity;
            most_sold_pizza = pizza_quantities[i].pizza_name;
        }
    }

    return most_sold_pizza;
}

char* least_sold_pizza(int *size, struct order *orders) {
    // Create a map to store the quantity of each pizza sold
    struct {
        char pizza_name_id[50];
        char pizza_name[50];
        float quantity;
    } pizza_quantities[1000]; // Assuming there are at most 1000 different pizzas

    int pizza_count = 0;

    // Iterate over the orders
    for (int i = 0; i < *size; i++) {
        // Check if the pizza is already in the map
        int found = 0;
        for (int j = 0; j < pizza_count; j++) {
            if (strcmp(pizza_quantities[j].pizza_name_id, orders[i].pizza_name_id) == 0 &&
                strcmp(pizza_quantities[j].pizza_name, orders[i].pizza_name) == 0) {
                // If the pizza is already in the map, increment its quantity
                pizza_quantities[j].quantity += orders[i].quantity;
                found = 1;
                break;
            }
        }

        // If the pizza is not in the map, add it
        if (!found) {
            strcpy(pizza_quantities[pizza_count].pizza_name_id, orders[i].pizza_name_id);
            strcpy(pizza_quantities[pizza_count].pizza_name, orders[i].pizza_name);
            pizza_quantities[pizza_count].quantity = orders[i].quantity;
            pizza_count++;
        }
    }

    // Find the pizza with the lowest quantity
    int min_quantity = pizza_quantities[0].quantity;
    char *least_sold_pizza = pizza_quantities[0].pizza_name;
    for (int i = 1; i < pizza_count; i++) {
        if (pizza_quantities[i].quantity < min_quantity) {
            min_quantity = pizza_quantities[i].quantity;
            least_sold_pizza = pizza_quantities[i].pizza_name;
        }
    }

    return least_sold_pizza;
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
    sprintf(result, "Fecha: %s, Cantidad de pizzas: %d", min_date, min_sales);
    return result;
}

// ... Repeat for each metric

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
            result = most_sold_pizza(&i, orders);
            printf("Pizza mas vendida: %s\n", result);
        } else if (strcmp(argv[j], "pls") == 0) {
            result = least_sold_pizza(&i, orders);
            printf("Pizza menos vendida: %s\n", result);
        } else if (strcmp(argv[j], "dlsp") == 0) {
            result = date_least_sales(&i, orders);
            printf("Fecha con la menor venta: %s\n", result);
        } else {
            printf("Invalid metric: %s\n", argv[j]);
            return 1;
        }
    }
    return 0;
    
}