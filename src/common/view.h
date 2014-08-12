#ifndef COMMON_VIEW_H
#define COMMON_VIEW_H

#if defined(Q_OS_LINUX)
#include "linux-ui/view.h"
#elif defined(Q_OS_WIN)
#include "windows-ui/view.h"
#elif defined(Q_OS_OSX)
#include "mac-ui/view.h"
#endif

#endif // COMMON_VIEW_H
