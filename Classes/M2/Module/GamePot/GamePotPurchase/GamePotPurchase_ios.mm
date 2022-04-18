//
//  GamePotPurchase.cpp
//  MPBReboot
//
//  Created by 박동제 on 2021/03/19.
//

#include "GamePotPurchase.h"

#import <GamePot/GamePot.h>
#import <StoreKit/SKProduct.h>

#include "ItemShopData.h"

USING_NS_ACE

std::function<void(const std::string& productId, const std::string& productName)> GamePotPurchase::purchaseSuccessFunc = 0;
std::function<void(const std::string& errorMsg)> GamePotPurchase::purchaseFailFunc = 0;
std::function<void()> GamePotPurchase::purchaseCancelFunc = 0;

void GamePotPurchase::purchase(const std::string& productId, const std::function<void(const std::string&, const std::string&)>& _purchaseSuccessFunc,
                                                             const std::function<void()>& _purchaseCancelFunc,
                                                             const std::function<void(const std::string&)>& _purchaseFailFunc)
{
    purchaseSuccessFunc = _purchaseSuccessFunc;
    purchaseCancelFunc = _purchaseCancelFunc;
    purchaseFailFunc = _purchaseFailFunc;
    
    NSString* nsProductId = [NSString stringWithUTF8String:productId.c_str()];
    NSString* nsEtc = nil;
    
    auto mapProductInfo = ItemShopData::getInstance()->getMapProductInfo();

    if (mapProductInfo.find(productId) == mapProductInfo.end())
    {
        nsEtc = @"KRW|0";
    }
    else
    {
        nsEtc = [NSString stringWithFormat:@"%s|%d", mapProductInfo[productId].currencyCode.c_str(), mapProductInfo[productId].price];
    }


    [[GamePot getInstance] purchase:nsProductId uniqueId:@"" serverId:@"" playerId:@"" etc:nsEtc];
}

//std::vector<GamePotProduct> GamePotPurchase::getProductInfoList()
//{
//    std::vector<GamePotProduct> vecProduct;
//
//    NSArray<SKProduct*>* itemList = [[GamePot getInstance] getDetails];
//    for(SKProduct* pSKProduct in itemList)
//    {
//        NSString* nsProductId = [pSKProduct productIdentifier];
//        NSDecimalNumber* nsPrice =[pSKProduct price];
//        NSString* nsLocalizePrice = [[GamePot getInstance] getLocalizePrice:[pSKProduct productIdentifier]];
//
//        GamePotProduct product;
//        product.productId = [nsProductId UTF8String];
//        product.price = nsPrice.intValue;
//        product.localizePrice = [nsLocalizePrice UTF8String];
//
//        vecProduct.push_back(product);
//    }
//
//    return vecProduct;
//}

void GamePotPurchase::syncProductInfos()
{
    [[GamePot getInstance] getPurchaseDetailListAsync:^(BOOL _success, NSArray *_items, NSError *_error) {
        if (_success)
        {
            for(NSDictionary* item in _items)
            {
                NSString* nsProductId = [item objectForKey:@"productId"];
                NSString* nsCurrencyCode = [item objectForKey:@"price_currency_code"];
                int price = [[item objectForKey:@"price"] intValue];
                NSString* nsLocalizePrice = [item objectForKey:@"price_with_currency"];
                
                GamePotProduct product;
                product.productId = [nsProductId UTF8String];
                product.currencyCode = [nsCurrencyCode UTF8String];
                product.price = price;
                product.localizePrice = [nsLocalizePrice UTF8String];
                
                ItemShopData::getInstance()->setProductInfo(product.productId, product);
                
                CCLOG("상품정보: %s, %s, %s", product.productId.c_str(), product.currencyCode.c_str(), product.localizePrice.c_str());
            }
        }
        else
        {
            CCLOG("GamePot getPurchaseDetailListAsync error : %ld, %s", [_error code], [[_error localizedDescription] UTF8String]);
        }
    }];
}


std::string GamePotPurchase::getLocalizePrice(const std::string& productId)
{
    NSString* nsProductId = [NSString stringWithUTF8String:productId.c_str()];
    NSString* nsPrice = [[GamePot getInstance] getLocalizePrice:nsProductId];
    std::string price = [nsPrice UTF8String];

    return price;
}

void GamePotPurchase::onSyncProductInfosSuccess(const std::string& resultJson)
{
}

void GamePotPurchase::onPurchaseSuccess(const std::string& productId, const std::string& productName)
{    
    if (purchaseSuccessFunc != NULL)
        purchaseSuccessFunc(productId, productName);
}

void GamePotPurchase::onPurchaseFail(const std::string& errorMsg)
{   
    if (purchaseFailFunc != NULL)
        purchaseFailFunc(errorMsg);
}

void GamePotPurchase::onPurchaseCancel()
{    
    if (purchaseCancelFunc != NULL)
        purchaseCancelFunc();
}
