#ifndef CALLBACKS_H_
#define CALLBACKS_H_

#include <functional>
#include <memory>

#include "Timestamp.h"

typedef std::function<void()> TimerCallback;
typedef std::function<void()> PendingCallback;

#endif