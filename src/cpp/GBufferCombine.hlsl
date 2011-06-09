Texture2D RT0 : register(t0);
Texture2D RT1 : register(t1);
Texture2D RT2 : register(t2);
Texture2D RT3 : register(t3);

Texture2D LightMap : register(t4);

SamplerState PointSampler : register(s0);

struct PS_In_Combine
{
    float4 vPosition	: SV_POSITION;
    float2 vTexCoord	: TEXCOORD0;
	float2 vPosition2	: TEXCOORD1;
};

float4 PS_Combine(PS_In_Combine input) : SV_TARGET0
{
	// RT0 =       Diffuse.r	| Diffuse.g		| Diffuse.b		| Specular Intensity
    // RT1 =       Normal.x		| Normal.y		| Normal.z		| Specular Power
    // RT2 =       Emissive.r	| Emissive.g	| Emissive.b	| Ambient
	float4 rt0Sample = RT0.Sample(PointSampler, input.vTexCoord);
	float4 rt1Sample = RT1.Sample(PointSampler, input.vTexCoord);
	float4 rt2Sample = RT2.Sample(PointSampler, input.vTexCoord);
	float4 rt3Sample = RT3.Sample(PointSampler, input.vTexCoord);
	float4 lightSample = LightMap.Sample(PointSampler, input.vTexCoord);

	float3 vDiffuse = rt0Sample.rgb;
	float fAmbient = rt2Sample.a;
	float3 vLightColor = lightSample.rgb;
	float fSpecular = lightSample.a;
	float3 vEmissive = rt2Sample.rgb;

	float3 vFinalColour = (fAmbient * vDiffuse) +
						  (vEmissive) +
						  (vLightColor * vDiffuse) + 
						  (fSpecular * vLightColor * vDiffuse);

	return float4(vFinalColour, 1.0f);
}