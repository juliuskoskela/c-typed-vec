#ifndef TYPED_VECTOR_H
# define TYPED_VECTOR_H

#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "stdbool.h"
#include "memswap/memswap.h"

typedef unsigned char t_byte;

typedef struct s_vec
{
    unsigned char   *memory;
    size_t          alloc_size;
    size_t          len;
}   t_vec;

typedef struct s_vec_handle
{
    void *_private;
}   t_vec_handle;

# define TYPED_VECTOR_HEADER(def, type)                                          \
                                                                                 \
typedef t_vec_handle def;                                                        \
                                                                                 \
size_t	def##_len(def *src);                                                     \
size_t	def##_alloc_size(def *src);                                              \
size_t	def##_elem_size();                                                       \
size_t	def##_byte_size(def *src);                                               \
int		def##_new(def *dst, size_t init_len);                                    \
int		def##_new_from(def *dst, type *src, size_t len);                         \
void	def##_free(def *src);                                                    \
type	*def##_get(def *src, size_t index);                                      \
int		def##_rcopy(def *dst, def *src, size_t start, size_t end);               \
int		def##_ncopy(def *dst, def *src, size_t nelems);                          \
int		def##_copy(def *dst, def *src);                                          \
int		def##_push(def *dst, type src);                                          \
int		def##_pop(type *dst, def *src);                                          \
int 	def##_clear(def *src);                                                   \
int		def##_insert(def *dst, type src, size_t index);                          \
int		def##_remove(def *src, size_t index);                                    \
int		def##_append(def *dst, def *src);                                        \
int		def##_iter(def *src, void (*f) (type));                                  \
type	*def##_find(def *src, bool (*f) (type));                                 \
int		def##_map(def *dst, def *src, type (*f) (type));                         \
int		def##_filter(def *dst, def *src, bool (*f) (type));                      \
int		def##_reduce(type *dst, def *src, type (*f) (type, type));               \
int		def##_sort(def *src, int (*f)(type, type));                              \

/////////////////////////////////////////////////////////////////////////////////

# define TYPED_VECTOR(def, type)                                                 \
                                                                                 \
typedef t_vec_handle def;                                                        \
                                                                                 \
size_t def##_len(def *src)                                                       \
{                                                                                \
    t_vec *opaq;                                                                 \
                                                                                 \
    if (!src)                                                                    \
        return (-1);                                                             \
    if (!src->_private)                                                          \
        return (0);                                                              \
    opaq = (t_vec *)src->_private;                                               \
    return (opaq->len);                                                          \
}                                                                                \
                                                                                 \
static void def##_len_increment(def *src, size_t amount)                         \
{                                                                                \
    t_vec *opaq;                                                                 \
                                                                                 \
    opaq = (t_vec *)src->_private;                                               \
    opaq->len += amount;                                                         \
}                                                                                \
                                                                                 \
static void def##_len_decrement(def *src, size_t amount)                         \
{                                                                                \
    t_vec *opaq;                                                                 \
                                                                                 \
    opaq = (t_vec *)src->_private;                                               \
    opaq->len -= amount;                                                         \
}                                                                                \
                                                                                 \
static void def##_len_set(def *src, size_t amount)                               \
{                                                                                \
    t_vec *opaq;                                                                 \
                                                                                 \
    opaq = (t_vec *)src->_private;                                               \
    opaq->len = amount;                                                          \
}                                                                                \
                                                                                 \
size_t def##_alloc_size(def *src)                                                \
{                                                                                \
    t_vec *opaq;                                                                 \
                                                                                 \
    if (!src)                                                                    \
        return (-1);                                                             \
    if (!src->_private)                                                          \
        return (0);                                                              \
    opaq = (t_vec *)src->_private;                                               \
    return (opaq->alloc_size);                                                   \
}                                                                                \
                                                                                 \
size_t def##_elem_size()                                                         \
{                                                                                \
    return (sizeof(type));                                                       \
}                                                                                \
                                                                                 \
size_t def##_byte_size(def *src)                                                 \
{                                                                                \
    if (!src)                                                                    \
        return (-1);                                                             \
    if (!src->_private)                                                          \
        return (0);                                                              \
    return (def##_elem_size() * def##_len(src));                                 \
}                                                                                \
                                                                                 \
static int def##_alloc(def *src, size_t alloc_size)                              \
{                                                                                \
    t_vec *opaq;                                                                 \
                                                                                 \
    if (!src)                                                                    \
        return (-1);                                                             \
    src->_private = malloc(sizeof(t_vec));                                       \
    if (!src->_private)                                                          \
        return (-1);                                                             \
    opaq = (t_vec *)src->_private;                                               \
    opaq->memory = malloc(alloc_size);                                           \
    if (!opaq->memory)                                                           \
        return (-1);                                                             \
    opaq->alloc_size = alloc_size;                                               \
    opaq->len = 0;                                                               \
    return (1);                                                                  \
}                                                                                \
                                                                                 \
static bool def##_null(def *src)                                                 \
{                                                                                \
    if (!src)                                                                    \
        return (-1);                                                             \
    return (src->_private == NULL);                                              \
}                                                                                \
                                                                                 \
static type *def##_memory(def *src, size_t byte_index)                           \
{                                                                                \
    t_vec *opaq;                                                                 \
                                                                                 \
    if (!src || def##_null(src) || byte_index > def##_alloc_size(src))           \
        return (NULL);                                                           \
    opaq = (t_vec *)src->_private;                                               \
    return ((type *)&opaq->memory[byte_index]);                                  \
}                                                                                \
                                                                                 \
int def##_new(def *dst, size_t init_len)                                         \
{                                                                                \
    if (!dst)                                                                    \
        return (-1);                                                             \
    dst->_private = NULL;                                                        \
    if (init_len > 0)                                                            \
        return (def##_alloc(dst, init_len * def##_elem_size()));                 \
    return (1);                                                                  \
}                                                                                \
                                                                                 \
int def##_new_from(def *dst, type *src, size_t len)                              \
{                                                                                \
    if (!dst || !src || def##_new(dst, len) < 0)                                 \
        return (-1);                                                             \
    memcpy(                                                                      \
        def##_memory(dst, 0),                                                    \
        src,                                                                     \
        def##_alloc_size(dst));                                                  \
    def##_len_set(dst, len);                                                     \
    return (1);                                                                  \
}                                                                                \
                                                                                 \
void def##_free(def *src)                                                        \
{                                                                                \
    t_vec *opaq;                                                                 \
                                                                                 \
    if (!src || def##_null(src))                                                 \
        return ;                                                                 \
    opaq = (t_vec *)src->_private;                                               \
    free(opaq->memory);                                                          \
	free(opaq);                                                                  \
    src->_private = NULL;                                                        \
}                                                                                \
                                                                                 \
static int def##_realloc(def *src, size_t target_len)                            \
{                                                                                \
    t_vec *opaq;                                                                 \
                                                                                 \
    if (!src || !src->_private)                                                  \
        return (-1);                                                             \
    if (def##_null(src))                                                         \
        return (def##_new(src, target_len));                                     \
    opaq = (t_vec *)src->_private;                                               \
    opaq->memory = realloc(opaq->memory, target_len * def##_elem_size());        \
    opaq->alloc_size = target_len * def##_elem_size();                           \
    return (1);                                                                  \
}                                                                                \
                                                                                 \
type *def##_get(def *src, size_t index)                                          \
{                                                                                \
    if (!src)                                                                    \
        return (NULL);                                                           \
    if (def##_null(src) || index >= def##_len(src))                              \
        return (NULL);                                                           \
    return (def##_memory(src, def##_elem_size() * index));                       \
}                                                                                \
                                                                                 \
int def##_rcopy(def *dst, def *src, size_t start, size_t end)                    \
{                                                                                \
    size_t  copy_size;                                                           \
                                                                                 \
    if (!src || !dst || def##_null(src)                                          \
        || def##_null(dst)                                                       \
        || def##_elem_size() != def##_elem_size()                                \
        || end > def##_len(src)                                                  \
        || start > end                                                           \
        || def##_alloc_size(dst) < (end - start) * def##_elem_size())            \
        return (-1);                                                             \
    if (def##_byte_size(src) < def##_alloc_size(dst))                            \
        copy_size = def##_byte_size(src);                                        \
    else                                                                         \
        copy_size = def##_alloc_size(dst);                                       \
    memcpy(                                                                      \
        def##_memory(dst, 0),                                                    \
        def##_memory(src, start * def##_elem_size()),                            \
        copy_size);                                                              \
    def##_len_set(dst, end - start);                                             \
    return (1);                                                                  \
}                                                                                \
                                                                                 \
int def##_ncopy(def *dst, def *src, size_t nelems)                               \
{                                                                                \
    return (def##_rcopy(dst, src, 0, nelems));                                   \
}                                                                                \
                                                                                 \
int def##_copy(def *dst, def *src)                                               \
{                                                                                \
    return (def##_ncopy(dst, src, def##_len(src)));                              \
}                                                                                \
                                                                                 \
int def##_push(def *dst, type src)                                               \
{                                                                                \
    if (!dst)                                                                    \
        return (-1);                                                             \
    if (def##_null(dst))                                                         \
        def##_new(dst, 1);                                                       \
    if (def##_byte_size(dst) >= def##_alloc_size(dst))                           \
        if (def##_realloc(dst, def##_len(dst) * 2) < 0)                          \
            return (-1);                                                         \
    memcpy(                                                                      \
        def##_memory(dst, def##_byte_size(dst)),                                 \
        &src,                                                                    \
        def##_elem_size());                                                      \
    def##_len_increment(dst, 1);                                                 \
    return (1);                                                                  \
}                                                                                \
                                                                                 \
int def##_pop(type *dst, def *src)                                               \
{                                                                                \
    if (!dst || !src)                                                            \
        return (-1);                                                             \
    else if (def##_null(src) || def##_len(src) == 0)                             \
        return (0);                                                              \
    memcpy(                                                                      \
        dst,                                                                     \
        def##_get(src, def##_len(src) - 1),                                      \
        def##_elem_size());                                                      \
    def##_len_decrement(src, 1);                                                 \
    return (1);                                                                  \
}                                                                                \
                                                                                 \
int def##_clear(def *src)                                                        \
{                                                                                \
    if (!src || def##_null(src))                                                 \
	    return (-1);                                                             \
	def##_len_set(src, 0);                                                       \
	return (1);                                                                  \
}                                                                                \
                                                                                 \
int def##_insert(def *dst, type src, size_t index)                               \
{                                                                                \
    if (!dst || index > def##_len(dst))                                          \
        return (-1);                                                             \
    else if (index == def##_len(dst))                                            \
        return (def##_push(dst, src));                                           \
    if (def##_byte_size(dst) >= def##_alloc_size(dst))                           \
        if (def##_realloc(dst, def##_byte_size(dst) * 2) < 0)                    \
            return (-1);                                                         \
    memmove(                                                                     \
        def##_get(dst, index + 1),                                               \
        def##_get(dst, index),                                                   \
        (def##_len(dst) - index) * def##_elem_size());                           \
    memcpy(                                                                      \
        def##_get(dst, index),                                                   \
        &src,                                                                    \
        def##_elem_size());                                                      \
    def##_len_increment(dst, 1);                                                 \
    return (1);                                                                  \
}                                                                                \
                                                                                 \
int def##_remove(def *src, size_t index)                                         \
{                                                                                \
    if (!src || index > def##_len(src))                                          \
        return (-1);                                                             \
    else if (index == def##_len(src))                                            \
    {                                                                            \
        def##_len_decrement(src, 1);                                             \
        return (1);                                                              \
    }                                                                            \
    memmove(                                                                     \
        def##_get(src, index),                                                   \
        def##_memory(src, def##_elem_size() * (index + 1)),                      \
        (def##_len(src) - index) * def##_elem_size());                           \
    def##_len_decrement(src, 1);                                                 \
    return (1);                                                                  \
}                                                                                \
                                                                                 \
int def##_append(def *dst, def *src)                                             \
{                                                                                \
    int     ret;                                                                 \
    size_t  alloc_size;                                                          \
                                                                                 \
    if (!dst || !src || def##_null(src))                                         \
        return (-1);                                                             \
    else if (def##_null(dst))                                                    \
        def##_new(dst, 1);                                                       \
    alloc_size = def##_byte_size(dst) + def##_byte_size(src);                    \
    if (def##_alloc_size(dst) < alloc_size)                                      \
    {                                                                            \
        if (def##_alloc_size(dst) * 2 < alloc_size)                              \
            ret = def##_realloc(dst, alloc_size);                                \
        else                                                                     \
            ret = def##_realloc(dst, def##_alloc_size(dst) * 2);                 \
        if (ret < 0)                                                             \
            return (-1);                                                         \
    }                                                                            \
    memcpy(                                                                      \
        def##_memory(dst, def##_byte_size(dst)),                                 \
        def##_memory(src, 0),                                                    \
        def##_byte_size(src));                                                   \
    def##_len_increment(dst, def##_len(src));                                    \
    return (1);                                                                  \
}                                                                                \
                                                                                 \
int def##_iter(def *src, void (*f) (type))                                       \
{                                                                                \
    t_vec   *opaq;                                                               \
    type    *mem;                                                                \
                                                                                 \
    if (!src || def##_null(src))                                                 \
        return (-1);                                                             \
    opaq = (t_vec *)src->_private;                                               \
    mem = (type *)opaq->memory;                                                  \
    for (size_t i = 0; i < def##_len(src); i++)                                  \
        f(mem[i]);                                                               \
    return (1);                                                                  \
}                                                                                \
                                                                                 \
type *def##_find(def *src, bool (*f) (type))                                     \
{                                                                                \
    t_vec   *opaq;                                                               \
    type    res;                                                                 \
    type    *mem;                                                                \
                                                                                 \
    if (!src || def##_null(src))                                                 \
        return (NULL);                                                           \
    opaq = (t_vec *)src->_private;                                               \
    mem = (type *)opaq->memory;                                                  \
    for (size_t i = 0; i < def##_len(src); i++)                                  \
    {                                                                            \
        res = mem[i];                                                            \
        if (f(res) == true)                                                      \
            return (&mem[i]);                                                    \
    }                                                                            \
    return (NULL);                                                               \
}                                                                                \
                                                                                 \
int def##_map(def *dst, def *src, type (*f) (type))                              \
{                                                                                \
    type    res;                                                                 \
    t_vec    *opaq;                                                              \
    type    *mem;                                                                \
                                                                                 \
    if (!dst || !src || def##_null(src))                                         \
        return (-1);                                                             \
    else if (def##_null(dst))                                                    \
        def##_new(dst, 1);                                                       \
    opaq = (t_vec *)src->_private;                                               \
    mem = (type *)opaq->memory;                                                  \
    for (size_t i = 0; i < def##_len(src); i++)                                  \
    {                                                                            \
        res = mem[i];                                                            \
        res = f(res);                                                            \
        def##_push(dst, res);                                                    \
    }                                                                            \
    return (1);                                                                  \
}                                                                                \
                                                                                 \
int def##_filter(def *dst, def *src, bool (*f) (type))                           \
{                                                                                \
    type    res;                                                                 \
    t_vec    *opaq;                                                              \
    type    *mem;                                                                \
                                                                                 \
    if (!dst || !src || def##_null(src))                                         \
        return (-1);                                                             \
    else if (def##_null(dst))                                                    \
        def##_new(dst, 1);                                                       \
    opaq = (t_vec *)src->_private;                                               \
    mem = (type *)opaq->memory;                                                  \
    for (size_t i = 0; i < def##_len(src); i++)                                  \
    {                                                                            \
        res = mem[i];                                                            \
        if (f(res) == true)                                                      \
            def##_push(dst, res);                                                \
    }                                                                            \
    return (1);                                                                  \
}                                                                                \
                                                                                 \
int def##_reduce(type *dst, def *src, type (*f) (type, type))                    \
{                                                                                \
    type    res;                                                                 \
    t_vec    *opaq;                                                              \
    type    *mem;                                                                \
                                                                                 \
    if (!dst || !src || def##_null(src))                                         \
        return (-1);                                                             \
    opaq = (t_vec *)src->_private;                                               \
    mem = (type *)opaq->memory;                                                  \
    for (size_t i = 0; i < def##_len(src); i++)                                  \
    {                                                                            \
        res = mem[i];                                                            \
        *dst = f(*dst, res);                                                     \
    }                                                                            \
    return (1);                                                                  \
}                                                                                \
                                                                                 \
static void def##_sort_recurse(def *src,                                         \
    long int low,                                                                \
    long int high,                                                               \
    int (*f)(type, type))                                                        \
{                                                                                \
    long int pivot;                                                              \
    long int a;                                                                  \
    long int b;                                                                  \
    t_vec *opaq;                                                                 \
    type    *mem;                                                                \
                                                                                 \
    opaq = (t_vec *)src->_private;                                               \
    mem = (type *)opaq->memory;                                                  \
    if (low >= high)                                                             \
        return ;                                                                 \
    pivot = low;                                                                 \
    a = low;                                                                     \
    b = high;                                                                    \
    while (a < b)                                                                \
    {                                                                            \
        while (a <= high && f(mem[a], mem[pivot]) <= 0)                          \
            a++;                                                                 \
        while (b >= low && f(mem[b], mem[pivot]) > 0)                            \
            b--;                                                                 \
        if (a < b)                                                               \
            memswap(&mem[a], &mem[b], def##_elem_size());    \
    }                                                                            \
    memswap(&mem[pivot], &mem[b], def##_elem_size());        \
    def##_sort_recurse(src, low, b - 1, f);                                      \
    def##_sort_recurse(src, b + 1, high, f);                                     \
}                                                                                \
                                                                                 \
int def##_sort(def *src, int (*f)(type, type))                                   \
{                                                                                \
    if (!src || def##_null(src))                                                 \
        return (-1);                                                             \
    def##_sort_recurse(src, 0, def##_len(src) - 1, f);                           \
    return (1);                                                                  \
}                                                                                \

#endif
