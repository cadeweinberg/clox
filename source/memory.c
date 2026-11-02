/**
 * Copyright (c) 2026 cade
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdlib.h>

#include "clox/memory.h"


/*
# malloc/free and friends; or, how I learned to stop worrying and love the heap.

Reading through [Memory Management Algorithms and Implementation] Described three
ways of organizing a heap. 

1. bitmap + binary-tree
    in this implementation a bitmap is used to denote used/free memory in distinct chunks/segments
    of some small unit. Then, a binary tree is used to maintain a mapping from address -> bitmap (bit, length)
    such that the bitmap can be used to identify used/free regions of memory, and the binary tree can 
    be used to associate regions of memory with specific pointers. 
    This technique has upsides and downsides:
        - the bitmap itself is small for the amount of memory it manages. the binary tree is not so much.
        - the lookup within the bitmap is relatively fast O(n) or linear complexity for searching. given 
            that it isnt ordered we cannot use binary search. The mapping in the binary tree can be O(log(n))
            if the tree is one that is naturally balanced. (AVL, Red-Black)
        - the overhead of both structures I expect to be large, though scalable. That is, my expectations about 
            the performance is that as the heap grows in size the cost of allocating and deallocating is large,
            yet roughly constant, and growing slowly. (mostly due to the high spacial locality of a bitmap and 
            the O(log(n)) complexity of a balanced binary tree.) it should be noted that this is speculation.
            as presented in the book, this was indeed the case. it was the slowest out of the three tested,
            within the limited performance testing done.
        - The choice of how to select a region of memory to satisfy a request is not decided by the choice of 
            data structure. so there is room for configuration in an implementation using this strategy.

2. sequential fit (or doubly linked list)
    in this implementation the heap is managed as a doubly linked list. each node can either be used or free.
    and to satisfy a request we take existing space and bisect it into two new nodes, one for the request and 
    the other to denote the free memory remaining. each has links to one another.
        - allocating and deallocating is relatively straightforward. there aren't that many cases to consider
            and it mostly comes down to pointer manipulation.
        - there is an overhead of two pointers per allocation, which can be a lot for many small allocations.
        - searching for a free node in the list can be arbitrarily expensive, and only gets worse with the 
            number of individual allocations.
        - There are techniques to mitigate these downsides, and reading other implementations, it looks as 
            though this basic strategy can be used as a building block in more complex schemes.
        - The overhead of this scheme is relatively small but constant for space, and O(n) in time, though
            this gets worse and worse with the number of allocations. 
        - the choice of how to select a region of memory to satisy a request is also not determined by the 
            data structure, it is again a policy choice of the implementation. 

3. segregated lists
    in this implementation multiple lists are maintained, each only allowed to hold an allocation of a particular
    size. so there would be a list for allocations of size 8, 16, 32, 64, and so on. This strategy works well, yet 
    it can suffer from internal fragmentation if it gets too full. there is no splitting, so it could happen that 
    a request of size 32 is satisfied by a element from a list of size 512. wasting a lot of memory.
    - allocating and deallocating is faster than with the sequential fit.


then I have been reading the source code for dlmalloc. apparently a classic malloc implementation. This uses what
they call, well, i'll just copy/paste in their docs 

    (The following includes lightly edited explanations by Colin Plumb.)

    Chunks of memory are maintained using a `boundary tag' method as
    described in e.g., Knuth or Standish.  (See the paper by Paul
    Wilson ftp://ftp.cs.utexas.edu/pub/garbage/allocsrv.ps for a
    survey of such techniques.)  Sizes of free chunks are stored both
    in the front of each chunk and at the end.  This makes
    consolidating fragmented chunks into bigger chunks very fast.  The
    size fields also hold bits representing whether chunks are free or
    in use.

    An allocated chunk looks like this:


    chunk-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            |             Size of previous chunk, if allocated            | |
            +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            |             Size of chunk, in bytes                         |P|
      mem-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            |             User data starts here...                          .
            .                                                               .
            .             (malloc_usable_space() bytes)                     .
            .                                                               |
nextchunk-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            |             Size of chunk                                     |
            +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+


    Where "chunk" is the front of the chunk for the purpose of most of
    the malloc code, but "mem" is the pointer that is returned to the
    user.  "Nextchunk" is the beginning of the next contiguous chunk.

    Chunks always begin on even word boundries, so the mem portion
    (which is returned to the user) is also on an even word boundary, and
    thus at least double-word aligned.

    Free chunks are stored in circular doubly-linked lists, and look like this:

    chunk-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            |             Size of previous chunk                            |
            +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    `head:' |             Size of chunk, in bytes                         |P|
      mem-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            |             Forward pointer to next chunk in list             |
            +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            |             Back pointer to previous chunk in list            |
            +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            |             Unused space (may be 0 bytes long)                .
            .                                                               .
            .                                                               |
nextchunk-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    `foot:' |             Size of chunk, in bytes                           |
            +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

    The P (PREV_INUSE) bit, stored in the unused low-order bit of the
    chunk size (which is always a multiple of two words), is an in-use
    bit for the *previous* chunk.  If that bit is *clear*, then the
    word before the current chunk size contains the previous chunk
    size, and can be used to find the front of the previous chunk.
    The very first chunk allocated always has this bit set,
    preventing access to non-existent (or non-owned) memory. If
    prev_inuse is set for any given chunk, then you CANNOT determine
    the size of the previous chunk, and might even get a memory
    addressing fault when trying to do so.

    Note that the `foot' of the current chunk is actually represented
    as the prev_size of the NEXT chunk. This makes it easier to
    deal with alignments etc but can be very confusing when trying
    to extend or adapt this code.

    The two exceptions to all this are

     1. The special chunk `top' doesn't bother using the
        trailing size field since there is no next contiguous chunk
        that would have to index off it. After initialization, `top'
        is forced to always exist.  If it would become less than
        MINSIZE bytes long, it is replenished.

     2. Chunks allocated via mmap, which have the second-lowest-order
        bit (IS_MMAPPED) set in their size fields.  Because they are
        allocated one-by-one, each must contain its own trailing size field.

it appears as though the basic implementation is base on the sequential fit algorithm,
with modifications. first of which is that used chunks are not stored in the list.
so the list becomes a "free" list only. this should help with performance while searching
for an available node. The second is that they hold arrays pointing into the allocation
a "fastnode" array, which is used for small allocations. It appears on first reading that 
each fast list node is a pointer into the free list, where a small allocation can happen. 
a binary map of the bins, which I think is a binary map showing which bins are used. This
acts as a small region of the segmented list structure. (I think), and then a pointer to 
the top of memory. The top of memory is the only location which may be extended by way of 
an sbrk. or similar function. They use the best-fit policy of finding 
an available node, to avoid internal fragmentation. and when they run out of memory, they aquire
more from the OS, via (on Linux) sbrk. They are carful to point out that intervening calls to sbrk
might create holes in their allocation space. which the algorithm must consider. 
and additionally, very large allocations are handled via mmap, instead of using the usual heap,
The code overall is not considered thread safe, and even the limited threadsafety they have is
both not tuned for performance, (they call it bare-minimum) it also does not handle calls to 
sbrk that they do not make themselves.

*/


// note that old size is unused, at most until we add garbage collection. when we get to that chapter.
bool reallocate(void **pointer, [[maybe_unused]] size_t old_size, size_t new_size) {
    assert(pointer != NULL);
    // note that *pointer is allowed to be NULL, as this implies a first time allocation.

    if (new_size == 0) {
        free(*pointer);
        *pointer = NULL;
        return true;
    }

    void *result = realloc(*pointer, new_size);
    if (result == NULL) { return false; }

    (*pointer) = result;
    return true;
}

bool array_grow(ArrayGrowInfo info, size_t element_size, void **array, uint32_t *capacity) {
    assert(array != NULL);
    assert(capacity != NULL);
    assert(element_size > 0);
    assert(info.growth_factor > 0);
    assert(info.initial_capacity > 0);
    
    if (*capacity == 0) { 
        (*capacity) = info.initial_capacity;
    } else {
        (*capacity) *= info.growth_factor;
    }

    size_t size = element_size * (*capacity);

    void *ptr = realloc(*array, size);
    if (ptr == NULL) { return false; }
    
    *array = ptr;
    return true;
}

