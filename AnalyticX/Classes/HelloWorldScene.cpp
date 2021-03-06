#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

// Uncomment the following line to define FLURRY, or define it as per project-based (see Build Setting->Apple LLVM 6.0 - Preprocessing in XCode)
//#define FLURRY

#ifdef FLURRY
#include "AnalyticX.h"
#endif

// always include this even FLURRY is not defined as it provides shorter code, and more flexible to log event with Flurry
// note: it makes no harm even FLURRY is not defined to include this header
#include "AnalyticXMacros.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#endif

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::node();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::node();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    // ask director the window size
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::itemWithNormalImage(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback) );
    pCloseItem->setPosition( ccp(size.width - 20, 20) );
    
    // add a menu font to activate crashing app
    CCMenuItemFont::setFontSize(25);
    CCMenuItemFont *menuCrashItem = CCMenuItemFont::itemWithString("Test Crash", this, menu_selector(HelloWorld::makeItCrash));
    menuCrashItem->setPosition(ccp(size.width/2, menuCrashItem->getContentSize().height));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::menuWithItems(pCloseItem, menuCrashItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    CCLabelTTF* pLabel = CCLabelTTF::labelWithString("Hello World", "Thonburi", 34);

    // position the label on the center of the screen
    pLabel->setPosition( ccp(size.width / 2, size.height - 20) );

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    // add "HelloWorld" splash screen"
    CCSprite* pSprite = CCSprite::spriteWithFile("HelloWorld.png");

    // position the sprite on the center of the screen
    pSprite->setPosition( ccp(size.width/2, size.height/2) );

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);
    
#ifdef FLURRY
    AnalyticX::flurrySetAppVersion("v_1_97");
    cocos2d::CCLog("--->>>get flurry version = %s", AnalyticX::flurryGetFlurryAgentVersion());
    AnalyticX::flurrySetDebugLogEnabled(true);
    AnalyticX::flurrySetSessionContinueSeconds(143);
    AnalyticX::flurrySetSecureTransportEnabled(false);
    AnalyticX::flurrySetCrashReportingEnabled(true);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    AnalyticX::flurryStartSession("S5K5JGB8HB4J2ZBBWZ4D");
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    AnalyticX::flurryStartSession("W7IBK43RJCHPT4IRP4HI");
#endif
    
    AnalyticX::flurrySetUserID("fake_user_id");
    AnalyticX::flurrySetAge(34);
    AnalyticX::flurrySetGender("f");
    AnalyticX::flurrySetReportLocation(false);
    
    AnalyticX::flurryLogEvent("event_3");
    AnalyticX::flurryLogEventTimed(" log event timed test...", false);

    CCDictionary *testDict = new CCDictionary();
    testDict->autorelease();

    CCString *testCCString;

    testCCString = CCString::stringWithCString("obj 0");

    testDict->setObject(testCCString, "key 0");

    testCCString = CCString::stringWithCString("obj 1");

    testDict->setObject(testCCString, "key 1");
    
    CCDictionary* testDictInternal = new CCDictionary();
    testDictInternal->autorelease();
    
    testDict->setObject(testDictInternal, "key 2 : dictionary");
    
    CCArray* testArrayInternal = new CCArray();
    testArrayInternal->autorelease();
    
    testDict->setObject(testArrayInternal, "key 3 : array");
    
    testDict->setObject(CCInteger::integerWithInt(5), "key 4 : int");
    
    testDict->setObject(CCDirector::sharedDirector(), "key 5 : CCDirector");

    AnalyticX::flurryLogEventWithParameters("event_5_with_params_no_timed", testDict);

    AnalyticX::flurryLogEventWithParametersTimed("test flurryLogEventWithParameters + timed", testDict, true);
    AnalyticX::flurryEndTimedEventWithParameters("test end event...", NULL);
    
    AnalyticX::flurryLogPageView();
    
#endif
    
    // test log event via macro
    const char* tempString = "Hello World";
    
    FLURRYLogEvent("event_logging_via_macro");
    FLURRYLogEvent("event_logging_via_macro_2, dummy-num: %d - %d", 1, 2);
    FLURRYLogEvent("event_logging_via_macro_3, dummy-str: %s", tempString);

#ifdef FLURRY
    //AnalyticX::flurryEndSession();
#endif

    
    return true;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::beginProcessToCrashApp(CCObject* pSender)
{
    // disable the menu (as we just want to crash it once and for all)
    CCMenuItem *item = static_cast<CCMenuItem*>(pSender);
    CCAssert(item != NULL, "pSender cannot but NULL");
    item->setIsEnabled(false);
    
    // create action to delay for a short amount of time, then crash the program
    CCDelayTime *delay = CCDelayTime::actionWithDuration(5.0f);
    CCCallFunc *crash = CCCallFunc::actionWithTarget(this, callfunc_selector(HelloWorld::makeItCrash));
    CCSequence *action = CCSequence::actionOneTwo(delay, crash);
    this->runAction(action);
}

void HelloWorld::makeItCrash()
{
    // just create a NULL object and call any function to make it crash
    CCObject *obj = NULL;
    obj->retain();
}
