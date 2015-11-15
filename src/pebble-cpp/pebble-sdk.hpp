
#ifndef PEBBLE_API_WRAPPER_HPP_
#define PEBBLE_API_WRAPPER_HPP_

// Hack to disable #include <time> in the pebble.h header. Pebble defines part of the standard time library itself for
// some Pebble platforms.
#define _TIME_H_

extern "C"
{
#include <pebble.h>
}

// If the pebble library did not define the time functions itself, pull them in.
#ifndef TZ_LEN
#undef _TIME_H_
#include <time.h>
#endif

#endif // PEBBLE_API_WRAPPER_HPP_