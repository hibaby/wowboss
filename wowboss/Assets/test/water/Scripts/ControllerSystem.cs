using UnityEngine;
using System.Collections;

public class ControllerSystem : MonoBehaviour {
	public enum ANI_STEP{
		ANI_STEP_ACTION
		, ANI_STEP_POSTURE
		, ANI_STEP_ACTION2POSTURE
		, ANI_STEP_POSTURE2ACTION
	}
	public AnimationClip ActionAttack;
	public AnimationClip ActionDefend;
	public AnimationClip PostureCombat;
	public string sAction = "idle";
	public string sPosture = "idle";
	public bool bAction = false;
	public bool bPosture = false;
 	public ANI_STEP aniStep = ANI_STEP.ANI_STEP_POSTURE;
	// Use this for initialization
	void Start () {
		animation[PostureCombat.name].wrapMode = WrapMode.Loop;
		animation.Play( PostureCombat.name );
		sPosture = PostureCombat.name;
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
	
	public void DoAction(string action) {
		sAction = action;
		animation.CrossFade( action );			
		aniStep = ANI_STEP.ANI_STEP_POSTURE2ACTION;
	}
	
	public void DoPosture(string posture) {
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
