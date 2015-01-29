
cbuffer _XE_CB_Color : register(b0)
{
	float4 u_Color : packoffset(c0);
};

float4 main() : SV_Target0
{
	return u_Color;
}