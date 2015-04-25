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

/*************************
*   3rd Party Includes   *
**************************/
#include "qstring.h"
#include "QtWidgets/QApplication"

/***************************
*   Game Engine Includes   *
****************************/
#include "XEQTDefs.h"
#include "GameContentDefs.h"
#include "Lights\LightDefs.h"
#include "Base\BaseFunctions.h"
#include "GameObject\GameObjectDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/****************
*   Constants   *
*****************/
const QString XE_QT_Pos_Vec_Names[4] = {"x", "y", "z", "w"};

/********************
*   Function Defs   *
*********************/

/******************
*   XEQTHelpers   *
*******************/
namespace XEQTHelpers
{

	void SetComboBoxGameContentType(QComboBox* comboBox)
	{
		XEAssert(comboBox != nullptr);

		if(comboBox == nullptr)
		{
			return;
		}
	
		comboBox->addItem(QString::fromStdWString(XEGameContentHelpers::GameContentTypeToString(GameContentType::Unknown)),				QVariant(static_cast<uint>(GameContentType::Unknown)));
		comboBox->addItem(QString::fromStdWString(XEGameContentHelpers::GameContentTypeToString(GameContentType::Model)),				QVariant(static_cast<uint>(GameContentType::Model)));
		comboBox->addItem(QString::fromStdWString(XEGameContentHelpers::GameContentTypeToString(GameContentType::Mesh)),				QVariant(static_cast<uint>(GameContentType::Mesh)));
		comboBox->addItem(QString::fromStdWString(XEGameContentHelpers::GameContentTypeToString(GameContentType::Animation)),			QVariant(static_cast<uint>(GameContentType::Animation)));
		comboBox->addItem(QString::fromStdWString(XEGameContentHelpers::GameContentTypeToString(GameContentType::Skeleton)),			QVariant(static_cast<uint>(GameContentType::Skeleton)));
		comboBox->addItem(QString::fromStdWString(XEGameContentHelpers::GameContentTypeToString(GameContentType::Shader)),				QVariant(static_cast<uint>(GameContentType::Shader)));
		comboBox->addItem(QString::fromStdWString(XEGameContentHelpers::GameContentTypeToString(GameContentType::Texture)),				QVariant(static_cast<uint>(GameContentType::Texture)));
		comboBox->addItem(QString::fromStdWString(XEGameContentHelpers::GameContentTypeToString(GameContentType::GameObjectScript)),	QVariant(static_cast<uint>(GameContentType::GameObjectScript)));
		comboBox->addItem(QString::fromStdWString(XEGameContentHelpers::GameContentTypeToString(GameContentType::Audio)),				QVariant(static_cast<uint>(GameContentType::Audio)));
	}

	void SetComboBoxGameContentSubType(QComboBox* comboBox)
	{
		XEAssert(comboBox != nullptr);
		if(comboBox == nullptr)
		{
			return;
		}

		comboBox->addItem(QString::fromStdWString(XEGameContentHelpers::GameContentSubtypeToString(GameContentSubtype::None)),					QVariant(static_cast<uint>(GameContentSubtype::None)));
		comboBox->addItem(QString::fromStdWString(XEGameContentHelpers::GameContentSubtypeToString(GameContentSubtype::AnimationClip)),			QVariant(static_cast<uint>(GameContentSubtype::AnimationClip)));
		comboBox->addItem(QString::fromStdWString(XEGameContentHelpers::GameContentSubtypeToString(GameContentSubtype::VertexShaderHLSL)),		QVariant(static_cast<uint>(GameContentSubtype::VertexShaderHLSL)));
		comboBox->addItem(QString::fromStdWString(XEGameContentHelpers::GameContentSubtypeToString(GameContentSubtype::PixelShaderHLSL)),		QVariant(static_cast<uint>(GameContentSubtype::PixelShaderHLSL)));
		comboBox->addItem(QString::fromStdWString(XEGameContentHelpers::GameContentSubtypeToString(GameContentSubtype::GeometryShaderHLSL)),	QVariant(static_cast<uint>(GameContentSubtype::GeometryShaderHLSL)));
		comboBox->addItem(QString::fromStdWString(XEGameContentHelpers::GameContentSubtypeToString(GameContentSubtype::ComputeShaderHLSL)),		QVariant(static_cast<uint>(GameContentSubtype::ComputeShaderHLSL)));
		comboBox->addItem(QString::fromStdWString(XEGameContentHelpers::GameContentSubtypeToString(GameContentSubtype::HullShaderHLSL)),		QVariant(static_cast<uint>(GameContentSubtype::HullShaderHLSL)));
		comboBox->addItem(QString::fromStdWString(XEGameContentHelpers::GameContentSubtypeToString(GameContentSubtype::DomainShaderHLSL)),		QVariant(static_cast<uint>(GameContentSubtype::DomainShaderHLSL)));
		comboBox->addItem(QString::fromStdWString(XEGameContentHelpers::GameContentSubtypeToString(GameContentSubtype::Texture2D)),				QVariant(static_cast<uint>(GameContentSubtype::Texture2D)));
		comboBox->addItem(QString::fromStdWString(XEGameContentHelpers::GameContentSubtypeToString(GameContentSubtype::TextureCube)),			QVariant(static_cast<uint>(GameContentSubtype::TextureCube)));
		comboBox->addItem(QString::fromStdWString(XEGameContentHelpers::GameContentSubtypeToString(GameContentSubtype::Unknown)),				QVariant(static_cast<uint>(GameContentSubtype::Unknown)));
	}

	void SetComboBoxGameObjectComponentTypeOption(QComboBox* comboBox)
	{
		XEAssert(comboBox != nullptr);
		if(comboBox == nullptr)
		{
			return;
		}

		comboBox->addItem("Game Object Script",		QVariant(static_cast<uint>(GameObjectComponentTypeOption::GameObjectScript)));
		comboBox->addItem("Mesh",					QVariant(static_cast<uint>(GameObjectComponentTypeOption::Mesh)));
		comboBox->addItem("Mesh Material",			QVariant(static_cast<uint>(GameObjectComponentTypeOption::MeshMaterial)));
		comboBox->addItem("Mesh Animation",			QVariant(static_cast<uint>(GameObjectComponentTypeOption::MeshAnimation)));
		comboBox->addItem("Light",					QVariant(static_cast<uint>(GameObjectComponentTypeOption::Light)));
		comboBox->addItem("Rigid Body",				QVariant(static_cast<uint>(GameObjectComponentTypeOption::RigidBody)));
		comboBox->addItem("Box Collider",			QVariant(static_cast<uint>(GameObjectComponentTypeOption::BoxCollider)));
		comboBox->addItem("Sphere Collider",		QVariant(static_cast<uint>(GameObjectComponentTypeOption::SphereCollider)));
		comboBox->addItem("Camera",					QVariant(static_cast<uint>(GameObjectComponentTypeOption::Camera)));
		comboBox->addItem("Audio Source",			QVariant(static_cast<uint>(GameObjectComponentTypeOption::AudioSource)));
		comboBox->addItem("Audio Listener",			QVariant(static_cast<uint>(GameObjectComponentTypeOption::AudioListener)));
	}

	DragDropType GetDragDropType(QDropEvent* dropEvent)
	{
		XEAssert(dropEvent != nullptr);
		if(dropEvent == nullptr)
		{
			return DragDropType::Unknown;
		}

		////////////////////////////////
		//Get Sender Object
		QObject* sender = dropEvent->source();

		////////////////////////////////
		//If Sender Object is null get type by mime
		if(sender == nullptr)
		{
			const QMimeData* qMimeData = dropEvent->mimeData();

			if(qMimeData->hasFormat(XE_QT_MIME_FILE_TYPE_1) || qMimeData->hasFormat(XE_QT_MIME_FILE_TYPE_2))
			{
				return DragDropType::File;
			}
		}
		else
		{
			XEQTUserTemplateData<XEQTObjType>* userData = reinterpret_cast<XEQTUserTemplateData<XEQTObjType>*>(sender->userData(XE_QT_USER_DATA_OBJ_TYPE_SLOT));

			if(userData == nullptr)
			{
				return DragDropType::Unknown;
			}

			switch (userData->m_Data)
			{
				case XEQTObjType::GameAssetsTree:
					return DragDropType::GameAsset;
				case XEQTObjType::RawGameAssetsTree:
					return DragDropType::RawGameAsset;
				case XEQTObjType::GameObjectsTree:
					return DragDropType::GameObject;
			}
		}

		return DragDropType::Unknown;
	}

	Color GetColorFromQColor(const QColor& qColor)
	{
		uint32_t alpha	= qColor.alpha();
		uint32_t red	= qColor.red();
		uint32_t blue	= qColor.blue();
		uint32_t green	= qColor.green();

		Color color(red, green, blue, alpha);

		return color;
	}

	QColor GetQColorFromColor(const Color& color)
	{
		QColor qColor(color.r, color.g, color.b, color.a);

		return qColor;
	}

	void SetComboBoxLightType(QComboBox* comboBox)
	{
		XEAssert(comboBox != nullptr);
		if(comboBox == nullptr)
		{
			return;
		}

		comboBox->addItem("Spot Light",			QVariant(static_cast<uint>(LightType::Spot)));
		comboBox->addItem("Omni Light",			QVariant(static_cast<uint>(LightType::Omni)));
		comboBox->addItem("Directional Light",	QVariant(static_cast<uint>(LightType::Directional)));
	}

}