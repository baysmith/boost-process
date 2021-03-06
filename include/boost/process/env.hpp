// Copyright (c) 2016 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROCESS_DETAIL_ENV_HPP_
#define BOOST_PROCESS_DETAIL_ENV_HPP_

#include <boost/process/environment.hpp>
#include <boost/none.hpp>


#if defined(BOOST_POSIX_API)
#include <boost/process/detail/posix/env_init.hpp>
#elif defined(BOOST_WINDOWS_API)
#include <boost/process/detail/windows/env_init.hpp>
#endif

/** \file boost/process/env.hpp
 *
 *    Header which provides the env property. It allows the modification of the
 *    environment the child process will run in, in a functional style.
 */


namespace boost { namespace process { namespace detail {


template<typename T>
struct env_set
{
    std::string key;
    T value;
};

template<typename T>
struct env_append
{
    std::string key;
    T value;
};

struct env_reset
{
    std::string key;
};

struct env_init
{
    environment env;
};

struct env_proxy
{
    std::string key;

    env_set<std::string> operator=(const std::string & value)
    {
        return {std::move(key), value};
    }
    env_set<std::vector<std::string>> operator=(const std::vector<std::string> & value)
    {
        return {std::move(key), value};
    }
    env_set<std::initializer_list<char*>> operator=(const std::initializer_list<char*> & value)
    {
        return {std::move(key), value};
    }

    env_append<std::string> operator+=(const std::string & value)
    {
        return {std::move(key), value};
    }
    env_append<std::vector<std::string>> operator+=(const std::vector<std::string> & value)
    {
        return {std::move(key), value};
    }
    env_append<std::initializer_list<char*>> operator+=(const std::initializer_list<char*> & value)
    {
        return {std::move(key), value};
    }

    env_reset operator=(boost::none_t)
    {
        return {std::move(key)};
    }
};

struct env_
{
    constexpr env_() {};

    env_set<std::string> operator()(const std::string & key, const std::string & value) const
    {
        return {key, value};
    }
    env_set<std::vector<std::string>> operator()(const std::string & key, const std::vector<std::string> & value) const
    {
        return {key, value};
    }
    env_set<std::vector<char*>> operator()(const std::string & key, const std::initializer_list<char*> & value) const
    {
        return {key, std::vector<char*>(value)};
    }
    env_reset operator()(const std::string & key, boost::none_t)
    {
        return {key};
    }
    env_proxy operator[](const std::string & key) const
    {
        return {key};
    }
    env_init operator()(const environment & env) const
    {
        return {env};
    }
    env_init operator= (const environment & env) const
    {
        return {env};
    }
};


struct env_builder
{
    environment env;
    void operator()(const environment & e)
    {
        env = e;
    }

    void operator()(env_init & ei)
    {
        env = std::move(ei.env);
    }
    template<typename T>
    void operator()(env_set<T> & es)
    {
        //env.set(es.key, es.value_to_string());
    }
    void operator()(env_reset & es)
    {
        env.erase(es.key);
    }
    template<typename T>
    void operator()(env_append<T> & es)
    {
        //env.set(es.key, es.value_to_string());
    }

    typedef api::env_init result_type;
    api::env_init get_initializer()
    {
        return api::env_init(std::move(env));
    }
};

template<>
struct initializer_builder<env_tag>
{
    typedef env_builder type;
};


constexpr static env_ env;
}

using boost::process::detail::env;

}}

#endif /* INCLUDE_BOOST_PROCESS_DETAIL_ENV_HPP_ */
