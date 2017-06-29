#define ADDRESSING_MODE_ABSOLUTE() \
    switch(cycle) { \
        case 0: \
            return -1; \
        case 1: \
            mos6507_increment_PC(); \
            mos6507_set_address_bus(mos6507_get_PC()); \
            memmap_read(&adl); \
            return -1; \
        case 2: \
            mos6507_increment_PC(); \
            mos6507_set_address_bus(mos6507_get_PC()); \
            memmap_read(&adh); \
            return -1; \
        case 3: \
            mos6507_set_address_bus_hl(adh, adl); \
            memmap_read(&data); \
        default: \
            break; \
    } \

#define ADDRESSING_MODE_ZERO_PAGE_Y_INDEXED() \
    switch(cycle) { \
        case 0: \
            return -1; \
        case 1: \
            mos6507_increment_PC(); \
            mos6507_set_address_bus(mos6507_get_PC()); \
            memmap_read(&bal); \
            return -1; \
        case 2: \
            mos6507_set_address_bus_hl(0, bal); \
            return -1; \
        case 3: \
            mos6507_get_register(MOS6507_REG_Y, &Y); \
            mos6507_set_address_bus_hl(0, bal + Y); \
            memmap_read(&data); \
        default: \
            break; \
    } \

