#include "stack.h"
#include <iostream>
stack::stack()
{
    stack_str = nullptr;
    stack_str_end = nullptr;
}

void stack::push(int x, int y)
{
    t_stack *tmp;
    if (!stack_str)
    {
        stack_str = new t_stack;
        stack_str->x = x;
        stack_str->y = y;
        stack_str->next = nullptr;
        stack_str_end = stack_str;
    }
    else
    {
        tmp = new t_stack;
        tmp->x = x;
        tmp->y = y;
        tmp->next = nullptr;
        stack_str_end->next = tmp;
        stack_str_end = stack_str_end->next;
    }
}

void stack::delete_elem()
{
    t_stack *tmp;

    tmp = stack_str;
    stack_str = stack_str->next;
    delete tmp;
}

t_stack *stack::get_elem()
{
    return stack_str;
}

stack::~stack()
{
}