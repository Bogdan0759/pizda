#include <kernel/drivers/fs/chainFS/chainfs.h>
#include <kernel/drivers/disk/pata/pata.h>

chainfs_t g_chainfs;

int chainfs_init(void) {
    com1_printf("ChainFS: Initializing...\n");
    
    pata_read_sector(0, g_chainfs.sector_buffer);
    
    chainfs_superblock_t* sb = (chainfs_superblock_t*)g_chainfs.sector_buffer;
    
    if (sb->magic != CHAINFS_MAGIC) {
        com1_printf("ChainFS: Invalid magic number 0x%x, expected 0x%x\n", 
                   sb->magic, CHAINFS_MAGIC);
        return -1;
    }
    
    g_chainfs.superblock = *sb;
    
    g_chainfs.data_area_start = 1 + g_chainfs.superblock.file_table_block_count + 
                               g_chainfs.superblock.block_map_block_count;
    
    com1_printf("ChainFS: Initialized successfully\n");
    com1_printf("  Total blocks: %u\n", g_chainfs.superblock.block_count);
    com1_printf("  File table blocks: %u\n", g_chainfs.superblock.file_table_block_count);
    com1_printf("  Block map blocks: %u\n", g_chainfs.superblock.block_map_block_count);
    com1_printf("  Data area start: %u\n", g_chainfs.data_area_start);
    
    return 0;
}

int chainfs_format(u32 total_blocks, u32 max_files) {
    com1_printf("ChainFS: Formatting disk with %u blocks, %u max files\n", 
               total_blocks, max_files);
    
    u32 entries_per_block = CHAINFS_BLOCK_SIZE / sizeof(chainfs_file_entry_t);
    u32 file_table_blocks = (max_files + entries_per_block - 1) / entries_per_block;
    
    u32 data_blocks = total_blocks - 1 - file_table_blocks;
    u32 map_entries_per_block = CHAINFS_BLOCK_SIZE / sizeof(u32);
    u32 block_map_blocks = (data_blocks + map_entries_per_block - 1) / map_entries_per_block;
    
    data_blocks = total_blocks - 1 - file_table_blocks - block_map_blocks;
    
    chainfs_superblock_t sb = {0};
    sb.magic = CHAINFS_MAGIC;
    sb.block_count = total_blocks;
    sb.file_table_block_count = file_table_blocks;
    sb.block_map_block_count = block_map_blocks;
    sb.total_files = max_files;
    
    for (int i = 0; i < CHAINFS_BLOCK_SIZE; i++) {
        g_chainfs.sector_buffer[i] = 0;
    }
    *((chainfs_superblock_t*)g_chainfs.sector_buffer) = sb;
    pata_write_sector(0, g_chainfs.sector_buffer);
    
    for (int i = 0; i < CHAINFS_BLOCK_SIZE; i++) {
        g_chainfs.sector_buffer[i] = 0;
    }
    
    for (u32 block = 1; block < 1 + file_table_blocks; block++) {
        pata_write_sector(block, g_chainfs.sector_buffer);
    }
    
    u32* block_map = (u32*)g_chainfs.sector_buffer;
    for (u32 i = 0; i < map_entries_per_block; i++) {
        block_map[i] = CHAINFS_FREE_BLOCK;
    }
    
    for (u32 block = 1 + file_table_blocks; block < 1 + file_table_blocks + block_map_blocks; block++) {
        pata_write_sector(block, g_chainfs.sector_buffer);
    }
    
    com1_printf("ChainFS: Format complete\n");
    
    return chainfs_init();
}

int chainfs_find_file(const char* filename, chainfs_file_entry_t* entry, 
                           u32* entry_block, u32* entry_offset) {
    u32 entries_per_block = CHAINFS_BLOCK_SIZE / sizeof(chainfs_file_entry_t);
    
    for (u32 block = 1; block < 1 + g_chainfs.superblock.file_table_block_count; block++) {
        pata_read_sector(block, g_chainfs.sector_buffer);
        chainfs_file_entry_t* entries = (chainfs_file_entry_t*)g_chainfs.sector_buffer;
        
        for (u32 i = 0; i < entries_per_block; i++) {
            if (entries[i].status == 1 && strcmp(entries[i].name, filename) == 0) {
                *entry = entries[i];
                *entry_block = block;
                *entry_offset = i;
                return 0;
            }
        }
    }
    
    return -1;
}

int chainfs_find_free_file_entry(u32* entry_block, u32* entry_offset) {
    u32 entries_per_block = CHAINFS_BLOCK_SIZE / sizeof(chainfs_file_entry_t);
    
    for (u32 block = 1; block < 1 + g_chainfs.superblock.file_table_block_count; block++) {
        pata_read_sector(block, g_chainfs.sector_buffer);
        chainfs_file_entry_t* entries = (chainfs_file_entry_t*)g_chainfs.sector_buffer;
        
        for (u32 i = 0; i < entries_per_block; i++) {
            if (entries[i].status == 0) {
                *entry_block = block;
                *entry_offset = i;
                return 0;
            }
        }
    }
    
    return -1;
}

int chainfs_read_block_map_entry(u32 block_index, u32* next_block) {
    u32 entries_per_block = CHAINFS_BLOCK_SIZE / sizeof(u32);
    u32 map_block = block_index / entries_per_block;
    u32 map_offset = block_index % entries_per_block;
    
    if (map_block >= g_chainfs.superblock.block_map_block_count) {
        return -1;
    }
    
    u32 sector = 1 + g_chainfs.superblock.file_table_block_count + map_block;
    pata_read_sector(sector, g_chainfs.sector_buffer);
    
    u32* map_entries = (u32*)g_chainfs.sector_buffer;
    *next_block = map_entries[map_offset];
    
    return 0;
}

int chainfs_write_block_map_entry(u32 block_index, u32 next_block) {
    u32 entries_per_block = CHAINFS_BLOCK_SIZE / sizeof(u32);
    u32 map_block = block_index / entries_per_block;
    u32 map_offset = block_index % entries_per_block;
    
    if (map_block >= g_chainfs.superblock.block_map_block_count) {
        return -1;
    }
    
    u32 sector = 1 + g_chainfs.superblock.file_table_block_count + map_block;
    pata_read_sector(sector, g_chainfs.sector_buffer);
    
    u32* map_entries = (u32*)g_chainfs.sector_buffer;
    map_entries[map_offset] = next_block;
    
    pata_write_sector(sector, g_chainfs.sector_buffer);
    
    return 0;
}

int chainfs_find_free_blocks(u32 count, u32* blocks) {
    u32 found = 0;
    u32 entries_per_block = CHAINFS_BLOCK_SIZE / sizeof(u32);
    u32 total_data_blocks = g_chainfs.superblock.block_count - g_chainfs.data_area_start;
    
    for (u32 i = 0; i < total_data_blocks && found < count; i++) {
        u32 next_block;
        if (chainfs_read_block_map_entry(i, &next_block) == 0) {
            if (next_block == CHAINFS_FREE_BLOCK) {
                blocks[found++] = i;
            }
        }
    }
    
    return (found == count) ? 0 : -1;
}

void chainfs_free_block_chain(u32 start_block) {
    u32 current_block = start_block;
    
    while (current_block != CHAINFS_EOF_MARKER) {
        u32 next_block;
        if (chainfs_read_block_map_entry(current_block, &next_block) != 0) {
            break;
        }
        
        chainfs_write_block_map_entry(current_block, CHAINFS_FREE_BLOCK);
        current_block = next_block;
    }
}

int chainfs_read_file(const char* filename, u8* buffer, u32 buffer_size, u32* bytes_read) {
    chainfs_file_entry_t entry;
    u32 entry_block, entry_offset;
    
    if (chainfs_find_file(filename, &entry, &entry_block, &entry_offset) != 0) {
        com1_printf("ChainFS: File '%s' not found\n", filename);
        return -1;
    }
    
    u32 remaining = entry.size;
    u32 copied = 0;
    u32 current_block = entry.start_block;
    
    while (remaining > 0 && current_block != CHAINFS_EOF_MARKER && copied < buffer_size) {
        u32 real_sector = g_chainfs.data_area_start + current_block;
        pata_read_sector(real_sector, g_chainfs.sector_buffer);
        
        u32 to_copy = remaining;
        if (to_copy > CHAINFS_BLOCK_SIZE) {
            to_copy = CHAINFS_BLOCK_SIZE;
        }
        if (copied + to_copy > buffer_size) {
            to_copy = buffer_size - copied;
        }
        
        for (u32 i = 0; i < to_copy; i++) {
            buffer[copied + i] = g_chainfs.sector_buffer[i];
        }
        
        copied += to_copy;
        remaining -= to_copy;
        
        if (remaining > 0) {
            u32 next_block;
            if (chainfs_read_block_map_entry(current_block, &next_block) != 0) {
                break;
            }
            current_block = next_block;
        }
    }
    
    *bytes_read = copied;
    com1_printf("ChainFS: Read %u bytes from '%s'\n", copied, filename);
    return 0;
}
int chainfs_write_file(const char* filename, const u8* data, u32 size) {
    chainfs_file_entry_t entry;
    u32 entry_block, entry_offset;
    int file_exists = (chainfs_find_file(filename, &entry, &entry_block, &entry_offset) == 0);
    
    if (file_exists) {
        chainfs_free_block_chain(entry.start_block);
    } else {
        if (chainfs_find_free_file_entry(&entry_block, &entry_offset) != 0) {
            com1_printf("ChainFS: No free file entries\n");
            return -1;
        }
    }
    
    u32 blocks_needed = (size + CHAINFS_BLOCK_SIZE - 1) / CHAINFS_BLOCK_SIZE;
    if (blocks_needed == 0) {
        blocks_needed = 1;
    }
    
    u32* allocated_blocks = (u32*)kmalloc(blocks_needed * sizeof(u32));
    if (!allocated_blocks) {
        com1_printf("ChainFS: Memory allocation failed\n");
        return -1;
    }
    
    if (chainfs_find_free_blocks(blocks_needed, allocated_blocks) != 0) {
        com1_printf("ChainFS: Not enough free blocks\n");
        kfree(allocated_blocks);
        return -1;
    }
    
    u32 remaining = size;
    u32 data_offset = 0;
    
    for (u32 i = 0; i < blocks_needed; i++) {
        for (int j = 0; j < CHAINFS_BLOCK_SIZE; j++) {
            g_chainfs.sector_buffer[j] = 0;
        }
        
        u32 to_copy = remaining;
        if (to_copy > CHAINFS_BLOCK_SIZE) {
            to_copy = CHAINFS_BLOCK_SIZE;
        }
        
        for (u32 j = 0; j < to_copy; j++) {
            g_chainfs.sector_buffer[j] = data[data_offset + j];
        }
        
        u32 real_sector = g_chainfs.data_area_start + allocated_blocks[i];
        pata_write_sector(real_sector, g_chainfs.sector_buffer);
        
        u32 next_block = (i + 1 < blocks_needed) ? allocated_blocks[i + 1] : CHAINFS_EOF_MARKER;
        chainfs_write_block_map_entry(allocated_blocks[i], next_block);
        
        remaining -= to_copy;
        data_offset += to_copy;
    }
    
    pata_read_sector(entry_block, g_chainfs.sector_buffer);
    chainfs_file_entry_t* entries = (chainfs_file_entry_t*)g_chainfs.sector_buffer;
    
    entries[entry_offset].status = 1;
    for (int i = 0; i < CHAINFS_MAX_FILENAME; i++) {
        entries[entry_offset].name[i] = 0;
    }
    
    int name_len = strlen(filename);
    if (name_len > CHAINFS_MAX_FILENAME) {
        name_len = CHAINFS_MAX_FILENAME;
    }
    for (int i = 0; i < name_len; i++) {
        entries[entry_offset].name[i] = filename[i];
    }
    
    entries[entry_offset].size = size;
    entries[entry_offset].start_block = allocated_blocks[0];
    
    pata_write_sector(entry_block, g_chainfs.sector_buffer);
    
    kfree(allocated_blocks);
    
    com1_printf("ChainFS: Wrote %u bytes to '%s' using %u blocks\n", 
               size, filename, blocks_needed);
    return 0;
}

int chainfs_delete_file(const char* filename) {
    chainfs_file_entry_t entry;
    u32 entry_block, entry_offset;
    
    if (chainfs_find_file(filename, &entry, &entry_block, &entry_offset) != 0) {
        com1_printf("ChainFS: File '%s' not found\n", filename);
        return -1;
    }
    
    chainfs_free_block_chain(entry.start_block);
    
    pata_read_sector(entry_block, g_chainfs.sector_buffer);
    chainfs_file_entry_t* entries = (chainfs_file_entry_t*)g_chainfs.sector_buffer;
    entries[entry_offset].status = 0;
    pata_write_sector(entry_block, g_chainfs.sector_buffer);
    
    com1_printf("ChainFS: Deleted file '%s'\n", filename);
    return 0;
}

int chainfs_get_file_list(chainfs_file_entry_t* files, u32 max_files, u32* file_count) {
    u32 entries_per_block = CHAINFS_BLOCK_SIZE / sizeof(chainfs_file_entry_t);
    u32 found = 0;
    
    for (u32 block = 1; block < 1 + g_chainfs.superblock.file_table_block_count && found < max_files; block++) {
        pata_read_sector(block, g_chainfs.sector_buffer);
        chainfs_file_entry_t* entries = (chainfs_file_entry_t*)g_chainfs.sector_buffer;
        
        for (u32 i = 0; i < entries_per_block && found < max_files; i++) {
            if (entries[i].status == 1) {
                files[found] = entries[i];
                found++;
            }
        }
    }
    
    *file_count = found;
    return 0;
}