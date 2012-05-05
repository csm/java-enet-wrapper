package org.bespin.enet.test;

import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.UnknownHostException;
import java.nio.charset.Charset;
import java.util.EnumSet;
import java.util.Queue;
import java.util.concurrent.CountDownLatch;
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
        final int msgCount = 100;
        final CountDownLatch connectLatch = new CountDownLatch(2);
        final CountDownLatch receiveLatch = new CountDownLatch(2 * msgCount);
        final Host host1 = new Host(new InetSocketAddress(Inet4Address.getLocalHost(), 8880), 10, 1, 0, 0);
        final Host host2 = new Host(new InetSocketAddress(Inet4Address.getLocalHost(), 8881), 10, 1, 0, 0);
        
        Thread serviceThread1 = new Thread(new Runnable(){
            @Override
            public void run()
            {
                for (;;)
                {
                    try
                    {
                        Event event = host1.service(100);
                        if (event == null)
                            continue;
                        Packet packet = event.packet();
                        System.out.printf("host 1 event; type: %s, peer: %s, data: %d, channel: %d, packet: %s%n",
                                          event.type(), event.peer(), event.data(), event.channelID(),
                                          packet != null ? Charset.forName("UTF-8").decode(packet.getBytes()).toString() : null);
                        if (event.type() == Event.Type.Connect)
                            connectLatch.countDown();
                        if (event.type() == Event.Type.Receive)
                            receiveLatch.countDown();
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
                        Event event = host2.service(100);
                        if (event == null)
                            continue;
                        Packet packet = event.packet();
                        System.out.printf("host 2 event; type: %s, peer: %s, data: %d, channel: %d, packet: %s%n",
                                          event.type(), event.peer(), event.data(), event.channelID(),
                                          packet != null ? Charset.forName("UTF-8").decode(packet.getBytes()).toString() : null);
                        if (event.type() == Event.Type.Receive)
                        {
                            receiveLatch.countDown();
                            Packet reply = new Packet(event.packet().getBytes(), EnumSet.of(Packet.Flag.RELIABLE));
                            Peer peer = event.peer();
                            peer.send(event.channelID(), reply);
                        }
                        if (event.type() == Event.Type.Connect)
                            connectLatch.countDown();
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
        
        serviceThread1.setDaemon(true);
        serviceThread1.start();
        serviceThread2.setDaemon(true);
        serviceThread2.start();
        
        Peer peer = host1.connect(new InetSocketAddress(Inet4Address.getLocalHost(), 8881), 1, 5);
        connectLatch.await();
        Charset utf = Charset.forName("UTF-8");
        for (int i = 0; i < 100; i++)
        {
            Packet packet = new Packet(utf.encode("test " + i), EnumSet.of(Packet.Flag.RELIABLE));
            peer.send(0, packet);
        }

        receiveLatch.await();
        System.gc();
        System.exit(0);
    }

}
