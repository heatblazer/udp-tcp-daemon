#ifndef CRYPTO_H
#define CRYPTO_H

#include "crypto_global.h"

class CRYPTOSHARED_EXPORT Crypto
{

public:
    explicit Crypto();
    virtual ~Crypto();
    void dummy();

private:
    struct {
        char* salt;
        int saltLen;
    } m_salt;
};

#endif // CRYPTO_H
