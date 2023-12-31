# WebServer-sirius

A WebServer written in C++, which could be suitable for beginners to learn.

## What does sirius contain?

+ epoll and reactor
+ one loop for one thread
+ sql connection pool combined with RAII(soci)
+ redis connection pool combined with RAII(redis++)

## What is the directory structure of sirius?

![structure](https://img1.imgtp.com/2023/07/24/buY8qlk6.png)

## How to establish?

To clone the repository, you can run:

```git
git clone https://github.com/sirius-zyj/WebServer-sirius.git
```

+ ***Setup boost***

Make sure your compuer have setup boost and libmysql, you can run:

```sh
sudo apt-get install libboost-all-dev
sudo apt-get install libmysqlclient-dev
```

+ ***Setup mysql***
  
Make sure your computer have setup mysql5.7 or 8.0.
Then create your own account and modify the content in the file `sirius/plugin/config.cpp` accordingly.
To create a simple database, you can run:

```sql
create database sirius;

USE sirius;
CREATE TABLE user(
    username char(50) NULL,
    password char(50) NULL
)ENGINE=InnoDB;

INSERT INTO user(username, password) VALUES('name', 'password');
```

+ ***Setup redis***

Make sure your computer have setup redis, you can run:

```sh
sudo apt install redis-server
sudo systemctl status redis
```

+ ***Make and run***
  
To make the object in your computer, you can run:

```sh
cd sirius
sh make.sh
```

To run the established object, you can run:

```sh
sh run.sh
```

## Reference

+ [muduo by chenshuo](https://github.com/chenshuo/muduo)
+ [TinyWebServer by qinguoyi](https://github.com/qinguoyi/TinyWebServer)
+ [WebServer by markparticle](https://github.com/markparticle/WebServer)
+ [soci](https://github.com/SOCI/soci)
+ *<<Linux高性能服务器编程>>*   游双 著