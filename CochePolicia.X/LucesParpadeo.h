// Filename: LucesParpadeo.h

#ifndef LUCES_PARPADEO_H
#define LUCES_PARPADEO_H

#include <xc.h>
#include <stdlib.h>

// Function prototypes
void InicializarTimer5(void);
void InterrupcionTimer5(void);
void PararTimer5(void);
void invertir_luces(void);
void incializar_luces_parpadeo(void);
void encender_todas_luces(void);
void encender_luces_rojas(void);
void encender_luces_azules(void);

static void luces_policia(void);
static void luces_rojas_parpadeo(void);
static void luces_azules_parpadeo(void);
static void aparcar_todas_luces(void);

#endif // LUCES_PARPADEO_H
