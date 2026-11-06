// SPDX-License-Identifier: Apache-2.0
/* Copyright (c) 2008-2018 LG Electronics, Inc.
 * Adapted from pinned calculator/resource_calculator_c.cpp (Apache-2.0).
 * TV C layout, lazy singleton and virtual dispatch verified at 0x16258..0x16542.
 * Platform initialization is deliberately left to ResourceCalculator::create(). */
#include "calculator.h"
#include "resource_calculator_c.h"
#include <cstdlib>
#include <cstdint>
#include <cstring>

#if UINTPTR_MAX == UINT32_MAX
static_assert(sizeof(MRCResource) == 8, "TV C resource ABI");
#endif

/* @0x16258: allocates a null-terminated C array, copying names and quantities. */
static MRCResourceList createResourceListFromCpp(const mrc::Resource *first, const mrc::Resource *last)
{
    if (first == last)
        return nullptr;
    const auto count = last - first;
    auto result = static_cast<MRCResourceList>(std::malloc(sizeof(MRCResource *) * (count + 1)));
    if (result) {
        for (std::ptrdiff_t i = 0; i < count; ++i) {
            auto resource = static_cast<MRCResource *>(std::malloc(sizeof(MRCResource)));
            if (resource) {
                resource->type = ::strdup(first[i].type.c_str());
                resource->quantity = first[i].quantity;
            }
            result[i] = resource;
        }
        result[count] = nullptr;
    }
    return result;
}

/* @0x162b8 = MRCDeleteResourceList: accepts null; frees each name, resource, then the pointer array. */
extern "C" void MRCDeleteResourceList(MRCResourceList list)
{
    if (!list)
        return;
    for (int i = 0; list[i]; ++i) {
        std::free(list[i]->type);
        std::free(list[i]);
    }
    std::free(list);
}

/* @0x162e4 = MRCDeleteResourceListOptions: options are themselves a null-terminated array of C lists. */
extern "C" void MRCDeleteResourceListOptions(MRCResourceListOptions options)
{
    if (!options)
        return;
    for (int i = 0; options[i]; ++i)
        MRCDeleteResourceList(options[i]);
    std::free(options);
}

static mrc::ResourceCalculator *calculator = nullptr;

/* @0x16308 = MRCCalcVdecResources: preserves codec flags and forwards dimensions without normalization. */
extern "C" MRCResourceList MRCCalcVdecResources(int codecs, int width, int height, int frameRate,
                                               MRCScanType scan, MRC3DType mode)
{
    if (!calculator)
        calculator = mrc::ResourceCalculator::create();
    auto resources = calculator->calcVdecResources(codecs, width, height, frameRate,
        static_cast<mrc::ResourceCalculator::ScanType>(scan),
        static_cast<mrc::ResourceCalculator::_3DType>(mode));
    return createResourceListFromCpp(resources.data(), resources.data() + resources.size());
}

/* @0x16380 = MRCCalcVdecResourceOptions: copies the ordered alternatives into separately owned C arrays. */
extern "C" MRCResourceListOptions MRCCalcVdecResourceOptions(int codecs, int width, int height, int frameRate,
                                                             MRCScanType scan, MRC3DType mode)
{
    if (!calculator)
        calculator = mrc::ResourceCalculator::create();
    auto resources = calculator->calcVdecResourceOptions(codecs, width, height, frameRate,
        static_cast<mrc::ResourceCalculator::ScanType>(scan),
        static_cast<mrc::ResourceCalculator::_3DType>(mode));
    if (resources.empty())
        return nullptr;
    auto options = static_cast<MRCResourceListOptions>(std::malloc(sizeof(MRCResourceList) * (resources.size() + 1)));
    if (options) {
        for (std::size_t i = 0; i < resources.size(); ++i)
            options[i] = createResourceListFromCpp(resources[i].data(), resources[i].data() + resources[i].size());
        options[resources.size()] = nullptr;
    }
    return options;
}

/* @0x16478 = MRCCalcAdecResources: version precedes channel count in the TV ABI. */
extern "C" MRCResourceList MRCCalcAdecResources(int codecs, int version, int channel)
{
    if (!calculator)
        calculator = mrc::ResourceCalculator::create();
    auto resources = calculator->calcAdecResources(codecs, version, channel);
    return createResourceListFromCpp(resources.data(), resources.data() + resources.size());
}

/* @0x164e0 = MRCCalcMiscResources: both C integer flags are converted to bool before virtual dispatch. */
extern "C" MRCResourceList MRCCalcMiscResources(int useSecureVideoPath, int useSecureReformatter)
{
    if (!calculator)
        calculator = mrc::ResourceCalculator::create();
    auto resources = calculator->calcMiscResources(useSecureVideoPath != 0, useSecureReformatter != 0);
    return createResourceListFromCpp(resources.data(), resources.data() + resources.size());
}
