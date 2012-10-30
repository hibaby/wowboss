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

    public enum GAFTTYPE
    {
        Planar,
        Spherical
    }

    public class GravityAffector : Affector
    {
        protected GAFTTYPE GType;
        protected MAGTYPE MType;
        protected float Magnitude;
        protected AnimationCurve MagCurve;
        protected Vector3 Dir;
        protected Transform GravityObj;
        protected bool IsAccelerate = true;

        public void SetAttraction(Transform goal)
        {
            GravityObj = goal;
        }

        public GravityAffector(Transform obj, GAFTTYPE gtype, MAGTYPE mtype,bool isacc, Vector3 dir, float mag,AnimationCurve curve,EffectNode node)
            : base(node, AFFECTORTYPE.GravityAffector)
        {
            GType = gtype;
            MType = mtype;
            Magnitude = mag;
            MagCurve = curve;
            Dir = obj.rotation * dir;
            Dir.Normalize();
            GravityObj = obj;
            IsAccelerate = isacc;
        }

        public override void Update(float deltaTime)
        {
            // not supported.
        }
    }
}
