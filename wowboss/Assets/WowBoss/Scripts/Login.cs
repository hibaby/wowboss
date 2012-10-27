using UnityEngine;
using System.Collections;

public class Login : MonoBehaviour {
	
	public GameObject account;
	public GameObject password;
	
	float mLastClick = 0f;
	bool mStarted = false;
	bool mHighlighted = false;

	void Start () { mStarted = true; }

	void OnEnable () { if (mStarted && mHighlighted) OnHover(UICamera.IsHighlighted(gameObject)); }

	void OnHover (bool isOver)
	{
		if (enabled)
		{
//			if (((isOver && trigger == Trigger.OnMouseOver) ||
//				(!isOver && trigger == Trigger.OnMouseOut))) Send();
			mHighlighted = isOver;
		}
	}

	void OnPress (bool isPressed)
	{
		if (enabled)
		{
			/*
			if (((isPressed && trigger == Trigger.OnPress) ||
				(!isPressed && trigger == Trigger.OnRelease)))
			{
				Send();
			}
			*/
		}
	}

	void OnClick ()
	{
		if (enabled)
		{
			float time = Time.realtimeSinceStartup;

			if (mLastClick + 0.2f > time)
			{
				//if (trigger == Trigger.OnDoubleClick)
				
			}
			//else if (trigger == Trigger.OnClick)
			//{
			//}
			GameData.Account = account.GetComponent<UILabel>().text;
			GameData.Password = password.GetComponent<UILabel>().text;
						
			Application.LoadLevel("test");
			
			
			
			mLastClick = time;
		}
	}
}
