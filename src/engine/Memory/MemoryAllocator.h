#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <gba_video.h>
#include <vector>

class MemoryManager {
public:
    MemoryManager(void* start, void* end) {
        freeBlocks.push_back(Block(alignAddress(start, 4), alignAddress(end, 4)));
    }

    void* allocate(u32 size) {
        size = alignSize(size, 4);
        for (auto& block : freeBlocks) {
            if (block.size() >= size) {
                void* allocatedAddress = block.start;
                block.start = static_cast<u8*>(block.start) + size;
                return allocatedAddress;
            }
        }
        return nullptr; // Allocation failed, no available block with enough space
    }

    void free(void* address, u32 size) {
        size = alignSize(size, 4);
        for (auto& block : freeBlocks) {
            if (block.end == address) {
                block.end = static_cast<u8*>(block.end) - size;
                return;
            }
            if (block.start == static_cast<u8*>(address) + size) {
                block.start = address;
                return;
            }
        }
        freeBlocks.push_back(Block(address, static_cast<u8*>(address) + size));
    }

public:
    struct Block {
        void* start;
        void* end;

        Block(void* start, void* end) : start(start), end(end) {}

        u32 size() const {
            return static_cast<u8*>(end) - static_cast<u8*>(start);
        }
    };

    std::vector<Block> freeBlocks;

    u32 alignSize(u32 size, u32 alignment) {
        return size + (size % alignment);
    }

    void* alignAddress(void* address, u32 alignment) {
        u32 addr = reinterpret_cast<u32>(address);
        addr = alignSize(addr, alignment);
        return reinterpret_cast<void*>(addr);
    }
};

#endif //MEMORY_ALLOCATOR_H