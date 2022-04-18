//
//  GamePotPush_ios.mm
//  MPBReboot
//
//  Created by 박동제 on 2021/03/05.
//

#import <GamePot/GamePot.h>

#include "GamePotPush.h"

#include "AceUserDefault.h"
#include "GameContext.h"
#include "TimeDefine.h"

USING_NS_ACE
USING_NS_CC;
using namespace std;

function<void()> GamePotPush::pushSettingsSuccessFunc_ = nullptr;
function<void()> GamePotPush::pushSettingsFailFunc_ = nullptr;
function<void()> GamePotPush::pushesSuccessFunc_ = nullptr;
function<void(int, const string&)> GamePotPush::pushesFailFunc_ = nullptr;
function<void()> GamePotPush::localPushSuccessFunc_ = nullptr;
function<void(int, const string&)>GamePotPush::localPushFailFunc_ = nullptr;
function<void()> GamePotPush::localPushNightSuccessFunc_ = nullptr;
function<void(int, const string&)> GamePotPush::localPushNightFailFunc_ = nullptr;

void GamePotPush::checkPushSettings(const function<void()>& successCallFunc, const function<void()>& failCallFunc)
{
    pushSettingsSuccessFunc_ = successCallFunc;
    pushSettingsFailFunc_ = failCallFunc;
    
    // iOS 10 이상 UNUserNotificationCenter 사용
    if(@available(iOS 10.0, *))
    {
        [[UNUserNotificationCenter currentNotificationCenter] getNotificationSettingsWithCompletionHandler:^(UNNotificationSettings * _Nonnull settings) {
            // 시스템 알림이 켜져있고, 배너나 알림 센터가 켜져있을 경우 성공
            dispatch_async(dispatch_get_main_queue(), ^{
                if([settings authorizationStatus] != UNAuthorizationStatusDenied &&
                   [settings alertStyle] != UNAlertStyleNone)
                {
                    onPushSettingsSuccess();
                }
                else
                {
                    onPushSettingsFail();
                }
            });
        }];
        
    }
    // iOS 9 이하 UILocalNotification 사용
    else
    {
        UIRemoteNotificationType notificationType = [[UIApplication sharedApplication] enabledRemoteNotificationTypes];
        // 시스템 알림이 켜져있고, 알림 센터가 켜져있을 경우 성공
        if((notificationType & UIRemoteNotificationTypeNone) == NO &&
           (notificationType & UIRemoteNotificationTypeAlert) == NO)
        {
            onPushSettingsSuccess();
        }
        else
        {
            onPushSettingsFail();
        }
    }
}

void GamePotPush::linkToSystemSettings()
{
    // 시스템 설정으로 이동
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:UIApplicationOpenSettingsURLString]];
}

void GamePotPush::setPush(bool isOn)
{
    // 푸시 수신 On/Off
    [[GamePot getInstance] setPushEnable:isOn success:^{
        CCLOG("Push Setting Success");
        onLocalPushSuccess();
    } fail:^(NSError *error) {
        CCLOG("Push Setting Failed");
        onLocalPushFail((int)[error code], [[error localizedDescription] UTF8String]);
    }];
}

void GamePotPush::setNightPush(bool isOn)
{
    // 푸시 수신 On/Off
    [[GamePot getInstance] setNightPushEnable:isOn success:^{
        CCLOG("Night Push Setting Success");
        onLocalPushNightSuccess();
    } fail:^(NSError *error) {
        CCLOG("Night Push Setting Failed");
        onLocalPushNightFail((int)[error code], [[error localizedDescription] UTF8String]);
    }];
}

void GamePotPush::setPushes(bool enablePush, bool enableNightPush, bool enableAd, const function<void()>& successCallFunc, const function<void(int errorCode, const string& errorMsg)>& failCallFunc)
{
    pushesSuccessFunc_ = successCallFunc;
    pushesFailFunc_ = failCallFunc;
    
    // 푸시/야간푸시를 한번에 설정
    // 로그인 전에 푸시/야간푸시 허용 여부를 받는 게임이라면 로그인 후에 아래 코드를 반드시 호출합니다.
    [[GamePot getInstance] setPushStatus:enablePush night:enableNightPush ad:enableAd success:^{
        onPushesSuccess();
    } fail:^(NSError *error) {
        onPushesFail((int)[error code], [[error localizedDescription] UTF8String]);
    }];
}

bool GamePotPush::isEnabledPushNotice()
{
    return [[GamePot getInstance] getPushEnable];
}

bool GamePotPush::isEnabledPushNight()
{
    return [[GamePot getInstance] getNightPushEnable];
}

bool GamePotPush::isEnabledPushAD()
{
    return [[GamePot getInstance] getAdPushEnable];
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
    NSDateFormatter* formatter = [[NSDateFormatter alloc] init];
    [formatter setDateFormat:@"yyyy-MM-dd HH:mm:ss"];

    NSString* strDate = [formatter stringFromDate:[[NSDate date] dateByAddingTimeInterval:afterSec]];

    int pushId = [[GamePot getInstance] sendLocalPush:[NSString stringWithUTF8String:title.c_str()] setMessage:[NSString stringWithUTF8String:msg.c_str()] setDateString:strDate];
    
    // 로컬 푸시 게임팟에 등록한 뒤, 얻은 pushId를 저장.
    AceUserDefault::getInstance()->setIntegerForKey(pushCode.c_str(), pushId);
}

void GamePotPush::unregisterLocalPush(const std::string& pushCode)
{
    // 게임팟 모듈에 요청
    [[GamePot getInstance] cancelLocalPush: AceUserDefault::getInstance()->getIntegerForKey(pushCode.c_str())];
    
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
