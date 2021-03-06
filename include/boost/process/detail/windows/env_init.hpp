// Copyright (c) 2016 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_PROCESS_DETAIL_WINDOWS_ENV_INIT_HPP_
#define BOOST_PROCESS_DETAIL_WINDOWS_ENV_INIT_HPP_

#include <boost/detail/winapi/error_codes.hpp>


#include <boost/process/detail/config.hpp>
#include <boost/process/detail/handler_base.hpp>
#include <boost/process/environment.hpp>

namespace boost { namespace process { namespace detail { namespace windows {

struct env_init : public ::boost::process::detail::handler_base
{
    boost::process::environment env;

    env_init(boost::process::environment && env) : env(std::move(env)) {};
    env_init(const boost::process::environment & env) : env(env) {};


    template <class WindowsExecutor>
    void on_setup(WindowsExecutor &exec) const
    {
        auto e = env.native_handle();

        if (*e == null_char<char>())
        {
            exec.set_error(std::error_code(::boost::detail::winapi::ERROR_BAD_ENVIRONMENT_, std::system_category()),
                    "Bad Environment");
        }

        exec.env = e;
    }

};

}}}}



#endif /* BOOST_PROCESS_DETAIL_WINDOWS_ENV_INIT_HPP_ */
