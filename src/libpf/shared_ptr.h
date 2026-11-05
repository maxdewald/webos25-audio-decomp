/* Fetch the pinned Boost source with: python3 tools/fetch-public-sources.py boost */
#ifndef LIBPF_SHARED_PTR_H
#define LIBPF_SHARED_PTR_H
#include <boost/version.hpp>
#include <boost/shared_ptr.hpp>
static_assert(BOOST_VERSION == 107800, "libpf was built with Boost 1.78");
static_assert(sizeof(boost::shared_ptr<void>) == 2 * sizeof(void *), "shared_ptr ABI");
#endif
