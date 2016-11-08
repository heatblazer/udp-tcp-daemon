#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

namespace iz {

// opaque pointers
struct udp_data_t;
struct tcp_data_t;


/// maybe inherit QPair
/// simple pair template
/// to access key by value
template <typename T1, typename T2> struct MPair
{
    MPair() {}
    MPair(const T1& t1, const T2& t2) :
        m_type1(t1), m_type2(t2)
    {}
    MPair(const MPair& ref)
    {
        m_type1 = ref.m_type1;
        m_type2 = ref.m_type2;
    }

    T2& operator[] (const T1& t1)
    {
        if(t1 == m_type1) {
            return m_type2;
        } else {
            static T2 none;
            return none;
        }
    }

    T1 m_type1;
    T2 m_type2;
};

} // iz

#endif // TYPES_H
