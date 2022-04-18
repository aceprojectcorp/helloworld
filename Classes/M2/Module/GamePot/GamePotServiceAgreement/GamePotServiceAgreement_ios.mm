//
//  GamePotServiceAgreement_ios.mm
//  MPBReboot
//
//  Created by 박동제 on 2021/02/25.
//

#import "AppController.h"

#import <UserNotifications/UserNotifications.h>
#import <GamePot/GamePot.h>

#include "GamePotServiceAgreement.h"
#include "GamePotPush.h"
#include "AceUserDefault.h"

USING_NS_ACE
USING_NS_ACE_USER_DEFAULT
using namespace std;

function<void(bool isAgree, bool isEnabledPush, bool isEnabledNightPush)> GamePotServiceAgreement::agreeSuccessFunc_ = nullptr;
function<void(int errorCode, const string& errorMsg)> GamePotServiceAgreement::agreeFailFunc_ = nullptr;

void GamePotServiceAgreement::initAgreement()
{
    // 약관 동의
    GamePotAgreeOption* option = [[GamePotAgreeOption alloc] init:MATERIAL_CYAN];
    [option setAllMessage:@"모두 동의"];
    [option setTermMessage:@"이용약관(필수)"];
    [option setPrivacyMessage:@"개인정보 취급 방침(필수)"];
    [option setPushMessage:@"일반 푸쉬 수신 동의(선택)"];
    [option setNightPushMessage:@"야간 푸쉬 수신 동의(선택)"];
    [option setFooterTitle:@"동의하고 게임을 시작합니다."];
    
    [option setShowPush:YES];
    [option setShowNightPush:YES];
    
    [[GamePot getInstance] setAgreeBuilder:option];
}
void GamePotServiceAgreement::setAutoAgreement(bool isAuto)
{
    CCLOG("auto agreement state: %d", isAuto);
    [[GamePot getInstance] setAutoAgree:isAuto];
}

void GamePotServiceAgreement::showAgreementPopup(const function<void(bool isAgree, bool isEnabledPush, bool isEnabledNightPush)>& successCallFunc, const function<void(int errorCode, const string& errorMsg)>& failCallFunc)
{
    agreeSuccessFunc_ = successCallFunc;
    agreeFailFunc_ = failCallFunc;
    
    AppController* appController = (AppController *)[[UIApplication sharedApplication] delegate];
    
    // 약관 동의
    GamePotAgreeOption* option = [[GamePot getInstance] agreeBuilder];
    
    [[GamePot getInstance] showAgreeView:(UIViewController*)[appController viewController] option:option handler:^(GamePotAgreeInfo *result)
    {
        bool isAgree = [result agree];
        bool isEnabledPush = [result agreePush];
        bool isEnabledNightPush = [result agreeNight];
        
        if(isAgree)
        {
            onAgreeSuccess(isAgree, isEnabledPush, isEnabledNightPush);
        }
        else
        {
            onAgreeFail(0, "약관동의 실패");
        }
    }];
}

void GamePotServiceAgreement::showTerms()
{
    AppController* appController = (AppController *)[[UIApplication sharedApplication] delegate];
    
    [[GamePot getInstance] showTerms:(UIViewController*)[appController viewController]];
}

void GamePotServiceAgreement::showPrivacy()
{
    AppController* appController = (AppController *)[[UIApplication sharedApplication] delegate];
    
    [[GamePot getInstance] showPrivacy:(UIViewController*)[appController viewController]];
}

void GamePotServiceAgreement::showPolicy()
{
    AppController* appController = (AppController *)[[UIApplication sharedApplication] delegate];
    
    [[GamePot getInstance] showWebView:(UIViewController*)[appController viewController] setType:WEBVIEW_NORMAL setURL:[NSString stringWithUTF8String:POLICY_URL]];
}

void GamePotServiceAgreement::showProb()
{
    AppController* appController = (AppController *)[[UIApplication sharedApplication] delegate];
    
    [[GamePot getInstance] showWebView:(UIViewController*)[appController viewController] setType:WEBVIEW_NORMAL setURL:[NSString stringWithUTF8String:PROB_URL]];
}

void GamePotServiceAgreement::showRefund()
{
    AppController* appController = (AppController *)[[UIApplication sharedApplication] delegate];
    
    [[GamePot getInstance] showRefund:(UIViewController*)[appController viewController]];
}

void GamePotServiceAgreement::showLiscence()
{
    AppController* appController = (AppController *)[[UIApplication sharedApplication] delegate];
    
    [[GamePot getInstance] showWebView:(UIViewController*)[appController viewController] setType:WEBVIEW_NORMAL setURL:[NSString stringWithUTF8String:LISCENCE_URL]];
}

void GamePotServiceAgreement::onAgreeSuccess(bool isAgree, bool isEnabledPush, bool isEnabledNightPush)
{
    if(nullptr != agreeSuccessFunc_)
    {
        agreeSuccessFunc_(isAgree, isEnabledPush, isEnabledNightPush);
    }
}

void GamePotServiceAgreement::showURL(const char* url)
{
    AppController* appController = (AppController *)[[UIApplication sharedApplication] delegate];
    
    [[GamePot getInstance] showWebView:(UIViewController*)[appController viewController] setType:WEBVIEW_NORMAL setURL:[NSString stringWithUTF8String:url]];
}

void GamePotServiceAgreement::onAgreeFail(int errorCode, const std::string& errorMsg)
{
    CCLOG("errorCode: %d, errorMsg: %s", errorCode, errorMsg.c_str());
    
    if(nullptr != agreeFailFunc_)
    {
        agreeFailFunc_(errorCode, errorMsg);
    }
}

