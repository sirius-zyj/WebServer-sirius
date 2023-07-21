#ifndef HEAD_H
#define HEAD_H

#include <string>
#include <vector>
#include <cstring>
#include <memory>
#include <assert.h>
#include <unistd.h>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <condition_variable>
#include <semaphore.h>
#include <regex>
#include <list>

#include "type.h"
#include "logger.h"
#include "config.h"

#define log Logger::get_single_instance() 

using std::string;
using std::vector;
using std::list;
using std::map;
using std::unordered_map;
using std::unordered_set;

using std::shared_ptr;
using std::unique_ptr;
using std::placeholders::_1;
using std::placeholders::_2;

using std::mutex;
using std::unique_lock;
using std::condition_variable;

#endif