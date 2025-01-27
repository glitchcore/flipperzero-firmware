#pragma once
#include "subghz_protocol_common.h"
#include "file-worker.h"


#include <furi.h>

/*
 * Keeloq
 * https://ru.wikipedia.org/wiki/KeeLoq
 * https://phreakerclub.com/forum/showthread.php?t=1094
 *
 */

#define KEELOQ_NLF          0x3A5C742E
#define bit(x,n)            (((x)>>(n))&1)
#define g5(x,a,b,c,d,e)     (bit(x,a)+bit(x,b)*2+bit(x,c)*4+bit(x,d)*8+bit(x,e)*16)

/*
 * KeeLoq learning types
 * https://phreakerclub.com/forum/showthread.php?t=67
 */
#define KEELOQ_LEARNING_UNKNOWN    0u
#define KEELOQ_LEARNING_SIMPLE     1u
#define KEELOQ_LEARNING_NORMAL     2u
#define KEELOQ_LEARNING_SECURE     3u


/** Simple Learning Encrypt
 * @param data - 0xBSSSCCCC, B(4bit) key, S(10bit) serial&0x3FF, C(16bit) counter
 * @param key - manufacture (64bit)
 * @return keelog encrypt data
 */
uint32_t subghz_protocol_keeloq_common_encrypt(const uint32_t data, const uint64_t key);

/** Simple Learning Decrypt
 * @param data - keelog encrypt data
 * @param key - manufacture (64bit)
 * @return 0xBSSSCCCC, B(4bit) key, S(10bit) serial&0x3FF, C(16bit) counter
 */
uint32_t subghz_protocol_keeloq_common_decrypt(const uint32_t data, const uint64_t key);

/** Normal Learning
 * @param data - serial number (28bit)
 * @param key - manufacture (64bit)
 * @return manufacture for this serial number (64bit)
 */
uint64_t subghz_protocol_keeloq_common_normal_learning(uint32_t data, const uint64_t key);
