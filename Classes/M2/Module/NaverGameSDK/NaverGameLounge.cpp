//
//  NaverGameLounge.cpp
//  MPBReboot
//
//  Created by 안현석 on 2021. 12. 22..
//
//

#include "NaverGameLounge.h"

using namespace std;

NaverGameLounge* NaverGameLounge::pInstance = nullptr;

NaverGameLounge::NaverGameLounge()
{
}

NaverGameLounge::~NaverGameLounge()
{
}

NaverGameLounge* NaverGameLounge::getInstance()
{
    if(nullptr == pInstance)
    {
        pInstance = new NaverGameLounge();
    }
    
    return pInstance;
}


void NaverGameLounge::init()
{
    nng::NaverGameSDK::init("s80pcM2HiFAMYKX2Ycph", "6QtxYewX2T", "9UP_Collectable");
    nng::NaverGameSDK::setSdkListener(this);
}

void NaverGameLounge::startHomeBanner(const function<void()>& _closeCallFunc)
{
    closeCallFunc = _closeCallFunc;
    
    nng::NaverGameSDK::startHomeBanner();
}

void NaverGameLounge::startSorryBanner(const function<void()>& _closeCallFunc)
{
    closeCallFunc = _closeCallFunc;
    
    nng::NaverGameSDK::startSorryBanner();
}

void NaverGameLounge::startBoard(int boardId, const function<void()>& _closeCallFunc)
{
    closeCallFunc = _closeCallFunc;
    
    nng::NaverGameSDK::startBoard(boardId);
}

void NaverGameLounge::startFeed(int feedId, bool isTempFeedId, const function<void()>& _closeCallFunc)
{
    closeCallFunc = _closeCallFunc;
    
    nng::NaverGameSDK::startFeed(feedId, isTempFeedId);
}

void NaverGameLounge::stopSdk()
{
    nng::NaverGameSDK::stopSdk();
}

void NaverGameLounge::onSdkStarted()
{
    CCLOG("NaverGameSDK onSdkStarted");
}

void NaverGameLounge::onSdkStopped()
{
    CCLOG("NaverGameSDK onSdkStopped");
    
    if(nullptr != closeCallFunc)
    {
        closeCallFunc();
    }
    
    closeCallFunc = nullptr;
}

void NaverGameLounge::onSdkReceiveInGameMenuCode(const string& inGameMenuCode)
{
    CCLOG("NaverGameSDK onSdkReceiveInGameMenuCode: %s", inGameMenuCode.c_str());
}

