package org.bespin.enet;

import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.util.concurrent.TimeUnit;

public class Host
{
    static
    {
        System.loadLibrary(System.mapLibraryName("java-enet-wrapper-native"));
    }
    
    ByteBuffer nativeState;
    
    public Host(InetSocketAddress address, int peerCount, int channelLimit, int incomingBandwidth, int outgoingBandwidth)
        throws EnetException
    {
        byte[] b = address.getAddress().getAddress();
        int addr = ((b[0] & 0xFF) << 24) | ((b[1] & 0xFF) << 16) | ((b[2] & 0xFF) << 8) | (b[0] & 0xFF); 
        nativeState = create(addr, address.getPort(), peerCount, channelLimit, incomingBandwidth, outgoingBandwidth);
    }
    
    public Peer connect(InetSocketAddress address, int channelCount, int data)
        throws EnetException
    {
        byte[] b = address.getAddress().getAddress();
        int addr = ((b[0] & 0xFF) << 24) | ((b[1] & 0xFF) << 16) | ((b[2] & 0xFF) << 8) | (b[0] & 0xFF);
        return new Peer(connect(nativeState, addr, address.getPort(), channelCount, data));
    }
    
    public void broadcast(int channelID, Packet packet)
    {
        broadcast(nativeState, channelID, packet.nativeState);
    }
    
    public void channelLimit(int channelLimit)
    {
        channel_limit(nativeState, channelLimit);
    }
    
    public void bandwidthLimit(int incomingBandwidth, int outgoingBandwidth)
    {
        bandwidth_limit(nativeState, incomingBandwidth, outgoingBandwidth);
    }
    
    public void flush()
    {
        flush(nativeState);
    }
    
    public Event checkEvents() throws EnetException
    {
        Event event = new Event();
        int ret = checkEvents(nativeState, event.nativeState);
        if (ret <= 0)
            return null;
        return event;
    }
    
    public Event service(int timeout) throws EnetException
    {
        Event event = new Event();
        int ret = service(nativeState, timeout, event.nativeState);
        if (ret <= 0)
            return null;
        return event;
    }
    
    public Event service(long timeout, TimeUnit unit) throws EnetException
    {
        return service((int) TimeUnit.MILLISECONDS.convert(timeout, unit));
    }

    @Override
    protected void finalize() throws Throwable
    {
        destroy(nativeState);
        super.finalize();
    }

    private static native ByteBuffer create(int address, int port, int peerCount, int channelLimit, int incomingBandwidth, int outgoingBandwidth) throws EnetException;
    private static native ByteBuffer connect(ByteBuffer ctx, int address, int port, int channelCount, int data) throws EnetException;
    private static native void broadcast(ByteBuffer ctx, int channelID, ByteBuffer packet);
    private static native void channel_limit(ByteBuffer ctx, int channelLimit);
    private static native void bandwidth_limit(ByteBuffer ctx, int in, int out);
    private static native void flush(ByteBuffer ctx);
    private static native int checkEvents(ByteBuffer ctx, ByteBuffer event) throws EnetException;
    private static native int service(ByteBuffer ctx, int timeout, ByteBuffer event) throws EnetException;
    private static native void destroy(ByteBuffer ctx) throws EnetException;
}
