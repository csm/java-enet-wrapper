/*
 * org_bespin_enet_Event.c
 *
 *  Created on: Feb 16, 2012
 *      Author: csm
 */

#include <jni.h>
#include <enet/enet.h>
#include <org_bespin_enet_Event.h>

/*
 * Class:     org_bespin_enet_Event
 * Method:    sizeof
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_bespin_enet_Event_sizeof
  (JNIEnv *env, jclass cls)
{
	return sizeof(ENetEvent);
}

/*
 * Class:     org_bespin_enet_Event
 * Method:    peer
 * Signature: (Ljava/nio/ByteBuffer;)Ljava/nio/ByteBuffer;
 */
JNIEXPORT jobject JNICALL Java_org_bespin_enet_Event_peer
  (JNIEnv *env, jclass cls, jobject ctx)
{
	ENetEvent *event = (*env)->GetDirectBufferAddress(env, ctx);
	if (event != NULL)
	{
		if (event->peer != NULL)
			return (*env)->NewDirectByteBuffer(env, event->peer, sizeof(ENetPeer));
	}
	return NULL;
}

/*
 * Class:     org_bespin_enet_Event
 * Method:    type
 * Signature: (Ljava/nio/ByteBuffer;)Lorg/bespin/enet/Event/Type;
 */
JNIEXPORT jobject JNICALL Java_org_bespin_enet_Event_type
  (JNIEnv *env, jclass cls, jobject ctx)
{
	ENetEvent *event = (*env)->GetDirectBufferAddress(env, ctx);
	if (event != NULL)
	{
		jclass enumClass = (*env)->FindClass(env, "org/bespin/enet/Event$Type");
		if (enumClass == NULL)
			return NULL;
		jfieldID field = NULL;
		switch (event->type)
		{
		case ENET_EVENT_TYPE_CONNECT:
			field = (*env)->GetStaticFieldID(env, enumClass, "Connect", "Lorg/bespin/enet/Event$Type;");
			break;

		case ENET_EVENT_TYPE_DISCONNECT:
			field = (*env)->GetStaticFieldID(env, enumClass, "Disconnect", "Lorg/bespin/enet/Event$Type;");
			break;

		case ENET_EVENT_TYPE_NONE:
			field = (*env)->GetStaticFieldID(env, enumClass, "None", "Lorg/bespin/enet/Event$Type;");
			break;

		case ENET_EVENT_TYPE_RECEIVE:
			field = (*env)->GetStaticFieldID(env, enumClass, "Receive", "Lorg/bespin/enet/Event$Type;");
			break;
		}

		if (field != NULL)
		{
			return (*env)->GetStaticObjectField(env, enumClass, field);
		}
	}
	return NULL;
}

/*
 * Class:     org_bespin_enet_Event
 * Method:    channelID
 * Signature: (Ljava/nio/ByteBuffer;)B
 */
JNIEXPORT jbyte JNICALL Java_org_bespin_enet_Event_channelID
  (JNIEnv *env, jclass cls, jobject ctx)
{
	ENetEvent *event = (*env)->GetDirectBufferAddress(env, ctx);
	if (event == NULL)
		return 0;
	return (jbyte) event->channelID;
}

/*
 * Class:     org_bespin_enet_Event
 * Method:    data
 * Signature: (Ljava/nio/ByteBuffer;)I
 */
JNIEXPORT jint JNICALL Java_org_bespin_enet_Event_data
  (JNIEnv *env, jclass cls, jobject ctx)
{
	ENetEvent *event = (*env)->GetDirectBufferAddress(env, ctx);
	if (event == NULL)
		return 0;
	return event->data;
}

/*
 * Class:     org_bespin_enet_Event
 * Method:    packet
 * Signature: (Ljava/nio/ByteBuffer;)Ljava/nio/ByteBuffer;
 */
JNIEXPORT jobject JNICALL Java_org_bespin_enet_Event_packet
  (JNIEnv *env, jclass cls, jobject ctx)
{
	ENetEvent *event = (*env)->GetDirectBufferAddress(env, ctx);
	if (event != NULL)
	{
		if (event->packet != NULL)
			return (*env)->NewDirectByteBuffer(env, event->packet, sizeof(ENetPacket));
	}
	return NULL;
}
