#include "exml_ns.h"

static ErlNifEnv* ns_env = NULL;
static ErlNifMutex* ns_mutex = NULL;
static ns_list* namespaces = NULL;

static ns_list* ns_lookup(const char* ns)
{
    ns_list* cur;
    for(cur = namespaces; cur != NULL; cur = cur->next) {
        if(strcmp(ns, cur->key) == 0) {
            return cur;
        }
    }
    return NULL;
};

static ns_list* ns_put(const char* ns)
{
    ns_list* new = malloc(sizeof(ns_list));
    size_t ns_len = strlen(ns);
    char* ns_string = malloc((ns_len+1)*sizeof(char));
    strcpy(ns_string, ns);

    new->next = namespaces;
    new->key = ns_string;
    new->term = enif_make_refc_binary(ns_env, ns_string, ns_len);

    namespaces = new;
    return new; 
};

static void ns_remove(ns_list* prev, ns_list* removed)
{
    if(removed != NULL && enif_release_refc_binary(removed->term) == 0)
    {
        if(prev == NULL)
            namespaces = removed->next;
        else 
            prev->next = removed->next;

        free(removed->key);
        free(removed);
    }
};

ERL_NIF_TERM ns_binary(const char* ns)
{
    ns_list* bin;
    if(ns_env == NULL)
        ns_env = enif_alloc_env();
    if(ns_mutex == NULL)
        ns_mutex = enif_mutex_create("ns");

    enif_mutex_lock(ns_mutex);
    bin = ns_lookup(ns);
    if(bin == NULL)
        bin = ns_put(ns);
    enif_mutex_unlock(ns_mutex);

    return bin->term;
};

int ns_cleanup()
{
    int cleans = 0;
    ns_list *cur, *prev;
    
    if(ns_mutex == NULL)
        ns_mutex = enif_mutex_create("ns");

    enif_mutex_lock(ns_mutex);
    for(prev = NULL, cur = namespaces; cur != NULL; cur = cur->next)
    {
        if(enif_binary_get_refc(cur->term) == 1) {
            cleans++;
            ns_remove(prev, cur);
        }
        else {
            prev = cur;
        }
    }
    enif_mutex_unlock(ns_mutex);

    return cleans;
};

void ns_release()
{
    ns_list *cur;
    for(cur = namespaces; cur != NULL; cur = cur->next)
        ns_remove(NULL, cur);

    if(ns_mutex != NULL)
        enif_mutex_destroy(ns_mutex);
    if(ns_env != NULL)
        enif_clear_env(ns_env);
};
