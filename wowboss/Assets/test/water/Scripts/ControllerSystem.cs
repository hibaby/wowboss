using UnityEngine;
using System.Collections;

public class ControllerSystem : MonoBehaviour {
	public enum ANI_STEP{
		ANI_STEP_ACTION
		, ANI_STEP_POSTURE
		, ANI_STEP_ACTION2POSTURE
		, ANI_STEP_POSTURE2ACTION
	}
	public string sAction = "jog";
	public string sPosture = "idle";
	public bool bAction = false;
	public bool bPosture = false;
 	public ANI_STEP aniStep = ANI_STEP.ANI_STEP_POSTURE;
	// Use this for initialization
	void Start () {
		animation["idle"].wrapMode = WrapMode.Loop;
		animation["run"].wrapMode = WrapMode.Loop;
		animation.Play( sPosture );
	}
	
	// Update is called once per frame
	void Update () {
		if ( bAction )
		{
			bAction = false;
			DoAction( sAction );
		}
		if ( bPosture )
		{
			bPosture = false;
			DoPosture( sPosture );
		}
		
		if ( aniStep == ANI_STEP.ANI_STEP_POSTURE2ACTION 
			|| aniStep == ANI_STEP.ANI_STEP_ACTION )
		{
			if ( (animation[sAction].time + 0.3f) > animation[sAction].length )
			{
				animation.CrossFade( sPosture );				
				aniStep = ANI_STEP.ANI_STEP_ACTION2POSTURE;
			}
		}
	}
	
	void DoAction(string action) {
		animation.CrossFade( action );			
		aniStep = ANI_STEP.ANI_STEP_POSTURE2ACTION;
	}
	
	void DoPosture(string posture) {
		if ( aniStep == ANI_STEP.ANI_STEP_POSTURE2ACTION 
			|| aniStep == ANI_STEP.ANI_STEP_ACTION )
		{
		}
		else
		{
			animation.CrossFade( sPosture );				
			aniStep = ANI_STEP.ANI_STEP_ACTION2POSTURE;
		}
	}
}
