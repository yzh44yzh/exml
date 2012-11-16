#ifndef EXML_GC_H
#define EXML_GC_H

#include <erl_nif.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "exml_ns.h"

// functions 'exported' by exml_gc.c module
static ERL_NIF_TERM cleanup(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

#endif
