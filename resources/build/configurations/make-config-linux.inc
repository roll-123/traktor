# Configuration file

C=ccache gcc
# gcc 14+ promoted incompatible-pointer-types to an error by default;
# the 2018 lwObject 3rdp loader has dozens of K&R-style violations.
# Demote to a warning so the build can proceed (stopgap).
C_FLAGS_COMMON=-fno-common -march=core2 -D _UNICODE -D UNICODE -D __LINUX__ -fPIC -fvisibility=hidden -Wno-error=incompatible-pointer-types
C_FLAGS_DEBUG_SHARED=$(C_FLAGS_COMMON) -g
C_FLAGS_RELEASE_SHARED=$(C_FLAGS_COMMON) -O3 -g
C_FLAGS_DEBUG_STATIC=$(C_FLAGS_COMMON) -g
C_FLAGS_RELEASE_STATIC=$(C_FLAGS_COMMON) -O3 -g
C_FLAGS_DEBUG_EXECUTABLE=$(C_FLAGS_COMMON) -g
C_FLAGS_RELEASE_EXECUTABLE=$(C_FLAGS_COMMON) -O3 -g
C_FLAG_OUTPUT=-o 

CC=ccache gcc
# NOTE: -include <hdr> forces every translation unit to include the listed
# stdlib headers. This is a stopgap for porting the 2018 codebase to modern
# compilers (gcc 11+/clang 14+) which dropped many transitive includes.
# 785 headers in this tree use std::string without explicitly #including
# <string>, with similar gaps for vector/map/etc. Proper fix is to add
# explicit includes to each header; this flag unblocks the build in the
# meantime.
CC_FORCED_INCLUDES=-include cstddef -include cstdint -include string -include vector -include map -include set -include list -include memory -include functional -include utility -include algorithm -include limits
# -Wno-template-body: gcc 14+ checks template definitions even if uninstantiated.
# 3rdp/FbxSdk/2016.1.2 has a typo (mLefttChild) in dead code inside a template
# that we never instantiate that path of.
CC_FLAGS_COMMON=-fno-common -std=c++11 -march=core2 -D _UNICODE -D UNICODE -D __LINUX__ -fPIC -fvisibility=hidden -Wno-template-body $(CC_FORCED_INCLUDES)
CC_FLAGS_DEBUG_SHARED=$(CC_FLAGS_COMMON) -g
CC_FLAGS_RELEASE_SHARED=$(CC_FLAGS_COMMON) -O3 -g
CC_FLAGS_DEBUG_STATIC=$(CC_FLAGS_COMMON) -g
CC_FLAGS_RELEASE_STATIC=$(CC_FLAGS_COMMON) -O3 -g
CC_FLAGS_DEBUG_EXECUTABLE=$(CC_FLAGS_COMMON) -g
CC_FLAGS_RELEASE_EXECUTABLE=$(CC_FLAGS_COMMON) -O3 -g
CC_FLAG_OUTPUT=-o 

AR=ar
AR_FLAGS_COMMON=-r -s
AR_FLAGS_DEBUG_STATIC=$(AR_FLAGS_COMMON)
AR_FLAGS_RELEASE_STATIC=$(AR_FLAGS_COMMON)
AR_FLAG_OUTPUT=

LINK=ccache c++
LINK_FLAGS_COMMON=-g -Wl,-rpath=\$$ORIGIN
LINK_FLAGS_DEBUG_SHARED=$(LINK_FLAGS_COMMON) -shared
LINK_FLAGS_RELEASE_SHARED=$(LINK_FLAGS_COMMON) -shared
LINK_FLAGS_DEBUG_STATIC=$(LINK_FLAGS_COMMON)
LINK_FLAGS_RELEASE_STATIC=$(LINK_FLAGS_COMMON)
LINK_FLAGS_DEBUG_EXECUTABLE=$(LINK_FLAGS_COMMON)
LINK_FLAGS_RELEASE_EXECUTABLE=$(LINK_FLAGS_COMMON)
LINK_FLAG_OUTPUT=-o 
LINK_LIBS=

BINARY_INCLUDE=$(TRAKTOR_HOME)/bin/linux/releasestatic/Traktor.Run.App $(TRAKTOR_HOME)/scripts/binaryinclude/BinaryInclude.run
