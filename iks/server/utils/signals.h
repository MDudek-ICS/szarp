#ifndef __UTILS_SIGNALS_H__
#define __UTILS_SIGNALS_H__

#include <boost/signals2.hpp>

using boost::signals2::connection;

typedef boost::signals2::signal<void ()> sig_void;
typedef sig_void::slot_type sig_void_slot;

typedef boost::signals2::signal<void (const std::string&)> sig_line;
typedef sig_line::slot_type sig_line_slot;

#endif /* __UTILS_SIGNALS_H__ */

