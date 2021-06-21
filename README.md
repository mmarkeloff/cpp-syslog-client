[![Build Status](https://travis-ci.com/mmarkeloff/cpp-syslog-client.svg?branch=main)](https://travis-ci.com/mmarkeloff/cpp-syslog-client)

<h1 align="center">
cpp-syslog-client
</h1>

C++ header only stream-based syslog client library.

## Usage

```cpp
#include "syslog_client.hpp"

int main() {
    // auto syslog{syslog::makeUDPClient_mt()};
    auto syslog{syslog::makeUDPClient_st()};

    // by default UDP syslog client configured on 127.0.0.1:514
    // so you can overwrite it
    syslog.setAddr("127.0.0.1");
    syslog.setPort(1000);

    syslog << syslog::LogLvlMng::LL_INFO << "message" << std::endl;
}
```

## Examples

See [sample project](sample) for more complete usage examples.

## Library details

From [rfc5424](https://datatracker.ietf.org/doc/html/rfc5424#section-6.2.1)

### Log severity level

| syslog::LogLvlMng::LogLvl | Numerical code | Description                             |
| :---                      | :---           | :---:                                   |
| LL_EMERG                  | 0              | Emergency: system is unusable           |
| LL_ALERT                  | 1              | Alert: action must be taken immediately |
| LL_CRIT                   | 2              | Critical: critical conditions           |
| LL_ERR                    | 3              | Error: error conditions                 |
| LL_WARNING                | 4              | Warning: warning conditions             |
| LL_NOTICE                 | 5              | Notice: normal but significant condition |
| LL_INFO                   | 6              | Informational: informational messages   |
| LL_DEBUG                  | 7              | Debug: debug-level messages             |

### Log facility

| syslog::LogFacilityMng::LogFacility | Numerical code | Description                              |
| :---                                | :---           | :---:                                    |
| LF_KERN                             | 0              | kernel messages                          |
| LF_USER                             | 1              | user-level messages                      |
| LF_MAIL                             | 2              | mail system                              |
| LF_DAEMON                           | 3              | system daemons                           |
| LF_AUTH                             | 4              | security/authorization messages          |
| LF_SYSLOG                           | 5              | messages generated internally by syslogd |
| LF_LPR                              | 6              | line printer subsystem                   |
| LF_NEWS                             | 7              | network news subsystem                   |
| LF_UUCP                             | 8              | UUCP subsystem                           |
| LF_CRON                             | 9              | clock daemon                             |
| LF_AUTHPRIV                         | 10             | security/authorization messages          |
| LF_FTP                              | 11             | FTP daemon                               |
| LF_NTP                              | 12             | NTP subsystem                            |
| LF_AUDIT                            | 13             | log audit                                |
| LF_ALERT                            | 14             | log alert                                |
| LF_CRON2                            | 15             | clock daemon (note 2)                    |
| LF_LOCAL0                           | 16             | local use 0  (local0)                    |
| LF_LOCAL1                           | 17             | local use 1  (local1)                    |
| LF_LOCAL2                           | 18             | local use 2  (local2)                    |
| LF_LOCAL3                           | 19             | local use 3  (local3)                    |
| LF_LOCAL4                           | 20             | local use 4  (local4)                    |
| LF_LOCAL5                           | 21             | local use 5  (local5)                    |
| LF_LOCAL6                           | 22             | local use 6  (local6)                    |
| LF_LOCAL7                           | 23             | local use 7  (local7)                    |

## Documentation

See automatic generated [docs](https://mmarkeloff.github.io/cpp-syslog-client/) for more information.
