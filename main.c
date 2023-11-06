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
};

// Represents the logbook that contains info on each climbing route
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
void append_route(struct logbook *new_logbook);
void print_routes (struct logbook *new_logbook);
//2.1
void print_filter_route(struct logbook *new_logbook);

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
void command_loop(struct logbook *logbook){
    char command;
    while(scanf(" %c", &command) != EOF){
        if(command == 'r'){
            append_route(logbook);
        }else if(command == 'p'){
            print_routes(logbook);
        }else if(command == '?'){
            print_usage();
        }else if(command == 'f') {
            print_filter_route(logbook);
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

    struct logbook *new_logbook = create_logbook(NULL);
    command_loop(new_logbook);
    printf("\nGoodbye\n");

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////  YOUR FUNCTIONS //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
struct logbook *create_logbook(struct route *new_route) {
    struct logbook *new_logbook = malloc(sizeof(struct logbook));
    new_logbook->routes = NULL;

    return new_logbook;
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

//1.3
void append_route(struct logbook *new_logbook)
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
    if (new_logbook->routes == NULL) {
        new_logbook->routes = new_route;
    } else {
        //check if the route already exists
        struct route *current = new_logbook->routes;
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
        current = new_logbook->routes;
        while(current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_route;
    }
    printf("Route '%s' added successfully!\n", name);
}


void print_routes (struct logbook *new_logbook) {
    if (new_logbook->routes == NULL) {
        printf("There are no routes in this logbook!\n");
    } else {
        struct route *current = new_logbook->routes;
        int position = 1;
        while (current != NULL) {
            print_one_route(position, current);
            current = current->next;
            position++;
        }
    }
}

void print_filter_route(struct logbook *new_logbook){
    int difficulty_min, difficulty_max;
    scanf("%d %d", &difficulty_min, &difficulty_max);
    if(difficulty_min <= 0 || difficulty_min > 39 || difficulty_max <= 0 || difficulty_max > 39 || difficulty_min > difficulty_max)
    {
        printf("ERROR: Difficulty range invalid!\n");
        return;
    }
    int position = 1;
    struct route *current = new_logbook->routes;
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
