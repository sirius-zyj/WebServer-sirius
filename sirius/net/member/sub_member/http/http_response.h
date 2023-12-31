#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H


#include <sys/stat.h>    // stat
#include <sys/mman.h>    // mmap, munmap

#include "plugin/head.h"
#include "plugin/buffer/buffer.h"
#include "net/redis_connection_pool.h"

namespace sirius
{
class HttpResponse
{
 public:
    HttpResponse();
    ~HttpResponse();

    void Init(const std::string& srcDir, std::string& path, bool isKeepAlive = false, int code = -1);
    void MakeResponse(Buffer& buff);
    const char* File(){ return file_.value().c_str(); };
    size_t FileLen() const{ return mmFileStat_.st_size; };
    void ErrorContent(Buffer& buff, std::string message);
    int Code() const { return code_; }

 private:
    void AddStateLine_(Buffer &buff);
    void AddHeader_(Buffer &buff);
    void AddContent_(Buffer &buff);

    void ErrorHtml_();
    std::string GetFileType_();

    int code_;
    bool isKeepAlive_;

    std::string path_;
    std::string srcDir_;
    
    sw::redis::OptionalString file_;
    struct stat mmFileStat_;

    static const unordered_map<string, string> SUFFIX_TYPE;
    static const unordered_map<int, string> CODE_STATUS;
    static const unordered_map<int, string> CODE_PATH;
};
}

#endif