#ifndef _ST_HPC_PPL_COMMON_OCL_MEMORY_H_
#define _ST_HPC_PPL_COMMON_OCL_MEMORY_H_

#include <map>
#include <unistd.h>
#include <string.h>
#include "ppl/common/retcode.h"
#include "ppl/common/ocl/memory_option.h"

namespace ppl { namespace common { namespace ocl {

cl_mem OpenCLMemeoryAlloc(cl_context ctx, uint64_t size, cl_int* err, cl_mem_flags flags = CL_MEM_READ_WRITE);

std::map<const char*, int> getMemInfo();

void OCL_MEM_CHECK(std::string info_);

}}} // namespace ppl::common::ocl

#endif