#include <jni.h>
#include <stdio.h>
#include <freerdp/freerdp.h>

jstring Java_net_itskewpie_freerdp_FreeRDPActivity_test(JNIEnv* env, jobject thiz )
{

    //freerdp_new();
    android_main();
    //printf("%d",a);

    return (*env)->NewStringUTF(env, "HELLO~~~~~~~~~~~~~~~~~~~");
    /*
    androidfreerdp_new(
    printf("freerdp jni");
    return 
    */
}
