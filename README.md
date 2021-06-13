[![Build Status](https://travis-ci.com/mmarkeloff/cpp-syslog-client.svg?branch=main)](https://travis-ci.com/mmarkeloff/cpp-syslog-client)

<h1 align="center">
cpp-syslog-client
</h1>

C++ header only stream-based syslog client library.

## Usage

```cpp
#include "syslog_client.hpp"

int main(int argc, char* argv[]) {
    syslog::ostream syslogClient;

    // by default syslog client configured on 127.0.0.1:514
    // so you can overwrite it
    syslogClient.setAddr("127.0.0.1");
    syslogClient.setPort(1000);

    syslogClient << syslog::LogLvlMng::LL_INFO << "message" << std::endl;

    return 0;
}
```

## Examples

See [sample project](sample) for more complete usage examples.

## Library details

### Log levels

```cpp
enum LogLvl {
    LL_EMERG = 0, ///< EMERG
    LL_ALERT, ///< ALERT
    LL_CRIT, ///< CRIT
    LL_ERR, ///< ERR
    LL_WARNING, ///< WARNING
    LL_NOTICE, ///< NOTICE
    LL_INFO, ///< INFO
    LL_DEBUG ///< DEBUG
};
```

### Log facility

```cpp
enum Facility {
    LF_KERN = 0, ///< KERN
    LF_USER, ///< USER
    LF_MAIL, ///< MAIL
    LF_DAEMON, ///< DAEMON
    LF_AUTH, ///< AUTH
    LF_SYSLOG, ///< SYSLOG
    LF_LPR, ///< LPR
    LF_NEWS, ///< NEWS
    LF_UUCP, ///< UUCP
    LF_CRON, ///< CRON
    LF_AUTHPRIV, ///< AUTHPRIV
    LF_FTP, ///< FTP
    LF_NTP, ///< NTP
    LF_AUDIT, ///< AUDIT
    LF_ALERT, ///< ALERT
    LF_CRON2, ///< CRON2
    LF_LOCAL0, ///< LOCAL0
    LF_LOCAL1, ///< LOCAL1
    LF_LOCAL2, ///< LOCAL2
    LF_LOCAL3, ///< LOCAL3
    LF_LOCAL4, ///< LOCAL4
    LF_LOCAL5, ///< LOCAL5
    LF_LOCAL6, ///< LOCAL6
    LF_LOCAL7 ///< LOCAL7
};
```

## Documentation

See automatic generated [docs](https://mmarkeloff.github.io/cpp-syslog-client/) for more information.

