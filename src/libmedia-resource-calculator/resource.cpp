/* Source-assisted reconstruction against webosose/media-resource-calculator
 * 6e7b1658516208bf01719bb1b968dadb4c43c5b3, calculator/resource.{h,cpp}.
 * Upstream files and their Apache-2.0 notices are preserved in lg-opensource/public/.
 * The shipped ARM Resource is 0x20 bytes: vptr, cxx11 string, quantity at +0x1c.
 * Unlike current upstream, the shipped option product copies its temporary lists.
 */
#include "resource.h"
#include <cstdint>

namespace mrc {
#if UINTPTR_MAX == UINT32_MAX
static_assert(sizeof(Resource) == 0x20, "Resource ARM ABI");
#endif

/* @0x13bf0 */
Resource::Resource(const std::string &name, int count) : type(name), quantity(count) {}

/* @0x13864 = _ZN3mrc8ResourceD1Ev */
Resource::~Resource() {}

/* @0x13894 = _ZN3mrc8ResourceD0Ev */

/* @0x13c78: resource quantities combine by maximum, not addition. */
void concatResourceList(ResourceList *destination, const ResourceList *source)
{
    if (!source || source->empty())
        return;
    if (destination->empty()) {
        *destination = *source;
        return;
    }
    for (const auto &resource : *source) {
        auto found = destination->begin();
        for (; found != destination->end(); ++found) {
            if (found->type.compare(resource.type) == 0) {
                if (found->quantity < resource.quantity)
                    found->quantity = resource.quantity;
                break;
            }
        }
        if (found == destination->end())
            destination->push_back(resource);
    }
}

/* @0x14074: Cartesian product of resource alternatives; preserves input order. */
void concatResourceListOptions(ResourceListOptions *destination, const ResourceListOptions *source)
{
    if (!source || source->empty())
        return;
    if (destination->empty()) {
        *destination = *source;
        return;
    }
    ResourceListOptions product;
    for (const auto &left : *destination) {
        for (const auto &right : *source) {
            ResourceList combined = left;
            concatResourceList(&combined, &right);
            product.push_back(combined);
        }
    }
    *destination = product;
}
}
