#ifndef __STDX_MSGPACK_H
#define __STDX_MSGPACK_H

// C++ 98 header files
#include <vector>
#include <string>

#include <msgpack.hpp>

/******************************************************************************/
// two templates func to use lib:msgpack to pack/unpack

template<typename TYPE_TO_PACK>
inline std::string serialize_by_msgpack(const TYPE_TO_PACK& obj)
{
    msgpack::sbuffer sbuf;
    msgpack::pack(&sbuf, obj);

    return std::string(sbuf.data(), sbuf.size());
}

template<typename TYPE_TO_UNPACK>
inline bool deserialize_by_msgpack(const std::string& val, TYPE_TO_UNPACK& obj)
{
    msgpack::unpacked result;
    msgpack::unpack(&result, val.data(), val.size(), NULL);
    try {
        result.get().convert(&obj);
    }
    catch(msgpack::type_error& e) {
        assert(false);
        return false;
    }

    return true;
}

#endif /* __STDX_MSGPACK_H */
