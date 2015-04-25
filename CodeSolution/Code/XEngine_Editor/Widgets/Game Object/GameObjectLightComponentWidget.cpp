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
#include "qcolordialog.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Lights\Light.h"
#include "Utils\XEQTDefs.h"
#include "Lights\SpotLight.h"
#include "GameObject\GameObject.h"
#include "Engine Viewer\EngineViewer.h"
#include "GameObjectLightComponentWidget.h"
#include "GameObject\Components\LightGOC.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameObjectLightComponentWidget::GameObjectLightComponentWidget(GameObject* gameObject, EngineViewer* engineViewer, QWidget *parent)
	: QWidget(parent)
	, m_GameObject(gameObject)
	, m_EngineViewer(engineViewer)
{
	m_GameObjectLightComponentWidgetQtUI.setupUi(this);

	XEAssert(m_GameObject != nullptr);

	XEAssert(m_EngineViewer != nullptr);

	////////////////////////////////////////
	// Initialize Combo Box with Light Types
	XEQTHelpers::SetComboBoxLightType(m_GameObjectLightComponentWidgetQtUI.m_LightTypeComboBox);
	m_GameObjectLightComponentWidgetQtUI.m_LightTypeComboBox->setCurrentIndex(0);

	InitFields();
}

GameObjectLightComponentWidget::~GameObjectLightComponentWidget()
{
}

void GameObjectLightComponentWidget::SetColorToColorWidget(const QColor& qColor)
{
	/********************
	* Setting Background can work 2 ways, by style sheet (using specific name "QWidget#varName") or 
	* by setting the color through the Palette property we need to use the style way cause we are
	* cascading the style sheets from the parent
	*
	* IMPORTANT
	* If variable name changes, change it in the argument list.
	*********************/


	////////////////////////////////////////
	// Set Color to Widget
	QString newStyleSheet = tr("QWidget#%1 { background-color: rgb(%2, %3, %4); }")
							.arg(XESTR(m_ColorWidget))
							.arg(qColor.red())
							.arg(qColor.green())
							.arg(qColor.blue());

	m_GameObjectLightComponentWidgetQtUI.m_ColorWidget->setStyleSheet(newStyleSheet);

	/********************
	* Second way of doing the background color change
	*********************/
	//QPalette qPalette = m_GameObjectLightComponentWidgetQtUI.m_ColorWidget->palette();
	//qPalette.setColor(QPalette::Background, qColor);
	//m_GameObjectLightComponentWidgetQtUI.m_ColorWidget->setAutoFillBackground(true);
	//m_GameObjectLightComponentWidgetQtUI.m_ColorWidget->setPalette(qPalette);
}

void GameObjectLightComponentWidget::SetLightTypeComboBoxIndex(LightType lightType)
{
	uint32_t numItems = m_GameObjectLightComponentWidgetQtUI.m_LightTypeComboBox->count();

	bool found = false;

	for(uint32_t i = 0; i < numItems; i++)
	{
		LightType currentLT = static_cast<LightType>(m_GameObjectLightComponentWidgetQtUI.m_LightTypeComboBox->itemData(i).toUInt());

		if(currentLT == lightType)
		{
			m_GameObjectLightComponentWidgetQtUI.m_LightTypeComboBox->setCurrentIndex(i);

			found = true;
		}
	}

	//Should never Get here
	XEAssert(found);
}

LightGOC* GameObjectLightComponentWidget::GetLightGOC()
{
	XEAssert(m_GameObject != nullptr);
	if(m_GameObject == nullptr)
	{
		return nullptr;
	}

	XEAssert(m_GameObject->HasLightGOC());
	if(!m_GameObject->HasLightGOC())
	{
		return nullptr;
	}

	////////////////////////////////////////
	// Get Light GOC
	LightGOC* lightGOC = m_GameObject->GetLightGOC();

	return lightGOC;
}

Light* GameObjectLightComponentWidget::GetLight()
{
	////////////////////////////////////////
	// Get Light GOC
	LightGOC* lightGOC = GetLightGOC();

	XEAssert(lightGOC != nullptr);
	if(lightGOC == nullptr)
	{
		return nullptr;
	}

	////////////////////////////////////////
	// Get Light
	Light* light = lightGOC->GetLight();

	////////////////////////////////////////
	// Verify Light Instance
	XEAssert(light != nullptr);
	if(light == nullptr)
	{
		XETODO("Set Log");
		return nullptr;
	}

	return light;
}

void GameObjectLightComponentWidget::InitFields()
{
	Light* light = nullptr;

	////////////////////////////////////////
	// Set Ready to false, so Light cannot change
	// properties while been initialized
	m_IsReady = false;

	////////////////////////////////////////
	// Check Engine Viewer
	XEAssert(m_EngineViewer != nullptr);
	if (m_EngineViewer == nullptr)
	{
		return;
	}

	////////////////////////////////////////
	// Verify LOC and Get Light Object
	light = GetLight();

	if(light == nullptr)
	{
		XETODO("Add log");

		return;
	}

	////////////////////////////////////////
	// Set Light Type
	SetLightTypeComboBoxIndex(light->GetLightType());

	////////////////////////////////////////
	// Set Enabled
	m_GameObjectLightComponentWidgetQtUI.m_Enabled->setChecked(light->IsEnabled());

	////////////////////////////////////////
	// Set Intensity
	m_GameObjectLightComponentWidgetQtUI.m_IntensitySB->setValue(static_cast<double>(light->GetIntensity()));

	////////////////////////////////////////
	// Set Color
	QColor qColor = XEQTHelpers::GetQColorFromColor(light->GetColor());

	SetColorToColorWidget(qColor);

	////////////////////////////////////////
	// Set Near and Far Attenuation
	m_GameObjectLightComponentWidgetQtUI.m_NearAttSB->setValue(static_cast<double>(light->GetNearAttenuation()));
	m_GameObjectLightComponentWidgetQtUI.m_FarAttSB->setValue(static_cast<double>(light->GetFarAttenuation()));

	////////////////////////////////////////
	// Set Shadow Enabled
	m_GameObjectLightComponentWidgetQtUI.m_ShadowEnabled->setChecked(light->IsShadowEnabled());
	m_GameObjectLightComponentWidgetQtUI.m_DrawFrustum->setEnabled(light->IsShadowEnabled());

	////////////////////////////////////////
	// Enable/Visibility of Angle options
	if(light->GetLightType() == LightType::Spot)
	{
		m_GameObjectLightComponentWidgetQtUI.m_LabelAngle->setVisible(true);
		m_GameObjectLightComponentWidgetQtUI.m_LabelFalloffAngle->setVisible(true);

		m_GameObjectLightComponentWidgetQtUI.m_AngleSB->setVisible(true);
		m_GameObjectLightComponentWidgetQtUI.m_FalloffAngleSB->setVisible(true);

		////////////////////////////////////////
		// Get Spot Light
		SpotLight* spotLight = reinterpret_cast<SpotLight*>(light);

		////////////////////////////////////////
		// Set Angle and Fall out Angle
		m_GameObjectLightComponentWidgetQtUI.m_AngleSB->setValue(static_cast<double>(spotLight->GetAngle()));
		m_GameObjectLightComponentWidgetQtUI.m_FalloffAngleSB->setValue(static_cast<double>(spotLight->GetFallOffAngle()));
	}
	else
	{
		m_GameObjectLightComponentWidgetQtUI.m_LabelAngle->setVisible(false);
		m_GameObjectLightComponentWidgetQtUI.m_LabelFalloffAngle->setVisible(false);

		m_GameObjectLightComponentWidgetQtUI.m_AngleSB->setVisible(false);
		m_GameObjectLightComponentWidgetQtUI.m_FalloffAngleSB->setVisible(false);
	}

	////////////////////////////////////////
	// Enable/Visibility of Draw Frustum Cascades
	if (light->GetLightType() == LightType::Directional)
	{
		m_GameObjectLightComponentWidgetQtUI.m_DrawCascadeFrustums->setVisible(true);
	}
	else
	{
		m_GameObjectLightComponentWidgetQtUI.m_DrawCascadeFrustums->setVisible(false);
	}

	////////////////////////////////////////
	// Light is Ready to change properties
	m_IsReady = true;
}

void GameObjectLightComponentWidget::on_m_IntensitySB_valueChanged(double newValue)
{
	if (!m_IsReady)
	{
		return;
	}

	Light* light = nullptr;

	////////////////////////////////////////
	// Verify LOC and Get Light Object
	light = GetLight();

	if(light == nullptr)
	{
		XETODO("Add log");

		return;
	}

	float newIntensity = static_cast<float>(m_GameObjectLightComponentWidgetQtUI.m_IntensitySB->value());

	light->SetIntensity(newIntensity);
}

void GameObjectLightComponentWidget::on_m_NearAttSB_editingFinished()
{
	if (!m_IsReady)
	{
		return;
	}

	Light* light = nullptr;

	////////////////////////////////////////
	// Verify LOC and Get Light Object
	light = GetLight();

	if(light == nullptr)
	{
		XETODO("Add log");

		return;
	}

	float newNearAtt = static_cast<float>(m_GameObjectLightComponentWidgetQtUI.m_NearAttSB->value());

	float farAtt = light->GetFarAttenuation();

	if (newNearAtt > farAtt)
	{
		light->SetFarAtteniation(newNearAtt);
		m_GameObjectLightComponentWidgetQtUI.m_FarAttSB->setValue(newNearAtt);
	}

	light->SetNearAttenuation(newNearAtt);
}

void GameObjectLightComponentWidget::on_m_FarAttSB_editingFinished()
{
	if (!m_IsReady)
	{
		return;
	}

	Light* light = nullptr;

	////////////////////////////////////////
	// Verify LOC and Get Light Object
	light = GetLight();

	if(light == nullptr)
	{
		XETODO("Add log");

		return;
	}

	float newFarAtt = static_cast<float>(m_GameObjectLightComponentWidgetQtUI.m_FarAttSB->value());

	float nearAtt = light->GetNearAttenuation();

	if (newFarAtt < nearAtt)
	{
		light->SetNearAttenuation(newFarAtt);
		m_GameObjectLightComponentWidgetQtUI.m_NearAttSB->setValue(newFarAtt);
	}

	light->SetFarAtteniation(newFarAtt);
}

void GameObjectLightComponentWidget::on_m_AngleSB_editingFinished()
{
	if (!m_IsReady)
	{
		return;
	}

	Light* light = nullptr;

	////////////////////////////////////////
	// Verify LOC and Get Light Object
	light = GetLight();

	if(light == nullptr)
	{
		XETODO("Add log");

		return;
	}

	////////////////////////////////////////
	// Verify that it is Spot Light
	if(light->GetLightType() != LightType::Spot)
	{
		return;
	}

	////////////////////////////////////////
	// Get Spot Light
	SpotLight* spotLight = reinterpret_cast<SpotLight*>(light);

	float newAngle = static_cast<float>(m_GameObjectLightComponentWidgetQtUI.m_AngleSB->value());

	float fallOffAngle = spotLight->GetFallOffAngle();

	if (newAngle > fallOffAngle)
	{
		spotLight->SetFallOffAngle(newAngle);
		m_GameObjectLightComponentWidgetQtUI.m_FalloffAngleSB->setValue(newAngle);
	}

	spotLight->SetAngle(newAngle);
}

void GameObjectLightComponentWidget::on_m_FalloffAngleSB_editingFinished()
{
	if (!m_IsReady)
	{
		return;
	}

	Light* light = nullptr;

	////////////////////////////////////////
	// Verify LOC and Get Light Object
	light = GetLight();

	if(light == nullptr)
	{
		XETODO("Add log");

		return;
	}

	////////////////////////////////////////
	// Verify that it is Spot Light
	if(light->GetLightType() != LightType::Spot)
	{
		return;
	}

	////////////////////////////////////////
	// Get Spot Light
	SpotLight* spotLight = reinterpret_cast<SpotLight*>(light);

	float newFalloffAngle = static_cast<float>(m_GameObjectLightComponentWidgetQtUI.m_FalloffAngleSB->value());

	float angle = spotLight->GetFallOffAngle();

	if (newFalloffAngle < angle)
	{
		spotLight->SetAngle(newFalloffAngle);
		m_GameObjectLightComponentWidgetQtUI.m_AngleSB->setValue(newFalloffAngle);
	}

	spotLight->SetFallOffAngle(newFalloffAngle);
}

void GameObjectLightComponentWidget::on_m_ColorChangerButton_clicked()
{
	if (!m_IsReady)
	{
		return;
	}

	Light* light = nullptr;

	////////////////////////////////////////
	// Verify LOC and Get Light Object
	light = GetLight();

	if(light == nullptr)
	{
		XETODO("Add log");

		return;
	}

	////////////////////////////////////////
	// Get QColor from Dialog
	QColorDialog qColorDiaglog;

	int result = qColorDiaglog.exec();

	if(result != QDialog::Accepted)
	{
		return;
	}

	QColor qColor = qColorDiaglog.selectedColor();

	////////////////////////////////////////
	// Set Color to Widget
	SetColorToColorWidget(qColor);

	////////////////////////////////////////
	// Set Color to Instance
	Color color = XEQTHelpers::GetColorFromQColor(qColor);

	light->SetColor(color);
}

void GameObjectLightComponentWidget::on_m_Enabled_stateChanged(int newState)
{
	if (!m_IsReady)
	{
		return;
	}

	Light* light = nullptr;

	////////////////////////////////////////
	// Verify LOC and Get Light Object
	light = GetLight();

	if(light == nullptr)
	{
		XETODO("Add log");

		return;
	}

	light->SetEnabled(m_GameObjectLightComponentWidgetQtUI.m_Enabled->isChecked());
}

void GameObjectLightComponentWidget::on_m_ShadowEnabled_stateChanged(int newState)
{
	if (!m_IsReady)
	{
		return;
	}

	Light* light = nullptr;

	////////////////////////////////////////
	// Verify LOC and Get Light Object
	light = GetLight();

	if (light == nullptr)
	{
		XETODO("Add log");

		return;
	}

	bool shadowEnabled = m_GameObjectLightComponentWidgetQtUI.m_ShadowEnabled->isChecked();

	light->SetShadowEnabled(shadowEnabled);

	m_GameObjectLightComponentWidgetQtUI.m_DrawFrustum->setEnabled(shadowEnabled);

	if (!shadowEnabled)
	{
		m_GameObjectLightComponentWidgetQtUI.m_DrawFrustum->setChecked(false);
		m_GameObjectLightComponentWidgetQtUI.m_DrawCascadeFrustums->setChecked(false);
	}
}

void GameObjectLightComponentWidget::on_m_DrawFrustum_stateChanged(int newState)
{
	if (!m_IsReady)
	{
		return;
	}

	////////////////////////////////////////
	// Get Light GOC
	LightGOC* lightGOC = GetLightGOC();

	if (lightGOC == nullptr)
	{
		XETODO("Add log");

		return;
	}

	lightGOC->SetDrawFrustumEnabled(m_GameObjectLightComponentWidgetQtUI.m_DrawFrustum->isChecked());
}

void GameObjectLightComponentWidget::on_m_DrawCascadeFrustums_stateChanged(int newState)
{
	if (!m_IsReady)
	{
		return;
	}

	////////////////////////////////////////
	// Get Light GOC
	LightGOC* lightGOC = GetLightGOC();

	if (lightGOC == nullptr)
	{
		XETODO("Add log");

		return;
	}

	lightGOC->SetDrawFrustumCascadesEnabled(m_GameObjectLightComponentWidgetQtUI.m_DrawCascadeFrustums->isChecked());
}

void GameObjectLightComponentWidget::on_m_DrawSimpleFrustum_stateChanged(int newState)
{
	if (!m_IsReady)
	{
		return;
	}

	////////////////////////////////////////
	// Get Light GOC
	LightGOC* lightGOC = GetLightGOC();

	if (lightGOC == nullptr)
	{
		XETODO("Add log");

		return;
	}

	lightGOC->SetDrawFrustumSimple(m_GameObjectLightComponentWidgetQtUI.m_DrawSimpleFrustum->isChecked());
}

void GameObjectLightComponentWidget::on_m_DrawDebug_stateChanged(int newState)
{
	if (!m_IsReady)
	{
		return;
	}

	////////////////////////////////////////
	// Get Light GOC
	LightGOC* lightGOC = GetLightGOC();

	if (lightGOC == nullptr)
	{
		XETODO("Add log");

		return;
	}

	lightGOC->SetDrawDebugEnabled(m_GameObjectLightComponentWidgetQtUI.m_DrawDebug->isChecked());
}

void GameObjectLightComponentWidget::on_m_LightTypeComboBox_currentIndexChanged(int newIndex)
{
	if (!m_IsReady)
	{
		return;
	}

	LightGOC* lightGOC = nullptr;

	////////////////////////////////////////
	// Get Light GOC
	lightGOC = GetLightGOC();

	if(lightGOC == nullptr)
	{
		XETODO("Add log");

		return;
	}

	int currentIndex = m_GameObjectLightComponentWidgetQtUI.m_LightTypeComboBox->currentIndex();
	LightType currentLT = static_cast<LightType>(m_GameObjectLightComponentWidgetQtUI.m_LightTypeComboBox->itemData(currentIndex).toUInt());

	XEResult ret = lightGOC->ChangeLightType(currentLT);

	if(ret != XEResult::Ok)
	{
		XETODO("Log error");
	}

	InitFields();
}