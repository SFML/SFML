//
// ts/executor.hpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_TS_EXECUTOR_HPP
#define NET_TS_TS_EXECUTOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <experimental/__net_ts/handler_type.hpp>
#include <experimental/__net_ts/async_result.hpp>
#include <experimental/__net_ts/associated_allocator.hpp>
#include <experimental/__net_ts/execution_context.hpp>
#include <experimental/__net_ts/is_executor.hpp>
#include <experimental/__net_ts/associated_executor.hpp>
#include <experimental/__net_ts/bind_executor.hpp>
#include <experimental/__net_ts/executor_work_guard.hpp>
#include <experimental/__net_ts/system_executor.hpp>
#include <experimental/__net_ts/executor.hpp>
#include <experimental/__net_ts/dispatch.hpp>
#include <experimental/__net_ts/post.hpp>
#include <experimental/__net_ts/defer.hpp>
#include <experimental/__net_ts/strand.hpp>
#include <experimental/__net_ts/packaged_task.hpp>
#include <experimental/__net_ts/use_future.hpp>

#endif // NET_TS_TS_EXECUTOR_HPP
