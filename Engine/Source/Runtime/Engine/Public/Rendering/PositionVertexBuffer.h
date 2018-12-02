// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "RenderResource.h"

struct FStaticMeshBuildVertex;

/** A vertex that stores just position. */
struct FPositionVertex
{
	FVector	Position;

	friend FArchive& operator<<(FArchive& Ar, FPositionVertex& V)
	{
		Ar << V.Position;
		return Ar;
	}
};

/** A vertex buffer of positions. */
class FPositionVertexBuffer : public FVertexBuffer
{
public:

	/** Default constructor. */
	ENGINE_API FPositionVertexBuffer();

	/** Destructor. */
	ENGINE_API ~FPositionVertexBuffer();

	/** Delete existing resources */
	ENGINE_API void CleanUp();

	void ENGINE_API Init(uint32 NumVertices, bool bNeedsCPUAccess = true);

	/**
	* Initializes the buffer with the given vertices, used to convert legacy layouts.
	* @param InVertices - The vertices to initialize the buffer with.
	*/
	ENGINE_API void Init(const TArray<FStaticMeshBuildVertex>& InVertices, bool bNeedsCPUAccess = true);

	/**
	* Initializes this vertex buffer with the contents of the given vertex buffer.
	* @param InVertexBuffer - The vertex buffer to initialize from.
	*/
	void Init(const FPositionVertexBuffer& InVertexBuffer, bool bNeedsCPUAccess = true);

	ENGINE_API void Init(const TArray<FVector>& InPositions, bool bNeedsCPUAccess = true);

	/**
	 * Appends the specified vertices to the end of the buffer
	 *
	 * @param	Vertices	The vertex data to be appended.  Must not be nullptr.
	 * @param	NumVerticesToAppend		How many vertices should be added
	 */
	ENGINE_API void AppendVertices( const FStaticMeshBuildVertex* Vertices, const uint32 NumVerticesToAppend );

	/**
	* Serializer
	*
	* @param	Ar				Archive to serialize with
	* @param	bNeedsCPUAccess	Whether the elements need to be accessed by the CPU
	*/
	void Serialize(FArchive& Ar, bool bNeedsCPUAccess);

	/**
	* Specialized assignment operator, only used when importing LOD's.
	*/
	ENGINE_API void operator=(const FPositionVertexBuffer &Other);

	// Vertex data accessors.
	FORCEINLINE FVector& VertexPosition(uint32 VertexIndex)
	{
		checkSlow(VertexIndex < GetNumVertices());
		return ((FPositionVertex*)(Data + VertexIndex * Stride))->Position;
	}
	FORCEINLINE const FVector& VertexPosition(uint32 VertexIndex) const
	{
		checkSlow(VertexIndex < GetNumVertices());
		return ((FPositionVertex*)(Data + VertexIndex * Stride))->Position;
	}
	// Other accessors.
	FORCEINLINE uint32 GetStride() const
	{
		return Stride;
	}
	FORCEINLINE uint32 GetNumVertices() const
	{
		return NumVertices;
	}

	// FRenderResource interface.
	ENGINE_API virtual void InitRHI() override;
	ENGINE_API virtual void ReleaseRHI() override;
	virtual FString GetFriendlyName() const override { return TEXT("PositionOnly Static-mesh vertices"); }

	ENGINE_API void BindPositionVertexBuffer(const class FVertexFactory* VertexFactory, struct FStaticMeshDataType& Data) const;

	void* GetVertexData()
	{
		return Data;
	}

private:

	FShaderResourceViewRHIRef PositionComponentSRV;

	/** The vertex data storage type */
	class FPositionVertexData* VertexData;

	/** The cached vertex data pointer. */
	uint8* Data;

	/** The cached vertex stride. */
	uint32 Stride;

	/** The cached number of vertices. */
	uint32 NumVertices;

	bool NeedsCPUAccess = true;

	/** Allocates the vertex data storage type. */
	void AllocateData(bool bNeedsCPUAccess = true);
};