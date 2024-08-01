#include <common.h>
#include <spm/memory.h>
#include <spm/system.h>
#include <EASTL/algorithm.h>

using spm::memory::Heap;

void * operator new(size_t size)
{
    return spm::memory::__memAlloc(Heap::HEAP_MAIN, size);
}

void * operator new[](size_t size)
{
    return spm::memory::__memAlloc(Heap::HEAP_MAIN, size);
}

void operator delete(void * ptr)
{
    return spm::memory::__dl__FPv(ptr);
}

void operator delete[](void * ptr)
{
    return spm::memory::__dl__FPv(ptr);
}

void operator delete(void * ptr, size_t size)
{
    (void) size;
    return spm::memory::__dl__FPv(ptr);
}

void operator delete[](void * ptr, size_t size)
{
    (void) size;
    return spm::memory::__dl__FPv(ptr);
}

// With custom heap parameter
void * operator new(size_t size, Heap heap)
{
    return spm::memory::__memAlloc(heap, size);
}

void * operator new[](size_t size, Heap heap)
{
    return spm::memory::__memAlloc(heap, size);
}


// For EASTL

static void * alloc(size_t size, u32 alignment=0, spm::memory::Heap heap=spm::memory::HEAP_MAIN)
{
    // Enforce minimum alignment
    alignment = eastl::max(alignment, 0x20U);

    // Allocate from main heap
    wii::mem::MEMHeapHandle handle = spm::memory::memory_wp->heapHandle[heap];
    return wii::mem::MEMAllocFromExpHeapEx(handle, size, alignment);
}

void* operator new[](size_t size, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
    (void) pName;
    (void) flags;
    (void) debugFlags;
    (void) file;
    (void) line;

    return alloc(size);
}

void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
    assertf(
        alignmentOffset % alignment == 0, "EASTL tried to use unsupported alignment offset (0x%x 0x%x 0x%x %s 0x%x 0x%x %s %d)",
        size, alignment, alignmentOffset, pName, flags, debugFlags, file, line
    );

    return alloc(size, alignment);
}
