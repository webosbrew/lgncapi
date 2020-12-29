#pragma once

#ifdef __cplusplus
#define EXTERN_C_BEGIN \
    extern "C"         \
    {
#define EXTERN_C_END }
#else
#define EXTERN_C_BEGIN
#define EXTERN_C_END
#endif

#ifdef DEBUG
#include <json.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h>
EXTERN_C_BEGIN
static int detect_gdb(void)
{
    int rc = 0;
    FILE *fd = fopen("/tmp", "r");

    if (fileno(fd) > 3)
    {
        rc = 1;
    }

    fclose(fd);
    return rc;
}

static void debug_helper_setup(int argc, char **argv)
{
    if (argc < 2)
    {
        return;
    }
    json_object *json, *parameters, *param_wait_for_debugger;
    json = json_tokener_parse(argv[1]);
    if (json == NULL)
    {
        return;
    }
    if (!json_object_object_get_ex(json, "parameters", &parameters))
    {
        goto free_json;
    }
    if (!json_object_object_get_ex(parameters, "wait_for_debugger", &param_wait_for_debugger))
    {
        goto free_params;
    }
    if (strcmp("true", json_object_get_string(param_wait_for_debugger)) == 0)
    {
        raise(SIGSTOP);
    }
    json_object_put(param_wait_for_debugger);
free_params:
    json_object_put(parameters);
free_json:
    json_object_put(json);
}
EXTERN_C_END
#define DEBUG_HELPER(argc, argv) debug_helper_setup(argc, argv)
#else
#define DEBUG_HELPER(argc, argv)
#endif