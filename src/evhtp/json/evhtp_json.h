#ifndef __EVHTP_JSON_H__
#define __EVHTP_JSON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <evhtp/heap.h>
#include <evhtp/keyval.h>
#include <evhtp/array.h>

enum evhtp_json_vtype_e {
    evhtp_json_vtype_string = 0,
    evhtp_json_vtype_number,
    evhtp_json_vtype_object,
    evhtp_json_vtype_array,
    evhtp_json_vtype_bool,
    evhtp_json_vtype_null
};

struct evhtp_json_s
#ifdef EVHTP_IN_DOXYGEN
{ /*Empty body so that doxygen will generate documentation here.*/ }
#endif
;

typedef enum evhtp_json_vtype_e evhtp_json_vtype;
typedef struct evhtp_json_s     evhtp_json;

/**
 * @brief used for comparison filtering
 *
 * @param key key string
 * @param val json value
 *
 * @return 0 if matched, -1 if no match
 * @see evhtp_json_compare()
 */
typedef int (* evhtp_json_key_filtercb)(const char * key, evhtp_json * val);


/**
 * @brief creates a new unordered-keyval context
 *
 * @return evhtp_json context with a vtype of 'evhtp_json_vtype_object'
 * @see evhtp_json_free()
 */
EVHTP_EXPORT
evhtp_json * evhtp_json_object_new(void);


/**
 * @brief creates a new ordered array of evhtp_json context values
 *
 * @return evhtp_json context with a vtype of 'evhtp_json_vtype_array'
 * @see evhtp_json_free()
 */
EVHTP_EXPORT
evhtp_json * evhtp_json_array_new(void);


/**
 * @brief frees data associated with a evhtp_json context. Objects and arrays
 *        will free all the resources contained within in a recursive manner.
 *
 * @param js
 */
EVHTP_EXPORT
void evhtp_json_free(evhtp_json * js);

/**
 * @brief creates a evhtp_json context for a null terminated string
 *        and copies the original string
 *
 * @param str
 *
 * @return evhtp_json context with vtype of 'evhtp_json_vtype_string'
 * @see evhtp_json_free()
 * @see evhtp_json_string_len_new()
 */
EVHTP_EXPORT
evhtp_json * evhtp_json_string_new(const char * str);


/**
 * @brief a variant of evhtp_json_string_new which can be faster if you
 *          already know the length.
 *
 * @param str
 * @param len length of the string
 *
 * @return
 * @see evhtp_json_free()
 * @see evhtp_json_string_new()
 */
EVHTP_EXPORT
evhtp_json * evhtp_json_string_len_new(
    const char * str,
    size_t       len);


/**
 * @brief creates a evhtp_json context for a number. Note: this only supports
 *        unsigned int right now (casted for signed).
 *
 * @param num
 *
 * @return evhtp_json context with vtype of 'evhtp_json_vtype_number'
 * @see evhtp_json_free()
 */
EVHTP_EXPORT
evhtp_json * evhtp_json_number_new(unsigned int num);


/**
 * @brief creates a evhtp_json context for a boolean value.
 *
 * @param boolean either true or false
 *
 * @return evhtp_json context with vtype of 'evhtp_json_vtype_bool'
 * @see evhtp_json_free()
 */
EVHTP_EXPORT
evhtp_json * evhtp_json_boolean_new(bool boolean);


/**
 * @brief creates a evhtp_json context for a null value
 *
 * @return evhtp_json context with vtype of 'evhtp_json_vtype_null'
 * @see evhtp_json_free()
 */
EVHTP_EXPORT
evhtp_json * evhtp_json_null_new(void);

/**
 * @brief parse a buffer containing raw json and convert it to the
 *        internal evhtp_json.
 *
 * @param data the buffer to parse
 * @param len the length of the data in the buffer
 * @param n_read the number of bytes which were parsed will be stored
 *        here.
 *
 * @return evhtp_json context if valid json, NULL on error (also see n_read above)
 */
EVHTP_EXPORT
evhtp_json * evhtp_json_parse_buf(
    const char * data,
    size_t       len,
    size_t     * n_read);

/**
 * @brief wrapper around evhtp_json_parse_buf but opens, reads, parses, and closes
 *        a file with JSON data.
 *
 * @param filename
 * @param n_read number of bytes parsed
 *
 * @return see evhtp_json_parse_buf
 */
EVHTP_EXPORT
evhtp_json * evhtp_json_parse_file(
    const char * filename,
    size_t     * n_read);

/* these next set of parser functions are self explainatory, and will probably be
 * made private in the future.
 */
EVHTP_EXPORT
evhtp_json * evhtp_json_parse_object(
    const char * data,
    size_t       len,
    size_t     * n_read);

EVHTP_EXPORT
evhtp_json * evhtp_json_parse_array(
    const char * data,
    size_t       len,
    size_t     * n_read);

EVHTP_EXPORT
evhtp_json * evhtp_json_parse_value(
    const char * data,
    size_t len, size_t * n_read);

EVHTP_EXPORT
evhtp_json * evhtp_json_parse_string(
    const char * data,
    size_t len, size_t * n_read);

EVHTP_EXPORT
evhtp_json * evhtp_json_parse_number(
    const char * data,
    size_t len, size_t * n_read);

EVHTP_EXPORT
evhtp_json * evhtp_json_parse_key(
    const char * data,
    size_t len, size_t * n_read);

EVHTP_EXPORT
evhtp_json * evhtp_json_parse_null(
    const char * data,
    size_t len, size_t * n_read);

EVHTP_EXPORT
evhtp_json * evhtp_json_parse_boolean(
    const char * data,
    size_t len, size_t * n_read);

/**
 * @brief returns the underlying type of the evhtp_json context
 *
 * @param js
 *
 * @return
 */
EVHTP_EXPORT
evhtp_json_vtype evhtp_json_get_type(evhtp_json * js);


/**
 * @brief returns the underlying evhtp_keyval ADT for an object
 *
 * @param js
 *
 * @return evhtp_keyval on success, NULL if the underlying data
 *         is not an object.
 */
EVHTP_EXPORT
evhtp_keyval * evhtp_json_get_object(evhtp_json * js);

/**
 * @brief returns the underlying evhtp_array ADT for an array
 *
 * @param js
 *
 * @return evhtp_array on success, NULL if the underlying data is
 *         not an array.
 */
EVHTP_EXPORT
evhtp_array * evhtp_json_get_array(evhtp_json * js);


/**
 * @brief fetches the evhtp_json value in an array at a specific index
 *
 * @param js
 * @param index
 *
 * @return the evhtp_json value, NULL if not found
 */
EVHTP_EXPORT
evhtp_json * evhtp_json_get_array_index(
    evhtp_json * js,
    int          index);


/**
 * @brief return the underlying number
 *
 * @param js
 *
 * @return
 */
EVHTP_EXPORT
unsigned int evhtp_json_get_number(evhtp_json * js);


/**
 * @brief fetches the underlying string of the evhtp_json context
 *
 * @param js
 *
 * @return the string on success, NULL if underlying data is not a string
 */
EVHTP_EXPORT
const char * evhtp_json_get_string(evhtp_json * js);


/**
 * @brief boolean fetch
 *
 * @param js
 *
 * @return true or false
 */
EVHTP_EXPORT
bool evhtp_json_get_boolean(evhtp_json * js);


/**
 * @brief null fetch
 *
 * @param js
 *
 * @return 1 if null, -1 if underlying data is not a null
 */
EVHTP_EXPORT
char evhtp_json_get_null(evhtp_json * js);


/**
 * @brief size fetch of underlying evhtp_json data:
 *          evhtp_json_vtype_string : length of the string
 *          evhtp_json_vtype_array  : the number of entries in the array
 *          evhtp_json_vtype_object : number of entries in the object
 *
 * @param js
 *
 * @return 0 if the underlying type doesn't have a size, otherwise see above
 */
EVHTP_EXPORT
ssize_t evhtp_json_get_size(evhtp_json * js);


/**
 * @brieffetch evhtp_json data using a special syntax
 *
 *        if the json is { "a" : 1, "b" : [1, 2, 3, { "foo": "bar" }], "c" : {"hi" : null} }
 *
 *        and you would like to get the second element of the key "b" the syntax would be:
 *           "b.[2]"
 *
 *        or to get the value of "foo" in this example:
 *           "b.[4].foo"
 *
 * @param js
 * @param path
 *
 * @return
 */
EVHTP_EXPORT
evhtp_json * evhtp_json_get_path(
    evhtp_json * js,
    const char * path);


/**
 * @brief add a string : evhtp_json context to an existing evhtp_json object
 *
 * @param obj
 * @param key
 * @param val
 *
 * @return
 * @see evhtp_json_object_klen_add()
 */
EVHTP_EXPORT
int evhtp_json_object_add(
    evhtp_json * obj,
    const char * key,
    evhtp_json * val);


/**
 * @brief same as evhtp_json_object_add, except if the length of the key is known,
 *        you can reduce overhead
 *
 * @param o
 * @param k
 * @param kl
 * @param v
 *
 * @return
 * @see evhtp_json_object_add()
 */
EVHTP_EXPORT
int evhtp_json_object_klen_add(
    evhtp_json * o,
    const char * k,
    size_t       kl,
    evhtp_json * v);


/**
 * @brief add a evhtp_json context to a evhtp_json array context
 *
 * @param array
 * @param val
 *
 * @return
 */
EVHTP_EXPORT
int evhtp_json_array_add(
    evhtp_json * array,
    evhtp_json * val);

/**
 * @brief can be used on either evhtp_json arrays or objects. For arrays, just omit the key
 *
 * @param obj
 * @param k
 * @param val
 *
 * @return
 */
EVHTP_EXPORT
int evhtp_json_add(
    evhtp_json * obj,
    const char * k,
    evhtp_json * val);


/**
 * @brief converts the evhtp_json ctx to a valid JSON string
 *
 * @param json
 * @param buf user supplied buffer
 * @param buf_len the length of the buffer
 *
 * @return number of bytes copied into the buffer
 */
EVHTP_EXPORT
ssize_t evhtp_json_to_buffer(
    evhtp_json * json,
    char       * buf,
    size_t       buf_len);


/**
 * @brief same as evhtp_json_to_buffer but does not escape strings
 *
 * @param json
 * @param buf
 * @param buf_len
 *
 * @return
 */
EVHTP_EXPORT
ssize_t evhtp_json_to_buffer_nescp(
    evhtp_json * json,
    char       * buf,
    size_t       buf_len);


/**
 * @brief converts to a malloc'd JSON string
 *
 * @param json
 * @param len
 *
 * @return
 */
EVHTP_EXPORT
char * evhtp_json_to_buffer_alloc(
    evhtp_json * json,
    size_t     * len);


/**
 * @brief compares two evhtp_json contexts to determine if they match, if the filtercb
 *        argument is not NULL, and the type being compared currently is an array or object,
 *        each evhtp_json context is passed to the callback. If the callback returns -1, that
 *        value will not be compared to the other.
 *
 * @param j1
 * @param j2
 * @param cb
 *
 * @return
 */
EVHTP_EXPORT
int evhtp_json_compare(
    evhtp_json            * j1,
    evhtp_json            * j2,
    evhtp_json_key_filtercb cb);

#ifdef __cplusplus
}
#endif

#endif

