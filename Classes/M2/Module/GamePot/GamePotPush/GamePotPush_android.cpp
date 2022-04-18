//
//  GamePotPush_android.cpp
//  MPBReboot
//
//  Created by 박동제 on 2021/03/05.
//

#include "GamePotPush.h"

#include "AceUserDefault.h"
#include "GameContext.h"
#include "TimeDefine.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include <jni.h>
    #include <platform/android/jni/JniHelper.h>
#endif

USING_NS_ACE
USING_NS_CC;
using namespace std;

function<void()> GamePotPush::pushSettingsSuccessFunc_ = nullptr;
function<void()> GamePotPush::pushSettingsFailFunc_ = nullptr;
function<void()> GamePotPush::pushesSuccessFunc_ = nullptr;
function<void(int errorCode, const string& errorMsg)> GamePotPush::pushesFailFunc_ = nullptr;
function<void()> GamePotPush::localPushSuccessFunc_ = nullptr;
function<void(int errorCode, const string& errorMsg)>GamePotPush::localPushFailFunc_ = nullptr;
function<void()> GamePotPush::localPushNightSuccessFunc_ = nullptr;
function<void(int errorCode, const string& errorMsg)> GamePotPush::localPushNightFailFunc_ = nullptr;

void GamePotPush::checkPushSettings(const function<void()>& successCallFunc, const function<void()>& failCallFunc)
{
    pushSettingsSuccessFunc_ = successCallFunc;
    pushSettingsFailFunc_ = failCallFunc;
    
    JniMethodInfo info;
    if (JniHelper::getStaticMethodInfo(info,
                                       "com/aceproject/android/C2sCocos2dxActivity",
                                       "checkPushSettings", "()V")) {
        info.env->CallStaticVoidMethod(info.classID, info.methodID);
        info.env->DeleteLocalRef(info.classID);
    }
}

void GamePotPush::linkToSystemSettings()
{
    JniMethodInfo info;
    if (JniHelper::getStaticMethodInfo(info,
                                       "com/aceproject/android/C2sCocos2dxActivity",
                                       "linkToSystemSettings", "()V")) {
        info.env->CallStaticVoidMethod(info.classID, info.methodID);
        info.env->DeleteLocalRef(info.classID);
    }
}

void GamePotPush::setPush(bool isOn)
{
    JniMethodInfo info;
    if (JniHelper::getStaticMethodInfo(info,
                                       "com/aceproject/android/C2sCocos2dxActivity",
                                       "setPush", "(Z)V")) {
        CCLOG("@@@@ find jni method, classId:%d, methodId:%d", info.classID, info.methodID);
        
        info.env->CallStaticVoidMethod(info.classID, info.methodID, isOn);
        info.env->DeleteLocalRef(info.classID);
    }
}

void GamePotPush::setNightPush(bool isOn)
{
    JniMethodInfo info;
    if (JniHelper::getStaticMethodInfo(info,
                                       "com/aceproject/android/C2sCocos2dxActivity",
                                       "setNightPush", "(Z)V")) {
        CCLOG("@@@@ find jni method, classId:%d, methodId:%d", info.classID, info.methodID);
        
        info.env->CallStaticVoidMethod(info.classID, info.methodID, isOn);
        info.env->DeleteLocalRef(info.classID);
    }
}

void GamePotPush::setPushes(bool enablePush, bool enableNightPush, bool enableAd, const function<void()>& successCallFunc, const function<void(int errorCode, const string& errorMsg)>& failCallFunc)
{
    pushesSuccessFunc_ = successCallFunc;
    pushesFailFunc_ = failCallFunc;

    JniMethodInfo info;
    if (JniHelper::getStaticMethodInfo(info,
                                       "com/aceproject/android/C2sCocos2dxActivity",
                                       "setPushes", "(ZZZ)V")) {
        CCLOG("@@@@ find jni method, classId:%d, methodId:%d", info.classID, info.methodID);

        info.env->CallStaticVoidMethod(info.classID, info.methodID, enablePush, enableNightPush, enableAd);
        info.env->DeleteLocalRef(info.classID);
    }
}

bool GamePotPush::isEnabledPushNotice()
{
    bool isEnabledPush = false;

    JniMethodInfo info;
    if (JniHelper::getStaticMethodInfo(info,
                                       "com/aceproject/android/C2sCocos2dxActivity",
                                       "isEnabledPushNotice", "()Z")) {
        CCLOG("@@@@ find jni method, classId:%d, methodId:%d", info.classID, info.methodID);

        isEnabledPush = info.env->CallStaticIntMethod(info.classID, info.methodID);
        info.env->DeleteLocalRef(info.classID);
    }

    return isEnabledPush;
}

bool GamePotPush::isEnabledPushNight()
{
    bool isEnabledPushNight = false;

    JniMethodInfo info;
    if (JniHelper::getStaticMethodInfo(info,
                                       "com/aceproject/android/C2sCocos2dxActivity",
                                       "isEnabledPushNight", "()Z")) {
        CCLOG("@@@@ find jni method, classId:%d, methodId:%d", info.classID, info.methodID);

        isEnabledPushNight = info.env->CallStaticIntMethod(info.classID, info.methodID);
        info.env->DeleteLocalRef(info.classID);
    }

    return isEnabledPushNight;
}

bool GamePotPush::isEnabledPushAD()
{
    bool isEnabledPushAD = false;

    JniMethodInfo info;
    if (JniHelper::getStaticMethodInfo(info,
                                       "com/aceproject/android/C2sCocos2dxActivity",
                                       "isEnabledPushAD", "()Z")) {
        CCLOG("@@@@ find jni method, classId:%d, methodId:%d", info.classID, info.methodID);

        isEnabledPushAD = info.env->CallStaticIntMethod(info.classID, info.methodID);
        info.env->DeleteLocalRef(info.classID);
    }

    return isEnabledPushAD;
}

bool GamePotPush::canRegisterLocalPush(const long long& afterDate)
{
    tm time = AceTimeUtils::convertUTCtoKSTTM(afterDate);
    if((time.tm_hour > 21 || time.tm_hour < 8) && isEnabledPushNight())
        return true;
    else if((time.tm_hour <= 21 && time.tm_hour >= 8) && isEnabledPushNotice())
        return true;
    
    return false;
}

void GamePotPush::registerLocalPush(const std::string& pushCode, const std::string& title, const std::string& msg, long long afterDate)
{
    // 등록하려는 로컬 푸시가 설정 푸시에서 켜져있는지 확인
    if(false == canRegisterLocalPush(afterDate))
        return;
    
    // 과거 시간으로 로컬 푸시 등록 시 반환
    long afterSec = (long)((afterDate - GameContext::getInstance()->getTime()) / g_msSecond);
    if(afterSec <= 0)
    {
        CCLOG("[GamePotPush] Failed register local push. (past time)");
        return;
    }
    
    // 이미 등록된 같은타입의 로컬푸시가 있다면, 이미 등록된 로컬푸시부터 취소하자.
    if (AceUserDefault::getInstance()->getIntegerForKey(pushCode.c_str()) != 0)
    {
        unregisterLocalPush(pushCode.c_str());
    }
    
    // 게임팟 모듈에 요청
    JniMethodInfo info;
    if (JniHelper::getStaticMethodInfo(info,
                                       "com/aceproject/android/C2sCocos2dxActivity",
                                       "registerLocalPush", "(Ljava/lang/String;Ljava/lang/String;I)I")) {
        CCLOG("@@@@ find jni method, classId:%d, methodId:%d", info.classID, info.methodID);
        
        jstring arg1 = info.env->NewStringUTF(title.c_str());
        jstring arg2 = info.env->NewStringUTF(msg.c_str());
        
        jint pushId = info.env->CallStaticIntMethod(info.classID, info.methodID, arg1, arg2, afterSec);
        
        info.env->DeleteLocalRef(arg1);
        info.env->DeleteLocalRef(arg2);
        
        info.env->DeleteLocalRef(info.classID);
        
        // 로컬 푸시 게임팟에 등록한 뒤, 얻은 pushId를 저장.
        AceUserDefault::getInstance()->setIntegerForKey(pushCode.c_str(), (int)pushId);
    }
}

void GamePotPush::unregisterLocalPush(const std::string& pushCode)
{
    // 게임팟 모듈에 요청
    JniMethodInfo info;
    if (JniHelper::getStaticMethodInfo(info,
                                       "com/aceproject/android/C2sCocos2dxActivity",
                                       "unregisterLocalPush", "(I)V")) {
        CCLOG("@@@@ find jni method, classId:%d, methodId:%d", info.classID, info.methodID);
        
        info.env->CallStaticVoidMethod(info.classID, info.methodID, AceUserDefault::getInstance()->getIntegerForKey(pushCode.c_str()));
        info.env->DeleteLocalRef(info.classID);
    }
    
    // 저장된 pushId 제거
    AceUserDefault::getInstance()->removeStringForKey(pushCode.c_str());

}

void GamePotPush::unregisterAllLocalPushs()
{
    unregisterLocalPush(push::local_code::PLAYBALL_CHARGED);
}

void GamePotPush::onPushSettingsSuccess()
{
    if(nullptr != pushSettingsSuccessFunc_)
    {
        pushSettingsSuccessFunc_();
    }
}

void GamePotPush::onPushSettingsFail()
{
    if(nullptr != pushSettingsFailFunc_)
    {
        pushSettingsFailFunc_();
    }
}

void GamePotPush::onPushesSuccess()
{
    if(nullptr != pushesSuccessFunc_)
    {
        pushesSuccessFunc_();
    }
}

void GamePotPush::onPushesFail(int errorCode, const std::string& errorMsg)
{
    if(nullptr != pushesFailFunc_)
    {
        pushesFailFunc_(errorCode, errorMsg);
    }
}

void GamePotPush::onLocalPushSuccess()
{
}

void GamePotPush::onLocalPushFail(int errorCode, const std::string& errorMsg)
{
    if(nullptr != localPushFailFunc_)
    {
        localPushFailFunc_(errorCode, errorMsg);
    }
}

void GamePotPush::onLocalPushNightSuccess()
{
}

void GamePotPush::onLocalPushNightFail(int errorCode, const std::string& errorMsg)
{
    if(nullptr != localPushNightFailFunc_)
    {
        localPushNightFailFunc_(errorCode, errorMsg);
    }
}
