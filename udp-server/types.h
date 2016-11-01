#ifndef TYPES_H
#define TYPES_H

namespace iz {

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

    T2* operator[] (const T1& t1)
    {
        if (t1 == m_type1) {
            return &m_type2;
        } else {
            return nullptr;
        }
    }

    T1 m_type1;
    T2 m_type2;
};

} // iz

#endif // TYPES_H
