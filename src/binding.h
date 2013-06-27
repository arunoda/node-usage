#include <node.h>

#ifdef is_linux
#include <unistd.h>
#endif

#ifdef is_solaris
#include "solaris.h"
#endif

#ifdef is_mac
#include "mac.h"
#endif
