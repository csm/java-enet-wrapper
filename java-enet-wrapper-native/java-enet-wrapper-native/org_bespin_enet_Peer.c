/*
 * org_bespin_enet_Peer.c
 *
 *  Created on: Feb 15, 2012
 *      Author: csm
 */

#include <jni.h>
#include <enet/enet.h>
#include <org_bespin_enet_Peer.h>

/*
 * Class:     org_bespin_enet_Peer
 * Method:    throttleConfigure
 * Signature: (Ljava/nio/ByteBuffer;III)V
 */
JNIEXPORT void JNICALL Java_org_bespin_enet_Peer_throttleConfigure
  (JNIEnv *env, jclass cls, jobject ctx, jint interval, jint acceleration, jint deceleration)
{
	ENetPeer *peer = (ENetPeer *) (*env)->GetDirectBufferAddress(env, ctx);
	enet_peer_throttle_configure(peer, interval, acceleration, deceleration);
}

/*
 * Class:     org_bespin_enet_Peer
 * Method:    send
 * Signature: (Ljava/nio/ByteBuffer;ILjava/nio/ByteBuffer;)V
 */
JNIEXPORT void JNICALL Java_org_bespin_enet_Peer_send
  (JNIEnv *env, jclass cls, jobject ctx, jint channel, jobject packet)
{
	ENetPeer *peer = (ENetPeer *) (*env)->GetDirectBufferAddress(env, ctx);
	ENetPacket *_packet = (ENetPacket *) (*env)->GetDirectBufferAddress(env, packet);
	enet_peer_send(peer, (enet_uint8) channel, _packet);
}

/*
 * Class:     org_bespin_enet_Peer
 * Method:    receive
 * Signature: (Ljava/nio/ByteBuffer;I)Ljava/nio/ByteBuffer;
 */
JNIEXPORT jobject JNICALL Java_org_bespin_enet_Peer_receive
  (JNIEnv *env, jclass cls, jobject ctx, jobject channel)
{
	ENetPeer *peer = (ENetPeer *) (*env)->GetDirectBufferAddress(env, ctx);
	enet_uint8 channel_storage;
	ENetPacket *packet = enet_peer_receive(peer, &channel_storage);
	if (packet == NULL)
		return NULL;
	if (channel != NULL)
	{
		jmethodID setter = (*env)->GetMethodID(env, channel, "setValue", "(V)I");
		(*env)->CallVoidMethod(env, channel, setter, (jint) channel_storage);
	}
	return (*env)->NewDirectByteBuffer(env, packet, sizeof(ENetPacket));
}

/*
 * Class:     org_bespin_enet_Peer
 * Method:    ping
 * Signature: (Ljava/nio/ByteBuffer;)V
 */
JNIEXPORT void JNICALL Java_org_bespin_enet_Peer_ping
  (JNIEnv *env, jclass cls, jobject ctx)
{
	ENetPeer *peer = (ENetPeer *) (*env)->GetDirectBufferAddress(env, ctx);
	enet_peer_ping(peer);
}

/*
 * Class:     org_bespin_enet_Peer
 * Method:    disconnect_now
 * Signature: (Ljava/nio/ByteBuffer;)V
 */
JNIEXPORT void JNICALL Java_org_bespin_enet_Peer_disconnect_1now
  (JNIEnv *env, jclass cls, jobject ctx, jint data)
{
	ENetPeer *peer = (ENetPeer *) (*env)->GetDirectBufferAddress(env, ctx);
	enet_peer_disconnect_now(peer, data);
}

/*
 * Class:     org_bespin_enet_Peer
 * Method:    disconnect
 * Signature: (Ljava/nio/ByteBuffer;)V
 */
JNIEXPORT void JNICALL Java_org_bespin_enet_Peer_disconnect
(JNIEnv *env, jclass cls, jobject ctx, jint data)
{
	ENetPeer *peer = (ENetPeer *) (*env)->GetDirectBufferAddress(env, ctx);
	enet_peer_disconnect(peer, data);
}

/*
 * Class:     org_bespin_enet_Peer
 * Method:    disconnect_later
 * Signature: (Ljava/nio/ByteBuffer;)V
 */
JNIEXPORT void JNICALL Java_org_bespin_enet_Peer_disconnect_1later
(JNIEnv *env, jclass cls, jobject ctx, jint data)
{
	ENetPeer *peer = (ENetPeer *) (*env)->GetDirectBufferAddress(env, ctx);
	enet_peer_disconnect_later(peer, data);
}

/*
 * Class:     org_bespin_enet_Peer
 * Method:    get_address
 * Signature: (Ljava/nio/ByteBuffer;)[B
 */
JNIEXPORT jbyteArray JNICALL Java_org_bespin_enet_Peer_get_1address
  (JNIEnv *env, jclass cls, jobject ctx)
{
	ENetPeer *peer = (ENetPeer *) (*env)->GetDirectBufferAddress(env, ctx);
	jbyteArray array = (*env)->NewByteArray(env, 4);
	jbyte *buf = (*env)->GetByteArrayElements(env, array, NULL);
	buf[0] = (jbyte) (peer->address.host >> 24);
	buf[1] = (jbyte) (peer->address.host >> 16);
	buf[2] = (jbyte) (peer->address.host >>  8);
	buf[3] = (jbyte) peer->address.host;
	(*env)->ReleaseByteArrayElements(env, array, buf, JNI_COMMIT);
	return array;
}

/*
 * Class:     org_bespin_enet_Peer
 * Method:    get_port
 * Signature: (Ljava/nio/ByteBuffer;)I
 */
JNIEXPORT jint JNICALL Java_org_bespin_enet_Peer_get_1port
  (JNIEnv *env, jclass cls, jobject ctx)
{
	ENetPeer *peer = (ENetPeer *) (*env)->GetDirectBufferAddress(env, ctx);
	return peer->address.port & 0xFFFF;
}

/*
 * Class:     org_bespin_enet_Peer
 * Method:    reset
 * Signature: (Ljava/nio/ByteBuffer;)V
 */
JNIEXPORT void JNICALL Java_org_bespin_enet_Peer_reset
  (JNIEnv *env, jclass cls, jobject ctx)
{
	ENetPeer *peer = (ENetPeer *) (*env)->GetDirectBufferAddress(env, ctx);
	enet_peer_reset(peer);
}

