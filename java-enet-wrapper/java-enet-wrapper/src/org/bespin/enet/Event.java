package org.bespin.enet;

import java.nio.ByteBuffer;

public class Event
{
    ByteBuffer nativeState;
    
    public static enum Type
    {
        None,
        Connect,
        Disconnect,
        Receive
    }
    
    Event()
    {
        this(ByteBuffer.allocateDirect(sizeof()));
    }
    
    Event(ByteBuffer nativeState)
    {
        this.nativeState = nativeState;
    }
    
    public Peer peer()
    {
        ByteBuffer peer = peer(nativeState);
        if (peer == null)
            return null;
        return new Peer(peer);
    }
    
    public Type type()
    {
        return type(nativeState);
    }
    
    public int channelID()
    {
        return channelID(nativeState) & 0xFF;
    }
    
    public int data()
    {
        return data(nativeState);
    }
    
    public Packet packet()
    {
        ByteBuffer packet = packet(nativeState);
        if (packet == null)
            return null;
        return new Packet(packet);
    }
    
    private static native int sizeof();
    private static native ByteBuffer peer(ByteBuffer ctx);
    private static native Type type(ByteBuffer ctx);
    private static native byte channelID(ByteBuffer ctx);
    private static native int data(ByteBuffer ctx);
    private static native ByteBuffer packet(ByteBuffer ctx);
}
