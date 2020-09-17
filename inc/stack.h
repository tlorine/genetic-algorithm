#ifndef STACH_H
# define STACH_H

typedef struct s_stack
{
    int x;
    int y;
    struct s_stack *next;
}               t_stack;

class stack
{
private:
    int size;
    t_stack *stack_str;
    t_stack *stack_str_end;
public:
    stack();
    void push(int x, int y);
    void delete_elem();
    t_stack *get_elem();
    ~stack();
};

#endif