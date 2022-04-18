//
//  GamePotServiceAgreement.h
//  MPBReboot
//
//  Created by 박동제 on 2021/02/25.
//

#pragma once

#include "ace.h"

static const char* POLICY_URL = "https://kr.object.ncloudstorage.com/gamepot-otjji2qg/page/index.html?id=cbe9e515-2b2f-4bc9-804b-49163befb8a2";
static const char* PROB_URL = "https://docs.google.com/spreadsheets/d/e/2PACX-1vRewVmAnGCi3Gua6cyqPE-bB64tJQSPoD84ojRk3KZXfsUeax6Sv_3a1zGea4IhRYFdcMnxWKdLIxJG/pubhtml?widget=true&amp;headers=false";
static const char* LISCENCE_URL = "https://kr.object.ncloudstorage.com/gamepot-otjji2qg/page/index.html?id=471e8af6-955e-4088-b778-b736103e5ef4";

NS_ACE_BEGIN

class GamePotServiceAgreement
{
private:
    static std::function<void(bool isAgree, bool isEnabledPush, bool isEnabledNightPush)> agreeSuccessFunc_;
    static std::function<void(int errorCode, const std::string& errorMsg)> agreeFailFunc_;
    
public:
    static void initAgreement();
    static void setAutoAgreement(bool isAuto);
    static void showAgreementPopup(const std::function<void(bool isAgree, bool isEnabledPush, bool isEnabledNightPush)>& successCallFunc, const std::function<void(int errorCode, const std::string& errorMsg)>& failCallFunc);
    static void showTerms(); // 이용약관
    static void showPrivacy(); // 개인정보 취급방침
    static void showPolicy(); // 운영정책
    static void showProb(); // 확률표
    static void showRefund(); // 환불정책
    static void showLiscence(); // 라이센스정책
    static void showURL(const char* url);
    
public:
    static void onAgreeSuccess(bool isAgree, bool isEnabledPush, bool isEnabledNightPush);
    static void onAgreeFail(int errorCode, const std::string& errorMsg);
};

NS_ACE_END
