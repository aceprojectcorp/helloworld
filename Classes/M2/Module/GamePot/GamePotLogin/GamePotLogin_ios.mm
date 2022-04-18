//
//  AuthWrapper_ios.m
//  MPBReboot
//
//  Created by HYUNSUK AHN on 2020/08/20.
//

#import <GamePotChannel/GamePotChannel.h>
#import "AppController.h"

#include "GamePotLogin.h"

#include "JsonUtils.h"

// 로그인 타입 정의
// GamePotChannelType.GOOGLE
// GamePotChannelType.FACEBOOK
// GamePotChannelType.GUEST
// GamePotChannelType.LINE
// GamePotChannelType.TWITTER
// GamePotChannelType.NAVER
// GamePotChannelType.APPLE

USING_NS_ACE
USING_NS_CC;
using namespace std;

function<void(login::LoginType, const string&, const string&)> GamePotLogin::loginSuccessFunc_ = nullptr;
function<void()> GamePotLogin::loginCancelFunc_ = nullptr;
function<void(int, const string&)> GamePotLogin::loginFailFunc_ = nullptr;
function<void()> GamePotLogin::autoLoginFailFunc_ = nullptr;
function<void()> GamePotLogin::logoutSuccessFunc_ = nullptr;
function<void(int, const string&)> GamePotLogin::logoutFailFunc_ = nullptr;
function<void(login::LoginType, const string&, const string&)> GamePotLogin::linkingSuccessFunc_ = nullptr;
function<void()> GamePotLogin::linkingCancelFunc_ = nullptr;
function<void(int, const string&)> GamePotLogin::linkingFailFunc_ = nullptr;
function<void()> GamePotLogin::unlinkingSuccessFunc_ = nullptr;
function<void(int, const string&)> GamePotLogin::unlinkingFailFunc_ = nullptr;

login::LoginType GamePotLogin::loginType_ = login::LoginType::NONE;

void GamePotLogin::login(login::LoginType type, const function<void(login::LoginType type, const string& token, const string& memberId)>& successCallFunc, const function<void()>& cancelCallFunc, const function<void(int errorCode, const string& errorMsg)>& failCallFunc)
{
    loginSuccessFunc_ = successCallFunc;
    loginCancelFunc_ = cancelCallFunc;
    loginFailFunc_ = failCallFunc;
    loginType_ = type;
    
    AppController* appDelegate = (AppController*)[[UIApplication sharedApplication] delegate];
    
    // type에 따른 로그인 시도
    [[GamePotChannel getInstance] Login:(GamePotChannelType)type viewController:(UIViewController*)[appDelegate viewController] success:^(GamePotUserInfo* userInfo) {
        // 로그인 완료
        onLoginSuccess([[userInfo token] UTF8String], [[userInfo memberid] UTF8String]);
    } cancel:^{
        onLoginCancel();
    } fail:^(NSError *error) {
        onLoginFail((int)[error code], [[error localizedDescription] UTF8String]);
    }];
}

void GamePotLogin::autoLogin(const function<void(login::LoginType type, const string& token, const string& memberId)>& successCallFunc, const function<void()>& cancelCallFunc, const function<void(int errorCode, const string& errorMsg)>& failCallFunc, const function<void()>& autoLoginFailCallFunc)
{
    loginSuccessFunc_ = successCallFunc;
    loginCancelFunc_ = cancelCallFunc;
    loginFailFunc_ = failCallFunc;
    autoLoginFailFunc_ = autoLoginFailCallFunc;

    // 마지막 로그인된 정보를 가져와서 해당 정보로 자동 로그인
    // lastLoginType: 마지막 로그인 값을 가져올 수 있습니다.
    GamePotChannelType type = [[GamePotChannel getInstance] lastLoginType];
    if(type == NONE)
    {
        // 마지막 로그인된 정보가 없음. 로그인 버튼이 있는 로그인 화면으로 이동
        onAutoLoginFail();
        return;
    }
    
    // 마지막에 로그인했던 로그인 타입으로 로그인
    login((login::LoginType)type, loginSuccessFunc_, loginCancelFunc_, loginFailFunc_);
}

void GamePotLogin::logout(const function<void()>& successCallFunc, const function<void(int errorCode, const string& errorMsg)>& failCallFunc)
{
    logoutSuccessFunc_ = successCallFunc;
    logoutFailFunc_ = failCallFunc;
    
    [[GamePotChannel getInstance] LogoutWithSuccess:^{
        onLogoutSuccess();
    } fail:^(NSError *error) {
        onLogoutFail((int)[error code], [[error localizedDescription] UTF8String]);
    }];
}

void GamePotLogin::linking(login::LoginType type, const function<void(login::LoginType type, const string& token, const string& memberId)>& successCallFunc, const function<void()>& cancelCallFunc, const function<void(int errorCode, const string& errorMsg)>& failCallFunc)
{
    linkingSuccessFunc_ = successCallFunc;
    linkingCancelFunc_ = cancelCallFunc;
    linkingFailFunc_ = failCallFunc;
    loginType_ = type;
    
    AppController* appDelegate = (AppController*)[[UIApplication sharedApplication] delegate];
    
    [[GamePotChannel getInstance] CreateLinking:(GamePotChannelType)type viewController:(UIViewController*)[appDelegate viewController] success:^(GamePotUserInfo *userInfo) {
        onLinkingSuccess([[userInfo token] UTF8String], [[userInfo memberid] UTF8String]);
    } cancel:^{
        onLinkingCancel();
    } fail:^(NSError *error) {
        onLinkingFail((int)[error code], [[error localizedDescription] UTF8String]);
    }];
}

void GamePotLogin::unlinking(login::LoginType type, const function<void()>& successCallFunc, const function<void(int errorCode, const string& errorMsg)>& failCallFunc)
{
    unlinkingSuccessFunc_ = successCallFunc;
    unlinkingFailFunc_ = failCallFunc;
    
    [[GamePotChannel getInstance] DeleteLinking:(GamePotChannelType)type success:^{
        onUnlinkingSuccess();
    } fail:^(NSError *error) {
        onUnlinkingFail((int)[error code], [[error localizedDescription] UTF8String]);
    }];
}


login::LoginType GamePotLogin::getLastLoginType()
{
    GamePotChannelType type = [[GamePotChannel getInstance] lastLoginType];
    
    return (login::LoginType)type;
}

std::string GamePotLogin::getLoginTypeStr()
{
    auto iter = login::loginTypeToName.find(loginType_);
    if(iter == login::loginTypeToName.end())
        return "";
    
    return iter->second;
}

void GamePotLogin::onLoginSuccess(const string& token, const string& memberId)
{
    // 로그인 성공
    CCLOG("Login Success - token: %s - memberid: %s", token.c_str(), memberId.c_str());
    loginType_ = getLastLoginType();
    
    if(nullptr != loginSuccessFunc_)
    {
        loginSuccessFunc_(loginType_, token, memberId);
    }
}

void GamePotLogin::onLoginCancel()
{
    // 로그인 취소
    CCLOG("Login Cancel");
    loginType_ = login::LoginType::NONE;
    
    if(nullptr != loginCancelFunc_)
    {
        loginCancelFunc_();
    }
}

void GamePotLogin::onLoginFail(int errorCode, const string& errorMsg)
{
    // 로그인 중 오류 발생
    CCLOG("Login Failed errorCode : %d", errorCode);
    CCLOG("Login Failed errorMsg : %s", errorMsg.c_str());
    loginType_ = login::LoginType::NONE;
    
    if(nullptr != loginFailFunc_)
    {
        loginFailFunc_(errorCode, errorMsg);
    }
}

void GamePotLogin::onAutoLoginFail()
{
    CCLOG("AutoLogin Failed");
    loginType_ = login::LoginType::NONE;
    
    if(nullptr != autoLoginFailFunc_)
    {
        autoLoginFailFunc_();
    }
}

void GamePotLogin::onLogoutSuccess()
{
    CCLOG("Logout Success");
    loginType_ = login::LoginType::NONE;
    
    if(nullptr != logoutSuccessFunc_)
    {
        logoutSuccessFunc_();
    }
}

void GamePotLogin::onLogoutFail(int errorCode, const string& errorMsg)
{
    CCLOG("Logout Failed errorCode : %d", errorCode);
    CCLOG("Logout Failed errorMsg : %s", errorMsg.c_str());
    
    if(nullptr != logoutFailFunc_)
    {
        logoutFailFunc_(errorCode, errorMsg);
    }
}

void GamePotLogin::onLinkingSuccess(const string& token, const string& memberId)
{
    CCLOG("Linking Success - token: %s - memberid: %s", token.c_str(), memberId.c_str());
    
    if(nullptr != linkingSuccessFunc_)
    {
        linkingSuccessFunc_(loginType_, token, memberId);
    }
}

void GamePotLogin::onLinkingCancel()
{
    CCLOG("Linking Cancel");
    loginType_ = login::LoginType::GUEST;
    
    if(nullptr != linkingCancelFunc_)
    {
        linkingCancelFunc_();
    }
}

void GamePotLogin::onLinkingFail(int errorCode, const string& errorMsg)
{
    CCLOG("Linking Failed errorCode : %d", errorCode);
    CCLOG("Linking Failed errorMsg : %s", errorMsg.c_str());
    loginType_ = login::LoginType::GUEST;
    
    if(nullptr != linkingFailFunc_)
    {
        linkingFailFunc_(errorCode, errorMsg);
    }
}

void GamePotLogin::onUnlinkingSuccess()
{
    CCLOG("Unlinking Success");
    loginType_ = login::LoginType::GUEST;
    
    if(nullptr != unlinkingSuccessFunc_)
    {
        unlinkingSuccessFunc_();
    }
}

void GamePotLogin::onUnlinkingFail(int errorCode, const string& errorMsg)
{
    CCLOG("Unlinking Failed errorCode : %d", errorCode);
    CCLOG("Unlinking Failed errorMsg : %s", errorMsg.c_str());
    
    if(nullptr != unlinkingFailFunc_)
    {
        unlinkingFailFunc_(errorCode, errorMsg);
    }
}

void GamePotLogin::showHelpWebView()
{
    AppController* appDelegate = (AppController*)[[UIApplication sharedApplication] delegate];
    
    [[GamePot getInstance] showHelpWebView:(UIViewController*)[appDelegate viewController]];
}
