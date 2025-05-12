typedef enum {
    integer,
    floater
} type_t;

typedef struct {
    int num_parameters;
    type_t *parameter_types;
} parameters_t;