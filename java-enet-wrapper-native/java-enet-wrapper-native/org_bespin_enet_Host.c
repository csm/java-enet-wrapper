/*
 * org_bespin_enet_Host.c
 *
 *  Created on: Feb 16, 2012
 *      Author: csm
 */

#include <jni.h>
#include <enet/enet.h>
#include <org_bespin_enet_Host.h>

/*
 * Class:     org_bespin_enet_Host
 * Method:    create
 * Signature: (IIIIII)Ljava/nio/ByteBuffer;
 */
JNIEXPORT jobject JNICALL Java_org_bespin_enet_Host_create
  (JNIEnv *env, jclass cls, jint address, jint port, jint peerCount, jint channelCount, jint inbw, jint outbw)
{
	const ENetAddress addr = { (enet_uint32) address, (enet_uint16) port };
	ENetHost *host = enet_host_create(&addr, peerCount, channelCount, inbw, outbw);
	if (host == NULL)
	{
		(*env)->ThrowNew(env, (*env)->FindClass(env, "org/bespin/enet/EnetException"), "failed to create enet host");
		return NULL;
	}
	return (*env)->NewDirectByteBuffer(env, host, sizeof(ENetHost));
}

/*
 * Class:     org_bespin_enet_Host
 * Method:    connect
 * Signature: (Ljava/nio/ByteBuffer;IIII)Ljava/nio/ByteBuffer;
 */
JNIEXPORT jobject JNICALL Java_org_bespin_enet_Host_connect
  (JNIEnv *env, jclass cls, jobject ctx, jint address, jint port, jint channelCount, jint data)
{
	ENetHost *host = (ENetHost *) (*env)->GetDirectBufferAddress(env, ctx);
	if (host != NULL)
	{
		const ENetAddress addr = { (enet_uint32) address, (enet_uint16) port };
		ENetPeer *peer = enet_host_connect(host, &addr, channelCount, data);
		if (peer == NULL)
		{
			(*env)->ThrowNew(env, (*env)->FindClass(env, "org/bespin/enet/EnetException"), "failed to connect");
			return NULL;
		}
		return (*env)->NewDirectByteBuffer(env, peer, sizeof(ENetPeer));
	}
	return NULL;
}

/*
 * Class:     org_bespin_enet_Host
 * Method:    broadcast
 * Signature: (Ljava/nio/ByteBuffer;ILjava/nio/ByteBuffer;)V
 */
JNIEXPORT void JNICALL Java_org_bespin_enet_Host_broadcast
  (JNIEnv *env, jclass cls, jobject ctx, jint channel, jobject packet)
{
	ENetHost *host = (ENetHost *) (*env)->GetDirectBufferAddress(env, ctx);
	ENetPacket *p = (ENetPacket *) (*env)->GetDirectBufferAddress(env, packet);
	if (host != NULL && p != NULL)
		enet_host_broadcast(host, channel, p);
}

/*
 * Class:     org_bespin_enet_Host
 * Method:    channel_limit
 * Signature: (Ljava/nio/ByteBuffer;I)V
 */
JNIEXPORT void JNICALL Java_org_bespin_enet_Host_channel_1limit
  (JNIEnv *env, jclass cls, jobject ctx, jint channelLimit)
{
	ENetHost *host = (ENetHost *) (*env)->GetDirectBufferAddress(env, ctx);
	if (host != NULL)
		enet_host_channel_limit(host, channelLimit);
}

/*
 * Class:     org_bespin_enet_Host
 * Method:    bandwidth_limit
 * Signature: (Ljava/nio/ByteBuffer;II)V
 */
JNIEXPORT void JNICALL Java_org_bespin_enet_Host_bandwidth_1limit
  (JNIEnv *env, jclass cls, jobject ctx, jint inbw, jint outbw)
{
	ENetHost *host = (ENetHost *) (*env)->GetDirectBufferAddress(env, ctx);
	if (host != NULL)
		enet_host_bandwidth_limit(host, inbw, outbw);
}

/*
 * Class:     org_bespin_enet_Host
 * Method:    flush
 * Signature: (Ljava/nio/ByteBuffer;)V
 */
JNIEXPORT void JNICALL Java_org_bespin_enet_Host_flush
  (JNIEnv *env, jclass cls, jobject ctx)
{
	ENetHost *host = (ENetHost *) (*env)->GetDirectBufferAddress(env, ctx);
	if (host != NULL)
		enet_host_flush(host);
}

/*
 * Class:     org_bespin_enet_Host
 * Method:    checkEvents
 * Signature: (Ljava/nio/ByteBuffer;Ljava/nio/ByteBuffer;)I
 */
JNIEXPORT jint JNICALL Java_org_bespin_enet_Host_checkEvents
  (JNIEnv *env, jclass cls, jobject ctx, jobject ev)
{
	ENetHost *host = (ENetHost *) (*env)->GetDirectBufferAddress(env, ctx);
	if (host != NULL)
	{
		ENetEvent *event = NULL;
		if (ev != NULL)
			event = (ENetEvent *) (*env)->GetDirectBufferAddress(env, ev);
		int ret = enet_host_check_events(host, event);
		if (ret < 0)
		{
			(*env)->ThrowNew(env, (*env)->FindClass(env, "net/bespin/enet/EnetException"), "failed to check events");
			return -1;
		}
		return ret;
	}
	return 0;
}

/*
 * Class:     org_bespin_enet_Host
 * Method:    service
 * Signature: (Ljava/nio/ByteBuffer;ILjava/nio/ByteBuffer;)I
 */
JNIEXPORT jint JNICALL Java_org_bespin_enet_Host_service
  (JNIEnv *env, jclass cls, jobject ctx, jint timeout, jobject ev)
{
	ENetHost *host = (ENetHost *) (*env)->GetDirectBufferAddress(env, ctx);
	if (host != NULL)
	{
		ENetEvent *event = NULL;
		if (ev != NULL)
			event = (ENetEvent *) (*env)->GetDirectBufferAddress(env, ev);
		int ret = enet_host_service(host, event, timeout);
		if (ret < 0)
		{
			(*env)->ThrowNew(env, (*env)->FindClass(env, "net/bespin/enet/EnetException"), "failed to service host");
			return -1;
		}
		return ret;
	}
	return 0;
}

/*
 * Class:     org_bespin_enet_Host
 * Method:    destroy
 * Signature: (Ljava/nio/ByteBuffer;)V
 */
JNIEXPORT void JNICALL Java_org_bespin_enet_Host_destroy
  (JNIEnv *env, jclass cls, jobject ctx)
{
	ENetHost *host = (ENetHost *) (*env)->GetDirectBufferAddress(env, ctx);
	if (host != NULL)
		enet_host_destroy(host);
}

