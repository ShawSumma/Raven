#pragma once

#define Q(x) #x
#define QUOTE(x) Q(x)

#define VERSION_MAJOR 0
#define VERSION_MINOR 0
#define VERSION_PATCH 1
#define RAVEN_VERSION QUOTE(VERSION_MAJOR) "." QUOTE(VERSION_MINOR) "." QUOTE(VERSION_PATCH)
