// PainShooterPropertyUtils.h
#pragma once

#include "CoreMinimal.h"

/**
 * Utilidades para leer/escribir propiedades Blueprint desde C++
 */
namespace PainShooterPropertyUtils
{
	/**
	 * Intenta leer una propiedad int32 de un Blueprint
	 */
	bool TryGetIntProperty(UObject* Object, FName PropertyName, int32& OutValue);

	/**
	 * Intenta escribir una propiedad int32 en un Blueprint
	 */
	bool TrySetIntProperty(UObject* Object, FName PropertyName, int32 Value);
}