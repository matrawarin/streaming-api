#ifndef __LIBERP64_H
#define __LIBERP64_H

#include <stdio.h>
#include <string.h>

typedef void* _rpc_int;


_rpc_int server_create(char *addr, int port, _rpc_int handle);
_rpc_int server_close(_rpc_int stack);

_rpc_int server_recv(_rpc_int stack, char *buffer, int lenght);
_rpc_int server_send(_rpc_int stack, char *buffer, int length);

typedef struct _rpc_gui_stack {
    int pid;
    int top;
    int left;
    int right;
    int bottom;
}rpc_gui_stack;

_rpc_int guid_create(_rpc_int handle);

#endif
