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
	// Use this for initialization
	void Start () {
		NMHello h = NMHello.CreateBuilder()
			.SetVersion(1000)
			.Build();
		m = NetMessage.CreateBuilder()
			.SetId(NetMessage.Types.MessageType.HELLO)
			.SetHello(h)
			.Build();
	}
	
	// Update is called once per frame
	void Update () { 
	}
	
	void OnGUI() {
		GUI.Label(new Rect(10, 50, 100, 20), "asdasda" );
		
		NMHello h = m.Hello;
		int i = h.Version;
		GUI.Label(new Rect(10, 10, 100, 20), m.ToString() );
		GUI.Label(new Rect(10, 20, 100, 20), h.ToString() );
	}
}
