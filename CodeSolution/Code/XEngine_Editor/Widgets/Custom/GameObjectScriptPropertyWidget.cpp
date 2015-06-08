/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

/**********************
*   System Includes   *
***********************/
#include <cfloat>
#include <climits>

/*************************
*   3rd Party Includes   *
**************************/
#include "qheaderview.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "GraphicsDefs.h"
#include "Utils\XEQTDefs.h"
#include "Base\BaseFunctions.h"
#include "AngelScript\AngelScriptDefs.h"
#include "GameObjectScriptPropertyWidget.h"
#include "GameObject\Scripts\GameObjectScriptDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameObjectScriptPropertyWidget::GameObjectScriptPropertyWidget(QWidget *parent)
	: QTreeWidget(parent)
{
}

GameObjectScriptPropertyWidget::~GameObjectScriptPropertyWidget()
{
}

XEResult GameObjectScriptPropertyWidget::Initialize()
{
	///////////////////////////////////////////
	//Set preferred indentation
	XETODO("Remove if not needed");
	//this->setIndentation(XE_QT_TREE_INDENTATION);

	///////////////////////////////////////////
	//Set number of columns and their names
	this->setColumnCount(2);

	QAbstractItemModel* itemModel = this->model();

	itemModel->setHeaderData(0, Qt::Orientation::Horizontal, "Property", Qt::ItemDataRole::DisplayRole);
	itemModel->setHeaderData(1, Qt::Orientation::Horizontal, "Value", Qt::ItemDataRole::DisplayRole);

	return XEResult::Ok;
}

XEResult GameObjectScriptPropertyWidget::Populate(GameObjectScriptProperties* gameObjectScriptProperties)
{
	XEAssert(gameObjectScriptProperties != nullptr);
	if (gameObjectScriptProperties == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEResult ret = XEResult::Ok;

	m_IsReady = false;

	this->clear();

	///////////////////////////////////////////
	//Set Properties to Rows
	XETODO("check returns");
	ret = FillPropertiesRows(gameObjectScriptProperties);

	///////////////////////////////////////////
	//Finish
	m_IsReady = true;

	return XEResult::Ok;
}

XEResult GameObjectScriptPropertyWidget::FillPropertiesRows(GameObjectScriptProperties* gameObjectScriptProperties)
{
	XEAssert(gameObjectScriptProperties != nullptr);
	if (gameObjectScriptProperties == nullptr)
	{
		return XEResult::NullParameter;
	}

	for (auto gosProperty : gameObjectScriptProperties->m_PropertiesMap)
	{
		GameObjectScriptProperty* gosProp = gosProperty.second;

		///////////////////////////////////////////
		//Create Constant Buffer Branch
		QTreeWidgetItem* propItem = new QTreeWidgetItem();
		QString name = QString::fromStdWString(gosProp->m_PropertyName);
		propItem->setData(0, Qt::ItemDataRole::DisplayRole, name);

		///////////////////////////////////////////
		//Add to Tree
		this->addTopLevelItem(propItem);

		///////////////////////////////////////////
		//Add Scalar Widget
		XETODO("check return");
		AddScalarWidget(propItem, gosProp);
	}

	return XEResult::Ok;
}

XEResult GameObjectScriptPropertyWidget::AddScalarWidget(QTreeWidgetItem* item, GameObjectScriptProperty* gosProp)
{
	//////////////////////////////////////////
	//Pre-checking parameters
	XEAssert(item != nullptr);
	XEAssert(gosProp != nullptr);
	if (item == nullptr || gosProp == nullptr)
	{
		return XEResult::NullParameter;
	}
		
	//////////////////////////////////////////
	//Base Widget
	QWidget* baseWidget = nullptr;

	switch (gosProp->m_PropertyType)
	{
		case asTYPEID_BOOL:
			{
				//////////////////////////////////////////
				//Create Check Box Widget to Hold Data
				QCheckBox* checkBox = CreateCheckBox(gosProp);

				//////////////////////////////////////////
				//Set to Base Widget
				baseWidget = checkBox;
			}
			break;

		case asTYPEID_INT8:
		case asTYPEID_INT16:
		case asTYPEID_INT32:
		case asTYPEID_INT64:
		case asTYPEID_UINT8:
		case asTYPEID_UINT16:
		case asTYPEID_UINT32:
		case asTYPEID_UINT64:
			{
				//////////////////////////////////////////
				//Create Spin Box Widget to Hold Data
				QSpinBox* numBox = CreateSpinBox(gosProp);

				//////////////////////////////////////////
				//Set to Base Widget
				baseWidget = numBox;
			}
			break;

		case asTYPEID_FLOAT:
		case asTYPEID_DOUBLE:
			{
				//////////////////////////////////////////
				//Create Spin Box Widget to Hold Data
				QDoubleSpinBox* numBox = CreateDoubleSpinBox(gosProp);

				//////////////////////////////////////////
				//Set to Base Widget
				baseWidget = numBox;
			}
			break;

		default:
			XEAssert(false);
			return XEResult::InvalidObjType;
	}

	//////////////////////////////////////////
	//Add Widget to Branch
	this->setItemWidget(item, 1, baseWidget);

	//////////////////////////////////////////
	//Finish
	return XEResult::Ok;
}

QSpinBox* GameObjectScriptPropertyWidget::CreateSpinBox(GameObjectScriptProperty* gosProp)
{
	int64_t value = 0;

	int64_t minSize = 0;
	int64_t maxSize = 0;

	switch (gosProp->m_PropertyType)
	{
		case asTYPEID_INT8:
			{
				maxSize = (int64_t)INT8_MAX;
				minSize = (int64_t)INT8_MIN;

				int8_t valueI = 0;

				memcpy(&valueI, gosProp->m_PropertyAddress, sizeof(int8_t));

				value = (int64_t)valueI;
			}
			break;

		case asTYPEID_INT16:
			if (gosProp->m_PropertyType == asTYPEID_INT16)
			{
				maxSize = (int64_t)INT16_MAX;
				minSize = (int64_t)INT16_MIN;

				int16_t valueI = 0;

				memcpy(&valueI, gosProp->m_PropertyAddress, sizeof(int16_t));

				value = (int64_t)valueI;
			}
			break;

		case asTYPEID_INT32:
			if (gosProp->m_PropertyType == asTYPEID_INT32)
			{
				maxSize = (int64_t)INT32_MAX;
				minSize = (int64_t)INT32_MIN;

				int32_t valueI = 0;

				memcpy(&valueI, gosProp->m_PropertyAddress, sizeof(int32_t));

				value = (int64_t)valueI;
			}
			break;

		case asTYPEID_INT64:
			if (gosProp->m_PropertyType == asTYPEID_INT64)
			{
				maxSize = INT64_MAX;
				minSize = INT64_MIN;

				memcpy(&value, gosProp->m_PropertyAddress, sizeof(int64_t));
			}
			break;

		case asTYPEID_UINT8:
			if (gosProp->m_PropertyType == asTYPEID_UINT8)
			{
				maxSize = (int64_t)UINT8_MAX;
				minSize = 0;

				uint8_t valueI = 0;

				memcpy(&valueI, gosProp->m_PropertyAddress, sizeof(uint8_t));

				value = (int64_t)valueI;
			}
			break;

		case asTYPEID_UINT16:
			if (gosProp->m_PropertyType == asTYPEID_UINT16)
			{
				maxSize = (int64_t)UINT16_MAX;
				minSize = 0;

				uint16_t valueI = 0;

				memcpy(&valueI, gosProp->m_PropertyAddress, sizeof(uint16_t));

				value = (int64_t)valueI;
			}
			break;

		case asTYPEID_UINT32:
			if (gosProp->m_PropertyType == asTYPEID_UINT32)
			{
				maxSize = (int64_t)UINT32_MAX;
				minSize = 0;

				uint32_t valueI = 0;

				memcpy(&valueI, gosProp->m_PropertyAddress, sizeof(uint32_t));

				value = (int64_t)valueI;
			}
			break;

		case asTYPEID_UINT64:
			if (gosProp->m_PropertyType == asTYPEID_UINT64)
			{
				maxSize = (int64_t)UINT64_MAX;
				minSize = 0;

				uint64_t valueI = 0;

				memcpy(&valueI, gosProp->m_PropertyAddress, sizeof(uint64_t));

				value = (int64_t)valueI;
			}
			break;

		default:
			XEAssert(false);
			return nullptr;
	}

	//////////////////////////////////////////
	//Create Spin Box Widget to Hold Data
	QSpinBox* numBox = new QSpinBox();

	//////////////////////////////////////////
	//Set Variable Name as User Data to be Able to retrieve information
	XEQTUserTemplateData<GameObjectScriptProperty*>* userDataInfo = new XEQTUserTemplateData<GameObjectScriptProperty*>(gosProp);
	numBox->setUserData(XE_QT_USER_DATA_SHADER_VAR_INFO, userDataInfo);

	//////////////////////////////////////////
	//Set Maximum and Minimum limits to spin box
	numBox->setMinimum(minSize);
	numBox->setMaximum(maxSize);

	numBox->setValue(value);

	//////////////////////////////////////////
	//Connect to methods to verify and update data
	connect(numBox, SIGNAL(editingFinished()), this, SLOT(SpinBoxChanged()));

	//////////////////////////////////////////
	//Finish
	return numBox;
}

QDoubleSpinBox* GameObjectScriptPropertyWidget::CreateDoubleSpinBox(GameObjectScriptProperty* gosProp)
{
	double maxSize = 0.0;
	double minSize = 0.0;
	double value = 0.0;

	switch (gosProp->m_PropertyType)
	{
		case asTYPEID_FLOAT:
			{
				maxSize = (double)FLT_MAX;
				minSize = (double)-FLT_MAX;

				float valuef = 0.0f;

				memcpy(&valuef, gosProp->m_PropertyAddress, sizeof(float));

				value = (double)valuef;
			}
			break;

		case asTYPEID_DOUBLE:
			maxSize = DBL_MAX;
			minSize = -DBL_MAX;

			memcpy(&value, gosProp->m_PropertyAddress, sizeof(double));

			break;

		default:
			XEAssert(false);
			return nullptr;
	}

	//////////////////////////////////////////
	//Create Spin Box Widget to Hold Data
	QDoubleSpinBox* numBox = new QDoubleSpinBox();

	//////////////////////////////////////////
	//Set Variable Name as User Data to be Able to retrieve information
	XEQTUserTemplateData<GameObjectScriptProperty*>* userDataInfo = new XEQTUserTemplateData<GameObjectScriptProperty*>(gosProp);
	numBox->setUserData(XE_QT_USER_DATA_SHADER_VAR_INFO, userDataInfo);

	//////////////////////////////////////////
	//Set Maximum and Minimum limits to spin box
	numBox->setMinimum(minSize);
	numBox->setMaximum(maxSize);

	numBox->setValue(value);

	//////////////////////////////////////////
	//Connect to methods to verify and update data
	connect(numBox, SIGNAL(editingFinished()), this, SLOT(SpinBoxChanged()));

	//////////////////////////////////////////
	//Finish
	return numBox;
}

QCheckBox* GameObjectScriptPropertyWidget::CreateCheckBox(GameObjectScriptProperty* gosProp)
{
	bool value = false;

	switch (gosProp->m_PropertyType)
	{
		case asTYPEID_BOOL:
			memcpy(&value, gosProp->m_PropertyAddress, sizeof(bool));
			break;

		default:
			XEAssert(false);
			return nullptr;
	}

	//////////////////////////////////////////
	//Create Check Box Widget to Hold Data
	QCheckBox* checkBox = new QCheckBox();

	//////////////////////////////////////////
	//Set Variable Name as User Data to be Able to retrieve information
	XEQTUserTemplateData<GameObjectScriptProperty*>* userDataInfo = new XEQTUserTemplateData<GameObjectScriptProperty*>(gosProp);
	checkBox->setUserData(XE_QT_USER_DATA_SHADER_VAR_INFO, userDataInfo);

	checkBox->setChecked(value);

	//////////////////////////////////////////
	//Connect to methods to verify and update data
	connect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(CheckBoxChanged(int)));

	//////////////////////////////////////////
	//Finish
	return checkBox;
}

void GameObjectScriptPropertyWidget::SpinBoxChanged()
{
	if(!m_IsReady)
	{
		return;
	}

	//////////////////////////////////////////
	//Get Sender and Verify it is not null
	QObject* objSender = QObject::sender();

	XEAssert(objSender != nullptr);
	if(objSender == nullptr)
	{
		return;
	}

	//////////////////////////////////////////
	//Gets sender User Data
	XEQTUserTemplateData<GameObjectScriptProperty*>* userInfo = reinterpret_cast<XEQTUserTemplateData<GameObjectScriptProperty*>*>(objSender->userData(XE_QT_USER_DATA_SHADER_VAR_INFO));

	XEAssert(userInfo != nullptr);
	if(userInfo == nullptr)
	{
		return;
	}

	//////////////////////////////////////////
	//Get Game Object Script Property
	GameObjectScriptProperty* gosProp = userInfo->m_Data;

	XEAssert(gosProp != nullptr);
	if (gosProp == nullptr)
	{
		XETODO("Add log");
		return;
	}

	switch (gosProp->m_PropertyType)
	{
		case asTYPEID_UINT8:
			{
				QSpinBox* numBox = reinterpret_cast<QSpinBox*>(objSender);

				uint8_t newData = (uint8_t)numBox->value();

				memcpy(gosProp->m_PropertyAddress, &newData, sizeof(uint8_t));
			}
			break;

		case asTYPEID_UINT16:
			{
				QSpinBox* numBox = reinterpret_cast<QSpinBox*>(objSender);

				uint16_t newData = (uint16_t)numBox->value();

				memcpy(gosProp->m_PropertyAddress, &newData, sizeof(uint16_t));
			}
			break;

		case asTYPEID_UINT32:
			{
				QSpinBox* numBox = reinterpret_cast<QSpinBox*>(objSender);

				uint32_t newData = (uint32_t)numBox->value();

				memcpy(gosProp->m_PropertyAddress, &newData, sizeof(uint32_t));
			}
			break;

		case asTYPEID_UINT64:
			{
				QSpinBox* numBox = reinterpret_cast<QSpinBox*>(objSender);

				uint64_t newData = (uint64_t)numBox->value();

				memcpy(gosProp->m_PropertyAddress, &newData, sizeof(uint64_t));
			}
			break;

		case asTYPEID_INT8:
			{
				QSpinBox* numBox = reinterpret_cast<QSpinBox*>(objSender);

				int8_t newData = (int8_t)numBox->value();

				memcpy(gosProp->m_PropertyAddress, &newData, sizeof(int8_t));
			}
			break;

		case asTYPEID_INT16:
			{
				QSpinBox* numBox = reinterpret_cast<QSpinBox*>(objSender);

				int16_t newData = (int16_t)numBox->value();

				memcpy(gosProp->m_PropertyAddress, &newData, sizeof(int16_t));
			}
			break;

		case asTYPEID_INT32:
			{
				QSpinBox* numBox = reinterpret_cast<QSpinBox*>(objSender);

				int32_t newData = (int32_t)numBox->value();

				memcpy(gosProp->m_PropertyAddress, &newData, sizeof(int32_t));
			}
			break;

		case asTYPEID_INT64:
			{
				QSpinBox* numBox = reinterpret_cast<QSpinBox*>(objSender);

				int64_t newData = (int64_t)numBox->value();

				memcpy(gosProp->m_PropertyAddress, &newData, sizeof(uint64_t));
			}
			break;

		case asTYPEID_FLOAT:
			{
				QDoubleSpinBox* numBox = reinterpret_cast<QDoubleSpinBox*>(objSender);

				float newData = (float)numBox->value();

				memcpy(gosProp->m_PropertyAddress, &newData, sizeof(float));
			}
			break;

		case asTYPEID_DOUBLE:
			{
				QDoubleSpinBox* numBox = reinterpret_cast<QDoubleSpinBox*>(objSender);

				double newData = numBox->value();

				memcpy(gosProp->m_PropertyAddress, &newData, sizeof(double));
			}
			break;

		//////////////////////////////////////////
		//Log and Assert if Type is not what we expect in this function
		default:
			XEAssert(false);
			XETODO("log error");
			break;
	}
}

void GameObjectScriptPropertyWidget::CheckBoxChanged(int newState)
{
	if (!m_IsReady)
	{
		return;
	}

	//////////////////////////////////////////
	//Get Sender and Verify it is not null
	QObject* objSender = QObject::sender();

	XEAssert(objSender != nullptr);
	if (objSender == nullptr)
	{
		return;
	}

	//////////////////////////////////////////
	//Gets sender User Data
	XEQTUserTemplateData<GameObjectScriptProperty*>* userInfo = reinterpret_cast<XEQTUserTemplateData<GameObjectScriptProperty*>*>(objSender->userData(XE_QT_USER_DATA_SHADER_VAR_INFO));

	XEAssert(userInfo != nullptr);
	if (userInfo == nullptr)
	{
		return;
	}

	//////////////////////////////////////////
	//Get Game Object Script Property
	GameObjectScriptProperty* gosProp = userInfo->m_Data;

	XEAssert(gosProp != nullptr);
	if (gosProp == nullptr)
	{
		XETODO("Add log");
		return;
	}

	switch (gosProp->m_PropertyType)
	{
		//////////////////////////////////////////
		//Get Data for Bool and set to Variable
		case asTYPEID_BOOL:
		{
			//////////////////////////////////////////
			//Get Data for Bool and set to Variable
			QCheckBox* checkBox = reinterpret_cast<QCheckBox*>(objSender);

			bool newData = checkBox->isChecked();

			XETODO("check return");
			memcpy(gosProp->m_PropertyAddress, &newData, sizeof(bool));
		}
		break;

		//////////////////////////////////////////
		//Log and Assert if Type is not what we expect in this function
		default:
			XEAssert(false);
			XETODO("log error");
			break;
	}
}
