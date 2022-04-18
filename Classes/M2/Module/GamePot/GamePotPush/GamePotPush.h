//
//  GamePotPush.h
//  MPBReboot
//
//  Created by 박동제 on 2021/03/05.
//

#pragma once

#include "ace.h"

#include "AceTimeUtils.h"
#include "AceUtils.h"

#include "PushDefine.h"

NS_ACE_BEGIN

class GamePotPush
{
private:
    static std::function<void()> pushSettingsSuccessFunc_;
    static std::function<void()> pushSettingsFailFunc_;
    static std::function<void()> pushesSuccessFunc_;
    static std::function<void(int, const std::string&)> pushesFailFunc_;
    static std::function<void()> localPushSuccessFunc_;
    static std::function<void(int, const std::string&)> localPushFailFunc_;
    static std::function<void()> localPushNightSuccessFunc_;
    static std::function<void(int, const std::string&)> localPushNightFailFunc_;
        
public:
    static void checkPushSettings(const std::function<void()>& successCallFunc, const std::function<void()>& failCallFunc);
    static void linkToSystemSettings();
    
    static void setPush(bool isOn);
    static void setNightPush(bool isOn);
    static void setPushes(bool enablePush, bool enableNightPush, bool enableAd, const std::function<void()>& successCallFunc, const std::function<void(int errorCode, const std::string& errorMsg)>& failCallFunc);
    
    static bool isEnabledPushNotice();
    static bool isEnabledPushNight();
    static bool isEnabledPushAD();
    
    static bool canRegisterLocalPush(const long long& afterDate);
    static void registerLocalPush(const std::string& pushCode, const std::string& title, const std::string& msg, long long afterDate);
    static void unregisterLocalPush(const std::string& pushCode);
    static void unregisterAllLocalPushs();
    
    static void onPushSettingsSuccess();
    static void onPushSettingsFail();
    static void onPushesSuccess();
    static void onPushesFail(int errorCode, const std::string& errorMsg);
    static void onLocalPushSuccess();
    static void onLocalPushFail(int errorCode, const std::string& errorMsg);
    static void onLocalPushNightSuccess();
    static void onLocalPushNightFail(int errorCode, const std::string& errorMsg);
};

NS_ACE_END
