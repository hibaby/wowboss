Shader "Hidden/OutLine" {

Properties {
	_MainTex ("", 2D) = "white" {}
}

Category {
	ZTest Always Cull Off ZWrite Off Fog { Mode Off }

	Subshader {
		Pass {
			CGPROGRAM
// Upgrade NOTE: excluded shader from Xbox360; has structs without semantics (struct v2f members off)
#pragma exclude_renderers xbox360
				#pragma vertex vert
				#pragma fragment frag
				#pragma fragmentoption ARB_precision_hint_fastest

				#include "UnityCG.cginc"

				struct v2f {
					float4 pos : POSITION;
					half2 uv : TEXCOORD0;
					float2 off;
				};

				float4 _MainTex_TexelSize;
				float4 _BlurOffsets;

				v2f vert (appdata_img v)
				{
					v2f o;
					float offX = _MainTex_TexelSize.x * _BlurOffsets.x;
					float offY = _MainTex_TexelSize.y * _BlurOffsets.y;

					o.pos = mul (UNITY_MATRIX_MVP, v.vertex);
					o.uv = MultiplyUV (UNITY_MATRIX_TEXTURE0, v.texcoord.xy-float2(offX, offY));
					o.off.x = offX;
					o.off.y = offY;
					return o;
				}
				
				sampler2D _MainTex;
				fixed4 _Color;

				fixed4 frag( v2f i ) : COLOR
				{
					
					fixed4 c;
					c = tex2D( _MainTex, i.uv+ float2( i.off.x, i.off.y));
					c += tex2D( _MainTex, i.uv+ float2(i.off.x, -i.off.y));
					c += tex2D( _MainTex, i.uv+ float2(-i.off.x, i.off.y));
					c += tex2D( _MainTex, i.uv+ float2(-i.off.x, -i.off.y));
					c += tex2D( _MainTex, i.uv+ float2(i.off.x, 0));
					c += tex2D( _MainTex, i.uv+ float2(-i.off.x, 0));
					c += tex2D( _MainTex, i.uv+ float2(0, i.off.y));
					c += tex2D( _MainTex, i.uv+ float2(0, -i.off.y));
					fixed4 cc = tex2D( _MainTex, i.uv.xy);
					
					c.rgb /=4;
					if(cc.r>0)
					{
						c.a = 0;
						c.rgb = (0,0,0);
					}
					else if(c.r>0)
					{
						c.a = 1;
					}
					else
					{	
						c.a = 0;
					}
					return c ;
				}
			ENDCG
		}
		
	}

	Subshader {
		Pass {
			SetTexture [_MainTex] {constantColor [_Color] combine texture * constant alpha}
			SetTexture [_MainTex] {constantColor [_Color] combine texture * constant + previous}
			SetTexture [_MainTex] {constantColor [_Color] combine texture * constant + previous}
			SetTexture [_MainTex] {constantColor [_Color] combine texture * constant + previous}		
		}

	}
}

Fallback off

}
