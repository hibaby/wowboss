using UnityEngine;
using System.Collections;

public class Defender : MonoBehaviour {

	// Use this for initialization
	public int attackPower = 1;
	public int attackRange = 1;
	
	public float attackCD = 2.0f;
	public float lastAttackTime = 0.0f;
	
	private Attacker enemy;
	
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		if ( enemy )
		{
			Attack();
		}
	
	}
	
	void Attack() {
		if ( (lastAttackTime + attackCD) < Time.realtimeSinceStartup )
		{
			enemy.Damage( attackPower );
			lastAttackTime = Time.realtimeSinceStartup;
		}
	}
	
	void OnCollisionEnter(Collision collision) {
		enemy = collision.gameObject.GetComponent<Attacker>() as Attacker;
		if ( enemy )
		{
			Debug.Log( collision.gameObject.name );
		}
	}
	
	void OnCollisionExit (Collision collision) {
		if ( enemy )
		{
			enemy = null;
		}
	}
}
