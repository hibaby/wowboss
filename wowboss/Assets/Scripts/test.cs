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

	NetMessage m;
	int y;
	// Use this for initialization
	void Start () {
		NMHello h = NMHello.CreateBuilder()
			.SetVersion(1000)
			.Build();
		m = NetMessage.CreateBuilder()
			.SetId(NetMessage.Types.MessageType.HELLO)
			.SetHello(h)
			.Build();
		
		Thread t1 = new Thread( new ThreadStart( Thread1 ) );
		t1.Start();
	}
	
	// Update is called once per frame
	void Update () { 
	}
	
	void OnGUI() {
		y = 10;
		GUIShowString( "asdasda" );
		
		NMHello h = m.Hello;
		int i = h.Version;
		GUIShowString( m.ToString() );
		GUIShowString( h.ToString() );
		
		LocalHostInfo();
	}
	
	public static void Thread1() {
		Debug.Log("Hello");
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
}
