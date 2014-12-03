#ifndef DIY_THREAD_H
#define DIY_THREAD_H

#include "thread/tinythread.h"
#include "thread/fast_mutex.h"

#ifdef DIY_NO_THREADS
#include "no-thread.hpp"
#else
namespace diy
{
  using tthread::thread;
  using tthread::mutex;
  using tthread::fast_mutex;
  using tthread::recursive_mutex;
  using tthread::lock_guard;
  namespace this_thread = tthread::this_thread;
}
#endif

#include "critical-resource.hpp"

#endif
