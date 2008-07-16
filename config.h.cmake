//
// C++ Interface: config.h
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG                               // debug

#cmakedefine SV_DICEVIEWER                      // versione senza filtri e con interfaccia fissa 
#cmakedefine SV_SIMPLEINTERFACE                 // interfaccia fissa
#cmakedefine SV_DANZ                            // label "Danz"


#cmakedefine SV_SLICECONTROLS_2COL              // controlli slice su 2 colonne
#cmakedefine SV_ORTHOCONTROLS_3COL              // controlli proiezioni ortogonali su 3 colonne
#cmakedefine SV_3_SURFACES                      // apre fino a 3 superfici

#cmakedefine SV_FASTLOAD                        // niente sleep all'apertura
#cmakedefine SV_NEUROLOGICALCONVENTION          // convenzione neurologica invece che convenzione radiologica


/*
  DiceViewer:
    Attiva automaticamente
        SV_SIMPLEINTERFACE
        SV_FASTLOAD
    Disattiva automaticamente
        DEBUG
        SV_DANZ
        SV_3_SURFACES
*/
#ifdef SV_DICEVIEWER

  #ifndef SV_SIMPLEINTERFACE
    #define SV_SIMPLEINTERFACE
  #endif // SV_SIMPLEINTERFACE

  #ifdef DEBUG
    #undef DEBUG
  #endif // DEBUG

  #ifdef SV_DANZ
    #undef SV_DANZ
  #endif // SV_DANZ

  #ifdef SV_FASTLOAD
    #undef SV_FASTLOAD
  #endif // SV_FASTLOAD

  #ifdef SV_3_SURFACES
    #undef SV_3_SURFACES
  #endif // SV_FASTLOAD

#endif // SV_DICEVIEWER




/*
  Interfaccia semplificata:
    Attiva automaticamente
        SV_SLICECONTROLS_2COL
        SV_ORTHOCONTROLS_3COL
*/
#ifdef SV_SIMPLEINTERFACE
  #ifndef SV_SLICECONTROLS_2COL
    #define SV_SLICECONTROLS_2COL
  #endif // SV_SLICECONTROLS_2COL
  #ifndef SV_ORTHOCONTROLS_3COL
    #define SV_ORTHOCONTROLS_3COL
  #endif // SV_ORTHOCONTROLS_3COL
#endif // SV_SIMPLEINTERFACE_


/*
#define VTK_12BIT_MIN VTK_TYPE_CAST(unsigned short, 0x0800)
#define VTK_12BIT_MAX VTK_TYPE_CAST(unsigned short, 0x07ff)
#define VTK_UNSIGNED_12BIT_MIN VTK_TYPE_CAST(unsigned short, 0u)
#define VTK_UNSIGNED_12BIT_MAX VTK_TYPE_CAST(unsigned short, 0x0fff)
*/

#endif // CONFIG_H
