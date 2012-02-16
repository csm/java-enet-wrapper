package org.bespin.enet;

import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;

public class Peer
{
    ByteBuffer nativeState;
    
    Peer(ByteBuffer nativeState)
    {
        this.nativeState = nativeState;
    }
    
    public void throttleConfigure(int interval, int acceleration, int deceleration)
    {
        throttleConfigure(nativeState, interval, acceleration, deceleration);
    }
    
    public void send(int channelID, Packet packet)
        throws EnetException
    {
        send(nativeState, channelID, packet.nativeState);
    }
    
    public Packet receive(MutableInteger channelID) throws EnetException
    {
        ByteBuffer b = receive(nativeState, channelID);
        if (b == null)
            return null;
        return new Packet(b);
    }

    public void ping()
    {
        ping(nativeState);
    }
    
    public void disconnectNow(int data)
    {
        disconnect_now(nativeState, data);
    }
    
    public void disconnect(int data)
    {
        disconnect(nativeState, data);
    }
    
    public void disconnectLater(int data)
    {
        disconnect_later(nativeState, data);
    }
    
    public void reset()
    {
        reset(nativeState);
    }
    
    public InetSocketAddress address() throws UnknownHostException
    {
        return new InetSocketAddress(InetAddress.getByAddress(get_address(nativeState)), get_port(nativeState));
    }
    
    private static native void throttleConfigure(ByteBuffer ctx, int interval, int acceleration, int deceleration);
    private static native void send(ByteBuffer ctx, int channelID, ByteBuffer packet);
    private static native ByteBuffer receive(ByteBuffer ctx, MutableInteger channelID);
    private static native void ping(ByteBuffer ctx);
    private static native void disconnect_now(ByteBuffer ctx, int data);
    private static native void disconnect(ByteBuffer ctx, int data);
    private static native void disconnect_later(ByteBuffer ctx, int data);
    private static native byte[] get_address(ByteBuffer ctx);
    private static native int get_port(ByteBuffer ctx);
    private static native void reset(ByteBuffer ctx);
}
