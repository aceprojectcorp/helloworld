//
//  GamePotServiceAgreement_android.cpp
//  MPBReboot
//
//  Created by 박동제 on 2021/02/25.
//

#include "GamePotServiceAgreement.h"
#include "GamePotPush.h"

#include <jni.h>
#include <platform/android/jni/JniHelper.h>

#include "AceUserDefault.h"

USING_NS_ACE
USING_NS_ACE_USER_DEFAULT
using namespace std;

function<void(bool isAgree, bool isEnabledPush, bool isEnabledNightPush)> GamePotServiceAgreement::agreeSuccessFunc_ = nullptr;
function<void(int errorCode, const string& errorMsg)> GamePotServiceAgreement::agreeFailFunc_ = nullptr;

void GamePotServiceAgreement::initAgreement()
{
    CCLOG("agreement init");
    
    cocos2d::JniMethodInfo info;
    if (cocos2d::JniHelper::getStaticMethodInfo(info,
                                       "com/aceproject/android/C2sCocos2dxActivity",
                                       "initAgreement", "()V"))
    {
        CCLOG("@@@@ find jni method, classId:%d, methodId:%d", info.classID, info.methodID);

        info.env->CallStaticVoidMethod(info.classID, info.methodID);
        info.env->DeleteLocalRef(info.classID);
    }
}

void GamePotServiceAgreement::setAutoAgreement(bool isAuto)
{
    CCLOG("auto agreement state: %d", isAuto);
    
    cocos2d::JniMethodInfo info;
    if (cocos2d::JniHelper::getStaticMethodInfo(info,
                                       "com/aceproject/android/C2sCocos2dxActivity",
                                       "setAutoAgreement", "(Z)V"))
    {
        CCLOG("@@@@ find jni method, classId:%d, methodId:%d", info.classID, info.methodID, isAuto);

        info.env->CallStaticVoidMethod(info.classID, info.methodID);
        info.env->DeleteLocalRef(info.classID);
    }
}

void GamePotServiceAgreement::showAgreementPopup(const function<void(bool isAgree, bool isEnabledPush, bool isEnabledNightPush)>& successCallFunc, const function<void(int errorCode, const string& errorMsg)>& failCallFunc)
{
    CCLOG("ShowAgreePopup");
    
    agreeSuccessFunc_ = successCallFunc;
    agreeFailFunc_ = failCallFunc;
    
    cocos2d::JniMethodInfo info;
    if (cocos2d::JniHelper::getStaticMethodInfo(info,
                                       "com/aceproject/android/C2sCocos2dxActivity",
                                       "showAgreementPopup", "()V"))
    {
        CCLOG("@@@@ find jni method, classId:%d, methodId:%d", info.classID, info.methodID);

        info.env->CallStaticVoidMethod(info.classID, info.methodID);
        info.env->DeleteLocalRef(info.classID);
    }
}

void GamePotServiceAgreement::showTerms()
{
    cocos2d::JniMethodInfo info;
    if (cocos2d::JniHelper::getStaticMethodInfo(info,
                                       "com/aceproject/android/C2sCocos2dxActivity",
                                       "showTerms", "()V"))
    {
        CCLOG("@@@@ find jni method, classId:%d, methodId:%d", info.classID, info.methodID);

        info.env->CallStaticVoidMethod(info.classID, info.methodID);
        info.env->DeleteLocalRef(info.classID);
    }
}

void GamePotServiceAgreement::showPrivacy()
{
    cocos2d::JniMethodInfo info;
    if (cocos2d::JniHelper::getStaticMethodInfo(info,
                                       "com/aceproject/android/C2sCocos2dxActivity",
                                       "showPrivacy", "()V"))
    {
        CCLOG("@@@@ find jni method, classId:%d, methodId:%d", info.classID, info.methodID);

        info.env->CallStaticVoidMethod(info.classID, info.methodID);
        info.env->DeleteLocalRef(info.classID);
    }
}

void GamePotServiceAgreement::showPolicy()
{
    cocos2d::JniMethodInfo info;
    if (cocos2d::JniHelper::getStaticMethodInfo(info,
                                       "com/aceproject/android/C2sCocos2dxActivity",
                                       "showWebView", "(Ljava/lang/String;)V"))
    {
        CCLOG("@@@@ find jni method, classId:%d, methodId:%d", info.classID, info.methodID);
        
        jstring arg1 = info.env->NewStringUTF(POLICY_URL);

        info.env->CallStaticVoidMethod(info.classID, info.methodID, arg1);
        
        info.env->DeleteLocalRef(arg1);
        
        info.env->DeleteLocalRef(info.classID);
    }
}

void GamePotServiceAgreement::showProb()
{
    cocos2d::JniMethodInfo info;
    if (cocos2d::JniHelper::getStaticMethodInfo(info,
                                       "com/aceproject/android/C2sCocos2dxActivity",
                                       "showWebView", "(Ljava/lang/String;)V"))
    {
        CCLOG("@@@@ find jni method, classId:%d, methodId:%d", info.classID, info.methodID);
        
        jstring arg1 = info.env->NewStringUTF(PROB_URL);

        info.env->CallStaticVoidMethod(info.classID, info.methodID, arg1);
        
        info.env->DeleteLocalRef(arg1);
        
        info.env->DeleteLocalRef(info.classID);
    }
}

void GamePotServiceAgreement::showRefund()
{
    cocos2d::JniMethodInfo info;
    if (cocos2d::JniHelper::getStaticMethodInfo(info,
                                       "com/aceproject/android/C2sCocos2dxActivity",
                                       "showRefund", "()V"))
    {
        CCLOG("@@@@ find jni method, classId:%d, methodId:%d", info.classID, info.methodID);

        info.env->CallStaticVoidMethod(info.classID, info.methodID);
        info.env->DeleteLocalRef(info.classID);
    }
}

void GamePotServiceAgreement::showLiscence()
{
    cocos2d::JniMethodInfo info;
    if (cocos2d::JniHelper::getStaticMethodInfo(info,
                                       "com/aceproject/android/C2sCocos2dxActivity",
                                       "showWebView", "(Ljava/lang/String;)V"))
    {
        CCLOG("@@@@ find jni method, classId:%d, methodId:%d", info.classID, info.methodID);
        
        jstring arg1 = info.env->NewStringUTF(LISCENCE_URL);

        info.env->CallStaticVoidMethod(info.classID, info.methodID, arg1);
        
        info.env->DeleteLocalRef(arg1);
        
        info.env->DeleteLocalRef(info.classID);
    }
}

void GamePotServiceAgreement::showURL(const char* url)
{
    cocos2d::JniMethodInfo info;
    if (cocos2d::JniHelper::getStaticMethodInfo(info,
                                       "com/aceproject/android/C2sCocos2dxActivity",
                                       "showWebView", "(Ljava/lang/String;)V"))
    {
        CCLOG("@@@@ find jni method, classId:%d, methodId:%d", info.classID, info.methodID);
        
        jstring arg1 = info.env->NewStringUTF(url);

        info.env->CallStaticVoidMethod(info.classID, info.methodID, arg1);
        
        info.env->DeleteLocalRef(arg1);
        
        info.env->DeleteLocalRef(info.classID);
    }
}

void GamePotServiceAgreement::onAgreeSuccess(bool isAgree, bool isEnabledPush, bool isEnabledNightPush)
{
    if(nullptr != agreeSuccessFunc_)
    {
        agreeSuccessFunc_(isAgree, isEnabledPush, isEnabledNightPush);
    }
}

void GamePotServiceAgreement::onAgreeFail(int errorCode, const std::string& errorMsg)
{
    CCLOG("errorCode: %d, errorMsg: %s", errorCode, errorMsg.c_str());
    
    if(nullptr != agreeFailFunc_)
    {
        agreeFailFunc_(errorCode, errorMsg);
    }
}

