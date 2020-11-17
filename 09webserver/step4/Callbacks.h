#ifndef CALLBACKS_H_
#define CALLBACKS_H_

#include <functional>
#include <memory>

#include "Timestamp.h"

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

typedef std::function<void()> TimerCallback;

#endif