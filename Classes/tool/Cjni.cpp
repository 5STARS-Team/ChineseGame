#include "Cjni.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#endif
#include <string>
#include "ClientDefine.h"
#include "Logic/GameLogic.h"

#define CLASS_NAME "org/cocos2dx/cpp/GameLogic"


	void JNI::cppCharge(std::string payjson){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		JniMethodInfo t;
		if(JniHelper::getStaticMethodInfo(t, CLASS_NAME, "Charge","(Ljava/lang/String;)V"))
	    {
				jstring jpayjson = t.env->NewStringUTF(payjson.c_str());
				t.env->CallStaticVoidMethod(t.classID, t.methodID,jpayjson);
	    }
#endif
	}

	void JNI::cppLogin(){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		JniMethodInfo t;
		if(JniHelper::getStaticMethodInfo(t, CLASS_NAME, "Login","()V"))
		{
				t.env->CallStaticVoidMethod(t.classID, t.methodID);
		}
#endif
	}

	std::string JNI::cppGetUUID(){
        std::string imeiStr = "";
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		JniMethodInfo t;
		
		if(JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getUUid","()Ljava/lang/String;"))
		{
			jstring uuid = (jstring)t.env->CallStaticObjectMethod(t.classID,t.methodID);
			imeiStr = JniHelper::jstring2string(uuid);
		}
#endif
		return imeiStr;
	}

	int JNI::cppGetCodeID(){
        int codeid = 0;
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		JniMethodInfo t;		
		if(JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getCodeId","()I"))
		{
			 codeid = (int)t.env->CallStaticObjectMethod(t.classID,t.methodID);
		}
#endif
		return codeid;
	}

	void JNI::cppShare(std::string title,std::string content,std::string imgFileName){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		JniMethodInfo t;
		if(JniHelper::getStaticMethodInfo(t, CLASS_NAME, "Share","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"))
		{
			jstring jtitle = t.env->NewStringUTF(title.c_str());
			jstring jcontent = t.env->NewStringUTF(content.c_str());
            jstring jimgFileName = t.env->NewStringUTF(imgFileName.c_str());
			t.env->CallStaticVoidMethod(t.classID, t.methodID,jtitle,jcontent,jimgFileName);
		}
#endif
	} 

    void JNI::openNewApp( int index )
    {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo t;
        if(JniHelper::getStaticMethodInfo(t, CLASS_NAME, "openNewApp","(I)V"))
        {
            t.env->CallStaticVoidMethod(t.classID, t.methodID,index);
        }
#endif
    }

    void JNI::cppLoginOut(std::string str)
    {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo t;
        if(JniHelper::getStaticMethodInfo(t, CLASS_NAME, "LoginOut","(Ljava/lang/String;)V"))
        {
            jstring jstr = t.env->NewStringUTF(str.c_str());
            t.env->CallStaticVoidMethod(t.classID, t.methodID,jstr);
        }
#else
		Director::getInstance()->end();
#endif
    }

    void JNI::cppDownUrl( int id, std::string downurl,std::string sign ,std::string version)
    {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        //TimeLoadingLayer::StartWaiting();
        JniMethodInfo t;
        if(JniHelper::getStaticMethodInfo(t, CLASS_NAME, "DownUrl","(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"))
        {
            jstring jdownurl = t.env->NewStringUTF(downurl.c_str());
            jstring jsign = t.env->NewStringUTF(sign.c_str());
            jstring jversion = t.env->NewStringUTF(version.c_str());
            string code = GameLogic::GetInstance()->getCode(id);
            jstring jcode = t.env->NewStringUTF(code.c_str());
            t.env->CallStaticVoidMethod(t.classID, t.methodID,id,jdownurl,jsign,jversion,jcode);
        }
#endif
    }

    void JNI::sendApkDownPercent( int id, int _percent )
    {
        static int percent = 0;
        if(percent != _percent){
            if(percent>_percent+5){

            }
            percent = _percent;
			/* DownloadLayer* layer = dynamic_cast<DownloadLayer*>(LayerManager::GetInstance()->GetLayerByTag(DOWNLOAD_LAYER));
			 if(layer ){
			 if(layer->m_GameId == id)
			 layer->setDownloadPercent(_percent/10.0f);
			 }*/
        }
        //
        
    }
    //return 0:û�а�װ 1:�Ѿ���װ 2:�汾��Ҫ����
    int JNI::cppGetVersionFlag( int id,std::string version )
    {
        int flag = 0;
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo t;		
        if(JniHelper::getStaticMethodInfo(t, CLASS_NAME, "CheckVersion","(ILjava/lang/String;)I"))
        {
            jstring jversion = t.env->NewStringUTF(version.c_str());
            flag = (int)t.env->CallStaticObjectMethod(t.classID,t.methodID,id,jversion);
        }
#endif
        return flag;
    }

    int JNI::cppCheckEditText()
    {
        int flag = 0;
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo t;		
        if(JniHelper::getStaticMethodInfo(t, CLASS_NAME, "CheckEditText","()I"))
        {
            flag = (int)t.env->CallStaticObjectMethod(t.classID,t.methodID);
        }
#endif
        return flag;
    }

	void JNI::cppInfo(std::string msg)
	{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "SendInfo", "(Ljava/lang/String;)V"))
		{
			jstring jmsg = t.env->NewStringUTF(msg.c_str());
			t.env->CallStaticObjectMethod(t.classID, t.methodID,jmsg);
		}
#endif
	}

extern "C"
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	void Java_org_cocos2dx_cpp_JniHelper_SetLoginCode(JNIEnv* env, jobject obj,jstring jmessage)
	{
		const char* c_sessionId=env->GetStringUTFChars(jmessage,NULL);
		string message = c_sessionId;
		GameLogic::GetInstance()->loginCode = message;
		log("%s",message.c_str());
		delete c_sessionId;
	}
#endif
}


