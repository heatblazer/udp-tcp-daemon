#include "crypto.h"

// std //
#include <iostream>

// ansi C //
#include <string.h> // strlen for the keys
#include <stdint.h>

// some default strings used for keys
static const char crypt_key_non_capital[] = "abcdefghijklmnopqrstuvwxyz";
static const char crypt_key_capital[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char crytpt_key_digits[] = "0123456789";
static const char crypt_key_symbols[] = "!@#$%^&*()+-=/\\";

/// generate small unsigned checksum
/// based on the LSB of a data stream
/// with a fixed length
/// \brief gen8bitCheckSum
/// \param data
/// \param len
/// \return
///
static uint8_t gen8bitCheckSum(char* data, int len=8)
{
    (void)gen8bitCheckSum;
    uint8_t key = 0;
    if (data == NULL) {
        return key;
    }
    for(int i=0; i < len; ++i) {
        key |= ((data[i] & 0x1) << i);
    }
    return key;
}

/// generate a bigger chechksum
/// based on the LSB of the data stream
/// with a fixed length
/// \brief gen16bitCheckSum
/// \param data
/// \param len
/// \return
///
static uint16_t gen16bitCheckSum(char* data, int len=16)
{
    (void)gen16bitCheckSum;
    uint16_t key = 0;
    if (data == NULL) {
        return key;
    }
    for(int i=0; i < len; ++i) {
        key |= ((data[i] & 0x1) << i);
    }
    return key;
}

static uint16_t gen16Salt(const char* data, int len=16)
{
    uint16_t salt = 0;
    for(int i=0; i < len; ++i) {
        salt += ((data[i] & 0x1) << i);
    }
    return salt;
}


Crypto::Crypto()
    : m_salt({nullptr, 0})
{
}

Crypto::~Crypto()
{
}

void Crypto::dummy()
{
    m_salt.salt = new char[32];
    for(int i=0; i < 32; ++i) {
        m_salt.salt[i] = (unsigned char)(i+24);
        m_salt.saltLen++;
    }
}
