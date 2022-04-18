//
//  GamePotPurchase_android.cpp
//  MPBReboot
//
//  Created by 박동제 on 2021/03/19.
//

#include "libjson.h"
#include "JsonUtils.h"

#include "GamePotPurchase.h"
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
    
    // 우리 서버에 보내줘야 하는 추가 정보 셋팅
    std::ostringstream oss;
    
    auto mapProductInfo = ItemShopData::getInstance()->getMapProductInfo();
    if (mapProductInfo.find(productId) == mapProductInfo.end())
    {
        oss << "KRW|0";
    }
    else
    {
        oss << mapProductInfo[productId].currencyCode << "|" << std::to_string(mapProductInfo[productId].price);
    }
    
    cocos2d::JniMethodInfo info;
    if (cocos2d::JniHelper::getStaticMethodInfo(info,
                                       "com/aceproject/android/C2sCocos2dxActivity",
                                       "purchase", "(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        CCLOG("@@@@ find jni method, classId:%d, methodId:%d", info.classID, info.methodID);
        
        jstring _productId = info.env->NewStringUTF(productId.c_str());
        jstring _etc = info.env->NewStringUTF(oss.str().c_str());
        
        info.env->CallStaticVoidMethod(info.classID, info.methodID, _productId, _etc);
        
        info.env->DeleteLocalRef(_productId);
        info.env->DeleteLocalRef(_etc);
        info.env->DeleteLocalRef(info.classID);
    }
}

//std::vector<GamePotProduct> GamePotPurchase::getProductInfoList()
//{
//    std::vector<GamePotProduct> vecProduct;
//
//    cocos2d::JniMethodInfo info;
//    if (!cocos2d::JniHelper::getStaticMethodInfo(info,
//                                       "com/aceproject/android/C2sCocos2dxActivity",
//                                       "getProductInfoList", "()[Ljava/lang/Object;"))
//        return vecProduct;
//
//    JNIEnv *env = info.env;
//
//    jobjectArray objArray = (jobjectArray)env->CallStaticObjectMethod(info.classID, info.methodID);
//    if (objArray)
//    {
//        int size = env->GetArrayLength(objArray);
//        for (int i = 0; i < size; i++)
//        {
//            jobject obj = env->GetObjectArrayElement(objArray, i);
//            jclass objClass = env->GetObjectClass(obj);
//
//            jfieldID productIdField = env->GetFieldID(objClass, "productId", "Ljava/lang/String;");
//            jfieldID priceField = env->GetFieldID(objClass, "price_amount", "D");
//            jfieldID localizePriceField = env->GetFieldID(objClass, "price_with_currency", "Ljava/lang/String;");
//
//            jstring jproductId = (jstring)env->GetObjectField(obj, productIdField);
//            std::string productId = env->GetStringUTFChars(jproductId, NULL);
//
//            int price = (int)env->GetDoubleField(obj, priceField);
//
//            jstring jlocalizePrice = (jstring)env->GetObjectField(obj, localizePriceField);
//            std::string localizePrice = env->GetStringUTFChars(jlocalizePrice, NULL);
//
//            GamePotProduct product;
//            product.productId = productId;
//            product.price = price;
//            product.localizePrice = localizePrice;
//
//            vecProduct.push_back(product);
//
//            env->ReleaseStringUTFChars(jproductId, productId.c_str());
//            env->DeleteLocalRef(jproductId);
//            env->ReleaseStringUTFChars(jlocalizePrice, localizePrice.c_str());
//            env->DeleteLocalRef(jlocalizePrice);
//
//            env->DeleteLocalRef(obj);
//            env->DeleteLocalRef(objClass);
//        }
//
//        env->DeleteLocalRef(objArray);
//    }
//
//    env->DeleteLocalRef(info.classID);
//
//    return vecProduct;
//}

void GamePotPurchase::syncProductInfos()
{
    cocos2d::JniMethodInfo info;
    if (cocos2d::JniHelper::getStaticMethodInfo(info,
                                        "com/aceproject/android/C2sCocos2dxActivity",
                                        "syncProductInfos", "()V"))
    {
        CCLOG("@@@@ find jni method, classId:%d, methodId:%d", info.classID, info.methodID);
                
        info.env->CallStaticVoidMethod(info.classID, info.methodID);
        
        info.env->DeleteLocalRef(info.classID);
    }
}


std::string GamePotPurchase::getLocalizePrice(const std::string& productId)
{
    
    return "";
}

void GamePotPurchase::onSyncProductInfosSuccess(const std::string& resultJson)
{
    JSONNode node = libjson::parse(resultJson);
    
    CCLOG("상품정보 resultJson: %s", resultJson.c_str());

    JSONNode list = node.as_array();
    for(JSONNode::const_iterator it = list.begin(); it != list.end(); ++it)
    {
        const JSONNode& n = it->as_node();

        GamePotProduct product;
        product.productId = JsonUtils::parseString(n, "productId");
        product.currencyCode = JsonUtils::parseString(n, "price_currency_code");
                
        // 원스토어에서는 price_with_currency에서 통화기호를 붙여서 보내주지 않는다.
        // 일단 국내만 서비스하기 때문에 원화기호를 강제로 붙여주자.
#ifdef ONESTORE
        product.price = JsonUtils::parseInt(n, "price"); // 원스토어는 price를 그대로 써도 됨...
        
        product.localizePrice = "₩" + JsonUtils::parseString(n, "price_with_currency");
#else
        std::string priceStr = JsonUtils::parseString(n, "price_amount_micros"); // 구글 price는 기호가 붙어서 와서 못씀...
        product.price = std::atoi(priceStr.substr(0, priceStr.size() - 6).c_str()); // 000000 삭제
        
        product.localizePrice = JsonUtils::parseString(n, "price_with_currency");
#endif
        
        ItemShopData::getInstance()->setProductInfo(product.productId, product);
        
        CCLOG("상품정보: %s, %s, %d, %s", product.productId.c_str(), product.currencyCode.c_str(), product.price, product.localizePrice.c_str());
    }
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

