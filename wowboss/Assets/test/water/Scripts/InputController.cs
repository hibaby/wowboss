using UnityEngine;
using System.Collections;

public class InputController : MonoBehaviour {
	public enum Platform { PC=0, Mobile=1 };
	public Platform platform 			= Platform.PC;
	
	public Vector2 positionBegan 		= Vector2.zero;
	public Vector2 positionCurrent 	= Vector2.zero;

	private bool isTouched 		= false;
	private bool isTouchEnd 	= false;
	
	private bool bEnableFingerDistance 	= false;
	private float lastFingerDistance 	= 0.0f;
	private float fingerDistanceDelta 	= 0.0f;
	
	void Awake ()
	{
		if (Application.platform == RuntimePlatform.Android ||
			Application.platform == RuntimePlatform.IPhonePlayer)
		{
			platform = Platform.Mobile;
		}
		else if (Application.platform == RuntimePlatform.PS3 ||
				 Application.platform == RuntimePlatform.XBOX360)
		{
		}
		else if (Application.platform == RuntimePlatform.WindowsEditor ||
				 Application.platform == RuntimePlatform.OSXEditor)
		{
		}
	}
	
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		
		bool lastTouched = isTouched;
		UpdateTouchStatus ();
		isTouchEnd = false;
		if ( lastTouched )
		{
			if ( isTouched == false )
			{
				isTouchEnd = true;
			}
		}	
		
		if ( IsTouchEnd() )
		{
			if ( positionBegan == positionCurrent )
			{
				Ray ray = camera.ScreenPointToRay (positionCurrent);	
				RaycastHit hit;
				if (Physics.Raycast(ray, out hit)) 
				{
					if (hit.transform ) 
					{
						hit.transform.gameObject.SendMessage( "OnMouseUp" );
					}
				}
			}
		}
	}
	
	void UpdateTouchStatus ()
	{
		if ( Platform.Mobile == platform )
		{
			var count = Input.touchCount;
			if ( count == 1 )
			{
				Touch touch = Input.GetTouch(0);
				if( touch.phase == TouchPhase.Began)
				{
					isTouched = true;
					
					positionBegan = touch.position;
				}
				
				positionCurrent = touch.position;
			}
			else
			{
				isTouched = false;
			}
			
			if ( count == 2 )
			{
				Touch touch0 = Input.GetTouch(0);
				Touch touch1 = Input.GetTouch(1);	
				if ( !bEnableFingerDistance )
				{
					bEnableFingerDistance = true;
					
					lastFingerDistance = Vector2.Distance( touch0.position, touch1.position );
				}
				float fingerDistance = Vector2.Distance( touch0.position, touch1.position );
				
				fingerDistanceDelta = (fingerDistance - lastFingerDistance)/lastFingerDistance;
				lastFingerDistance = fingerDistance;
			}
			else
			{
				bEnableFingerDistance = false;
			}
		}
		else
		{
			if( Input.GetButtonDown( "Fire1" ) )
			{
				isTouched = true;
				   
				positionBegan = Input.mousePosition; 
				
			}
			if ( Input.GetButtonUp( "Fire1" ) )
			{
				isTouched = false;
			}
			
			positionCurrent = Input.mousePosition;	
		}
	}
	
	public bool IsTouched ()
	{
		return isTouched;
	}
	
	public bool IsTouchEnd ()
	{
		return isTouchEnd;
	}
	
	public float FingerDistanceDelta ()
	{
		if ( bEnableFingerDistance )
		{
			return fingerDistanceDelta;
		}
		return 0.0f;
	}
	
}
