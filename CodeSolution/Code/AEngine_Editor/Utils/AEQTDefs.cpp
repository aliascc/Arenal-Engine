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
#include "AEQTDefs.h"
#include "GameContentDefs.h"
#include "Lights\LightDefs.h"
#include "Base\BaseFunctions.h"
#include "GameObject\GameObjectDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/****************
*   Constants   *
*****************/
const QString AE_QT_Pos_Vec_Names[4] = {"x", "y", "z", "w"};

/********************
*   Function Defs   *
*********************/

/******************
*   AEQTHelpers   *
*******************/
namespace AEQTHelpers
{

	void SetComboBoxGameContentType(QComboBox* comboBox)
	{
		AEAssert(comboBox != nullptr);

		if(comboBox == nullptr)
		{
			return;
		}
	
		comboBox->addItem(QString::fromStdWString(AEGameContentHelpers::GameContentTypeToString(GameContentType::Unknown)),				QVariant(static_cast<uint>(GameContentType::Unknown)));
		comboBox->addItem(QString::fromStdWString(AEGameContentHelpers::GameContentTypeToString(GameContentType::Model)),				QVariant(static_cast<uint>(GameContentType::Model)));
		comboBox->addItem(QString::fromStdWString(AEGameContentHelpers::GameContentTypeToString(GameContentType::Mesh)),				QVariant(static_cast<uint>(GameContentType::Mesh)));
		comboBox->addItem(QString::fromStdWString(AEGameContentHelpers::GameContentTypeToString(GameContentType::Animation)),			QVariant(static_cast<uint>(GameContentType::Animation)));
		comboBox->addItem(QString::fromStdWString(AEGameContentHelpers::GameContentTypeToString(GameContentType::Skeleton)),			QVariant(static_cast<uint>(GameContentType::Skeleton)));
		comboBox->addItem(QString::fromStdWString(AEGameContentHelpers::GameContentTypeToString(GameContentType::Shader)),				QVariant(static_cast<uint>(GameContentType::Shader)));
		comboBox->addItem(QString::fromStdWString(AEGameContentHelpers::GameContentTypeToString(GameContentType::Texture)),				QVariant(static_cast<uint>(GameContentType::Texture)));
		comboBox->addItem(QString::fromStdWString(AEGameContentHelpers::GameContentTypeToString(GameContentType::GameObjectScript)),	QVariant(static_cast<uint>(GameContentType::GameObjectScript)));
		comboBox->addItem(QString::fromStdWString(AEGameContentHelpers::GameContentTypeToString(GameContentType::Audio)),				QVariant(static_cast<uint>(GameContentType::Audio)));
	}

	void SetComboBoxGameContentSubType(QComboBox* comboBox)
	{
		AEAssert(comboBox != nullptr);
		if(comboBox == nullptr)
		{
			return;
		}

		comboBox->addItem(QString::fromStdWString(AEGameContentHelpers::GameContentSubtypeToString(GameContentSubtype::None)),					QVariant(static_cast<uint>(GameContentSubtype::None)));
		comboBox->addItem(QString::fromStdWString(AEGameContentHelpers::GameContentSubtypeToString(GameContentSubtype::AnimationClip)),			QVariant(static_cast<uint>(GameContentSubtype::AnimationClip)));
		comboBox->addItem(QString::fromStdWString(AEGameContentHelpers::GameContentSubtypeToString(GameContentSubtype::VertexShaderHLSL)),		QVariant(static_cast<uint>(GameContentSubtype::VertexShaderHLSL)));
		comboBox->addItem(QString::fromStdWString(AEGameContentHelpers::GameContentSubtypeToString(GameContentSubtype::PixelShaderHLSL)),		QVariant(static_cast<uint>(GameContentSubtype::PixelShaderHLSL)));
		comboBox->addItem(QString::fromStdWString(AEGameContentHelpers::GameContentSubtypeToString(GameContentSubtype::GeometryShaderHLSL)),	QVariant(static_cast<uint>(GameContentSubtype::GeometryShaderHLSL)));
		comboBox->addItem(QString::fromStdWString(AEGameContentHelpers::GameContentSubtypeToString(GameContentSubtype::ComputeShaderHLSL)),		QVariant(static_cast<uint>(GameContentSubtype::ComputeShaderHLSL)));
		comboBox->addItem(QString::fromStdWString(AEGameContentHelpers::GameContentSubtypeToString(GameContentSubtype::HullShaderHLSL)),		QVariant(static_cast<uint>(GameContentSubtype::HullShaderHLSL)));
		comboBox->addItem(QString::fromStdWString(AEGameContentHelpers::GameContentSubtypeToString(GameContentSubtype::DomainShaderHLSL)),		QVariant(static_cast<uint>(GameContentSubtype::DomainShaderHLSL)));
		comboBox->addItem(QString::fromStdWString(AEGameContentHelpers::GameContentSubtypeToString(GameContentSubtype::Texture2D)),				QVariant(static_cast<uint>(GameContentSubtype::Texture2D)));
		comboBox->addItem(QString::fromStdWString(AEGameContentHelpers::GameContentSubtypeToString(GameContentSubtype::TextureCube)),			QVariant(static_cast<uint>(GameContentSubtype::TextureCube)));
		comboBox->addItem(QString::fromStdWString(AEGameContentHelpers::GameContentSubtypeToString(GameContentSubtype::Unknown)),				QVariant(static_cast<uint>(GameContentSubtype::Unknown)));
	}

	void SetComboBoxGameObjectComponentTypeOption(QComboBox* comboBox)
	{
		AEAssert(comboBox != nullptr);
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
		AEAssert(dropEvent != nullptr);
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

			if(qMimeData->hasFormat(AE_QT_MIME_FILE_TYPE_1) || qMimeData->hasFormat(AE_QT_MIME_FILE_TYPE_2))
			{
				return DragDropType::File;
			}
		}
		else
		{
			AEQTUserTemplateData<AEQTObjType>* userData = reinterpret_cast<AEQTUserTemplateData<AEQTObjType>*>(sender->userData(AE_QT_USER_DATA_OBJ_TYPE_SLOT));

			if(userData == nullptr)
			{
				return DragDropType::Unknown;
			}

			switch (userData->m_Data)
			{
				case AEQTObjType::GameAssetsTree:
					return DragDropType::GameAsset;
				case AEQTObjType::RawGameAssetsTree:
					return DragDropType::RawGameAsset;
				case AEQTObjType::GameObjectsTree:
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
		AEAssert(comboBox != nullptr);
		if(comboBox == nullptr)
		{
			return;
		}

		comboBox->addItem("Spot Light",			QVariant(static_cast<uint>(LightType::Spot)));
		comboBox->addItem("Omni Light",			QVariant(static_cast<uint>(LightType::Omni)));
		comboBox->addItem("Directional Light",	QVariant(static_cast<uint>(LightType::Directional)));
	}

}