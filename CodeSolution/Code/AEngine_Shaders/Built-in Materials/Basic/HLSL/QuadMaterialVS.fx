/*
* Copyright (c) 2018 <Carlos Chacón>
* All rights reserved
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "Structures.fx"

cbuffer _AE_CB_HalfPixel : register(b0)
{
	float2 _AE_HalfPixel	: packoffset(c0);
};

VSOutputPosTex main(VSInputPosTex input)
{
	VSOutputPosTex output = (VSOutputPosTex) 0;
	
	output.Position = float4(input.Position, 1.0f);
	output.TexCoord = input.TexCoord - _AE_HalfPixel;

	return output;
}