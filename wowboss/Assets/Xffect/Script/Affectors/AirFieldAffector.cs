//----------------------------------------------
//            Xffect Editor
// Copyright © 2012- Shallway Studio
// http://shallway.net
//----------------------------------------------

using UnityEngine;
using System.Collections;
using Xft;

namespace Xft
{
    public class AirFieldAffector : Affector
    {
        protected Transform AirObj;
        protected Vector3 Direction;
        protected MAGTYPE MType;
        protected float Magnitude;
        protected AnimationCurve MagCurve;
        protected float Attenuation;
        protected bool UseMaxDistance;
        protected float MaxDistance;
        protected float MaxDistanceSqr;
        protected bool EnableSpread;
        protected float Spread;
        protected float InheritVelocity;
        protected bool InheritRotation;

        protected Vector3 LastFieldPos;

        public AirFieldAffector(Transform airObj, Vector3 dir, MAGTYPE mtype,float mag, AnimationCurve curve,float atten, bool useMaxdist, 
            float maxDist,bool enableSpread, float spread, float inhV, bool inhRot, EffectNode node)
            : base(node, AFFECTORTYPE.AirFieldAffector)
        {
            AirObj = airObj;
            Direction = dir.normalized;
            MType = mtype;
            Magnitude = mag;
            MagCurve = curve;
            Attenuation = atten;
            UseMaxDistance = useMaxdist;
            MaxDistance = maxDist;
            MaxDistanceSqr = MaxDistance * MaxDistance;
            EnableSpread = enableSpread;
            Spread = spread;
            InheritVelocity = inhV;
            InheritRotation = inhRot;
            LastFieldPos = AirObj.position;
        }

        public override void Reset()
        {
            LastFieldPos = AirObj.position;
        }

        public override void Update(float deltaTime)
        {
			//not support in free edition.
		}
    }
}

