
cmake_policy(SET CMP0057 NEW) # Enable IN_LIST operator
cmake_policy(SET CMP0064 NEW) # Support if (TEST) operator

set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_C_STANDARD 90)

set(uv_sources
    fs-poll.c
    idna.c
    inet.c
    random.c
    strscpy.c
    threadpool.c
    timer.c
    uv-common.c
    uv-data-getter-setters.c
    version.c)

if(WIN32)
  list(APPEND uv_defines WIN32_LEAN_AND_MEAN _WIN32_WINNT=0x0600)
  list(APPEND uv_libraries
       psapi
       user32
       advapi32
       iphlpapi
       userenv
       ws2_32)
  list(APPEND uv_sources
       win/async.c
       win/core.c
       win/detect-wakeup.c
       win/dl.c
       win/error.c
       win/fs.c
       win/fs-event.c
       win/getaddrinfo.c
       win/getnameinfo.c
       win/handle.c
       win/loop-watcher.c
       win/pipe.c
       win/thread.c
       win/poll.c
       win/process.c
       win/process-stdio.c
       win/signal.c
       win/snprintf.c
       win/stream.c
       win/tcp.c
       win/tty.c
       win/udp.c
       win/util.c
       win/winapi.c
       win/winsock.c)
else()
  list(APPEND uv_defines _FILE_OFFSET_BITS=64 _LARGEFILE_SOURCE)
  if(NOT CMAKE_SYSTEM_NAME MATCHES "Android|OS390|QNX")
    # TODO: This should be replaced with find_package(Threads) if possible
    # Android has pthread as part of its c library, not as a separate
    # libpthread.so.
    list(APPEND uv_libraries pthread)
  endif()
  list(APPEND uv_sources
       unix/async.c
       unix/core.c
       unix/dl.c
       unix/fs.c
       unix/getaddrinfo.c
       unix/getnameinfo.c
       unix/loop-watcher.c
       unix/loop.c
       unix/pipe.c
       unix/poll.c
       unix/process.c
       unix/random-devurandom.c
       unix/signal.c
       unix/stream.c
       unix/tcp.c
       unix/thread.c
       unix/tty.c
       unix/udp.c)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "AIX")
  list(APPEND uv_defines
       _ALL_SOURCE
       _LINUX_SOURCE_COMPAT
       _THREAD_SAFE
       _XOPEN_SOURCE=500
       HAVE_SYS_AHAFS_EVPRODS_H)
  list(APPEND uv_libraries perfstat)
  list(APPEND uv_sources
       unix/aix.c
       unix/aix-common.c)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Android")
  list(APPEND uv_defines _GNU_SOURCE)
  list(APPEND uv_libraries dl)
  list(APPEND uv_sources
       unix/android-ifaddrs.c
       unix/linux-core.c
       unix/linux-inotify.c
       unix/linux-syscalls.c
       unix/procfs-exepath.c
       unix/pthread-fixes.c
       unix/random-getentropy.c
       unix/random-getrandom.c
       unix/random-sysctl-linux.c)
endif()

if(APPLE OR CMAKE_SYSTEM_NAME MATCHES "Android|Linux|OS390")
  list(APPEND uv_sources unix/proctitle.c)
endif()

if(CMAKE_SYSTEM_NAME MATCHES "DragonFly|FreeBSD")
  list(APPEND uv_sources unix/freebsd.c)
endif()

if(CMAKE_SYSTEM_NAME MATCHES "DragonFly|FreeBSD|NetBSD|OpenBSD")
  list(APPEND uv_sources unix/posix-hrtime.c unix/bsd-proctitle.c)
endif()

if(APPLE OR CMAKE_SYSTEM_NAME MATCHES "DragonFly|FreeBSD|NetBSD|OpenBSD")
  list(APPEND uv_sources unix/bsd-ifaddrs.c unix/kqueue.c)
endif()

if(CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
  list(APPEND uv_sources unix/random-getrandom.c)
endif()

if(APPLE OR CMAKE_SYSTEM_NAME STREQUAL "OpenBSD")
  list(APPEND uv_sources unix/random-getentropy.c)
endif()

if(APPLE)
  list(APPEND uv_defines _DARWIN_UNLIMITED_SELECT=1 _DARWIN_USE_64_BIT_INODE=1)
  list(APPEND uv_sources
       unix/darwin-proctitle.c
       unix/darwin.c
       unix/fsevents.c)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  list(APPEND uv_defines _GNU_SOURCE _POSIX_C_SOURCE=200112)
  list(APPEND uv_libraries dl rt)
  list(APPEND uv_sources
       unix/linux-core.c
       unix/linux-inotify.c
       unix/linux-syscalls.c
       unix/procfs-exepath.c
       unix/random-getrandom.c
       unix/random-sysctl-linux.c)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "NetBSD")
  list(APPEND uv_sources unix/netbsd.c)
  list(APPEND uv_libraries kvm)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "OpenBSD")
  list(APPEND uv_sources unix/openbsd.c)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "OS390")
  list(APPEND uv_defines PATH_MAX=255)
  list(APPEND uv_defines _AE_BIMODAL)
  list(APPEND uv_defines _ALL_SOURCE)
  list(APPEND uv_defines _ISOC99_SOURCE)
  list(APPEND uv_defines _LARGE_TIME_API)
  list(APPEND uv_defines _OPEN_MSGQ_EXT)
  list(APPEND uv_defines _OPEN_SYS_FILE_EXT)
  list(APPEND uv_defines _OPEN_SYS_IF_EXT)
  list(APPEND uv_defines _OPEN_SYS_SOCK_EXT3)
  list(APPEND uv_defines _OPEN_SYS_SOCK_IPV6)
  list(APPEND uv_defines _UNIX03_SOURCE)
  list(APPEND uv_defines _UNIX03_THREADS)
  list(APPEND uv_defines _UNIX03_WITHDRAWN)
  list(APPEND uv_defines _XOPEN_SOURCE_EXTENDED)
  list(APPEND uv_sources
       unix/pthread-fixes.c
       unix/os390.c
       unix/os390-syscalls.c)
  list(APPEND uv_cflags -Wc,DLL -Wc,exportall -Wc,xplink)
  list(APPEND uv_libraries -Wl,xplink)
  list(APPEND uv_test_libraries -Wl,xplink)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "OS400")
  list(APPEND uv_defines
       _ALL_SOURCE
       _LINUX_SOURCE_COMPAT
       _THREAD_SAFE
       _XOPEN_SOURCE=500)
  list(APPEND uv_sources
    unix/aix-common.c
    unix/ibmi.c
    unix/no-fsevents.c
    unix/posix-poll.c)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "SunOS")
  list(APPEND uv_defines __EXTENSIONS__ _XOPEN_SOURCE=500)
  list(APPEND uv_libraries kstat nsl sendfile socket)
  list(APPEND uv_sources unix/no-proctitle.c unix/sunos.c)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Haiku")
  list(APPEND uv_defines _BSD_SOURCE)
  list(APPEND uv_libraries bsd network)
  list(APPEND uv_sources
      unix/haiku.c
      unix/bsd-ifaddrs.c
      unix/no-fsevents.c
      unix/no-proctitle.c
      unix/posix-hrtime.c
      unix/posix-poll.c)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "QNX")
  list(APPEND uv_sources
    unix/posix-hrtime.c
    unix/posix-poll.c
    unix/qnx.c
    unix/bsd-ifaddrs.c
    unix/no-proctitle.c
    unix/no-fsevents.c)
  list(APPEND uv_cflags -fno-strict-aliasing)
  list(APPEND uv_libraries socket)
endif()

target_sources(sge PRIVATE ${uv_sources})
target_compile_options(sge PRIVATE ${uv_cflags})
target_link_libraries(sge ${uv_libraries})
