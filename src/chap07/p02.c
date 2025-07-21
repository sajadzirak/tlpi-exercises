#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define ALLOCATE_SIZE 512

struct Block {
    size_t size;
    char free;
    struct Block *next;
    struct Block *prev;
};

typedef struct Block Block;
#define BLOCK_SIZE sizeof(Block)

Block *base = NULL;

Block *search_for_space(size_t size)
{
	if (!base)
		return NULL;

    Block *itr = base;
    do
    {
        if (itr->free && itr->size >= size) // using first-fit strategy
            return itr;

        itr = itr->next;
    } while (itr != base);

    return NULL;
}

Block *allocate_block(size_t size)
{
    Block *addr;

    if ((addr = (Block *) sbrk(size)) == (void *) -1)
        return NULL;

    addr->free = 0;
    addr->size = size;
	if (base == NULL)
    {
		base = addr;
        base->prev = base;
        base->next = base;
    } else
    {
        base->prev->next = addr;
        addr->prev = base->prev;
        base->prev = addr; 
        addr->next = base;
    }

    return addr;
}

void *malloc(size_t size)
{
    if (size == 0)
        return NULL;

    size_t needed_size = BLOCK_SIZE + size;
    
    Block *search_result = search_for_space(needed_size);
    if (search_result)
    {
        if (search_result->size - needed_size > BLOCK_SIZE)
        {
            Block *splitted_block = (Block *) (((void *) search_result) + needed_size);
            splitted_block->free = 1;
            splitted_block->size = search_result->size - needed_size;
            splitted_block->prev = search_result;
            splitted_block->next = search_result->next;
            search_result->next->prev = splitted_block;
            search_result->next = splitted_block;
            search_result->size = needed_size;
        }

        search_result->free = 0;
        return (void *) (search_result + 1);
    }

    Block *new_block = allocate_block(needed_size);
	if (new_block)
	{
		return (void *) (new_block + 1);
	}

	return NULL;
}

void coalesce(Block *block)
{
    while (block->next != base && block->next->free)
    {
        block->size += block->next->size;
        block->next->prev = block;
        block->next = block->next->next;
    }

    while (block != base && block->prev->free)
    {
        block->prev->size += block->size;
        block->prev->next = block->next;
        block->next->prev = block->prev;
        block = block->prev;
    }
}

void free(void *ptr)
{
    if (ptr == NULL)
        return;
    
    Block *block = ((Block *)ptr) - 1;
    block->free = 1;

    coalesce(block);
}