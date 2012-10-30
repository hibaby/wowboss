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
    public enum BOMBTYPE
    {
        Planar,
        Spherical,
        Cylindrical
    }

    public enum BOMBDECAYTYPE
    {
        None,
        Linear,
        Exponential
    }

    public class BombAffector : Affector
    {
        protected BOMBTYPE BombType;
        protected MAGTYPE MType;
        protected BOMBDECAYTYPE DecayType;
        protected float Magnitude;
        protected AnimationCurve MagCurve;
        protected float Decay;
        protected Vector3 BombAxis;
        protected Transform BombObj;

        protected float ElapsedTime = 0f;

        public BombAffector(Transform obj, BOMBTYPE gtype, MAGTYPE mtype,BOMBDECAYTYPE dtype, float mag, AnimationCurve curve,
            float decay, Vector3 axis, EffectNode node)
            : base(node, AFFECTORTYPE.BombAffector)
        {
            BombType = gtype;
            MType = mtype;
            DecayType = dtype;
            Magnitude = mag;
            MagCurve = curve;
            Decay = decay;
            BombAxis = axis;
            BombAxis.Normalize();
            BombObj = obj;
        }


        public override void Reset()
        {
            ElapsedTime = 0f;
        }

        public override void Update(float deltaTime)
        {
			//not supported
        }
    }
}
