#include "exml_gc.h"

static ERL_NIF_TERM cleanup(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    int cleanups = ns_cleanup();
    return enif_make_int(env, cleanups);
}

static int load(ErlNifEnv* env, void **priv, ERL_NIF_TERM info)
{
    return 0;
};

static int reload(ErlNifEnv* env, void** priv, ERL_NIF_TERM info)
{
    return 0;
}

static int upgrade(ErlNifEnv* env, void** priv, void** old_priv, ERL_NIF_TERM info)
{
    *priv = *old_priv;
    return 0;
}

static void unload(ErlNifEnv* env, void* priv)
{
    ns_release();
}

static ErlNifFunc funcs[] =
    {
        {"cleanup", 0, cleanup}
    };

ERL_NIF_INIT(exml_gc, funcs, &load, &reload, &upgrade, &unload);
