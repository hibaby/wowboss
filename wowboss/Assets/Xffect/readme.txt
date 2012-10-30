----------------------------------------------
            Xffect Editor Free Edition
 Copyright 2012- Shallway Studio
                Version 2.0.0
    http://shallway.net/forum.php?mod=viewthread&tid=40&page=1&extra=#pid40
            shallwaycn@gmail.com
----------------------------------------------


This is a free edition of Xffect Pro, if you like it, please buy the pro version, it will greatly enhance your game visual:
http://forum.unity3d.com/threads/142245-Xffect-Editor-Pro-powerful-tool-to-create-amazing-effects!


---------------------------------------
documentation, and tutorials
---------------------------------------

All can be found here:
http://shallway.net/forum.php?mod=viewthread&tid=39&extra=page%3D1


--------------------
 How To Update Xffect
--------------------
1. In Unity, File -> New Scene
2. Delete the Xffect folder from the Project View.
3. Import Xffect from the updated Unity Package.


---------------------------------------
Release Notes
---------------------------------------
Version 2.0
-*Feature Added: Event System, support CameraShake, Sound, Light and ImageEffect Event, which will greatly enhance your game visual.
-Fixed: heat_distortion shader add "ZTest Always" to avoid being clipped by other game object.
-Fixed: "Cone" node's direction can change from inspector now.
-Added: Example RadialBlur.
-Added: Example VolumetricLight.
-Improved: Example explosion1, use CameraShake and CameraGlow Event.

Version 1.3.2
-Added: support "random speed" in "Direction Configuration" now.
-Added: xft_beam_light shader.
-Added: WindowLight Example.

Version 1.3.1
-*Performance Improved* No GC Alloc During Run Time Now!!!
-Removed: Fog Of War Example, since it has nothing to do with xffect editor. but i will release "Fog Of War For RTS" package to asset store soon.
-Added: Dissolve Example.


Version 1.3.0(from this version, i will continue to share my own shaders to Xffect package.)
-Fixed: parent's rotation don't influence XffectComponent's mesh now, you can put XffectComponent to any parent with any rotation(mainly fixed for slash trails).
-Added: each EffectLayer can custom the debug color now.
-Added: fog of war shader(in xft_fog_of_war scene), Best and Fast solution, but currently is not supported with AA and Mobile device.
-Added: Volume Fog shader(in xft_demo scene ),not supported in mobile.
-Added: IceImpact Example, Swing Around Example.
-Added: MobileMaterialSetting.cs, useful to change all the materials in a folder to fit the mobile device(many thanks to kukuÐ¡Ø²).  

Version 1.2.2
-Useful function added: now, you can manually adjust the xffect's scale by only one option in XffectComponent¡ªthe "scale" edit box, which is very useful for prefabs to reuse in diffrent size's world.
-Example Added: LevelUp.


Version 1.2.1
1,Added: Vortex Affector support random direction now.
2,Added: Ribbon Trail support random width and random length now.
3,Added: "material.renderqueue" can be configured from EffectLayer, so removed "xft_heat_distortion_forward" shader.
4,Example Added: SpreadSlash, PinkSoul,Sakura.

Version 1.2.0
1,fixed: "XZ Plane" sprite's direction is influenced by "Direction" now.
2,Added: "Mesh" Emitter, support emit by vertex and emit by triangle.
3,Added: "Direction Configuration" is more specific now, support "Sphere" "Cone" "Cylindrical" direction.
4,Fixed some xffect component can't sync to parent Transform.

Version 1.1.2
1,bug fixed: BombAffector's force fixed, now it's in units of mass / time^2.
2,add a BombAffector example to show the fixed Bomb Affector feature.
3,fixed: now the parent's scale don't influence XffectComponent's position now, you can put XffectComponent to anyparent with any scale.

Version 1.1.1
1,example improved: "lightning_explode", "water_fall"
2,example added: "transform_spell"
3,fixed: if Gravity Affector is non-accelerate, the velocity is constant.

Version 1.1.0
1,now the box emitter's rotation can inherit from client rotation
2,direction can always sync to client rotation now
3,provide slash trail option, with heat distortion shader, it can render amazing weapon trails.
4,add a phantom_sword prefab which shows all the features above.