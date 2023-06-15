/*
* Copyright (c) <2021> Side Effects Software Inc.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice,
*    this list of conditions and the following disclaimer.
*
* 2. The name of Side Effects Software may not be used to endorse or
*    promote products derived from this software without specific prior
*    written permission.
*
* THIS SOFTWARE IS PROVIDED BY SIDE EFFECTS SOFTWARE "AS IS" AND ANY EXPRESS
* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
* NO EVENT SHALL SIDE EFFECTS SOFTWARE BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include "RawMesh.h"
#include "GameFramework/Actor.h"
#include "Editor.h"
#include "HAPI/HAPI_Common.h"
#include "Runtime/Launch/Resources/Version.h"


struct FHoudiniPackageParams;
struct FHoudiniDataLayer;

// Determine if we can enable data layers or not. The public API still exists in all versions
// to minimize the number of defines in the code.

#if ENGINE_MINOR_VERSION >= 2 && WITH_EDITOR
#define HOUDINI_ENABLE_DATA_LAYERS (1)
#else
#define HOUDINI_ENABLE_DATA_LAYERS (0)
#endif

class HOUDINIENGINE_API FHoudiniDataLayerUtils
{
public:
	// Extracts the data layer from the Houdini Geo/Part and applies it to the Actor. Normally
	// called after cooking/baking.
	static void ApplyDataLayersToActor(const FHoudiniPackageParams& Params, AActor* Actor, const TArray<FHoudiniDataLayer>& Layers);

	static TArray<FHoudiniDataLayer> GetDataLayers(HAPI_NodeId NodeId, HAPI_PartId PartId);

#if HOUDINI_ENABLE_DATA_LAYERS
	static void AddActorToLayer(const FHoudiniPackageParams& Params, AWorldDataLayers* WorldDataLayers, AActor* Actor, const FHoudiniDataLayer& Layer);
	static UDataLayerAsset* CreateDataLayerAsset(const FHoudiniPackageParams& Params, const FString & LayerName);
#endif
};

class AActor;
class FName;


struct FHoudiniUnrealDataLayerInfo
{
	// Information about he data layer being exported. Currently, just the name.
	FString Name;
};

struct FHoudiniUnrealDataLayersCache
{
	// There doesn't seem to be an efficient way to get a list of data layers for each actor in Unreal,
	// you would have to go through each data layer and see if the actor is a member. So this struct is
	// used to cache off the data layer information once so the information is available efficently.

	TMap<AActor*, TArray<int> > ActorDataLayers;
	TArray<FHoudiniUnrealDataLayerInfo> DataLayerInfos;

	static FHoudiniUnrealDataLayersCache MakeCache(UWorld*);

	// Using this cache, create Houdini Groups for this Actor.
	bool CreateHapiGroups(AActor* Actor, HAPI_NodeId NodeId, HAPI_PartId PartId);
};

