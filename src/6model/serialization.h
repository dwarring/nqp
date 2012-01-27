/* This represents the root of the serialization data; everything hangs
 * off this. In read mode, we don't do much besides populate and then
 * read this. In write mode, however, the tables and data chunks will be
 * filled out and grown as needed. */
typedef struct {
    /* The version of the serialization format. */
    Parrot_Int4   version;

    /* The number of dependencies, as well as a pointer to the
     * dependencies table. */
    Parrot_Int4   num_dependencies;
    char         *dependencies_table;
    
    /* The SC we're serializing/deserializing. */
    PMC          *sc;

    /* List of the serialization context objects that we depend on. */
    PMC          *dependent_scs;
    
    /* The number of STables, as well as pointers to the STables
     * table and data chunk. */
    Parrot_Int4   num_stables;
    char         *stables_table;
    char         *stables_data;
    
    /* The number of objects, as well as pointers to the objects
     * table and data chunk. */
    Parrot_Int4   num_objects;
    char         *objects_table;
    char         *objects_data;
    
    /* Array of STRINGs. */
    PMC          *string_heap;
} SerializationRoot;

/* Represents the serialization reader and the various functions available
 * on it. */
typedef struct SerializationReader {
    /* Serialization root data. */
    SerializationRoot root;
    
    /* The stables list and object list we're deserializing things into. */
    PMC *stables_list;
    PMC *objects_list;
    
    /* Flag for if we're reading the stable data chunk or object data chunk. */
    char reading_object;
    
    /* Current offsets for the data chunks (also correspond to the amount of
     * data written in to them). */
    Parrot_Int4 stables_data_offset;
    Parrot_Int4 objects_data_offset;
    
    /* Limits up to where we can read stables and objects data. */
    char *stables_data_end;
    char *objects_data_end;
    
    /* Various reading functions. */
    INTVAL   (*read_int) (PARROT_INTERP, struct SerializationReader *reader);
    FLOATVAL (*read_num) (PARROT_INTERP, struct SerializationReader *reader);
    STRING * (*read_str) (PARROT_INTERP, struct SerializationReader *reader);
    PMC *    (*read_ref) (PARROT_INTERP, struct SerializationReader *reader);
} SerializationReader;

/* Represents the serialization writer and the various functions available
 * on it. */
typedef struct SerializationWriter {
    /* Serialization root data. */
    SerializationRoot root;
    
    /* The stables list and object list we're working through. */
    PMC *stables_list;
    PMC *objects_list;
    
    /* Current position in the stables list and objects list. */
    INTVAL stables_list_pos;
    INTVAL objects_list_pos;

    /* Hash of strings we've already seen while serializing to the index they
     * are placed at in the string heap. */
    PMC *seen_strings;
    
    /* Amount of memory allocated for various things. */
    Parrot_Int4 dependencies_table_alloc;
    Parrot_Int4 stables_table_alloc;
    Parrot_Int4 stables_data_alloc;
    Parrot_Int4 objects_table_alloc;
    Parrot_Int4 objects_data_alloc;
    
    /* Current offsets for the data chunks (also correspond to the amount of
     * data written in to them). */
    Parrot_Int4 stables_data_offset;
    Parrot_Int4 objects_data_offset;
    
    /* Flag for if we're writing to stable data chunk or object data chunk. */
    char writing_object;
    
    /* Various writing functions. */
    void (*write_int) (PARROT_INTERP, struct SerializationWriter *writer, INTVAL value);
    void (*write_num) (PARROT_INTERP, struct SerializationWriter *writer, FLOATVAL value);
    void (*write_str) (PARROT_INTERP, struct SerializationWriter *writer, STRING *value);
    void (*write_ref) (PARROT_INTERP, struct SerializationWriter *writer, PMC *value);
} SerializationWriter;

/* Core serialize and deserialize functions. */
STRING * Serialization_serialize(PARROT_INTERP, PMC *sc, PMC *empty_string_heap);
void Serialization_deserialize(PARROT_INTERP, PMC *sc, PMC *string_heap, STRING *data);