using UnityEngine;
using System.Collections;

public class Position : MonoBehaviour {
	
	public GameObject[] gos;
	public GameObject prefabSoldier;
	public GameObject soldier;
	
	private float lastActionTime;
	void OnMouseDown () 
	{
		Vector3 v = transform.position;
		v.y = 1.0f;
		Instantiate(gos[0], v, transform.rotation);
	}
	void OnMouseUp () 
	{
		if ( soldier )
		{
			Debug.Log( "soldier is here" );
			return;
		}
		
		lastActionTime = Time.realtimeSinceStartup;
		
		Vector3 v = transform.position;
		v.y = 1.0f;
		soldier = Instantiate(prefabSoldier, v, transform.rotation) as GameObject;
	}
	
	void OnDestroy ()
	{
		Destroy( soldier );
	}
	
	void Update ()
	{
		if ( soldier )
		{
			if ( (lastActionTime + 5) < Time.realtimeSinceStartup )
			{
				lastActionTime = Time.realtimeSinceStartup;
				ControllerSystem cs = soldier.GetComponentInChildren<ControllerSystem>();
				cs.DoAction( cs.ActionAttack.name );
				
				
		Vector3 v = transform.position;
		v.y = 1.0f;
				Instantiate(gos[1], v, transform.rotation);
			}
		}
	}
}
