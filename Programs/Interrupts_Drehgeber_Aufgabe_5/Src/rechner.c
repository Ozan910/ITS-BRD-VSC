#include "rechner.h"

// --- KONSTANTEN ---
// Ein Drehgeber-Umdrehung hat 300 Schlitze * 4 Phasen = 1200 Schritte [cite: 149]
#define STEPS_PER_REVOLUTION  1200.0 
#define DEGREES_PER_CIRCLE    360.0

// Der Timer der ITS_BRD_LIB inkrementiert 90 Mal pro Mikrosekunde 
#define TICKS_PER_MICROSECOND 90.0
#define MICROSECONDS_PER_SEC  1000000.0


double calcAngle(long position) {
    // 1. Schritte auf den Bereich einer Umdrehung bringen (-1199 bis +1199)
    long stepsInCurrentTurn = position % (long)STEPS_PER_REVOLUTION;

    // 2. Negative Werte korrigieren
    // Beispiel: -300 Schritte (=-90°) sollen als 270° (Position) angezeigt werden.
    if (stepsInCurrentTurn < 0) {
        stepsInCurrentTurn += (long)STEPS_PER_REVOLUTION;
    }

    // 3. In Grad umrechnen: (Schritte / 1200) * 360
    double angle = ((double)stepsInCurrentTurn / STEPS_PER_REVOLUTION) * DEGREES_PER_CIRCLE;
    
    return angle;
}

double calcAngularSpeed(long stepDelta, uint32_t timeDeltaTicks) {
    // Schutz vor Division durch Null
    if (timeDeltaTicks == 0) {
        return 0.0;
    }

    if(stepDelta < 0){
        stepDelta *= (-1);
    }

    // 1. Wegdifferenz in Grad berechnen
    // Hier behalten wir das Vorzeichen bei! (-10 Schritte = Rückwärtsbewegung)
    double degreeDelta = ((double)stepDelta / STEPS_PER_REVOLUTION) * DEGREES_PER_CIRCLE;

    // 2. Zeitdifferenz in Sekunden berechnen
    // Formel: Ticks / (90 Ticks/µs * 1.000.000 µs/s)
    double timeSeconds = (double)timeDeltaTicks / (TICKS_PER_MICROSECOND * MICROSECONDS_PER_SEC);

    // 3. Geschwindigkeit v = s / t
    return degreeDelta / timeSeconds;
}