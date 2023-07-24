#include "http_request.h"

using namespace sirius;

const unordered_set<string> HttpRequest::DEFAULT_HTML{
            "/index", "/register", "/login",
             "/welcome", "/video", "/picture", };

const unordered_map<string, int> HttpRequest::DEFAULT_HTML_TAG {
            {"/register.html", 0}, {"/login.html", 1},  };

void HttpRequest::Init() {
    state_ = REQUEST_LINE;
    
    method_ = path_ = version_ = body_ = "";
    
    header_.clear();
    post_.clear();
}

bool HttpRequest::IsKeepAlive() const {
    if(header_.count("Connection") == 1) {
        return header_.find("Connection")->second == "keep-alive" && version_ == "1.1";
    }
    return false;
}

bool HttpRequest::parse(Buffer& buff) {
    const char CRLF[] = "\r\n";
    if(buff.readable_chars() <= 0) {
        return false;
    }
    while(buff.readable_chars() && state_ != FINISH) {
        const char* lineEnd = std::search(buff.peek(), buff.begin_write(), CRLF, CRLF + 2);
        std::string line(buff.peek(), lineEnd);
        switch(state_)
        {
            case REQUEST_LINE:
                if(!ParseRequestLine_(line)) {
                    return false;
                }
                ParsePath_();
                break;    
            case HEADERS:
                ParseHeader_(line);
                if(buff.readable_chars() <= 2) {
                    state_ = FINISH;
                }
                break;
            case BODY:
                ParseBody_(line);
                break;
            default:
                break;
        }
        if(lineEnd == buff.begin_write()) { break; }
        buff.retrieve_until(lineEnd + 2);
    }
    log->log_info("[%s], [%s], [%s]", method_.c_str(), path_.c_str(), version_.c_str());
    return true;
}

void HttpRequest::ParsePath_() {
    if(path_ == "/") {
        path_ = "/index.html"; 
    }
    else {
        for(auto &item: DEFAULT_HTML) {
            if(item == path_) {
                path_ += ".html";
                break;
            }
        }
    }
}

bool HttpRequest::ParseRequestLine_(const string& line) {
    std::regex patten("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");
    std::smatch subMatch;
    if(regex_match(line, subMatch, patten)) {   
        method_ = subMatch[1];
        path_ = subMatch[2];
        version_ = subMatch[3];
        state_ = HEADERS;
        return true;
    }
    log->log_error("RequestLine Error");
    return false;
}

void HttpRequest::ParseHeader_(const string& line) {
    std::regex patten("^([^:]*): ?(.*)$");
    std::smatch subMatch;
    if(regex_match(line, subMatch, patten)) {
        header_[subMatch[1].str()] = subMatch[2].str();
    }
    else {
        state_ = BODY;
    }
}

void HttpRequest::ParseBody_(const string& line) {
    body_ = line;
    ParsePost_();
    state_ = FINISH;
    log->log_info("Body:%s, len:%d", line.c_str(), line.size());
}

int HttpRequest::ConverHex(char ch) {
    if(ch >= 'A' && ch <= 'F') return ch -'A' + 10;
    if(ch >= 'a' && ch <= 'f') return ch -'a' + 10;
    return ch;
}

void HttpRequest::ParsePost_() {
    if(method_ == "POST" && header_["Content-Type"] == "application/x-www-form-urlencoded") {
        ParseFromUrlencoded_();
        if(DEFAULT_HTML_TAG.count(path_)) {
            int tag = DEFAULT_HTML_TAG.find(path_)->second;
            if(tag == 0 || tag == 1) {
                bool isLogin = (tag == 1);
                if(isLogin){
                    path_ = UserLogin(post_["username"], post_["password"]) ? "/welcome.html" : "/error.html";
                }
                else {
                    path_ = UserRegister(post_["username"], post_["password"]) ? "/register_success.html" : "/error.html";
                }
            }
        }
    }   
}

void HttpRequest::ParseFromUrlencoded_() {
    if(body_.size() == 0) { return; }

    string key, value;
    int num = 0;
    int n = body_.size();
    int i = 0, j = 0;

    for(; i < n; i++) {
        char ch = body_[i];
        switch (ch) {
        case '=':
            key = body_.substr(j, i - j);
            j = i + 1;
            break;
        case '+':
            body_[i] = ' ';
            break;
        case '%':
            num = ConverHex(body_[i + 1]) * 16 + ConverHex(body_[i + 2]);
            body_[i + 2] = num % 10 + '0';
            body_[i + 1] = num / 10 + '0';
            i += 2;
            break;
        case '&':
            value = body_.substr(j, i - j);
            j = i + 1;
            post_[key] = value;
            log->log_info("%s = %s", key.c_str(), value.c_str());
            break;
        default:
            break;
        }
    }
    assert(j <= i);
    if(post_.count(key) == 0 && j < i) {
        value = body_.substr(j, i - j);
        post_[key] = value;
    }
}

bool HttpRequest::UserRegister(const string &name, const string &pwd)
{
    if(name == "" || pwd == "") { return false; }
    log->log_info("Login name:%s pwd:%s", name.c_str(), pwd.c_str());

    SqlConnRAII sql;

    /* 查询用户及密码 */
    string selectPwd;
    sql << "SELECT password FROM user WHERE username = :name LIMIT 1",into(selectPwd), use(name);
    if(selectPwd != ""){//用户名已存在
        log->log_info("User has existed!");
        return false;
    }

    /* 注册行为 */
    soci::statement st = (sql->prepare << "INSERT INTO user(username, password) VALUES(:name,:pwd)", use(name), use(pwd));
    if(st.execute()) { 
        log->log_error( "Insert error!");
        return false;
    }
    log->log_info("Register success!");
    return true;
}

bool HttpRequest::UserLogin(const string &name, const string &pwd) {
    if(name == "" || pwd == "") { return false; }
    log->log_info("Login name:%s pwd:%s", name.c_str(), pwd.c_str());

    SqlConnRAII sql;

    /* 查询用户及密码 */
    string selectPwd;
    sql << "SELECT password FROM user WHERE username = :name LIMIT 1",into(selectPwd), use(name);
    
    /* 登录行为 */
    if(selectPwd == "") {//用户名不存在
        log->log_info("User does not has existed!");
        return false;
    }
    else if(pwd == selectPwd) { //密码正确
        log->log_info("Login success!");
        return true; 
    }
    else {//密码错误
        log->log_info("pwd error!");
        return false;
    }
    
    return true;
}


std::string HttpRequest::GetPost(const std::string& key) const {
    assert(key != "");
    if(post_.count(key) == 1) {
        return post_.find(key)->second;
    }
    return "";
}

std::string HttpRequest::GetPost(const char* key) const {
    assert(key != nullptr);
    if(post_.count(key) == 1) {
        return post_.find(key)->second;
    }
    return "";
}