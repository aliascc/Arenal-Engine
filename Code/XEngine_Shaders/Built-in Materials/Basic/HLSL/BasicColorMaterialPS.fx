
#include "Structures.fx"

float4 main(VSOutputPosColor input) : SV_Target0
{
	return input.Color;
}