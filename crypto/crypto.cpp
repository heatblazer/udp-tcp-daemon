#include "crypto.h"

#include <string.h> // strlen for the keys

// some default strings used for keys
static const char crypt_key_non_capital[] = "abcdefghijklmnopqrstuvwxyz";
static const char crypt_key_capital[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char crytpt_key_digits[] = "0123456789";
static const char crypt_key_symbols[] = "!@#$%^&*()+-=/\\";

Crypto::Crypto()
{
}

Crypto::~Crypto()
{

}
