#include <jni.h>
#include <string>
#include "Playstatus.h"
#include "FFmpeg.h"
extern "C"
{
   #include <libavformat/avformat.h>
}

_JavaVM  *javaVM=NULL;
CallJava   *callJava=NULL;
FFmpeg  *fFmpeg=NULL;
Playstatus  *playstatus=NULL;

bool   nexit= true;
pthread_t   thread_start;



extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
    jint result = -1;
    javaVM = vm;
    JNIEnv *env;
    if(vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK)
    {
        return result;
    }
    return JNI_VERSION_1_4;

}

extern "C"
JNIEXPORT void JNICALL
Java_com_yzg_audioplayer_UniversalPlayer_n_1parpared(JNIEnv *env, jobject instance, jstring source_) {
    const char *source = env->GetStringUTFChars(source_, 0);
    // TODO
    if(fFmpeg==NULL)
    {
        if(callJava==NULL){
            callJava=new CallJava(javaVM,env,&instance);
        }
        callJava->onCallLoad(MAIN_THREAD, true);
        playstatus=new Playstatus( );
        fFmpeg=new FFmpeg(playstatus,callJava,source);
        fFmpeg->parpared();
    }
}

void  *startCallBack(void *data){
    FFmpeg *fFmpeg= (FFmpeg *) data;
    fFmpeg->start();
    pthread_exit(&thread_start);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_yzg_audioplayer_UniversalPlayer_n_1start(JNIEnv *env, jobject instance) {
    // TODO
    if(fFmpeg!=NULL){
        pthread_create(&thread_start, NULL, startCallBack, fFmpeg);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_yzg_audioplayer_UniversalPlayer_n_1pause(JNIEnv *env, jobject instance) {
    // TODO
    if(fFmpeg!=NULL){
        fFmpeg->pause();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_yzg_audioplayer_UniversalPlayer_n_1resume(JNIEnv *env, jobject instance) {
    // TODO
    if(fFmpeg!=NULL){
        fFmpeg->resume();
    }
}


extern "C"
JNIEXPORT jboolean JNICALL
Java_com_yzg_audioplayer_UniversalPlayer_n_1cutaudioplay(JNIEnv *env, jobject instance, jint start_time, jint end_time, jboolean showPcm) {
    // TODO
      if(fFmpeg!=NULL){
          return  fFmpeg->cutAudioPlay(start_time,end_time,showPcm);
      }
    return false;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_yzg_audioplayer_UniversalPlayer_n_1startstoprecord(JNIEnv *env, jobject instance, jboolean start) {
    // TODO
     if(fFmpeg!=NULL){
         fFmpeg->startStopRecord(start);
     }
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_yzg_audioplayer_UniversalPlayer_n_1samplerate(JNIEnv *env, jobject instance) {
    // TODO
    if(fFmpeg!=NULL){
        return fFmpeg->getSampleRate();
    }
    return 0;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_yzg_audioplayer_UniversalPlayer_n_1speed(JNIEnv *env, jobject instance, jfloat speed) {
    // TODO
     if(fFmpeg!=NULL){
         fFmpeg->setSpeed(speed);
     }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_yzg_audioplayer_UniversalPlayer_n_1pitch(JNIEnv *env, jobject instance, jfloat pitch) {
    // TODO
     if(fFmpeg!=NULL){
         fFmpeg->setPitch(pitch);
     }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_yzg_audioplayer_UniversalPlayer_n_1mute(JNIEnv *env, jobject instance, jint mute) {

    // TODO
   if (fFmpeg!=NULL){
       fFmpeg->setMute(mute);
   }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_yzg_audioplayer_UniversalPlayer_n_1volume(JNIEnv *env, jobject instance, jint percent) {
    // TODO
    if(fFmpeg!=NULL){
        fFmpeg->setVolume(percent);
    }
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_yzg_audioplayer_UniversalPlayer_n_1duration(JNIEnv *env, jobject instance) {
    // TODO
    if(fFmpeg!=NULL){
        fFmpeg->duration;
    }
    return 0;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_yzg_audioplayer_UniversalPlayer_n_1seek(JNIEnv *env, jobject instance, jint secds) {
    // TODO
    if(fFmpeg!=NULL){
        fFmpeg->seek(secds);
    }
}



extern "C"
JNIEXPORT void JNICALL
Java_com_yzg_audioplayer_UniversalPlayer_n_1stop(JNIEnv *env, jobject instance) {
    // TODO
   if(!nexit){
       return;
   }
   jclass  clz=env->GetObjectClass(instance);
   jmethodID  jmid_next=env->GetMethodID(clz,"onCallNext","()V");
   nexit= false;
   if(fFmpeg!=NULL){
       fFmpeg->release();
       delete(fFmpeg);
       fFmpeg=NULL;
       if(callJava!=NULL){
           delete(callJava);
           callJava=NULL;
       }
       if(playstatus!=NULL){
           delete (playstatus);
           playstatus=NULL;
       }
   }
   nexit= true;
   env->CallVoidMethod(instance,jmid_next);
}








