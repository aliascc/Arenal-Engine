/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
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

		AEResult InitFields();

		AEResult FillTextureBindingsRows();

		AEResult FillConstantBuffersRows();

		AEResult AddConstantBufferChildStruct(QTreeWidgetItem* parentItem, ConstantBuffer* cb, const std::wstring& variableName, ShaderVariableType shaderVariableType);

		AEResult AddConstantBufferChildScalar(QTreeWidgetItem* parentItem, ConstantBuffer* cb, const std::wstring& variableName, ShaderVariableType shaderVariableType);

		AEResult AddConstantBufferChildMatrix(QTreeWidgetItem* parentItem, ConstantBuffer* cb, const std::wstring& variableName, ShaderVariableType shaderVariableType, uint32_t numColumns, uint32_t numRows);

		AEResult AddConstantBufferChildVector(QTreeWidgetItem* parentItem, ConstantBuffer* cb, const std::wstring& variableName, ShaderVariableType shaderVariableType, uint32_t numColumns);
		
		AEResult CreateVectorBooleanBranch(ConstantBuffer* cb, const std::wstring& variableName, ShaderVariableType shaderVariableType, uint32_t numColumns, QTreeWidgetItem* parentItem, uint32_t row = 0);
		
		AEResult CreateVectorIntBranch(ConstantBuffer* cb, const std::wstring& variableName, ShaderVariableType shaderVariableType, uint32_t numColumns, QTreeWidgetItem* parentItem, uint32_t row);

		AEResult CreateVectorFloatDoubleBranch(ConstantBuffer* cb, const std::wstring& variableName, ShaderVariableType shaderVariableType, uint32_t numColumns, QTreeWidgetItem* parentItem, uint32_t row);

		template<class T>
		AEResult CreateMatrixBranchTemplate(ConstantBuffer* cb, const std::wstring& variableName, UserDataVariableInfo userDataVariableInfo, uint32_t numColumns, uint32_t numRows, QTreeWidgetItem* parentItem);

		template<class T>
		AEResult CreateVectorMainBranchTemplate(ConstantBuffer* cb, const std::wstring& variableName, UserDataVariableInfo userDataVariableInfo, uint32_t numColumns, QTreeWidgetItem* parentItem);

		template<class T>
		AEResult CreateVectorBranchTemplate(T* varData, UserDataVariableInfo userDataVariableInfo, uint32_t numColumns, QTreeWidgetItem* parentItem, uint32_t row = 0);

		template<class T>
		AEResult SetValueToVarConstantBuffer(T newValue, ConstantBuffer* cb, const UserDataVariableInfo& userDataVariableInfo);

		AEResult UpdateParentText(QTreeWidgetItem* parentItem, ShaderVariableType shaderVariableType);

		template<class T>
		AEResult CreateChildBranch(QTreeWidgetItem* parentItem, const QString& childDisplayText, T value, const UserDataVariableInfo& userInfo, QString* displayString = nullptr);

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

		AEResult InitShaderPropsTree(GameAssetManager* gameAssetManager, ShaderProperties* shaderProperties);
};


#endif