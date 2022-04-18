//
//  FirebaseAuthWrapper_android.cpp
//  MPBreboot
//
//  Created by HYUNSUK AHN on 2020/08/20.
//

#include "GamePotLogin.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include <jni.h>
    #include <platform/android/jni/JniHelper.h>
#endif

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

    string typeName = "NONE";
    if(login::loginTypeToName.count(type) <= 0)
    {
        onLoginFail(0, "잘못된 로그인 접근입니다.");
        return;
    }
    
    typeName = login::loginTypeToName.at(type);

    JniMethodInfo info;

    if (JniHelper::getStaticMethodInfo(info,
                                       "com/aceproject/android/C2sCocos2dxActivity",
                                       "login", "(Ljava/lang/String;)V")) {

        CCLOG("@@@@ find jni method, classId:%d, methodId:%d", info.classID, info.methodID);

        jstring _typeName = info.env->NewStringUTF(typeName.c_str());

        info.env->CallStaticVoidMethod(info.classID, info.methodID, _typeName);

        info.env->DeleteLocalRef(_typeName);
        info.env->DeleteLocalRef(info.classID);
    }
}

void GamePotLogin::autoLogin(const function<void(login::LoginType type, const string& token, const string& memberId)>& successCallFunc, const function<void()>& cancelCallFunc, const function<void(int errorCode, const string& errorMsg)>& failCallFunc, const function<void()>& autoLoginFailCallFunc)
{
    loginSuccessFunc_ = successCallFunc;
    loginCancelFunc_ = cancelCallFunc;
    loginFailFunc_ = failCallFunc;
    autoLoginFailFunc_ = autoLoginFailCallFunc;
    
    JniMethodInfo info;

    if (JniHelper::getStaticMethodInfo(info,
                                       "com/aceproject/android/C2sCocos2dxActivity",
                                       "autoLogin", "()V"))
    {
        CCLOG("@@@@ find jni method, classId:%d, methodId:%d", info.classID, info.methodID);

        info.env->CallStaticVoidMethod(info.classID, info.methodID);
        info.env->DeleteLocalRef(info.classID);
    }
}

void GamePotLogin::logout(const function<void()>& successCallFunc, const function<void(int errorCode, const string& errorMsg)>& failCallFunc)
{
    logoutSuccessFunc_ = successCallFunc;
    logoutFailFunc_ = failCallFunc;

    JniMethodInfo info;

    if (JniHelper::getStaticMethodInfo(info,
                                       "com/aceproject/android/C2sCocos2dxActivity",
                                       "logout", "()V")) {

        CCLOG("@@@@ find jni method, classId:%d, methodId:%d", info.classID, info.methodID);

        info.env->CallStaticVoidMethod(info.classID, info.methodID);
        info.env->DeleteLocalRef(info.classID);
    }
}

void GamePotLogin::linking(login::LoginType type, const function<void(login::LoginType type, const string& token, const string& memberId)>& successCallFunc, const function<void()>& cancelCallFunc, const function<void(int errorCode, const string& errorMsg)>& failCallFunc)
{
    CCLOG("linktest : linkStart");

    linkingSuccessFunc_ = successCallFunc;
    linkingCancelFunc_ = cancelCallFunc;
    linkingFailFunc_ = failCallFunc;
    loginType_ = type;

    string typeName = "NONE";
    if(login::loginTypeToName.count(type) <= 0)
    {
        onLinkingFail(0, "잘못된 계정 연동 접근입니다.");
        return;
    }
    
    typeName = login::loginTypeToName.at(type);

    JniMethodInfo info;

    if (JniHelper::getStaticMethodInfo(info,
                                       "com/aceproject/android/C2sCocos2dxActivity",
                                       "linking", "(Ljava/lang/String;)V")) {

        CCLOG("@@@@ find jni method, classId:%d, methodId:%d", info.classID, info.methodID);

        jstring _typeName = info.env->NewStringUTF(typeName.c_str());

        info.env->CallStaticVoidMethod(info.classID, info.methodID, _typeName);

        info.env->DeleteLocalRef(_typeName);
        info.env->DeleteLocalRef(info.classID);
    }
}

void GamePotLogin::unlinking(login::LoginType type, const function<void()>& successCallFunc, const function<void(int errorCode, const string& errorMsg)>& failCallFunc)
{
    unlinkingSuccessFunc_ = successCallFunc;
    unlinkingFailFunc_ = failCallFunc;
    
    string typeName = "NONE";
    if(login::loginTypeToName.count(type) <= 0)
    {
        onLinkingFail(0, "잘못된 계정 연동 해제 접근입니다.");
        return;
    }
    
    typeName = login::loginTypeToName.at(type);

    JniMethodInfo info;

    if (JniHelper::getStaticMethodInfo(info,
                                       "com/aceproject/android/C2sCocos2dxActivity",
                                       "unlinking", "(Ljava/lang/String;)V")) {

        CCLOG("@@@@ find jni method, classId:%d, methodId:%d", info.classID, info.methodID);

        jstring _typeName = info.env->NewStringUTF(typeName.c_str());

        info.env->CallStaticVoidMethod(info.classID, info.methodID, _typeName);

        info.env->DeleteLocalRef(_typeName);
        info.env->DeleteLocalRef(info.classID);
    }
}

login::LoginType GamePotLogin::getLastLoginType()
{
    std::string strLoginType;

    JniMethodInfo info;
 
    if (JniHelper::getStaticMethodInfo(info,
                                       "com/aceproject/android/C2sCocos2dxActivity",
                                       "getLastLoginType", "()Ljava/lang/String;"))

    {
        CCLOG("@@@@ find jni method, classId:%d, methodId:%d", info.classID, info.methodID);
 
        jboolean isCopy;
        jstring jloginType = (jstring)info.env->CallStaticObjectMethod(info.classID, info.methodID);
        strLoginType = info.env->GetStringUTFChars(jloginType, &isCopy);
 
        info.env->DeleteLocalRef(info.classID);
                
        if(login::loginNameToType.count(strLoginType) <= 0)
        {
            return login::LoginType::NONE;
        }
        
        return login::loginNameToType.at(strLoginType);
    }
    
    return login::LoginType::NONE;
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
        
    // 애플 로그인시 애플 아이디를 9up의 계정에서 사용하시겠습니까? 에서 취소를 누른 경우 실패가 발생하는데 바로 로그인 취소로 넘어가도록 수정 (iOS는 onLoginCancel로 넘어옴.. ㅋㅋ)
    if (errorCode == 2)
    {
        onLoginCancel();
        return;
    }
    
    // 네이버 로그인 개인정보 제공거절 취소의 경우 바로 로그인 취소로 넘어가도록 수정 (iOS는 onLoginCancel로 넘어옴.. ㅋㅋ)
    if (errorCode == 2003)
    {
        onLoginCancel();
        return;
    }
    
    // 그 외의 실패인 경우
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
    JniMethodInfo info;

    if (JniHelper::getStaticMethodInfo(info,
                                       "com/aceproject/android/C2sCocos2dxActivity",
                                       "showHelpWebView", "()V")) {

        CCLOG("@@@@ find jni method, classId:%d, methodId:%d", info.classID, info.methodID);

        info.env->CallStaticVoidMethod(info.classID, info.methodID);
        info.env->DeleteLocalRef(info.classID);
    }
}
