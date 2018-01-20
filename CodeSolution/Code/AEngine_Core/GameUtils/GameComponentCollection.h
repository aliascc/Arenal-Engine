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

#pragma once

#ifndef _GAME_COMPONENT_COLLECTION_H
#define _GAME_COMPONENT_COLLECTION_H

/**********************
*   System Includes   *
***********************/
#include <map>
#include <list>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "GameUtilsDefs.h"
#include "Base\AEObject.h"

/********************
*   Forward Decls   *
*********************/
struct TimerParams;
class GameComponent;

/*****************
*   Class Decl   *
******************/
class GameComponentCollection : public AEObject
{
    //Typedef for STL Game Component List 
    typedef std::list<GameComponent*>           GCList;
    typedef std::map<uint64_t, GameComponent*>  GCMap;
    
    //Function to sort container
    struct SortGCCol : public std::binary_function<GameComponent*, GameComponent*, bool>
    {
        bool operator()(GameComponent* left, GameComponent* right) const;
    };

    private:
        
        /*****************************
        *      Private Variables     *
        ******************************/
#pragma region Private Variables

        //Game Component Collection Map for fast access
        GCMap m_Map;

        //Game Component Collection Container
        GCList m_Container;

        //If true we have to sort the list next update call
        bool m_NeedSort = false;

        SortGCCol m_SortGCContainer;

#pragma endregion

        /***************************
        *      Private Methods     *
        ****************************/
#pragma region Private Methods
        
        void SortContainer();

        inline void NeedSortChange()
        { 
            m_NeedSort = true; 
        }

#pragma endregion

    public:

        /****************************************
         *   Constructor & Destructor Methods   *
         ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// GameComponentCollection Constructor
        /// </summary>
        GameComponentCollection();

        /// <summary>
        /// Default GameComponentCollection Destructor
        /// </summary>
        virtual ~GameComponentCollection();

#pragma endregion

        //Framework Methods
        AEResult Add(GameComponent* gc);
        AEResult Remove(GameComponent* gc);
        AEResult SetGCDrawCallOrder(uint32_t id, uint32_t drawCallOrder);
        AEResult SetGCVisible(uint32_t id, bool visible);
        AEResult SetGCEnable(uint32_t id, bool enable);
        GCBasicProps GetGCBasicProps(uint32_t index);
        GCBasicProps GetGCBasicPropsID(uint32_t id);

        //Function to find out if GameComponent Exists
        bool DoesGCExist(GameComponent* gc) const;
             
        bool DoesIDExist(uint64_t id) const;
             
        bool ValidIndex(uint32_t index) const;

        //Get Methods
        uint32_t GetSize() const;


        //Update and Render Methods
        void ConstantUpdateCollection(const TimerParams& timerParams);
        void UpdateCollection(const TimerParams& timerParams);
        void PostUpdateCollection(const TimerParams& timerParams);
        void RenderCollection(const TimerParams& timerParams);
        void InitializeCollection();
        void LoadContentCollection();
        void UnLoadContentCollection();
        void OnLostDeviceCollection();
        void OnResetDeviceCollection();
};

#endif
