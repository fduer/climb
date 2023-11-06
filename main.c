#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

////////////////////////////////////////////////////////////////////////////////
///////////////////////////      Constants       ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Provided constants
#define MAX_STR_LEN 20
#define HELP_COMMAND '?'

// Provided enums
// This enum only has to be used in Stage 3.1 onwards
enum attempt_type {FIRST_GO, SUCCESS, FAIL, INVALID_TYPE};

// TODO: Your #defines/enums can go here:

////////////////////////////////////////////////////////////////////////////////
//////////////////////////  USER DEFINED TYPES  ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// TODO: Any additional structs you want to add can go here:

// Provided structs

// Represents a single climbing route in the logbook
struct route {
    // The name of the climbing route
    char name[MAX_STR_LEN];
    // The difficulty of the route
    int difficulty;
    // The length of the route in metres
    int length;
    // A pointer to the next `struct route` in the logbook
    struct route *next;
    // A pointer to the first `struct attempt` for this route
    struct attempt *attempts;
};
struct attempt {
    // The name of the climber who attempted the route
    char climber[MAX_STR_LEN];
    // The type of attempt (FIRST_GO, SUCCESS, or FAIL)
    enum attempt_type type;
    // The rating the climber gave the route (0-3)
    int rating;
    // A pointer to the next `struct attempt` for this route
    struct attempt *next;
};
struct route_name{
    char name[MAX_STR_LEN];
    struct route_name *next;
};
struct climber_details{
    char climber[MAX_STR_LEN];
    int first_go;
    int success;
    int fail;
    int total;
    struct climber_details *next;
};
struct logbook {
    // A pointer to the first `struct route` in the list
    struct route *routes;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////// PROVIDED FUNCTION PROTOTYPE  ////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void print_usage(void);
void print_one_route(int position, struct route *route);
void scan_string(char string[MAX_STR_LEN]);
enum attempt_type scan_attempt_type();
void print_one_attempt(
        char climber[MAX_STR_LEN],
        enum attempt_type type,
        int rating
);

// Additional provided function prototypes
// You won't need to use these functions!
// We use them just to implement some of the provided helper functions.
int scan_token(char *buffer, int buffer_size);
enum attempt_type string_to_type(char *type_str);
void type_to_string(char *buf, enum attempt_type type);
void append_route(struct logbook *logbook);
void print_routes (struct logbook *logbook);
//2.1
void print_filter_route(struct logbook *logbook);
//2.2
void insert_route(struct logbook *logbook);
//2.3
void change_length(struct logbook *logbook);
//2.4
void swap_route(struct logbook *logbook);
//3.1
void add_attempt(struct logbook *logbook, struct route_name **most_recent_route);
void update_recent_route_name(struct route_name **head, char name[MAX_STR_LEN]);
//3.2
void print_logbook(struct logbook *logbook);
//3.3
void remove_route(struct logbook **logbook, int combine);
//3.4
void delete_climber_attempts(struct logbook *logbook);
//3.5
int has_climbed(struct route *route, char climber[MAX_STR_LEN]);
void duplicate_attempts(struct logbook *logbook, struct route_name **most_recent_route);
void add_attempt_in_route(struct route *route, char climber[MAX_STR_LEN], enum attempt_type type, int rating, struct route_name **most_recent_route, int combine);
//4.1
void delete_route(struct logbook *logbook, struct route *route);
void combine_routes(struct logbook *logbook, struct route_name *most_recent_route);
//4.2
struct climber_details *count_climber_details(struct logbook *logbook, struct route *route);
void print_leaderboard(struct logbook *logbook);
////////////////////////////////////////////////////////////////////////////////
////////////////////////  YOUR FUNCTION PROTOTYPE  /////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct logbook *create_logbook(struct route *new_route);
struct route *create_route(
        char name[MAX_STR_LEN],
        int difficulty,
        int length
);

// TODO: Put your function prototypes here
void command_loop(struct logbook *logbook, struct route_name *most_recent_route_name){
    char command;
    while(scanf(" %c", &command) != EOF) {
        if (command == 'r') {
            append_route(logbook);
        } else if (command == 'p') {
            print_routes(logbook);
        } else if (command == '?') {
            print_usage();
        } else if (command == 'f') {
            print_filter_route(logbook);
        } else if (command == 'i') {
            insert_route(logbook);
        } else if (command == 'l') {
            change_length(logbook);
        } else if (command == 's') {
            swap_route(logbook);
        } else if (command == 'a') {
            add_attempt(logbook, &most_recent_route_name);
        }else if(command == 'P') {
            print_logbook(logbook);
        }else if(command == 'R') {
            remove_route(&logbook, 0);
        }else if(command == 'D') {
            delete_climber_attempts(logbook);
        }else if(command == 'd'){
            duplicate_attempts(logbook, &most_recent_route_name);
        }else if(command == 'c') {
            combine_routes(logbook, most_recent_route_name);
        }else if(command == 'L') {
            print_leaderboard(logbook);
        }
        printf("Enter command: ");
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(void) {
    printf("Welcome to 1511 Climb! \n");
    printf("Log all of your climbing adventures here! \n");
    printf("Enter command: ");

    struct logbook *logbook = create_logbook(NULL);
    struct route_name most_recent_route_name = {"tail", NULL};
    command_loop(logbook, &most_recent_route_name);
    printf("\nGoodbye\n");
    //free memory
    struct route *route = logbook->routes;
    while(route != NULL){
        struct attempt *attempt = route->attempts;
        while(attempt != NULL){
            struct attempt *temp = attempt;
            attempt = attempt->next;
            free(temp);
        }
        struct route *temp = route;
        route = route->next;
        free(temp);
    }
    struct route_name *route_name = &most_recent_route_name;
    while(route_name != NULL){
        struct route_name *temp = route_name;
        route_name = route_name->next;
        free(temp);
    }
    free(logbook);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////  YOUR FUNCTIONS //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
struct logbook *create_logbook(struct route *new_route) {
    struct logbook *logbook = malloc(sizeof(struct logbook));
    logbook->routes = NULL;

    return logbook;
}

struct route *create_route(
        char name[MAX_STR_LEN],
        int difficulty,
        int length
) {
    // STAGE 1.1
    // TODO: malloc, initialise and return a new route
    struct route *new_route = malloc(sizeof(struct route));
    new_route->difficulty = difficulty;
    new_route->length = length;
    strcpy(new_route->name, name);
    new_route->next = NULL;

    // hint: you will have to replace NULL in this return statement.
    return new_route;
}

void append_route(struct logbook *logbook)
{
    char name[MAX_STR_LEN];
    int difficulty, length;
    scan_string(name);
    scanf("%d", &difficulty);
    scanf("%d", &length);
    struct route *new_route = create_route(name, difficulty, length);
    if(difficulty < 1 || difficulty > 40)
    {
        printf("ERROR: Route difficulty must be between 1 and 39\n");
        return;
    }
    if(length < 1 || length > 60)
    {
        printf("ERROR: Route length must be between 1m and 60m\n");
        return;
    }
    // If logbook is empty, add the first route
    if (logbook->routes == NULL) {
        logbook->routes = new_route;
    } else {
        //check if the route already exists
        struct route *current = logbook->routes;
        while(current != NULL)
        {
            if(strcmp(current->name, name) == 0)
            {
                printf("ERROR: A route with the name '%s' already exists in this logbook\n", name);
                return;
            }
            current = current->next;
        }
        // Otherwise, add the route to the end of the logbook
        current = logbook->routes;
        while(current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_route;
    }
    printf("Route '%s' added successfully!\n", name);
}

void print_routes (struct logbook *logbook) {
    if (logbook->routes == NULL) {
        printf("There are no routes in this logbook!\n");
    } else {
        struct route *current = logbook->routes;
        int position = 1;
        while (current != NULL) {
            print_one_route(position, current);
            current = current->next;
            position++;
        }
    }
}

void print_filter_route(struct logbook *logbook){
    int difficulty_min, difficulty_max;
    scanf("%d %d", &difficulty_min, &difficulty_max);
    if(difficulty_min <= 0 || difficulty_min > 39 || difficulty_max <= 0 || difficulty_max > 39 || difficulty_min > difficulty_max)
    {
        printf("ERROR: Difficulty range invalid!\n");
        return;
    }
    int position = 1;
    struct route *current = logbook->routes;
    printf("Routes between difficulty %d and %d:\n", difficulty_min, difficulty_max);
    while(current != NULL)
    {
        if(current->difficulty >= difficulty_min && current->difficulty <= difficulty_max)
        {
            print_one_route(position, current);
        }
        current = current->next;
        position++;
    }
}

void insert_route(struct logbook *logbook){
    char name[MAX_STR_LEN], route_to_insert_before[MAX_STR_LEN];
    int difficulty, length;
    scan_string(name);
    scanf("%d %d", &difficulty, &length);
    scan_string(route_to_insert_before);
    struct route *new_route = create_route(name, difficulty, length);
    if(difficulty <= 0 || difficulty > 39)
    {
        printf("ERROR: Route difficulty must be between 1 and 39\n");
        return;
    }
    if(length <= 0 || length > 60)
    {
        printf("ERROR: Route length must be between 1m and 60m\n");
        return;
    }
    struct route *current = logbook->routes;
    while(current != NULL)
    {
        if(strcmp(current->name, name) == 0)
        {
            printf("ERROR: A route with the name '%s' already exists in this logbook\n", name);
            return;
        }
        current = current->next;
    }
    current = logbook->routes;
    //if the route to insert before does not exist
    while(current != NULL)
    {
        if(strcmp(current->name, route_to_insert_before) == 0)
        {
            break;
        }
        current = current->next;
    }
    if(current == NULL)
    {
        printf("ERROR: No route with the name '%s' exists in this logbook\n", route_to_insert_before);
        return;
    }
    //if the route to insert before is the first route
    current = logbook->routes;
    if(strcmp(current->name, route_to_insert_before) == 0)
    {
        new_route->next = current;
        logbook->routes = new_route;
        printf("Route '%s' inserted successfully!\n", name);
        return;
    }
    //if the route to insert before is not the first route
    current = logbook->routes;
    while(current->next != NULL)
    {
        if(strcmp(current->next->name, route_to_insert_before) == 0)
        {
            new_route->next = current->next;
            current->next = new_route;
            printf("Route '%s' inserted successfully!\n", name);
            return;
        }
        current = current->next;
    }
}

void change_length(struct logbook *logbook){
    char route_1[MAX_STR_LEN], route_2[MAX_STR_LEN];
    int length_change;
    scan_string(route_1);
    scan_string(route_2);
    scanf("%d", &length_change);
    if(length_change < -60 || length_change > 60)
    {
        printf("ERROR: Invalid length change! No route lengths have been changed\n");
        return;
    }
    struct route *current = logbook->routes;
    struct route *route_1_ptr = NULL;
    struct route *route_2_ptr = NULL;
    int position = 1;
    int position_1 = 1, position_2 = 1;
    while(current != NULL)
    {
        if(strcmp(current->name, route_1) == 0)
        {
            route_1_ptr = current;
            position_1 = position;
        }
        if(strcmp(current->name, route_2) == 0)
        {
            route_2_ptr = current;
            position_2 = position;
        }
        current = current->next;
        ++position;
    }
    if(route_1_ptr == NULL)
    {
        printf("ERROR: No route with the name '%s' exists in this logbook\n", route_1);
        return;
    }
    if(route_2_ptr == NULL)
    {
        printf("ERROR: No route with the name '%s' exists in this logbook\n", route_2);
        return;
    }
    if(position_1 >= position_2)
    {
        //route_2 is before route_1, change the length of all routes between route_2 and route_1
        //include route_2 and route_1
        current = logbook->routes;
        position = 1;
        while(current != NULL)
        {
            if(position >= position_2 && position <= position_1)
            {
                if(current->length + length_change <= 0 || current->length + length_change > 60)
                {
                    printf("ERROR: Invalid length change! No route lengths have been changed\n");
                    return;
                }
            }
            current = current->next;
            ++position;
        }
        current = logbook->routes;
        position = 1;
        while(current != NULL)
        {
            if(position >= position_2 && position <= position_1)
            {
                printf("Length of '%s' updated from %dm to %dm\n", current->name, current->length, current->length + length_change);
                current->length += length_change;
            }
            current = current->next;
            ++position;
        }
    }
    else if(position_1 <= position_2)
    {
        //route_1 is before route_2, change the length of all routes between route_1 and route_2
        //include route_1 and route_2
        current = logbook->routes;
        position = 1;
        while(current != NULL)
        {
            if(position >= position_1 && position <= position_2)
            {
                if(current->length + length_change <= 0 || current->length + length_change > 60)
                {
                    printf("ERROR: Invalid length change! No route lengths have been changed\n");
                    return;
                }
            }
            current = current->next;
            ++position;
        }
        current = logbook->routes;
        position = 1;
        while(current != NULL)
        {
            if(position >= position_1 && position <= position_2)
            {
                printf("Length of '%s' updated from %dm to %dm\n", current->name, current->length, current->length + length_change);
                current->length += length_change;
            }
            current = current->next;
            ++position;
        }
    }
}

void swap_route(struct logbook *logbook){
    char route_1[MAX_STR_LEN], route_2[MAX_STR_LEN];
    scan_string(route_1);
    scan_string(route_2);
    if(strcmp(route_1, route_2) == 0)
    {
        printf("ERROR: Cannot swap '%s' with itself\n", route_1);
        return;
    }
    struct route *current = logbook->routes;
    struct route *route_1_ptr = NULL;
    struct route *route_2_ptr = NULL;
    while(current != NULL)
    {
        if(strcmp(current->name, route_1) == 0)
        {
            route_1_ptr = current;
        }
        if(strcmp(current->name, route_2) == 0)
        {
            route_2_ptr = current;
        }
        current = current->next;
    }
    if(route_1_ptr == NULL)
    {
        printf("ERROR: No route with the name '%s' exists in this logbook\n", route_1);
        return;
    }
    if(route_2_ptr == NULL)
    {
        printf("ERROR: No route with the name '%s' exists in this logbook\n", route_2);
        return;
    }
    char route_1_name[MAX_STR_LEN], route_2_name[MAX_STR_LEN];
    int route_1_difficulty = route_1_ptr->difficulty, route_2_difficulty = route_2_ptr->difficulty;
    int route_1_length = route_1_ptr->length, route_2_length = route_2_ptr->length;
    strcpy(route_1_name, route_1_ptr->name);
    strcpy(route_2_name, route_2_ptr->name);
    route_1_ptr->difficulty = route_2_difficulty;
    route_2_ptr->difficulty = route_1_difficulty;
    route_1_ptr->length = route_2_length;
    route_2_ptr->length = route_1_length;
    strcpy(route_1_ptr->name, route_2_name);
    strcpy(route_2_ptr->name, route_1_name);
    printf("'%s' swapped positions with '%s'!\n", route_1, route_2);
}

void add_attempt(struct logbook *logbook, struct route_name **most_recent_route) {
    char climber[MAX_STR_LEN];
    enum attempt_type type;
    int rating;
    char route_name[MAX_STR_LEN];
    scan_string(climber);
    type = scan_attempt_type();
    scanf("%d", &rating);
    scan_string(route_name);
    update_recent_route_name(most_recent_route, route_name);
    if (type == INVALID_TYPE) {
        printf("ERROR: Attempt type invalid\n");
        return;
    }
    if (rating < 0 || rating > 3) {
        printf("ERROR: Rating must be between 0 and 3\n");
        return;
    }
    struct route *route = logbook->routes;
    while (route != NULL) {
        if (strcmp(route->name, route_name) == 0) {
            struct attempt *attempt = route->attempts;
            if (attempt == NULL) {
                attempt = malloc(sizeof(struct attempt));
                strcpy(attempt->climber, climber);
                attempt->type = type;
                attempt->rating = rating;
                attempt->next = NULL;
                route->attempts = attempt;
            } else {
                struct attempt *new_attempt = malloc(sizeof(struct attempt));
                strcpy(new_attempt->climber, climber);
                new_attempt->type = type;
                new_attempt->rating = rating;
                new_attempt->next = NULL;
                //Attempts should be added in alphabetical order by climber name.
                // the most recent attempt always appears first.
                if (strcmp(new_attempt->climber, attempt->climber) <= 0) {
                    //new attempt is the first attempt
                    if (strcmp(new_attempt->climber, attempt->climber) == 0 && new_attempt->type == FIRST_GO) {
                        printf("ERROR: %s has already attempted '%s' - they can't get it on their first go!\n",
                               new_attempt->climber, route_name);
                        return;
                    }
                    new_attempt->next = attempt;
                    route->attempts = new_attempt;
                }
                else {
                    //new attempt is not the first attempt
                    while (attempt->next != NULL) {
                        if (strcmp(new_attempt->climber, attempt->next->climber) <= 0) {
                            //new attempt is not the last attempt
                            if (strcmp(new_attempt->climber, attempt->next->climber) == 0 &&
                                new_attempt->type == FIRST_GO) {
                                printf("ERROR: %s has already attempted '%s' - they can't get it on their first go!\n",
                                       new_attempt->climber, route_name);
                                return;
                            }
                            new_attempt->next = attempt->next;
                            attempt->next = new_attempt;
                            break;
                        }
                        attempt = attempt->next;
                    }
                    attempt->next = new_attempt;
                }
            }
            break;
        }
        route = route->next;
    }
    printf("Logged attempt of '%s' by %s\n", route_name, climber);
}
void update_recent_route_name(struct route_name **head, char name[MAX_STR_LEN]){
    struct route_name *new_route_name = malloc(sizeof(struct route_name));
    strcpy(new_route_name->name, name);
    new_route_name->next = NULL;
    if(*head == NULL){
        *head = new_route_name;
    }else{
        new_route_name->next = *head;
        *head = new_route_name;
    }
}

void print_logbook(struct logbook *logbook) {
    struct route *route = logbook->routes;
    int position = 1;
    if (route == NULL) {
        printf("There are no routes in this logbook!\n");
        return;
    } else {
        while (route != NULL) {
            struct attempt *attempt = route->attempts;
            double average_rating = 0;
            int count = 0;
            while (attempt != NULL) {
                average_rating += attempt->rating;
                count++;
                attempt = attempt->next;
            }
            if (count == 0) {
                average_rating = 0;
            } else {
                average_rating = average_rating / count;
            }
            printf("Route #%d: %s\n", position, route->name);
            printf("Difficulty: %d | Length: %dm | Avg rating: %.1lf/3.0\n", route->difficulty, route->length,
                   average_rating);
            attempt = route->attempts;
            while (attempt != NULL) {
                print_one_attempt(attempt->climber, attempt->type, attempt->rating);
                attempt = attempt->next;
            }
            route = route->next;
            position++;
        }
    }
}

void remove_route(struct logbook **logbook, int combine) {
    char route_name[MAX_STR_LEN];
    scan_string(route_name);
    int found = 0;
    struct route *route = (*logbook)->routes;
    struct route *pre = NULL;
    while (route != NULL) {
        if (strcmp(route->name, route_name) == 0) {
            found = 1;
            if (pre == NULL) {
                (*logbook)->routes = route->next;
            } else {
                pre->next = route->next;
            }
            struct attempt *attempt = route->attempts;
            while (attempt != NULL) {
                struct attempt *temp = attempt;
                attempt = attempt->next;
                free(temp);
            }
            free(route);
            break;
        }
        pre = route;
        route = route->next;
    }
    if(!combine){
        if (found == 0) {
            printf("ERROR: No route with the name '%s' exists in this logbook\n", route_name);
        } else {
            printf("Removed route '%s' from logbook\n", route_name);
        }
    }
}

void delete_climber_attempts(struct logbook *logbook) {
    char climber[MAX_STR_LEN];
    scan_string(climber);
    int found = 0;
    int count = 0;
    struct route *route = logbook->routes;
    while (route != NULL) {
        struct attempt *attempt = route->attempts;
        struct attempt *pre = NULL;
        while (attempt != NULL) {
            if (strcmp(attempt->climber, climber) == 0) {
                found = 1;
                ++count;
                if (pre == NULL) {
                    route->attempts = attempt->next;
                } else {
                    pre->next = attempt->next;
                }
                struct attempt *temp = attempt;
                attempt = attempt->next;
                free(temp);
            } else {
                pre = attempt;
                attempt = attempt->next;
            }
        }
        route = route->next;
    }
    if (!found) {
        printf("ERROR: %s has not logged any attempts\n", climber);
    } else {
        printf("Deleted %d attempt(s) logged by %s\n", count, climber);

    }
}//暂时不写most_recent_route的更新

int has_climbed(struct route *route, char climber[MAX_STR_LEN]) {
    struct attempt *attempt = route->attempts;
    while (attempt != NULL) {
        if (strcmp(attempt->climber, climber) == 0) {
            return 1;
        }
        attempt = attempt->next;
    }
    return 0;
}
void duplicate_attempts(struct logbook *logbook, struct route_name **most_recent_route) {
    char climber_1[MAX_STR_LEN]; //climber_1 is the climber who will have their attempts duplicated
    char climber_2[MAX_STR_LEN]; //climber_2 is the climber whose attempts will be duplicated
    scan_string(climber_1);
    scan_string(climber_2);
    if(strcmp(climber_1, climber_2) == 0){
        printf("ERROR: Cannot duplicate attempts made by the same climber\n");
        return;
    }
    int found = 0;
    struct route *route = logbook->routes;
    while (route != NULL) {
        struct attempt *attempt = route->attempts;
        struct attempt *addList = NULL;
        while (attempt != NULL) {
            if(strcmp(attempt->climber, climber_2) == 0){
                //attempt is made by climber_2
                found = 1;
                struct attempt *new_attempt = malloc(sizeof(struct attempt));
                strcpy(new_attempt->climber, climber_1);
                new_attempt->type = attempt->type;
                new_attempt->rating = attempt->rating;
                new_attempt->next = NULL;
                // insert new attempt into the head of the addList
                if(addList == NULL){
                    addList = new_attempt;
                }else{
                    new_attempt->next = addList;
                    addList = new_attempt;
                }
            }
            attempt = attempt->next;
        }
        if(addList != NULL){
            // addList is not empty
            struct attempt *new_attempt = addList;
            while(new_attempt != NULL){
                if(has_climbed(route, climber_1) && new_attempt->type == FIRST_GO){
                    add_attempt_in_route(route, new_attempt->climber, SUCCESS, new_attempt->rating, most_recent_route, 0);
                }
                else{
                    add_attempt_in_route(route, new_attempt->climber, new_attempt->type, new_attempt->rating, most_recent_route, 0);
                }
                new_attempt = new_attempt->next;
            }
        }
        //free addList
        if(addList != NULL){
            struct attempt *temp = addList;
            while(temp != NULL){
                addList = addList->next;
                free(temp);
                temp = addList;
            }
        }
        route = route->next;
    }
    if (!found) {
        printf("ERROR: %s has not logged any attempts\n", climber_2);
    }
}
void add_attempt_in_route(struct route *route, char climber[MAX_STR_LEN], enum attempt_type type, int rating, struct route_name **most_recent_route, int combine) {
    struct attempt *attempt = route->attempts;
    struct attempt *new_attempt = malloc(sizeof(struct attempt));
    strcpy(new_attempt->climber, climber);
    new_attempt->type = type;
    new_attempt->rating = rating;
    new_attempt->next = NULL;
    if(!combine) {
        update_recent_route_name(most_recent_route, route->name);
    }
    //Attempts should be added in alphabetical order by climber name.
    // the most recent attempt always appears first.
    // the type won't be first go
    if (strcmp(new_attempt->climber, attempt->climber) <= 0) {
        //new attempt is the first attempt
        new_attempt->next = attempt;
        route->attempts = new_attempt;
    }
    else {
        //new attempt is not the first attempt
        while (attempt->next != NULL) {
            if (strcmp(new_attempt->climber, attempt->next->climber) <= 0) {
                //new attempt is not the last attempt
                new_attempt->next = attempt->next;
                attempt->next = new_attempt;
                break;
            }
            attempt = attempt->next;
        }
        attempt->next = new_attempt;
    }
    if(!combine)
        printf("Logged attempt of '%s' by %s\n", route->name, climber);
}

void delete_route(struct logbook *logbook, struct route *route){
    struct route *pre = NULL;
    struct route *tempRoute = logbook->routes;
    while(tempRoute != NULL){
        if(tempRoute == route){
            if(pre == NULL){
                logbook->routes = route->next;
            }else{
                pre->next = route->next;
            }
            struct attempt *attempt = route->attempts;
            while(attempt != NULL){
                struct attempt *temp = attempt;
                attempt = attempt->next;
                free(temp);
            }
            free(route);
            break;
        }
        pre = tempRoute;
        tempRoute = tempRoute->next;
    }
}
void combine_routes(struct logbook *logbook, struct route_name *most_recent_route){
    char route_1[MAX_STR_LEN];
    char route_2[MAX_STR_LEN];
    scan_string(route_1);
    scan_string(route_2);
    if(strcmp(route_1, route_2) == 0){
        printf("ERROR: Cannot combine '%s' with itself\n", route_1);
        return;
    }
    struct route *route = logbook->routes;
    struct route *pre = NULL;
    struct route *route_1_ptr = NULL;
    struct route *route_2_ptr = NULL;
    while(route != NULL){
        if(strcmp(route->name, route_1) == 0){
            route_1_ptr = route;
        }
        if(strcmp(route->name, route_2) == 0){
            route_2_ptr = route;
        }
        route = route->next;
    }
    if(route_1_ptr == NULL){
        printf("ERROR: No route with the name '%s' exists in this logbook\n", route_1);
        return;
    }
    if(route_2_ptr == NULL){
        printf("ERROR: No route with the name '%s' exists in this logbook\n", route_2);
        return;
    }
    if(route_1_ptr->length + route_2_ptr->length > 60){
        printf("ERROR: Combined route cannot be longer than 60m\n");
        return;
    }
    if(route_1_ptr->attempts == NULL && route_2_ptr->attempts == NULL){
        //consider route_1_ptr is the most recent route
        //update route_1_ptr's difficulty and length
        if(route_1_ptr->difficulty < route_2_ptr->difficulty){
            route_1_ptr->difficulty = route_2_ptr->difficulty;
        }
        route_1_ptr->length += route_2_ptr->length;
        //delete route_2_ptr
        delete_route(logbook, route_2_ptr);
        printf("Successfully combined routes '%s' and '%s'\n", route_1, route_2);
        return;
    }
    else{
        //find which is the most recent route in route_1_ptr and route_2_ptr
        struct route_name *temp = most_recent_route;
        int route_1_ptr_is_most_recent = 0;
        int route_2_ptr_is_most_recent = 0;
        while(temp != NULL){
            if(strcmp(temp->name, route_1_ptr->name) == 0){
                route_1_ptr_is_most_recent = 1;
                break;
            }
            if(strcmp(temp->name, route_2_ptr->name) == 0){
                route_2_ptr_is_most_recent = 1;
                break;
            }
            temp = temp->next;
        }
        if(route_1_ptr_is_most_recent){
            //update route_1_ptr's difficulty and length
            if(route_1_ptr->difficulty < route_2_ptr->difficulty){
                route_1_ptr->difficulty = route_2_ptr->difficulty;
            }
            route_1_ptr->length += route_2_ptr->length;
            //add all attempts in route_2_ptr to route_1_ptr
            struct attempt *attempt = route_2_ptr->attempts;
            struct attempt *addList = NULL;
            //addList is a list of reversed attempts in route_2_ptr
            while(attempt != NULL){
                struct attempt *new_attempt = malloc(sizeof(struct attempt));
                strcpy(new_attempt->climber, attempt->climber);
                new_attempt->type = attempt->type;
                new_attempt->rating = attempt->rating;
                new_attempt->next = NULL;
                if(addList == NULL){
                    addList = new_attempt;
                }else{
                    new_attempt->next = addList;
                    addList = new_attempt;
                }
                attempt = attempt->next;
            }
            //add all attempts in addList to route_1_ptr
            if(addList != NULL){
                struct attempt *new_attempt = addList;
                while(new_attempt != NULL){
                    if(has_climbed(route_1_ptr, new_attempt->climber) && new_attempt->type == FIRST_GO){
                        add_attempt_in_route(route_1_ptr, new_attempt->climber, SUCCESS, new_attempt->rating, &most_recent_route, 1);
                    }
                    else{
                        add_attempt_in_route(route_1_ptr, new_attempt->climber, new_attempt->type, new_attempt->rating, &most_recent_route, 1);
                    }
                    new_attempt = new_attempt->next;
                }
            }
            //free addList
            if(addList != NULL){
                struct attempt *tempList = addList;
                while(tempList != NULL){
                    addList = addList->next;
                    free(tempList);
                    tempList = addList;
                }
            }
            //delete route_2_ptr
            delete_route(logbook, route_2_ptr);
        }
        else if(route_2_ptr_is_most_recent) {
            //update route_2_ptr's difficulty and length
            if (route_2_ptr->difficulty < route_1_ptr->difficulty) {
                route_2_ptr->difficulty = route_1_ptr->difficulty;
            }
            route_2_ptr->length += route_1_ptr->length;
            //add all attempts in route_1_ptr to route_2_ptr
            struct attempt *attempt = route_1_ptr->attempts;
            struct attempt *addList = NULL;
            //addList is a list of reversed attempts in route_1_ptr
            while (attempt != NULL) {
                struct attempt *new_attempt = malloc(sizeof(struct attempt));
                strcpy(new_attempt->climber, attempt->climber);
                new_attempt->type = attempt->type;
                new_attempt->rating = attempt->rating;
                new_attempt->next = NULL;
                if (addList == NULL) {
                    addList = new_attempt;
                } else {
                    new_attempt->next = addList;
                    addList = new_attempt;
                }
                attempt = attempt->next;
            }
            //add all attempts in addList to route_2_ptr
            if (addList != NULL) {
                struct attempt *new_attempt = addList;
                while (new_attempt != NULL) {
                    if (has_climbed(route_2_ptr, new_attempt->climber) && new_attempt->type == FIRST_GO) {
                        add_attempt_in_route(route_2_ptr, new_attempt->climber, SUCCESS, new_attempt->rating,
                                             &most_recent_route, 1);
                    } else {
                        add_attempt_in_route(route_2_ptr, new_attempt->climber, new_attempt->type, new_attempt->rating,
                                             &most_recent_route, 1);
                    }
                    new_attempt = new_attempt->next;
                }
            }
            //free addList
            if (addList != NULL) {
                struct attempt *tempList = addList;
                while (tempList != NULL) {
                    addList = addList->next;
                    free(tempList);
                    tempList = addList;
                }
            }
            //delete route_1_ptr
            delete_route(logbook, route_1_ptr);
        }
        printf("Successfully combined routes '%s' and '%s'\n", route_1, route_2);
    }
}

struct climber_details *count_climber_details(struct logbook *logbook, struct route *route){
    struct attempt *attempt = route->attempts;
    struct climber_details *climber_list = NULL;
    char now_climber_name[MAX_STR_LEN];
    while(attempt != NULL){
        if(climber_list == NULL){
            struct climber_details *new_climber = malloc(sizeof(struct climber_details));
            strcpy(new_climber->climber, attempt->climber);
            strcpy(now_climber_name, attempt->climber);
            new_climber->first_go = 0;
            new_climber->success = 0;
            new_climber->fail = 0;
            new_climber->total = 0;
            new_climber->next = NULL;
            climber_list = new_climber;
            continue;
        }
        else{
            if(strcmp(attempt->climber, now_climber_name) == 0){
                //attempt is made by the same climber as the previous attempt
                //do nothing
            }
            else{
                //attempt is made by a different climber
                //add this climber to climber_list
                struct climber_details *new_climber = malloc(sizeof(struct climber_details));
                strcpy(new_climber->climber, attempt->climber);
                strcpy(now_climber_name, attempt->climber);
                new_climber->first_go = 0;
                new_climber->success = 0;
                new_climber->fail = 0;
                new_climber->total = 0;
                new_climber->next = NULL;
                //insert new_climber into the tail of climber_list
                struct climber_details *temp = climber_list;
                while(temp->next != NULL){
                    temp = temp->next;
                }
                temp->next = new_climber;
            }
        }
        attempt = attempt->next;
    }
    //count the number of first_go, success, fail and total for each climber
    attempt = route->attempts;
    while(attempt != NULL){
        struct climber_details *temp = climber_list;
        while(temp != NULL){
            if(strcmp(attempt->climber, temp->climber) == 0){
                //attempt is made by the same climber as temp
                if(attempt->type == FIRST_GO){
                    temp->first_go++;
                    temp->success++;
                }
                if(attempt->type == SUCCESS){
                    temp->success++;
                }
                if(attempt->type == FAIL){
                    temp->fail++;
                }
                temp->total++;
                break;
            }
            temp = temp->next;
        }
        attempt = attempt->next;
    }
    //sort climber_list by the number of first_go, success and total
    struct climber_details *temp = climber_list;
    while(temp != NULL) {
        struct climber_details *temp2 = temp->next;
        while (temp2 != NULL) {
            if (temp->first_go < temp2->first_go) {
                //swap temp and temp2
                char temp_climber[MAX_STR_LEN];
                strcpy(temp_climber, temp->climber);
                strcpy(temp->climber, temp2->climber);
                strcpy(temp2->climber, temp_climber);
                int temp_first_go = temp->first_go;
                temp->first_go = temp2->first_go;
                temp2->first_go = temp_first_go;
                int temp_success = temp->success;
                temp->success = temp2->success;
                temp2->success = temp_success;
                int temp_fail = temp->fail;
                temp->fail = temp2->fail;
                temp2->fail = temp_fail;
                int temp_total = temp->total;
                temp->total = temp2->total;
                temp2->total = temp_total;
            } else if (temp->first_go == temp2->first_go) {
                if (temp->success < temp2->success) {
                    //swap temp and temp2
                    char temp_climber[MAX_STR_LEN];
                    strcpy(temp_climber, temp->climber);
                    strcpy(temp->climber, temp2->climber);
                    strcpy(temp2->climber, temp_climber);
                    int temp_first_go = temp->first_go;
                    temp->first_go = temp2->first_go;
                    temp2->first_go = temp_first_go;
                    int temp_success = temp->success;
                    temp->success = temp2->success;
                    temp2->success = temp_success;
                    int temp_fail = temp->fail;
                    temp->fail = temp2->fail;
                    temp2->fail = temp_fail;
                    int temp_total = temp->total;
                    temp->total = temp2->total;
                    temp2->total = temp_total;
                } else if (temp->success == temp2->success) {
                    if (temp->total < temp2->total) {
                        //swap temp and temp2
                        char temp_climber[MAX_STR_LEN];
                        strcpy(temp_climber, temp->climber);
                        strcpy(temp->climber, temp2->climber);
                        strcpy(temp2->climber, temp_climber);
                        int temp_first_go = temp->first_go;
                        temp->first_go = temp2->first_go;
                        temp2->first_go = temp_first_go;
                        int temp_success = temp->success;
                        temp->success = temp2->success;
                        temp2->success = temp_success;
                        int temp_fail = temp->fail;
                        temp->fail = temp2->fail;
                        temp2->fail = temp_fail;
                        int temp_total = temp->total;
                        temp->total = temp2->total;
                        temp2->total = temp_total;
                    }
                }
            }
            temp2 = temp2->next;
        }
        temp = temp->next;
    }
    return climber_list;
}
void print_leaderboard(struct logbook *logbook){
    char route_name[MAX_STR_LEN];
    scan_string(route_name);
    struct route *route = logbook->routes;
    int found = 0;
    while(route != NULL){
        if(strcmp(route->name, route_name) == 0){
            found = 1;
            break;
        }
        route = route->next;
    }
    if(!found){
        printf("ERROR: No route with the name '%s' exists in this logbook\n", route_name);
        return;
    }
    struct attempt *attempt = route->attempts;
    if(attempt == NULL){
        printf("No attempts have been logged for the route '%s'\n", route_name);
        return;
    }
    struct climber_details *climber_list = NULL;
    climber_list = count_climber_details(logbook, route);
    //print the leaderboard
    int ranking = 1;
    int first_go = 0;
    printf("'%s' Leaderboard\n", route_name);
    struct climber_details *temp = climber_list;
    while(temp != NULL){
        printf("     #%d %s\n",ranking, temp->climber);
        if(temp->first_go > 0 && first_go == 0){
            printf("        First go attempt!\n");
            first_go = 1;
        }
        printf("        Successful attempts: %d\n", temp->success);
        printf("        Total attempts:      %d\n", temp->total);
        printf("\n");
        temp = temp->next;
        ++ranking;
    }
    //free climber_list
    if(climber_list != NULL){
        struct climber_details *tempList = climber_list;
        while(tempList != NULL){
            climber_list = climber_list->next;
            free(tempList);
            tempList = climber_list;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////////  PROVIDED FUNCTIONS  ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Prints the 1511 Climb usage instructions,
// displaying the different commands and their arguments.
//
// Parameters:
//      None
//
// Returns:
//      None
void print_usage(void) {
    printf(
            "=====================[     1511 Climb     ]=====================\n"
            "      ===============[     Usage Info     ]===============      \n"
            "  r [name] [difficulty] [length]                                \n"
            "    Append a climbing route to the end of the logbook           \n"
            "  p                                                             \n"
            "    Print out all the climbing routes in the logbook            \n"
            "                                                                \n"
            "  f [difficulty_min] [difficulty_max]                           \n"
            "    Filter routes by difficulty                                 \n"
            "  i [name] [difficulty] [length] [route_to_insert_before]       \n"
            "    Insert a climbing route into the logbook before             \n"
            "    `route_to_insert_before`                                    \n"
            "  l [route_1] [route_2] [length_change]                         \n"
            "    Change length of all routes between `route_1` and `route_2` \n"
            "  s [route_1] [route_2]                                         \n"
            "    Swap 2 routes                                               \n"
            "                                                                \n"
            "  a [climber] [attempt_type] [rating] [route]                   \n"
            "    Add an attempt of `route` by `climber`                      \n"
            "  P                                                             \n"
            "    Print out all the routes and attempts in the logbook        \n"
            "  R [route]                                                     \n"
            "    Remove a route and all associated attempts                  \n"
            "  D [climber]                                                   \n"
            "    Delete all attempts by `climber`                            \n"
            "  d [climber_1] [climber_2]                                     \n"
            "    Duplicate all of `climber_2`s attempts for `climber_1`      \n"
            "                                                                \n"
            "  c [route_1] [route_2]                                         \n"
            "    Combine 2 routes                                            \n"
            "  L [route]                                                     \n"
            "    Print the climber leaderboard for `route`                   \n"
            "                                                                \n"
            "  ?                                                             \n"
            "    Show help                                                   \n"
            "================================================================\n"
    );
}

// Prints a single route, displaying its position in the logbook, name, grade
// and length.
//
// Parameters:
//      position    - The position of the route in the logbook (the first
//                        route will be route_number 1, second will be 2, etc.)
//      route       - A pointer to the struct route to be printed
//
// Returns:
//      None
void print_one_route(int position, struct route *route) {
    int line_length = MAX_STR_LEN + 2;
    printf("/--------- \\/ ---------\\\n");

    char position_str[MAX_STR_LEN];
    sprintf(position_str, "Route #%d", position);
    int padding = line_length - strlen(position_str);
    printf(
            "|%*s%s%*s|\n", padding / 2, "", position_str, (padding + 1) / 2, ""
    );

    padding = line_length - strlen(route->name);
    printf("|%*s%s%*s|\n", padding / 2, "", route->name, (padding + 1) / 2, "");

    printf("|    Difficulty: %2d    |\n", route->difficulty);
    printf("|    Length (m): %2d    |\n", route->length);
    printf("\\--------- || ---------/\n");
}

// Scans a string into the provided char array, placing a '\0' at the end
//
// Parameters:
//      string      - a char array of length MAX_STR_LEN to store the string
//                    that is scanned in
//
// Returns:
//      None
//
// Usage:
// ```
//      char string[MAX_STR_LEN];
//      scan_string(string);
// ```
void scan_string(char string[MAX_STR_LEN]) {
    scan_token(string, MAX_STR_LEN);
}

// Scans a string and converts it to an enum attempt_type.
//
// Parameters:
//      None
//
// Returns:
//      The corresponding attempt_type, if the string was valid,
//      Otherwise, returns INVALID_TYPE.
//
// Usage:
// ```
//      enum attempt_type type = scan_attempt_type();
// ```
enum attempt_type scan_attempt_type(void) {
    char type[MAX_STR_LEN];
    scan_token(type, MAX_STR_LEN);
    return string_to_type(type);
}

// Prints a single attempt, displaying the climber name, attempt type and rating
//
// Parameters:
//      climber     - The name of the climber who attempted to climb a route
//      type        - The attempt type that was made (should be either
//                    FIRST_GO, SUCCESS, or FAIL)
//      rating      - The star rating that the climber gave the route
//
// Returns:
//      None
void print_one_attempt(
        char climber[MAX_STR_LEN],
        enum attempt_type type,
        int rating
) {
    char type_string[MAX_STR_LEN];
    type_to_string(type_string, type);
    printf("        --> %s\n", type_string);
    printf("            Climber: %s\n", climber);
    printf("            Rating:  ");
    for (int i = 0; i < rating; i++) {
        printf("*");
    }
    printf("\n\n");
}

////////////////////////////////////////////////////////////////////////////////
////////////////////// ADDITIONAL PROVIDED FUNCTIONS ///////////////////////////
////////////////////////////////////////////////////////////////////////////////

// You don't need to use any of these, or understand how they work!
// We use them to implement some of the provided helper functions.

// Scans a single word into `buffer`, ignoring whitespace
//
// Parameters:
//      buffer      - the string to store the scanned word
//      buffer_size - the maximum size of string that can be scanned (including
//                    the null terminator)
// Returns:
//      The number of chars scanned successfully
int scan_token(char *buffer, int buffer_size) {
    if (buffer_size == 0) {
        return 0;
    }

    char c;
    int i = 0;
    int num_scanned = 0;

    // Consume all leading whitespace
    scanf(" ");

    // Scan in characters until whitespace
    while (i < buffer_size - 1
           && (num_scanned = scanf("%c", &c)) == 1
           && !isspace(c)) {
        buffer[i++] = c;
    }

    if (i > 0) {
        buffer[i] = '\0';
    }

    return num_scanned;
}

// Converts a string to the corresponding enum attempt_type
//
// Parameters:
//      type_str    - The string to be converted
//
// Returns:
//      The matching enum attempt_type, or `INVALID_TYPE` if `type_str` matches
//      no valid attempt type
enum attempt_type string_to_type(char *type_str) {
    int len = MAX_STR_LEN;

    if (strncasecmp(type_str, "first_go", len) == 0) {
        return FIRST_GO;
    }
    if (strncasecmp(type_str, "success", len) == 0) {
        return SUCCESS;
    }
    if (strncasecmp(type_str, "fail", len) == 0) {
        return FAIL;
    }

    return INVALID_TYPE;
}

// Converts an enum attempt_type to a string
//
// Parameters:
//      buf         - The string to store the attempt type
//              - The enum attempt_type to be converted
//
// Returns:
//      None
void type_to_string(char *buf, enum attempt_type type) {
    if (type == FIRST_GO) {
        strcpy(buf, "FIRST GO");
    } else if (type == SUCCESS) {
        strcpy(buf, "SUCCESS");
    } else if (type == FAIL) {
        strcpy(buf, "FAIL");
    } else {
        strcpy(buf, "INVALID");
    }
}
