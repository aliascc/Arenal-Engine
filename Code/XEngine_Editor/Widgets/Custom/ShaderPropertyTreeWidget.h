
/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 7/29/2013
*
* Desc:
*
*
* Copyright 2013
*
*********************************************************/

#pragma once

#ifndef _SHADER_PROPERTY_TREE_WIDGET_H
#define _SHADER_PROPERTY_TREE_WIDGET_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "qspinbox.h"
#include "qcheckbox.h"
#include "qtreewidget.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Shaders\ShaderDefs.h"
#include "Textures\TextureDefs.h"

/********************
*   Forward Decls   *
*********************/
class ConstantBuffer;
class GameAssetManager;
class ShaderProperties;

/*****************
*   Class Decl   *
******************/
class ShaderPropertyTreeWidget sealed : public QTreeWidget
{
	Q_OBJECT

	struct UserDataVariableInfo
	{
		std::wstring m_ConstantBufferName;
		std::wstring m_VariableName;
		ShaderVariableType m_ShaderVariableType;
		uint32_t m_Row;
		uint32_t m_Column;
		uint32_t m_NumColums;
		QTreeWidgetItem* m_ParentItem;
		
		UserDataVariableInfo(const std::wstring& cbName, const std::wstring& varName, ShaderVariableType shaderVariableType)
			: m_ConstantBufferName(cbName)
			, m_VariableName(varName)
			, m_ShaderVariableType(shaderVariableType)
			, m_Row(0)
			, m_Column(0)
			, m_NumColums(1)
			, m_ParentItem(nullptr)
		{
		}
	};

	private:

		ShaderProperties* m_ShaderProperties = nullptr;

		GameAssetManager* m_GameAssetManager = nullptr;

		bool m_IsReady = false;

		XEResult InitFields();

		XEResult FillTextureBindingsRows();

		XEResult FillConstantBuffersRows();

		XEResult AddConstantBufferChildStruct(QTreeWidgetItem* parentItem, ConstantBuffer* cb, const std::wstring& variableName, ShaderVariableType shaderVariableType);

		XEResult AddConstantBufferChildScalar(QTreeWidgetItem* parentItem, ConstantBuffer* cb, const std::wstring& variableName, ShaderVariableType shaderVariableType);

		XEResult AddConstantBufferChildMatrix(QTreeWidgetItem* parentItem, ConstantBuffer* cb, const std::wstring& variableName, ShaderVariableType shaderVariableType, uint32_t numColumns, uint32_t numRows);

		XEResult AddConstantBufferChildVector(QTreeWidgetItem* parentItem, ConstantBuffer* cb, const std::wstring& variableName, ShaderVariableType shaderVariableType, uint32_t numColumns);
		
		XEResult CreateVectorBooleanBranch(ConstantBuffer* cb, const std::wstring& variableName, ShaderVariableType shaderVariableType, uint32_t numColumns, QTreeWidgetItem* parentItem, uint32_t row = 0);
		
		XEResult CreateVectorIntBranch(ConstantBuffer* cb, const std::wstring& variableName, ShaderVariableType shaderVariableType, uint32_t numColumns, QTreeWidgetItem* parentItem, uint32_t row);

		XEResult CreateVectorFloatDoubleBranch(ConstantBuffer* cb, const std::wstring& variableName, ShaderVariableType shaderVariableType, uint32_t numColumns, QTreeWidgetItem* parentItem, uint32_t row);

		template<class T>
		XEResult CreateMatrixBranchTemplate(ConstantBuffer* cb, const std::wstring& variableName, UserDataVariableInfo userDataVariableInfo, uint32_t numColumns, uint32_t numRows, QTreeWidgetItem* parentItem);

		template<class T>
		XEResult CreateVectorMainBranchTemplate(ConstantBuffer* cb, const std::wstring& variableName, UserDataVariableInfo userDataVariableInfo, uint32_t numColumns, QTreeWidgetItem* parentItem);

		template<class T>
		XEResult CreateVectorBranchTemplate(T* varData, UserDataVariableInfo userDataVariableInfo, uint32_t numColumns, QTreeWidgetItem* parentItem, uint32_t row = 0);

		template<class T>
		XEResult SetValueToVarConstantBuffer(T newValue, ConstantBuffer* cb, const UserDataVariableInfo& userDataVariableInfo);

		XEResult UpdateParentText(QTreeWidgetItem* parentItem, ShaderVariableType shaderVariableType);

		template<class T>
		XEResult CreateChildBranch(QTreeWidgetItem* parentItem, const QString& childDisplayText, T value, const UserDataVariableInfo& userInfo, QString* displayString = nullptr);

		QSpinBox* CreateSpinBox(int32_t maxSize, int32_t minSize, int32_t value, const UserDataVariableInfo& userInfo);

		QDoubleSpinBox* CreateDoubleSpinBox(double maxSize, double minSize, double value, const UserDataVariableInfo& userInfo);
		
		QCheckBox* CreateCheckBox(bool value, const UserDataVariableInfo& userInfo);

	private slots:

		void ConstantBufferShaderVarSpinBoxChanged();

		void ConstantBufferShaderVarCheckBoxChanged(int newState);

		void SeletedTextureAssetIDChanged(uint64_t assetID);

	public:
		ShaderPropertyTreeWidget(QWidget *parent = nullptr);
		~ShaderPropertyTreeWidget();

		XEResult InitShaderPropsTree(GameAssetManager* gameAssetManager, ShaderProperties* shaderProperties);
};


#endif