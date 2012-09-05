package org.bespin.enet;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.concurrent.TimeUnit;

public class Host
{
    static
    {
        try
        {
            ClassLoader loader = Host.class.getClassLoader();
            InputStream in = loader.getResourceAsStream("libjava-enet-wrapper-native.so");
            File tmpdir = new File(System.getProperty("java.io.tmpdir"));
            File libout = new File(tmpdir, "libjava-enet-wrapper-native.so");
            FileOutputStream out = new FileOutputStream(libout);
            byte[] b = new byte[1024];
            int len = 0;
            while ((len = in.read(b)) != -1)
                out.write(b, 0, len);
            in.close();
            out.close();
            System.load(libout.getAbsolutePath());
        }
        catch (UnsatisfiedLinkError ule)
        {
            System.err.println("failed to load embedded native library!");
            ule.printStackTrace();
        }
        catch (IOException ioe)
        {
            System.err.println("failed to load embedded native library!");
            ioe.printStackTrace();
        }
        /*try
        {
            System.out.println("load from " + System.getProperty("java.library.path"));
            System.loadLibrary(System.mapLibraryName("java-enet-wrapper-native"));
        }
        catch (UnsatisfiedLinkError ule)
        {
            ule.printStackTrace();
            String lib = System.getProperty("enet.jnilib");
            System.load(lib);
        }*/
    }
    
    ByteBuffer nativeState;
    
    static int addressToInt(InetAddress address) throws EnetException
    {
        if (!(address instanceof Inet4Address))
            throw new EnetException("enet only supports IPv4");
        ByteBuffer buf = ByteBuffer.wrap(address.getAddress());
        buf.order(ByteOrder.nativeOrder());
        return buf.getInt(0);
    }
    
    public Host(InetSocketAddress address, int peerCount, int channelLimit, int incomingBandwidth, int outgoingBandwidth)
        throws EnetException
    {
        nativeState = create(addressToInt(address.getAddress()), address.getPort(), peerCount, channelLimit, incomingBandwidth, outgoingBandwidth);
    }
    
    public Peer connect(InetSocketAddress address, int channelCount, int data)
        throws EnetException
    {
        return new Peer(connect(nativeState, addressToInt(address.getAddress()), address.getPort(), channelCount, data));
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
