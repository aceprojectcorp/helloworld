//
//  AuthWrapper.h
//  MPBReboot
//
//  Created by HYUNSUK AHN on 2020/08/20.
//

#pragma once

#include "ace.h"

#include "LoginDefine.h"

NS_ACE_BEGIN

class GamePotLogin
{
private:
    static std::function<void(login::LoginType, const std::string&, const std::string&)> loginSuccessFunc_;
    static std::function<void()> loginCancelFunc_;
    static std::function<void(int, const std::string&)> loginFailFunc_;
    static std::function<void()> autoLoginFailFunc_;
    static std::function<void()> logoutSuccessFunc_;
    static std::function<void(int, const std::string&)> logoutFailFunc_;
    static std::function<void(login::LoginType, const std::string&, const std::string&)> linkingSuccessFunc_;
    static std::function<void()> linkingCancelFunc_;
    static std::function<void(int, const std::string&)> linkingFailFunc_;
    static std::function<void()> unlinkingSuccessFunc_;
    static std::function<void(int, const std::string&)> unlinkingFailFunc_;
    
    static login::LoginType loginType_;
    
public:
    static void login(login::LoginType type, const std::function<void(login::LoginType type, const std::string& token, const std::string& memberId)>& successCallFunc, const std::function<void()>& cancelCallFunc, const std::function<void(int errorCode, const std::string& errorMsg)>& failCallFunc);
    static void autoLogin(const std::function<void(login::LoginType type, const std::string& token, const std::string& memberId)>& successCallFunc, const std::function<void()>& cancelCallFunc, const std::function<void(int errorCode, const std::string& errorMsg)>& failCallFunc, const std::function<void()>& autoLoginFailCallFunc);
    static void logout(const std::function<void()>& successCallFunc, const std::function<void(int errorCode, const std::string& errorMsg)>& failCallFunc);
    
public:
    static void linking(login::LoginType type, const std::function<void(login::LoginType type, const std::string& token, const std::string& memberId)>& successCallFunc, const std::function<void()>& cancelCallFunc, const std::function<void(int errorCode, const std::string& errorMsg)>& failCallFunc);
    static void unlinking(login::LoginType type, const std::function<void()>& successCallFunc, const std::function<void(int errorCode, const std::string& errorMsg)>& failCallFunc);
    
public:
    static login::LoginType getLastLoginType();
    
public:
    static login::LoginType getLoginType() { return loginType_; }
    static std::string getLoginTypeStr();
    
public:
    static void onLoginSuccess(const std::string& token, const std::string& memberId);
    static void onLoginCancel();
    static void onLoginFail(int errorCode, const std::string& errorMsg);
    static void onAutoLoginFail();
    static void onLogoutSuccess();
    static void onLogoutFail(int errorCode, const std::string& errorMsg);
    static void onLinkingSuccess(const std::string& token, const std::string& memberId);
    static void onLinkingCancel();
    static void onLinkingFail(int errorCode, const std::string& errorMsg);
    static void onUnlinkingSuccess();
    static void onUnlinkingFail(int errorCode, const std::string& errorMsg);

public:
    //고객센터
    static void showHelpWebView();
};

NS_ACE_END
