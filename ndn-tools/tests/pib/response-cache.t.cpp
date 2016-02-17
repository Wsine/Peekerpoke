/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2014-2015,  Regents of the University of California.
 *
 * This file is part of ndn-tools (Named Data Networking Essential Tools).
 * See AUTHORS.md for complete list of ndn-tools authors and contributors.
 *
 * ndn-tools is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndn-tools is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndn-tools, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @author Yingdi Yu <yingdi@cs.ucla.edu>
 */

#include "tools/pib/response-cache.hpp"

#include "tests/test-common.hpp"

namespace ndn {
namespace pib {
namespace tests {

BOOST_AUTO_TEST_SUITE(PibResponseCache)

BOOST_AUTO_TEST_CASE(Basic)
{
  ResponseCache cache;

  Name dataName("/test/data");
  dataName.appendVersion();
  shared_ptr<Data> data = make_shared<Data>(dataName);

  Name dataNameNoVersion("/test/data");
  Name anotherDataName("/test/another");

  BOOST_CHECK_EQUAL(static_cast<bool>(cache.find(dataNameNoVersion)), false);
  BOOST_CHECK_EQUAL(static_cast<bool>(cache.find(dataName, true)), false);

  cache.insert(*data);

  BOOST_CHECK(static_cast<bool>(cache.find(dataNameNoVersion)));
  BOOST_CHECK(static_cast<bool>(cache.find(dataName, true)));
  BOOST_CHECK_EQUAL(static_cast<bool>(cache.find(anotherDataName)), false);
  BOOST_CHECK_EQUAL(static_cast<bool>(cache.find(anotherDataName, true)), false);

  cache.erase(dataNameNoVersion);

  BOOST_CHECK_EQUAL(static_cast<bool>(cache.find(dataNameNoVersion)), false);
  BOOST_CHECK_EQUAL(static_cast<bool>(cache.find(dataName, true)), false);
}


BOOST_AUTO_TEST_SUITE_END()

} // namespace tests
} // namespace pib
} // namespace ndn
