Shader "Hidden/OutterLineComposer" {

Properties {
	_MainTex ("", 2D) = "white" {}
}

Category {
	ZTest Always Cull Off ZWrite Off Fog { Mode Off }
	Blend SrcAlpha OneMinusSrcAlpha

	Subshader {
		Pass {
			CGPROGRAM
				#pragma vertex vert
				#pragma fragment frag
				#pragma fragmentoption ARB_precision_hint_fastest

				#include "UnityCG.cginc"

				struct v2f {
					float4 pos : POSITION;
					half2 uv : TEXCOORD0;
				};

				v2f vert (appdata_img v)
				{
					v2f o;
					o.pos = mul (UNITY_MATRIX_MVP, v.vertex);
					o.uv = v.texcoord.xy;
					return o;
				}

				sampler2D _MainTex;

				fixed4 frag( v2f i ) : COLOR
				{
					return tex2D( _MainTex, i.uv );
				}
			ENDCG
		}
	}

	SubShader {
		Pass {
			SetTexture [_MainTex] {constantColor [_Color] combine constant * texture DOUBLE}
		}
	}
}

Fallback off

}
