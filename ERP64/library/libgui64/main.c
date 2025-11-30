#include <liberp64.h>

_rpc_int guid_create(_rpc_int handle)
{
    rpc_gui_stack *stack = (rpc_gui_stack*) handle;

    return stack;
}
