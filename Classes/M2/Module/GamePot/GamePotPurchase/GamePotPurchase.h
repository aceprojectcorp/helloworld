//
//  GamePotPurchase.h
//  MPBReboot
//
//  Created by 박동제 on 2021/03/19.
//

#pragma once

#include "ace.h"
#include "AceObject.h"

NS_ACE_BEGIN

struct GamePotProduct : public AceObject
{
    std::string productId;
    std::string currencyCode;
    int price = 0;
    std::string localizePrice;
}; using GamePotProductPtr = std::shared_ptr<GamePotProduct>;


class GamePotPurchase
{
private:
    // 당장은 productId외의 다른 정보는 굳이 필요없을것 같아서, productId와 productName만 넘김. 추후 다른정보 필요시 추가.
    static std::function<void(const std::string& productId, const std::string& productName)> purchaseSuccessFunc;
    static std::function<void(const std::string& errorMsg)> purchaseFailFunc;
    static std::function<void()> purchaseCancelFunc;
    
public:
    static void purchase(const std::string& productId, const std::function<void(const std::string&, const std::string&)>& _purchaseSuccessFunc,
                                                       const std::function<void()>& _purchaseCancelFunc,
                                                       const std::function<void(const std::string&)>& _purchaseFailFunc); // 결제 시도

    // 비동기식로 변경. 기존 가격정보 가져오는 함수는 가격을 못가져오는 경우 생김.
    // static std::vector<GamePotProduct> getProductInfoList(); // 결제 아이템 리스트 획득
    static void syncProductInfos(); // 결제 아이템 리스트 획득
    static std::string getLocalizePrice(const std::string& productId); // Device 설정에 따른 통화 가격 얻어올때
    
    static void onSyncProductInfosSuccess(const std::string& resultJson);
    static void onPurchaseSuccess(const std::string& productId, const std::string& productName);
    static void onPurchaseFail(const std::string& errorMsg);
    static void onPurchaseCancel();
    
};

NS_ACE_END
