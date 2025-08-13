// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/daw_thunk
//

#ifndef DAW_THUNK_H
#define DAW_THUNK_H

#if defined(_WIN32)
#include "thunk/daw_thunk_win.h"
#elif defined(__GNUC__)
#include "thunk/daw_thunk_posix.h"
#endif

#endif /* ifndef DAW_THUNK_H */
