// Copyright (c) 2006, 2007 Julio M. Merino Vidal
// Copyright (c) 2008 Ilya Sokolov, Boris Schaeling
// Copyright (c) 2009 Boris Schaeling
// Copyright (c) 2010 Felipe Tanus, Boris Schaeling
// Copyright (c) 2011, 2012 Jeff Flinn, Boris Schaeling
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MAIN
#define BOOST_TEST_IGNORE_SIGCHLD
#include <boost/test/included/unit_test.hpp>

#include <boost/process/error.hpp>
#include <boost/process/async.hpp>
#include <boost/process/io.hpp>
#include <boost/process/child.hpp>

#include <boost/thread.hpp>
#include <future>

#include <boost/system/error_code.hpp>

#include <boost/algorithm/string/predicate.hpp>


using namespace std;

namespace bp = boost::process;

BOOST_AUTO_TEST_CASE(async_out_future, *boost::unit_test::timeout(2))
{

    using boost::unit_test::framework::master_test_suite;

    boost::asio::io_service io_service;


    std::error_code ec;

    std::future<std::string> fut;
    boost::asio::streambuf in_buf;


    std::ostream ostr(&in_buf);
    ostr << "-string" << endl ;

    bp::child c(
        master_test_suite().argv[1],
        "test", "--prefix-once", "test",
        bp::std_in  < in_buf,
        bp::std_out > fut,
        io_service,
        ec
    );
    BOOST_REQUIRE(!ec);


    io_service.run();

    BOOST_REQUIRE(fut.valid());
    std::string line;

    BOOST_CHECK_NO_THROW(line = fut.get());

    std::string val = "test-string";
    BOOST_REQUIRE_GE(line.size(), val.size());
    if (line >= val)
        BOOST_CHECK(boost::algorithm::starts_with(line, val));


    c.wait();
}


