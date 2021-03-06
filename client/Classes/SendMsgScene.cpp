#include "SendMsgScene.h"
#include "Client.h"
#include "LoginScene.h"

SendMsg::~SendMsg() {
}

CCScene* SendMsg::scene() {
    // 'scene' is an autorelease object
    CCScene* scene = CCScene::create();
    
    // 'layer' is an autorelease object
    SendMsg* layer = SendMsg::create();
	
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

bool SendMsg::init() {
    // super init first
    if (!CCLayer::init()) {
        return false;
    }
    
    // surface size
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    // login item
    CCLabelTTF* label = CCLabelTTF::create("Send", "Helvetica", 40 / CC_CONTENT_SCALE_FACTOR());
    CCMenuItemLabel* sendItem = CCMenuItemLabel::create(label, this, menu_selector(SendMsg::onSendClicked));
    sendItem->setPosition(ccp(origin.x + visibleSize.width / 2,
                              origin.y + visibleSize.height * 6 / 10));
    
    // menu
    CCMenu* menu = CCMenu::create(sendItem, NULL);
    menu->setPosition(CCPointZero);
    addChild(menu);
    
    // message edit
    CCSprite* frame = CCSprite::create("images/textbox_normal.png");
    CCRect rect = CCRectMake(0,
                             0,
                             frame->getContentSize().width,
                             frame->getContentSize().height);
    CCRect insets = CCRectMake(11 / CC_CONTENT_SCALE_FACTOR(),
                               21 / CC_CONTENT_SCALE_FACTOR(),
                               1 / CC_CONTENT_SCALE_FACTOR(),
                               1 / CC_CONTENT_SCALE_FACTOR());
    CCSize editSize = CCSizeMake(visibleSize.width * 4 / 5, 50 / CC_CONTENT_SCALE_FACTOR());
    {
        CCScale9Sprite* editNormal = CCScale9Sprite::create("images/textbox_normal.png",
                                                            rect,
                                                            insets);
        CCScale9Sprite* editPressed = CCScale9Sprite::create("images/textbox_pressed.png",
                                                             rect,
                                                             insets);
        m_msgEdit = CCEditBox::create(editSize, editNormal, editPressed);
        m_msgEdit->setReturnType(kKeyboardReturnTypeDone);
        m_msgEdit->setFontSize(12);
        m_msgEdit->setText("");
        m_msgEdit->setMaxLength(8);
        m_msgEdit->setFontColor(ccc3(158, 122, 83));
        m_msgEdit->setPosition(ccp(origin.x + visibleSize.width / 2,
                                   origin.y + visibleSize.height * 8 / 10));
        addChild(m_msgEdit);
    }
    
    // add notification observer
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, callfuncO_selector(SendMsg::onTCPSocketConnected), kCCNotificationTCPSocketConnected, NULL);
    nc->addObserver(this, callfuncO_selector(SendMsg::onTCPSocketDisonnected), kCCNotificationTCPSocketDisconnected, NULL);
    nc->addObserver(this, callfuncO_selector(SendMsg::onPacketReceived), kCCNotificationPacketReceived, NULL);
    
    return true;
}

void SendMsg::onExit() {
    CCLayer::onExit();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->removeObserver(this, kCCNotificationPacketReceived);
    nc->removeObserver(this, kCCNotificationTCPSocketConnected);
    nc->removeObserver(this, kCCNotificationTCPSocketDisconnected);
}

void SendMsg::onSendClicked(CCObject* sender) {
    CCJSONObject* json = CCJSONObject::create();
    json->addString("message", m_msgEdit->getText());
    Client::sharedClient()->send(1, json, Client::TEST, Client::NOT);
}

void SendMsg::onTCPSocketConnected(CCTCPSocket* s) {
}

void SendMsg::onTCPSocketDisonnected(CCTCPSocket* s) {
    // socket disconnected, back to login
    CCDirector::sharedDirector()->replaceScene(Login::scene());
}

void SendMsg::onPacketReceived(CCPacket* p) {
    if(p->getHeader().command == Client::TEST) {
        CCJSONObject* json = CCJSONObject::create(p->getBody(), p->getBodyLength());
        Client::ErrCode err = (Client::ErrCode)json->optInt("errno");
        if(err != Client::E_OK) {
            string errMsg = json->optString("errmsg");
            errMsg = CCUtils::decodeHtmlEntities(errMsg);
            CCLOG("error message: %s", errMsg.c_str());
            Helper::showToast(errMsg, this);
        } else {
            CCJSONObject* data = json->optJSONObject("data");
            if(data) {
                string reply = data->optString("message");
                reply = CCUtils::decodeHtmlEntities(reply);
                CCLOG("server reply: %s", reply.c_str());
                Helper::showToast(reply, this);
            }
        }
    } else if(p->getHeader().command == Client::TEST_PUSH) {
        CCJSONObject* json = CCJSONObject::create(p->getBody(), p->getBodyLength());
        CCJSONObject* data = json->optJSONObject("data");
        string msg = data->optString("message");
        msg = CCUtils::decodeHtmlEntities(msg);
        CCLOG("server push: %s", msg.c_str());
        Helper::showToast(msg, this);
    }
}