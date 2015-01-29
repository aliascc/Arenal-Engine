/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 12/13/2012
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/

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