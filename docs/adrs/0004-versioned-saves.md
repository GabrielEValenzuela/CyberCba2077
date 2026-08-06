# ADR 0004: Persistencia versionada

El guardado clave/valor versionado evita una dependencia JSON adicional. Se valida al cargar y se escribe primero a temporal; cambios incompatibles deben aumentar la version y migrar explicitamente.
