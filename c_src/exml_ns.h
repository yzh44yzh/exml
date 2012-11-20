#ifndef EXML_NS_H
#define EXML_NS_H

#include <erl_nif.h>
#include <expat.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

ERL_NIF_TERM ns_binary(const char*);
int ns_cleanup();
void ns_release();

typedef struct ns_list {
    struct ns_list* next;
    char* key;
    ERL_NIF_TERM term;
} ns_list;

#endif
