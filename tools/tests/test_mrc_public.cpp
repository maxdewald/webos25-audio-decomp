// @sources src/libmedia-resource-calculator/resource.cpp
// @cflags -std=c++17 -Isrc/libmedia-resource-calculator -Ilg-opensource/public/media-resource-calculator/calculator
/* Regression checks for shipped resource-list semantics. */
#include "resource.h"
#include <cassert>
#include <iostream>

int main()
{
    using namespace mrc;
    ResourceList destination{{"ADEC",1},{"PCMMC",2}};
    ResourceList incoming{{"ADEC",3},{"PCMMC",1},{"VDEC",1}};
    concatResourceList(&destination, &incoming);
    assert(destination.size()==3 && destination[0].quantity==3 && destination[1].quantity==2);
    assert(destination[2].type=="VDEC");
    concatResourceList(&destination, nullptr);
    ResourceList empty;
    concatResourceList(&empty, &incoming);
    assert(empty.size()==incoming.size());
    ResourceListOptions left{{{"ADEC",1}},{{"PCMMC",1}}};
    ResourceListOptions right{{{"VDEC",1}},{{"ADEC",2}}};
    concatResourceListOptions(&left, &right);
    assert(left.size()==4);
    assert(left[0].size()==2 && left[0][0].type=="ADEC" && left[0][1].type=="VDEC");
    assert(left[1].size()==1 && left[1][0].quantity==2);
    assert(left[2][0].type=="PCMMC" && left[2][1].type=="VDEC");
    assert(left[3][0].type=="PCMMC" && left[3][1].type=="ADEC");
    ResourceListOptions copy;
    concatResourceListOptions(&copy,&right);
    assert(copy.size()==2);
    concatResourceListOptions(&copy,nullptr);
    assert(copy.size()==2);
    std::cout << "PASS: calculator maximum quantities, insertion order and Cartesian options\n";
}
