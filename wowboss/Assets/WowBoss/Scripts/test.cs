using UnityEngine;
using System;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using Google.ProtocolBuffers;
using System.Text;
using System.Collections;
using System.Collections.Generic;


using wowboss;


public class test : MonoBehaviour {
	
	EatAppleSmp smp;
	NetMessage m;
	int y;
	
	TcpClient m_client;
	NetworkStream ws;
	byte[] msgLenBuffer = new byte[4];
	Int32 msgLenBufferIndex = 0;
	byte[] recvBuffer = new byte[1024];
	Int32 recvBufferIndex = 0;
	
	NetMessage mRecv;
	// Use this for initialization
	void Start () {
		NMHello_SYN h = NMHello_SYN.CreateBuilder()
			.SetVersion(1000)
			.SetUsername( GameData.Account )
			.SetPassword( GameData.Password )
			.Build();
		m = NetMessage.CreateBuilder()
			.SetId(NetMessage.Types.MessageType.HELLO_SYN)
			.SetHelloSyn(h)
			.Build();
		
		
		
		//Thread t1 = new Thread( new ThreadStart( Thread1 ) );
		//t1.Start();
		//smp = new EatAppleSmp();
		Connect();
	}
	
	// Update is called once per frame
	void Update () { 
		Recv();
	}
	
	void OnGUI() {
		y = 10;
		GUIShowString( "asdasda" );
		
		NMHello_SYN h = m.HelloSyn;
		int i = h.Version;
		GUIShowString( m.ToString() );
		GUIShowString( h.ToString() );
		
		LocalHostInfo();
	}
	
	public static void Thread1() {
		Debug.Log("Hell");
	}
	
	void LocalHostInfo() {
		string strHostName = Dns.GetHostName();
		GUIShowString( strHostName );
		IPHostEntry ipEntry = Dns.GetHostEntry(strHostName);
		IPAddress[] addr = ipEntry.AddressList;
		
		GUIShowString( addr.Length.ToString() );
		if ( addr.Length > 0 )
		{
			GUIShowString( addr[0].ToString() );
			GUIShowString( addr[0].AddressFamily.ToString() );
		}
	}
	
	void GUIShowString(string s) {
		GUI.Label(new Rect(10, y, 100, 20), s );
		y += 15;
	}	
	
	void Connect() {
		m_client = new TcpClient();
		m_client.Connect( "127.0.0.1", 6000 );
		ws = m_client.GetStream();
		
		Int32 len = m.SerializedSize + 4;
		byte[] result = BitConverter.GetBytes(len);
		Debug.Log(len);		
		Debug.Log(result.Length);
		ws.Write( result, 0, result.Length );
		m.WriteTo( ws );
//		byte[] buffer = new byte[1024];
//		int r = ws.Read( buffer, 0, buffer.Length );
//		
//		NetMessage.ParseFrom(byte[] data);
	}
	
	void Recv() {
		if ( ws != null )
		{
			if ( ws.CanRead )
			{
				if ( ws.DataAvailable )
				{
					Debug.Log( "can read===================================" );
					Debug.Log( msgLenBufferIndex );
					Debug.Log( msgLenBuffer.Length );
					if ( msgLenBufferIndex < msgLenBuffer.Length )
					{
						int r = ws.Read( msgLenBuffer, msgLenBufferIndex, msgLenBuffer.Length-msgLenBufferIndex );
						Debug.Log( r );
						msgLenBufferIndex += r;		
	
					}
					
					if ( msgLenBufferIndex == msgLenBuffer.Length )
					{
						int msgLen = BitConverter.ToInt32( msgLenBuffer, 0 );
						msgLen -= 4;
						
						Debug.Log( msgLen );
						if ( recvBufferIndex < msgLen )
						{
							int r = ws.Read( recvBuffer, recvBufferIndex, msgLen-recvBufferIndex );
							recvBufferIndex += r;				
						}
						Debug.Log( recvBufferIndex );
						if ( recvBufferIndex == msgLen )
						{
							msgLenBufferIndex = 0;
							recvBufferIndex = 0;
							
							byte[] newBuffer = new byte[msgLen];

							for (int i = 0; i < newBuffer.Length; i++)
							{
								newBuffer[i] = recvBuffer[i];
							}
							
							mRecv = NetMessage.ParseFrom( newBuffer );
							
							Debug.Log( mRecv.ToString() );
							
						}
					}
				}

			}
		}
	}
}

public class EatAppleSmp
{
	public EatAppleSmp() {
		Thread th_mother, th_father, th_young, th_middle, th_old;
		Dish dish = new Dish( this, 30 );
		Productor mother = new Productor( "mother", dish );
		Productor father = new Productor( "father", dish );
		Consumer old = new Consumer( "old", dish, 1000 );
		Consumer middle = new Consumer( "middle", dish, 1200 );
		Consumer young = new Consumer( "young", dish, 1500 );
		th_mother = new Thread( new ThreadStart( mother.run ) );
		th_father = new Thread( new ThreadStart( father.run ) );
		th_old = new Thread( new ThreadStart( old.run ) );
		th_middle = new Thread( new ThreadStart( middle.run ) );
		th_young = new Thread( new ThreadStart( young.run ) );
		th_mother.Priority = System.Threading.ThreadPriority.Highest;
		th_father.Priority = System.Threading.ThreadPriority.Normal;
		th_old.Priority = System.Threading.ThreadPriority.Lowest;
		th_middle.Priority = System.Threading.ThreadPriority.Normal;
		th_young.Priority = System.Threading.ThreadPriority.Highest;
		th_mother.Start();
		th_father.Start();
		th_old.Start();
		th_middle.Start();
		th_young.Start();
		
	}
}

public class Dish
{
	int f = 5;
	EatAppleSmp oEAP;
	int EnabledNum;
	int n = 0;
	public Dish(EatAppleSmp oxEAP, int EnabledNum) {
		this.oEAP = oxEAP;
		this.EnabledNum = EnabledNum;
	}
	
	public void put(string name) {
		lock( this )
		{
			while ( f == 0 )
			{
				try
				{
					Debug.Log( name + "wait for put..." );
					Monitor.Wait( this ); 
				}
				catch(ThreadInterruptedException){}
			}
			f = f - 1;
			n = n + 1;
			Debug.Log( name + "put an apple" );
			Monitor.PulseAll(this);
			if ( n > EnabledNum	 )
				Thread.CurrentThread.Abort();
		}		
	}
	
	public void get(string name)
	{
		lock(this)
		{
			while( f == 5 )
			{
				try
				{
					Debug.Log( name + "wait for get..." );
					Monitor.Wait(this);
				}
				catch(ThreadInterruptedException){}
			}
			f = f + 1;
			Debug.Log( name + "get an apple" );
			Monitor.PulseAll(this);
		}
	}
}

class Productor
{
	private Dish dish;
	private string name;
	public Productor(string name, Dish dish)
	{
		this.name = name;
		this.dish = dish;
	}
	public void run()
	{
		while ( true )
		{
			dish.put( name );
			try
			{
				Thread.Sleep( 600 );
			}
			catch(ThreadInterruptedException){}
		}
	}
}

class Consumer
{
	private string name;
	private Dish dish;
	private int timelong;
	public Consumer(string name, Dish dish, int timelong)
	{
		this.name = name;
		this.dish = dish;
		this.timelong = timelong;
	}
	public void run()
	{
		while( true )
		{
			dish.get( name );
			try
			{
				Thread.Sleep( timelong );
			}
			catch(ThreadInterruptedException){}
		}
	}
}
