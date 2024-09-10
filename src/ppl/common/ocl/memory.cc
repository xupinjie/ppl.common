#include "memory.h"
#include "device.h"
#include "ppl/common/log.h"
namespace ppl { namespace common { namespace ocl {

cl_mem OpenCLMemeoryAlloc(cl_context ctx, uint64_t size, cl_int* err, cl_mem_flags flags) {
    cl_int rc = 0;
    cl_mem buffer = clCreateBuffer(ctx, flags, size, nullptr, &rc);
    if (CL_SUCCESS != rc) {
        LOG(ERROR) << "Call clCreateBuffer failed with code: " << rc;
        *err = rc;
        return nullptr;
    }

    *err = rc;
    return buffer;
}

std::string pid2string(pid_t pid) {
    std::ostringstream stream;
    stream << pid;
    return stream.str();
}

std::pair<std::string, int> normalMemInfo(const char* info) {
    std::string str(info);
    int index = 0;
    int firstPos = 100000;
    if(!str.empty()) {
        for(int i = 0; i < str.size(); i++)
        {
            if(str[i]>='0' && str[i]<='9'){
                firstPos = ( i < firstPos ? i : firstPos);
                break;
            }
        }
    }
    std::string numStr = str.substr(firstPos,str.size()-firstPos);
    char* a = (char*)numStr.c_str();
    char* res;
    res = strtok(a, " ");

    if(!str.empty()) {
        while( (index = str.find(' ')) != (int)std::string::npos) {
            str.erase(index, 1);
        }

        while( (index = str.find('\n')) != (int)std::string::npos) {
            str.erase(index, 1);
        }
    }

    char* cstr = (char*)str.c_str();
    char* tok;
    tok = strtok(cstr, ":");

    std::string sinfo(tok);
    int inter = atoi(res);
    return std::make_pair(sinfo, inter);
}

#define find_map(str); \
if(strstr(line, str)) {\
    native = normalMemInfo(line);\
    if(res.find(str)==res.end())\
        res.insert(std::make_pair(str, native.second));\
    else \
        res[str] = native.second; \
}

std::map<const char*, int> getMemInfo()
{
    pid_t pid = getpid();
    std::string meminfo = "dumpsys meminfo " + pid2string(pid);
    //system(meminfo.c_str());
    FILE *fp = popen(meminfo.c_str(), "r");
    std::map<const char*, int>res;
    if(nullptr == fp) {
        return res;
    }
    char line[512];
    bool ischeck = false;

    std::pair<std::string, int> native;
    while(!feof(fp)) {
        fgets(line, 512, fp);
        if(strstr(line, "App Summary")) ischeck = true;

        if(ischeck){
            find_map("Java Heap");
            find_map("Native Heap");
            find_map("Code");
            find_map("Stack");
            find_map("Graphics");
            find_map("Private Other");
            find_map("System");
            find_map("TOTAL");
        }
    }
    return res;
}

void OCL_MEM_CHECK(std::string info_)
{
    std::map<const char*, int>meminfo = getMemInfo();
    LOG(INFO) << "\033[32m" << info_ << "\033[0m";
    std::map<const char*, int>::iterator iter;
    for(iter=meminfo.begin();iter!=meminfo.end();iter++){
        LOG(INFO) << "\033[32m" << iter->first <<"(KB):"<< "\033[0m"<<iter->second;
    }
}

}}} // namespace ppl::common::ocl
