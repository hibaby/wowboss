Shader "Hidden/OutterLineCutoffBody" {
	Properties {
		_MainTex ("", 2D) = "white" {}
	}	
	Category {
	BlendOp RevSub
	Blend One One
	ZTest Always Cull Off ZWrite Off Fog { Mode Off }
	
	Subshader {
		Pass {
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			#pragma fragmentoption ARB_precision_hint_fastest 
			
			sampler2D _MainTex;
			sampler2D _MainTex1;
			struct appdata
			{
				float4 vertex : POSITION;
				float4 texcoord : TEXCOORD0;
			};
			struct v2f
			{
				float4 pos : SV_POSITION;
				float2 uv : TEXCOORD0;
			};
			v2f vert (appdata v)
			{
				v2f o;
				o.pos = mul(UNITY_MATRIX_MVP, v.vertex);
				o.uv = v.texcoord.xy;
				return o;
			}
			half4 frag(v2f i) : COLOR
			{
				fixed4 c =  tex2D(_MainTex, i.uv);
				/*if(c.r>0)
				{
					c = (0,0,0,0);
				}*/
				return c;
			}
			ENDCG
		}
		}	
	}
	FallBack "Diffuse"
}