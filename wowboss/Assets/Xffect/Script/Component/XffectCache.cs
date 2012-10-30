//----------------------------------------------
//            Xffect Editor
// Copyright © 2012- Shallway Studio
// http://shallway.net
//----------------------------------------------

using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Xft;


//**Notice, XffectCache's position should always be zero.

public class XffectCache : MonoBehaviour
{
    protected static XffectCache m_instance;
    Dictionary<string, ArrayList> EffectDic = new Dictionary<string, ArrayList>();
    Dictionary<string, ArrayList> CompEffectDic = new Dictionary<string, ArrayList>();
    void Awake()
    {
        m_instance = this;
        foreach (Transform child in transform)
        {
            XffectComponent xft = child.GetComponent<XffectComponent>();
            if (xft != null)
            {
                //make sure all children is inited.
                xft.Initialize();
                if (!EffectDic.ContainsKey(child.name))
                    EffectDic[child.name] = new ArrayList();
                EffectDic[child.name].Add(xft);
            }
            CompositeXffect cxft = child.GetComponent<CompositeXffect>();
            if (cxft != null)
            {
                cxft.Initialize();
                if (!CompEffectDic.ContainsKey(child.name))
                    CompEffectDic[child.name] = new ArrayList();
                CompEffectDic[child.name].Add(cxft);
            }
        }
    }
    static public XffectCache GetInstance()
    {
        if (m_instance == null)
        {
            Debug.LogError("xffectCache haven't initialized!");
            return null;
        }
        return m_instance;
    }


    protected XffectComponent AddXffect(string name)
    {
        Transform baseobj = transform.Find(name);
        if (baseobj == null)
        {
            Debug.Log("object:" + name + "doesn't exist!");
            return null;
        }
        Transform newobj = Instantiate(baseobj, Vector3.zero, Quaternion.identity) as Transform;
        newobj.parent = transform;
        newobj.gameObject.active = false;
        XffectComponent xft = newobj.GetComponent<XffectComponent>();
        EffectDic[name].Add(xft);
        if (xft != null)
            xft.Initialize();
        return xft;
    }


    #region APIs
    public XffectComponent GetEffect(string name)
    {
        ArrayList cache = EffectDic[name];
        if (cache == null)
        {
            return null;
        }
        foreach (XffectComponent xft in cache)
        {
            if (!xft.gameObject.active)
            {
                return xft;
            }
        }
        return AddXffect(name);
    }

    //don't support auto extend.
    public CompositeXffect GetCompositeXffect(string name)
    {
        ArrayList cache = CompEffectDic[name];
        if (cache == null)
        {
            return null;
        }
        foreach (CompositeXffect xftc in cache)
        {
            if (!xftc.gameObject.active)
            {
                return xftc;
            }
        }
        return null;
    }

    //manually emit effect node by expected position.
    public void EmitNode(string eftName, Vector3 pos)
    {
        ArrayList cache = EffectDic[eftName];
        if (cache == null)
        {
            Debug.LogError(name + ": cache doesnt exist!");
            return;
        }
        if (cache.Count > 1)
        {
            Debug.LogWarning("EmitNode() only support only-one xffect cache!");
        }
        XffectComponent xft = (XffectComponent)cache[0];
        if (xft.gameObject.active == false)
            xft.Active();
        xft.EmitByPos(pos);
    }

    //release effect by world position. in this case, the client transform's position should be zero.
    //notice: if the xffect is not auto-deactive, you should remember to manually deactive it.
    //** automaticaly set client to EffectCache, so you should make sure EffectCache's position is always zero.
    public XffectComponent ReleaseEffect(string name, Vector3 pos)
    {
        XffectComponent xft = GetEffect(name);
        if (xft == null)
        {
            Debug.LogWarning("can't find available effect in cache!:" + name);
            return null;
        }
        xft.Active();
        xft.SetClient(transform);
        xft.SetEmitPosition(pos);
        return xft;
    }

    //release effect with a new client transform.
    //notice: if the xffect is not auto-deactive, you should remember to manually deactive it.
    public XffectComponent ReleaseEffect(string name, Transform client)
    {
        XffectComponent xft = GetEffect(name);
        if (xft == null)
        {
            Debug.LogWarning("can't find available effect in cache!:" + name);
            return null;
        }
        xft.Active();
        xft.SetClient(client);
        return xft;
    }

    public XffectComponent ReleaseEffect(string name)
    {
        XffectComponent xft = GetEffect(name);
        if (xft == null)
        {
            Debug.LogWarning("can't find available effect in cache!:" + name);
            return null;
        }
        xft.Active();
        return xft;
    }

    //useful for only-one effect cache and the effect is looping.
    public void DeActiveEffect(string eftName)
    {
        ArrayList cache = EffectDic[eftName];
        if (cache == null)
        {
            Debug.LogError(name + ": cache doesnt exist!");
            return;
        }
        if (cache.Count > 1)
        {
            Debug.LogWarning("DeActive() only support one Xffect cache!");
        }
        XffectComponent xft = (XffectComponent)cache[0];
        if (xft.gameObject.active == false)
            return;
        xft.DeActive();
        xft.Reset();
        xft.gameObject.active = false;
    }
    #endregion
}