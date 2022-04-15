//
//  NaverGameLounge.h
//  MPBReboot
//
//  Created by 안현석 on 2021. 12. 22..
//
//

#pragma once

#include "NaverGameSDK.h"

class NaverGameLounge : public nng::NaverGameSDKListener
{
private:
    NaverGameLounge();
    virtual ~NaverGameLounge();
    
    static NaverGameLounge* pInstance;

private:
    std::function<void()> closeCallFunc;
    
public:
    static NaverGameLounge* getInstance();
    
    void init();
    void startHomeBanner(const std::function<void()>& _closeCallFunc);
    void startSorryBanner(const std::function<void()>& _closeCallFunc);
    void startBoard(int boardId, const std::function<void()>& _closeCallFunc);
    void startFeed(int feedId, bool isTempFeedId, const std::function<void()>& _closeCallFunc);
    void stopSdk();
    
    void onSdkStarted() override;
    void onSdkStopped() override;
    void onSdkReceiveInGameMenuCode(const std::string& inGameMenuCode) override;
};

