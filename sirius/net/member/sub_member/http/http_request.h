#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <regex>
#include <algorithm>
#include "plugin/head.h"
#include "plugin/buffer/buffer.h"
#include "net/sql_connection_pool.h"

namespace sirius
{

class HttpRequest
{
 public:
    enum PARSE_STATE {
        REQUEST_LINE,
        HEADERS,
        BODY,
        FINISH,        
    };

    enum HTTP_CODE {
        NO_REQUEST = 0,
        GET_REQUEST,
        BAD_REQUEST,
        NO_RESOURSE,
        FORBIDDENT_REQUEST,
        FILE_REQUEST,
        INTERNAL_ERROR,
        CLOSED_CONNECTION,
    };
    
    HttpRequest() { Init(); };
    ~HttpRequest() = default;

    void Init();
    bool parse(Buffer& buff);

    string path() const{ return path_; };
    string& path(){ return path_; };
    string method() const{ return method_; };
    string version() const{ return version_; };
    string GetPost(const string& key) const;
    string GetPost(const char* key) const;

    bool IsKeepAlive() const;

    /* 
    todo 
    void HttpConn::ParseFormData() {}
    void HttpConn::ParseJson() {}
    */

 private:
    bool ParseRequestLine_(const string& line);
    void ParseHeader_(const string& line);
    void ParseBody_(const string& line);

    void ParsePath_();
    void ParsePost_();
    void ParseFromUrlencoded_();

    static bool UserLogin(const string& name, const string& pwd);
    static bool UserRegister(const string& name, const string& pwd);

    PARSE_STATE state_;
    string method_, path_, version_, body_;
    unordered_map<string, string> header_;
    unordered_map<string, string> post_;

    static const unordered_set<string> DEFAULT_HTML;
    static const unordered_map<string, int> DEFAULT_HTML_TAG;
    static int ConverHex(char ch);
};
}
#endif