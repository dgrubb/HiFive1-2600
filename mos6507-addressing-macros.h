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

#define ADDRESSING_MODE_IMMEDIATE() \
    switch(cycle) { \
        case 0: \
            return -1; \
        case 1: \
            mos6507_increment_PC(); \
            mos6507_set_address_bus(mos6507_get_PC()); \
            memmap_read(&data); \
        default: \
            break; \
    } \

#define ADDRESSING_MODE_ZERO_PAGE() \
    switch(cycle) { \
        case 0: \
            return -1; \
        case 1: \
            mos6507_increment_PC(); \
            mos6507_set_address_bus(mos6507_get_PC()); \
            memmap_read(&adl); \
            return -1; \
        case 2: \
            mos6507_set_address_bus_hl(0, adl); \
            memmap_read(&data); \
        default: \
            break; \
    } \

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

#define ADDRESSING_MODE_INDIRECT_X_INDEXED() \
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
            mos6507_get_register(MOS6507_REG_X, &X); \
            mos6507_set_address_bus_hl(0, (bal + X)); \
            memmap_read(&adl); \
            return -1; \
        case 4: \
            mos6507_get_register(MOS6507_REG_X, &X); \
            mos6507_set_address_bus_hl(0, ((bal + X) + 1)); \
            memmap_read(&adh); \
            return -1; \
        case 5: \
            mos6507_set_address_bus_hl(adh, adl); \
            memmap_read(&data); \
        default: \
            break; \
    } \

#define ADDRESSING_MODE_ABSOLUTE_X_INDEXED() \
    switch(cycle) { \
        case 0: \
            return -1; \
        case 1: \
            mos6507_increment_PC(); \
            mos6507_set_address_bus(mos6507_get_PC()); \
            memmap_read(&bal); \
            return -1; \
        case 2: \
            mos6507_increment_PC(); \
            mos6507_set_address_bus(mos6507_get_PC()); \
            memmap_read(&bah); \
            return -1; \
        case 3: \
            mos6507_get_register(MOS6507_REG_X, &X); \
            adl = bal + X; \
            if ((bal + X) & 0x0100) { \
                c = 1; \
            } \
            adh = bah + c; \
            mos6507_set_address_bus_hl(adh, adl); \
            memmap_read(&data); \
            if (c) { \
                return -1; \
            } \
            break; \
        case 4: \
            mos6507_get_register(MOS6507_REG_X, &X); \
            adl = bal + X; \
            if ((bal + X) & 0x0100) { \
                c = 1; \
            } \
            adh = bah + c; \
            mos6507_set_address_bus_hl(adh, adl); \
            memmap_read(&data); \
        default: \
            break; \
    } \

#define ADDRESSING_MODE_ABSOLUTE_Y_INDEXED() \
    switch(cycle) { \
        case 0: \
            return -1; \
        case 1: \
            mos6507_increment_PC(); \
            mos6507_set_address_bus(mos6507_get_PC()); \
            memmap_read(&bal); \
            return -1; \
        case 2: \
            mos6507_increment_PC(); \
            mos6507_set_address_bus(mos6507_get_PC()); \
            memmap_read(&bah); \
            return -1; \
        case 3: \
            mos6507_get_register(MOS6507_REG_Y, &Y); \
            adl = bal + Y; \
            if ((bal + Y) & 0x0100) { \
                c = 1; \
            } \
            adh = bah + c; \
            mos6507_set_address_bus_hl(adh, adl); \
            memmap_read(&data); \
            if (c) { \
                return -1; \
            } \
            break; \
        case 4: \
            mos6507_get_register(MOS6507_REG_Y, &Y); \
            adl = bal + Y; \
            if ((bal + Y) & 0x0100) { \
                c = 1; \
            } \
            adh = bah + c; \
            mos6507_set_address_bus_hl(adh, adl); \
            memmap_read(&data); \
        default: \
            break; \
    } \

#define ADDRESSING_MODE_ZERO_PAGE_X_INDEXED() \
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
            mos6507_get_register(MOS6507_REG_X, &X); \
            mos6507_set_address_bus_hl(0, bal + X); \
            memmap_read(&data); \
            mos6507_ADC(data); \
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
            mos6507_ADC(data); \
        default: \
            break; \
    } \

#define FETCH_DATA() \
    if (OPCODE_ADDRESSING_MODE_IMMEDIATE == address_mode) { \
        ADDRESSING_MODE_IMMEDIATE() \
    } else if (OPCODE_ADDRESSING_MODE_ZERO_PAGE == address_mode) { \
        ADDRESSING_MODE_ZERO_PAGE() \
    } else if (OPCODE_ADDRESSING_MODE_ABSOLUTE == address_mode) { \
        ADDRESSING_MODE_ABSOLUTE() \
    } else if (OPCODE_ADDRESSING_MODE_INDIRECT_X_INDEXED == address_mode) { \
        ADDRESSING_MODE_INDIRECT_X_INDEXED() \
    } else if (OPCODE_ADDRESSING_MODE_ABSOLUTE_X_INDEXED == address_mode) { \
        ADDRESSING_MODE_ABSOLUTE_X_INDEXED() \
    } else if (OPCODE_ADDRESSING_MODE_ABSOLUTE_Y_INDEXED == address_mode) { \
        ADDRESSING_MODE_ABSOLUTE_Y_INDEXED() \
    } else if (OPCODE_ADDRESSING_MODE_ZERO_PAGE_X_INDEXED == address_mode) { \
        ADDRESSING_MODE_ZERO_PAGE_X_INDEXED() \
    } else if (OPCODE_ADDRESSING_MODE_ZERO_PAGE_Y_INDEXED == address_mode) { \
        ADDRESSING_MODE_ZERO_PAGE_Y_INDEXED() \
    } \

