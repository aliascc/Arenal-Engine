/*
* Copyright (c) 2018 <Carlos Chac�n>
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

/**********************
*   System Includes   *
***********************/
#include <new>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "GameUtilsDefs.h"

/********************
*   Forward Decls   *
*********************/

/********************
*   Function Defs   *
*********************/
GCBasicProps::GCBasicProps()
{
}
	
GCBasicProps::~GCBasicProps()
{
}

//Use for Angel Script
GCBasicProps& GCBasicProps::operator=(const GCBasicProps &other)
{
	m_GCID = other.m_GCID;
	m_GCName = other.m_GCName;
	m_GCCallOrder = other.m_GCCallOrder;
	m_Drawable = other.m_Drawable;
	m_Enabled = other.m_Enabled;
	m_Visibled = other.m_Visibled;

	return *this;
}

void GCBasicProps::Constructor(GCBasicProps* self)
{
	new(self) GCBasicProps();
}

void GCBasicProps::Destructor(GCBasicProps* self)
{
	self->~GCBasicProps();
}