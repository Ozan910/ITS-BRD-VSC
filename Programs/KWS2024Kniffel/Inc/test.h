/**
 ******************************************************************************
 * @file    test.h
 * @author  Franz Korf
 *
 * @date    20. Jan. 2025
 *
 * @brief  Interface des test Moduls.
 *
 ******************************************************************************
 */

#ifndef _TEST_H
#define _TEST_H

// Diese define Anweisungen aktivieren die Tests der 
// entsprechenden Aufgaben.

// #define AUFGABE3
// #define AUFGABE4
// #define AUFGABE5

#ifdef AUFGABE3
/**
 * @brief  Test des Moduls io. 
 */
void testIO(void);
#endif

#ifdef AUFGABE4
/**
 * @brief  Test des Moduls display. 
 */
void testDisplay(void);
#endif

#ifdef AUFGABE5 
/**
 * @brief  Test des Moduls kniffel. 
 */
void testKniffel(void);
#endif

#endif
// EOF
