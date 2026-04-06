#include <common.h>
#include <spm/memory.h>
#include <msl/string.h>
#include <patch.h>

namespace mod::mempatch
{

    /*
        Not actually a practice codes file, but I threw it in this folder since Seeky did all of this pretty much
        Copied directly from https://github.com/SeekyCt/spm-loaders/blob/memtest/memtest/main.cpp
    */
    static spm::memory::HeapSize size_table[HEAP_COUNT] =
        {
            // MEM1
            {// 0
             spm::memory::HEAPSIZE_ABSOLUTE_KB,
             0x2400},
            {// 1
             spm::memory::HEAPSIZE_ABSOLUTE_KB,
             0x1800},
            {// 2
             spm::memory::HEAPSIZE_PERCENT_REMAINING,
             100},

            // MEM2
            {// 3
             spm::memory::HEAPSIZE_ABSOLUTE_KB,
             0x100},
            {// 4
             spm::memory::HEAPSIZE_ABSOLUTE_KB,
             0x100},
            {// 5
             spm::memory::HEAPSIZE_ABSOLUTE_KB,
             0x80},
            {// 6
             spm::memory::HEAPSIZE_ABSOLUTE_KB,
             0x4400},
            {// 7 - smart heap
             spm::memory::HEAPSIZE_PERCENT_REMAINING,
             100},
            {// 8
             spm::memory::HEAPSIZE_ABSOLUTE_KB,
             1}};

    void memPatch()
    {

        msl::string::memcpy(&spm::memory::memory_size_table, &size_table, sizeof(size_table));

        writeWord(spm::memory::memInit, 0xa0, 0x2c1d0003);  // cmpwi r29, 0x3
        writeWord(spm::memory::memInit, 0x178, 0x2c1b0003); // cmpwi r27, 0x3
        writeWord(spm::memory::memInit, 0x1b0, 0x2c190003); // cmpwi r25, 0x3

        writeWord(spm::memory::memInit, 0x1e0, 0x3b600003); // li r27, 0x3
        writeWord(spm::memory::memInit, 0x1e4, 0x3b240018); // addi r25, r4, 0x18
        writeWord(spm::memory::memInit, 0x1e8, 0x3b40000c); // li r26, 0xc

        writeWord(spm::memory::memInit, 0x274, 0x3b600003); // li r27, 0x3
        writeWord(spm::memory::memInit, 0x278, 0x3be30018); // addi r31, r3, 0x18
        writeWord(spm::memory::memInit, 0x27c, 0x3bc0000c); // li r30, 0xc

        writeWord(spm::memory::memInit, 0x344, 0x3b200003); // li r25, 0x3
        writeWord(spm::memory::memInit, 0x348, 0x3b00000c); // li r24, 0xc
    }

}