package org.bespin.enet.test;

import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.UnknownHostException;
import java.nio.charset.Charset;
import java.util.EnumSet;
import java.util.Queue;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.concurrent.LinkedBlockingQueue;

import org.bespin.enet.EnetException;
import org.bespin.enet.Event;
import org.bespin.enet.Host;
import org.bespin.enet.Packet;
import org.bespin.enet.Peer;

public class SelfTest
{
    static Queue<Event> eventQueue1 = new LinkedBlockingQueue<Event>(100);
    static Queue<Event> eventQueue2 = new LinkedBlockingDeque<Event>(100);

    /**
     * @param args
     * @throws EnetException 
     * @throws UnknownHostException 
     * @throws InterruptedException 
     */
    public static void main(String[] args) throws EnetException, UnknownHostException, InterruptedException
    {
        final Host host1 = new Host(new InetSocketAddress(8880), 10, 1, 0, 0);
        final Host host2 = new Host(new InetSocketAddress(8881), 10, 1, 0, 0);
        
        Thread serviceThread1 = new Thread(new Runnable(){
            @Override
            public void run()
            {
                for (;;)
                {
                    try
                    {
                        Event event = host1.service(1000);
                        if (event == null)
                            continue;
                        System.out.printf("host 1 event; type: %s, peer: %s, data: %d, channel: %d, packet: %s",
                                          event.type(), event.peer(), event.data(), event.channelID(),
                                          Charset.forName("UTF-8").decode(event.packet().getBytes()).toString());
                    } catch (EnetException e)
                    {
                        // TODO Auto-generated catch block
                        e.printStackTrace();
                        break;
                    }
                }
            }
        });
        Thread serviceThread2 = new Thread(new Runnable(){
            @Override
            public void run()
            {
                for (;;)
                {
                    try
                    {
                        Event event = host2.service(1000);
                        if (event == null)
                            continue;
                        System.out.printf("host 2 event; type: %s, peer: %s, data: %d, channel: %d, packet: %s",
                                          event.type(), event.peer(), event.data(), event.channelID(),
                                          Charset.forName("UTF-8").decode(event.packet().getBytes()).toString());
                        if (event.type() == Event.Type.Receive)
                        {
                            Packet reply = new Packet(event.packet().getBytes(), EnumSet.of(Packet.Flag.RELIABLE));
                            Peer peer = event.peer();
                            peer.send(event.channelID(), reply);
                        }
                    }
                    catch (EnetException e)
                    {
                        // TODO Auto-generated catch block
                        e.printStackTrace();
                        break;
                    }
                }
            }
        });
        
        serviceThread1.start();
        serviceThread2.start();
        
        Peer peer = host1.connect(new InetSocketAddress(InetAddress.getLocalHost(), 8881), 1, 5);
        Charset utf = Charset.forName("UTF-8");
        Packet packet = new Packet(utf.encode("test 1"), EnumSet.of(Packet.Flag.RELIABLE));
        peer.send(0, packet);
        packet = new Packet(utf.encode("test 2"), EnumSet.of(Packet.Flag.RELIABLE));
        peer.send(0, packet);
        packet = new Packet(utf.encode("test 3"), EnumSet.of(Packet.Flag.RELIABLE));
        peer.send(0, packet);
        packet = new Packet(utf.encode("test 4"), EnumSet.of(Packet.Flag.RELIABLE));
        peer.send(0, packet);
        packet = new Packet(utf.encode("test 5"), EnumSet.of(Packet.Flag.RELIABLE));
        peer.send(0, packet);
        
        Thread.sleep(5000);
        System.exit(0);
    }

}
