#pragma once

namespace dash {

int   bytes_buffer_size();
void *bytes_buffer();

template<class Type> int buffer_size() {
    return bytes_buffer_size() / (int)sizeof(Type);
}

template<class Type> Type *buffer() {
    return (Type *)bytes_buffer();
}

} //end dash.
