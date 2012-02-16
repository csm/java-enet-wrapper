/*
 * org_bespin_enet_Packet.c
 *
 *  Created on: Feb 15, 2012
 *      Author: csm
 */

#include <jni.h>
#include <org_bespin_enet_Packet.h>
#include <enet/enet.h>

/*
 * Class:     org_bespin_enet_Packet
 * Method:    create
 * Signature: (Ljava/nio/ByteBuffer;I)Ljava/nio/ByteBuffer;
 */
JNIEXPORT jobject JNICALL Java_org_bespin_enet_Packet_create
  (JNIEnv *env, jclass clazz, jobject data, jint flags)
{
	void *ptr;
	int length;
	ENetPacket *packet;

	ptr = (*env)->GetDirectBufferAddress(env, data);
	length = (*env)->GetDirectBufferCapacity(env, data);

	packet = enet_packet_create(ptr, length, flags | ENET_PACKET_FLAG_NO_ALLOCATE);
	if (packet == NULL)
	{
		(*env)->ThrowNew(env, (*env)->FindClass(env, "org/bespin/enet/EnetException"), "failed to allocate packet");
		return NULL;
	}

	return (*env)->NewDirectByteBuffer(env, packet, sizeof(ENetPacket));
}

/*
 * Class:     org_bespin_enet_Packet
 * Method:    get_bytes
 * Signature: (Ljava/nio/ByteBuffer;)Ljava/nio/ByteBuffer;
 */
JNIEXPORT jobject JNICALL Java_org_bespin_enet_Packet_get_1bytes
  (JNIEnv *env, jclass cls, jobject ctx)
{
	ENetPacket *packet = (ENetPacket *) (*env)->GetDirectBufferAddress(env, ctx);
	return (*env)->NewDirectByteBuffer(env, packet->data, packet->dataLength);
}

/*
 * Class:     org_bespin_enet_Packet
 * Method:    get_flags
 * Signature: (Ljava/nio/ByteBuffer;)I
 */
JNIEXPORT jint JNICALL Java_org_bespin_enet_Packet_get_1flags
  (JNIEnv *env, jclass cls, jobject ctx)
{
	ENetPacket *packet = (ENetPacket *) (*env)->GetDirectBufferAddress(env, ctx);
	return packet->flags;
}

/*
 * Class:     org_bespin_enet_Packet
 * Method:    destroy
 * Signature: (Ljava/nio/ByteBuffer;)V
 */
JNIEXPORT void JNICALL Java_org_bespin_enet_Packet_destroy
  (JNIEnv *env, jclass cls, jobject ctx)
{
	ENetPacket *packet = (ENetPacket *) (*env)->GetDirectBufferAddress(env, ctx);
	enet_packet_destroy(packet);
}
