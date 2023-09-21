// Copyright Epic Games, Inc. All Rights Reserved.

#include "Task/TaskGraphSchema.h"

UTaskGraphSchema::UTaskGraphSchema(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {}

UK2Node_VariableGet* UTaskGraphSchema::SpawnVariableGetNode(const FVector2D GraphPosition, class UEdGraph* ParentGraph, FName VariableName, UStruct* Source) const
{
	return Super::SpawnVariableGetNode(GraphPosition, ParentGraph, VariableName, Source);
}

UK2Node_VariableSet* UTaskGraphSchema::SpawnVariableSetNode(const FVector2D GraphPosition, class UEdGraph* ParentGraph, FName VariableName, UStruct* Source) const
{
	return Super::SpawnVariableSetNode(GraphPosition, ParentGraph, VariableName, Source);
}
