/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

// This file is used to link dependencies library
#include "logger.hpp"

namespace logger {


    static std::vector<std::string> log_subscribers;

    void addLogSubscribers(const std::string& address){
      log_subscribers.push_back(address);
    }
    void removeLogSubscribers(const std::string& address){
      log_subscribers.erase(std::remove(
        log_subscribers.begin(),
        log_subscribers.end(),
        address
      ));
    }
    void Logger::post(const std::string& msg){
        if(! log_subscribers.empty()){
            for(auto &s: log_subscribers){
                connection::post(s,msg,iroha::time::now64());
            }
        }
    }


    enum LoggerStatue{
        LEADER,
        CANDIDATE,
        FOLLOWER
    };

    // To hide spdlog
    void setGlobalLogLevel(const level &l) {
        switch (l) {
            case level::debug: {
                spdlog::set_level(spdlog::level::debug);
                break;
            }
            case level::info: {
                spdlog::set_level(spdlog::level::info);
                break;
            }
            case level::warning: {
                spdlog::set_level(spdlog::level::warn);
                break;
            }
            case level::error: {
                spdlog::set_level(spdlog::level::err);
                break;
            }
        }
    }

}

