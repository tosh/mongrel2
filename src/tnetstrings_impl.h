#ifndef _tnetstrings_impl_h
#define _tnetstrings_impl_h

#include "dbg.h"
#include <assert.h>

static inline tns_value_t *tns_value_create(tns_type_tag tag)
{
    tns_value_t *val = malloc(sizeof(tns_value_t));
    val->type = tag;
    return val;
}


//  Functions for manipulating compound datatypes.
static inline void *tns_new_dict(void)
{
    tns_value_t *val = tns_value_create(tns_tag_dict);
    val->value.dict = hash_create(MAX_HASH_COUNT, (hash_comp_t)bstrcmp, bstr_hash_fun);
    hash_set_allocator(val->value.dict, tns_hnode_alloc, tns_hnode_free, NULL);

    return val;
}

static inline int tns_add_to_dict(void *dict, void *key, void *item)
{
    hash_t *h = ((tns_value_t *)dict)->value.dict;
    tns_value_t *k = key;

    check(k->type == tns_tag_string, "Only strings can be keys.");
    check(hash_alloc_insert(h, k->value.string, item), "Failed to insert key into dict.");

    free(k); // no longer needed
    return 0;
error:
    return -1;
}


//  Constructors to get constant primitive datatypes.
static inline void *tns_get_null(void)
{
    return tns_value_create(tns_tag_null);
}

static inline void *tns_get_true(void)
{
    tns_value_t *t = tns_value_create(tns_tag_bool);
    t->value.bool = 1;
    return t;
}

static inline void *tns_get_false(void)
{
    tns_value_t *t = tns_value_create(tns_tag_bool);
    t->value.bool = 0;
    return t;
}

static inline void *tns_new_list(void)
{
    tns_value_t *val = tns_value_create(tns_tag_list);
    val->value.list = list_create(LISTCOUNT_T_MAX);

    return val;
}

static inline int tns_add_to_list(void *list, void *item)
{
    list_t *L = ((tns_value_t *)list)->value.list;

    lnode_t *node = lnode_create(item);
    list_append(L, node);

    return 0;
}


//  Functions to introspect the type of a data object.
static inline tns_type_tag tns_get_type(void *val)
{
    tns_value_t *t = (tns_value_t *)val;
    return t->type;
}

static inline void *tns_parse_string(const char *data, size_t len)
{
    tns_value_t *t = tns_value_create(tns_tag_string);
    t->value.string = blk2bstr(data, len);
    return t;
}

static inline void *tns_parse_integer(const char *data, size_t len)
{
    tns_value_t *t = tns_value_create(tns_tag_number);
    t->value.number = strtol(data, NULL, 10);
    return t;
}


static inline void *tns_new_integer(long number)
{
    tns_value_t *t = tns_value_create(tns_tag_number);
    t->value.number = number;
    return t;
}


// convenience functions for doing stuff with dictionaries
static inline int tns_dict_setcstr(tns_value_t *d, const char *key, tns_value_t *val)
{
    return tns_add_to_dict(d, tns_parse_string(key, strlen(key)), val);
}

static inline int tns_dict_set(tns_value_t *d, bstring key, tns_value_t *val)
{
    return tns_add_to_dict(d, tns_parse_string(bdata(key), blength(key)), val);
}

static inline int tns_list_addstr(tns_value_t *d, bstring element)
{
    return tns_add_to_list(d, 
            tns_parse_string(bdata(element), blength(element)));
}

#endif
