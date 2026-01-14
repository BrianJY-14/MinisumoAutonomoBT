#include <DriverAutonomus.h>

namespace DriverAutonomus {
    
    // Variables globales
    float K_E = 0.35, K_PE = 0.4, K_PO = 0;
    float previous_error = 0, previous_output = 0;
    float setpoint = 0;
    bool init_backward = false;
    bool combat_started = false;  // Nueva variable para saber si ya empezó el combate
    bool turbo_mode = false;                     // Define Y asigna valor
    unsigned long close_contact_start = 0;      // Define Y asigna valor
    unsigned long TURBO_THRESHOLD = 2500;       // Define Y asigna valor  
    float CLOSE_DISTANCE_THRESHOLD = 8.0; 
    unsigned long target_lost_time = 0;
    bool target_recently_lost = false;

    const float ACCEL_LUT[451] = {
        0.870, 0.870, 0.870, 0.870, 0.870, 0.870, 0.870, 0.870, 
        0.870, 0.870, 0.870, 0.870, 0.870, 0.870, 0.870, 0.870, 
        0.870, 0.870, 0.870, 0.870, 0.870, 0.870, 0.870, 0.870, 
        0.870, 0.870, 0.870, 0.870, 0.870, 0.870, 0.869, 0.869, 
        0.869, 0.869, 0.869, 0.869, 0.869, 0.869, 0.869, 0.869, 
        0.869, 0.869, 0.869, 0.869, 0.869, 0.869, 0.869, 0.869, 
        0.869, 0.869, 0.869, 0.868, 0.868, 0.868, 0.868, 0.868, 
        0.868, 0.868, 0.868, 0.868, 0.868, 0.868, 0.868, 0.868, 
        0.868, 0.867, 0.867, 0.867, 0.867, 0.867, 0.867, 0.867, 
        0.867, 0.867, 0.867, 0.867, 0.867, 0.866, 0.866, 0.866, 
        0.866, 0.866, 0.866, 0.866, 0.866, 0.866, 0.866, 0.866, 
        0.865, 0.865, 0.865, 0.865, 0.865, 0.865, 0.865, 0.865, 
        0.865, 0.864, 0.864, 0.864, 0.864, 0.864, 0.864, 0.864, 
        0.864, 0.863, 0.863, 0.863, 0.863, 0.863, 0.863, 0.863, 
        0.863, 0.862, 0.862, 0.862, 0.862, 0.862, 0.862, 0.862, 
        0.861, 0.861, 0.861, 0.861, 0.861, 0.861, 0.861, 0.860, 
        0.860, 0.860, 0.860, 0.860, 0.860, 0.860, 0.859, 0.859, 
        0.859, 0.859, 0.859, 0.859, 0.858, 0.858, 0.858, 0.858, 
        0.858, 0.858, 0.857, 0.857, 0.857, 0.857, 0.857, 0.856, 
        0.856, 0.856, 0.856, 0.856, 0.856, 0.855, 0.855, 0.855, 
        0.855, 0.855, 0.854, 0.854, 0.854, 0.854, 0.854, 0.853, 
        0.853, 0.853, 0.853, 0.853, 0.852, 0.852, 0.852, 0.852, 
        0.852, 0.851, 0.851, 0.851, 0.851, 0.851, 0.850, 0.850, 
        0.850, 0.850, 0.849, 0.849, 0.849, 0.849, 0.849, 0.848, 
        0.848, 0.848, 0.848, 0.847, 0.847, 0.847, 0.847, 0.847, 
        0.846, 0.846, 0.846, 0.846, 0.845, 0.845, 0.845, 0.845, 
        0.844, 0.844, 0.844, 0.844, 0.843, 0.843, 0.843, 0.843, 
        0.842, 0.842, 0.842, 0.842, 0.841, 0.841, 0.841, 0.841, 
        0.840, 0.840, 0.840, 0.839, 0.839, 0.839, 0.839, 0.838, 
        0.838, 0.838, 0.838, 0.837, 0.837, 0.837, 0.836, 0.836, 
        0.836, 0.836, 0.835, 0.835, 0.835, 0.834, 0.834, 0.834, 
        0.834, 0.833, 0.833, 0.833, 0.832, 0.832, 0.832, 0.831, 
        0.831, 0.831, 0.831, 0.830, 0.830, 0.830, 0.829, 0.829, 
        0.829, 0.828, 0.828, 0.828, 0.827, 0.827, 0.827, 0.826, 
        0.826, 0.826, 0.826, 0.825, 0.825, 0.825, 0.824, 0.824, 
        0.824, 0.823, 0.823, 0.823, 0.822, 0.822, 0.822, 0.821, 
        0.821, 0.821, 0.820, 0.820, 0.819, 0.819, 0.819, 0.818, 
        0.818, 0.818, 0.817, 0.817, 0.817, 0.816, 0.816, 0.816, 
        0.815, 0.815, 0.815, 0.814, 0.814, 0.813, 0.813, 0.813, 
        0.812, 0.812, 0.812, 0.811, 0.811, 0.810, 0.810, 0.810, 
        0.809, 0.809, 0.809, 0.808, 0.808, 0.807, 0.807, 0.807, 
        0.806, 0.806, 0.805, 0.805, 0.805, 0.804, 0.804, 0.803, 
        0.803, 0.803, 0.802, 0.802, 0.801, 0.801, 0.801, 0.800, 
        0.800, 0.799, 0.799, 0.799, 0.798, 0.798, 0.797, 0.797, 
        0.797, 0.796, 0.796, 0.795, 0.795, 0.794, 0.794, 0.794, 
        0.793, 0.793, 0.792, 0.792, 0.791, 0.791, 0.791, 0.790, 
        0.790, 0.789, 0.789, 0.788, 0.788, 0.788, 0.787, 0.787, 
        0.786, 0.786, 0.785, 0.785, 0.784, 0.784, 0.784, 0.783, 
        0.783, 0.782, 0.782, 0.781, 0.781, 0.780, 0.780, 0.779, 
        0.779, 0.778, 0.778, 0.778, 0.777, 0.777, 0.776, 0.776, 
        0.775, 0.775, 0.774, 0.774, 0.773, 0.773, 0.772, 0.772, 
        0.771, 0.771, 0.770, 0.770, 0.769, 0.769, 0.768, 0.768, 
        0.767, 0.767, 0.766, 0.766, 0.765, 0.765, 0.764, 0.764, 
        0.763, 0.763, 0.762, 0.762, 0.761, 0.761, 0.760, 0.760, 
        0.759, 0.759, 0.758, 0.758, 0.757, 0.757, 0.756, 0.756, 
        0.755, 0.755, 0.754, 0.754, 0.753, 0.753, 0.752, 0.752, 
        0.751, 0.751, 0.750
    };

    const float DECEL_LUT[150] = {
        1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 
        1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 
        1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 
        1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 
        1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 0.999, 0.999, 
        0.999, 0.999, 0.999, 0.999, 0.999, 0.999, 0.999, 0.999, 
        0.999, 0.999, 0.998, 0.998, 0.998, 0.998, 0.998, 0.998, 
        0.997, 0.997, 0.997, 0.997, 0.997, 0.996, 0.996, 0.996, 
        0.996, 0.995, 0.995, 0.995, 0.995, 0.994, 0.994, 0.993, 
        0.993, 0.993, 0.992, 0.992, 0.991, 0.991, 0.990, 0.990, 
        0.989, 0.989, 0.988, 0.988, 0.987, 0.987, 0.986, 0.985, 
        0.985, 0.984, 0.983, 0.982, 0.982, 0.981, 0.980, 0.979, 
        0.978, 0.977, 0.976, 0.975, 0.974, 0.973, 0.972, 0.971, 
        0.970, 0.969, 0.968, 0.966, 0.965, 0.964, 0.962, 0.961, 
        0.960, 0.958, 0.957, 0.955, 0.954, 0.952, 0.950, 0.949, 
        0.947, 0.945, 0.943, 0.941, 0.939, 0.937, 0.935, 0.933, 
        0.931, 0.929, 0.927, 0.924, 0.922, 0.920, 0.917, 0.915, 
        0.912, 0.910, 0.907, 0.904, 0.901, 0.899, 0.896, 0.893, 
        0.890, 0.886, 0.883, 0.880, 0.877, 0.873
    };

    // === Helper interno
    static inline float clamp01(float v) {
        return v < 0.f ? 0.f : (v > 1.f ? 1.f : v);
    }

    // ===== Implementaciones =====

    float getSpeedFromLUT(float distance) {
        if (distance >= 15.0f) {
            int index = (int)((distance - 15.0f) * 10);
            if (index < 0) index = 0;
            if (index >= 451) index = 450;
            return ACCEL_LUT[index];
        } else {
            int index = (int)(distance * 10);
            if (index < 0) index = 0;
            if (index >= 150) index = 149;
            return DECEL_LUT[index];
        }
    }


    void start()
    {
        // Ya no necesitamos configurar pines para los modos
        // La configuración se hace directamente desde BTAutonomo
        
        // Valores por defecto (se sobrescriben cuando se selecciona modo)
        previous_error = 0;
        previous_output = 0;
        combat_started = false;
        init_backward = false;
        target_recently_lost = false;

    }

       float calculateDistanceToOpponent()
    {
        // Calcular distancia promedio ponderada basada en la estructura de Heisenberg
        float total_weight = 0;
        float weighted_distance = 0;
        
        // Usar el mismo array que usa readObjetive() para consistencia
        for (int i = 0; i < DriverSonic::PIN::ECHO_N; i++) {
            if(DriverSonic::distances[i] >= 0 && DriverSonic::distances[i] < CONFIG::RANGE) {
                float weight = 1.0;
                // Dar más peso al sensor central (índice 1)
                if (i == 1) weight = 2.0;  // Sensor central más importante
                
                weighted_distance += DriverSonic::distances[i] * weight;
                total_weight += weight;
            }
        }
        
        if (total_weight > 0) {
            return weighted_distance / total_weight;
        }
        
        // Si no detecta nada, retornar distancia máxima
        return CONFIG::RANGE;
    }

    float readObjetive() {
        DriverSonic::readSonic();

        float objective_sentinel = 7.0f;
        bool isReaded = false;
        float array_cercanias[DriverSonic::PIN::ECHO_N] = {0};

        for (int i = 0; i < DriverSonic::PIN::ECHO_N; i++) {
            float d = DriverSonic::distances[i];
            if (d >= 0 && d < CONFIG::RANGE) {
                array_cercanias[i] = CONFIG::RANGE - d;
                if (array_cercanias[i] > 0) isReaded = true;
            }
        }

        if (isReaded) {
            float x = (array_cercanias[0] * -0.7f + array_cercanias[2] * 0.7f);
            float y = (array_cercanias[0] * 0.3f + array_cercanias[2] * 0.3f) + array_cercanias[1];
            float h = sqrt(x * x + y * y);
            if (h < 1e-6f) return objective_sentinel;
            return x / h; // en rango [-1,1]
        }
        return objective_sentinel;
    }

    void checkTurboMode(float distance) {
        // Verificar si estamos en contacto cercano
        if (distance <= CLOSE_DISTANCE_THRESHOLD && combat_started) {
            // Si es la primera vez que detectamos contacto cercano
            if (close_contact_start == 0) {
                close_contact_start = millis();
            }
            // Si hemos estado en contacto cercano por más de 5 segundos
            else if (millis() - close_contact_start >= TURBO_THRESHOLD) {
                turbo_mode = true;
            }
        }
        else {
            // Si perdemos el contacto cercano, resetear
            close_contact_start = 0;
            // El modo turbo se mantiene hasta que se resetee manualmente
            // o se desactive el robot
        }
    }

    void executeOutput(float output) {
        float distance = calculateDistanceToOpponent();
        checkTurboMode(distance);

        float base_speed = getSpeedFromLUT(distance);

        float leftOutput = base_speed;
        float rightOutput = base_speed;

        if (output > 0)
            rightOutput = base_speed - (output * base_speed);
        else
            leftOutput = base_speed + (output * base_speed);

        leftOutput = clamp01(leftOutput);
        rightOutput = clamp01(rightOutput);

        if (turbo_mode)
            DriverMotor::writeMotorTurbo(leftOutput, rightOutput);
        else
            DriverMotor::writeMotor(leftOutput, rightOutput);
    }

    void executeSearch()
    {
        // Ejecutar búsqueda sobre su propio eje (giro por defecto hacia la derecha)
        DriverMotor::writeMotor(0.80,-0.80);
    }

    void controlLoop()
    {
        float objective = readObjetive();
        bool has_target = fabs(objective - 7.0f) > 1e-3f;

        if (has_target) {
            combat_started = true;
            init_backward = false;
            target_recently_lost = false;
        }
        
        // Lógica de control según el estado
        if(init_backward) {
            // Modo de batalla inicial hacia atrás (solo modo 3)
            executeSearch();
        } else if (has_target) {
            float error = objective - setpoint;
            float output = error * K_E + previous_error * K_PE + previous_output * K_PO;
            if (output > 1.0f) output = 1.0f;
            else if (output < -1.0f) output = -1.0f;
            
            // Ejecutar motores
            executeOutput(output);
            
            // Actualizar los previos 
            previous_error = error;
            previous_output = output;
        }
        else {
            // NO TIENE OBJETIVO: Decidir qué hacer según si ya empezó el combate
            if(combat_started) {
                // YA HABÍA EMPEZADO EL COMBATE: Controlar tiempo de seguimiento
                if(!target_recently_lost) {
                    // Acaba de perder el objetivo
                    target_lost_time = millis();
                    target_recently_lost = true;
                }
                
                // Verificar si ha pasado el tiempo máximo de seguimiento
                if(millis() - target_lost_time >= CONFIG::MAX_FOLLOW_TIME) {
                    // Tiempo agotado: ir a búsqueda activa
                    executeSearch();
                    // Resetear variables PID y seguimiento
                    previous_error = 0;
                    previous_output = 0;
                    target_recently_lost = false;
                } else {
                    // Continuar siguiendo en la última dirección conocida (con intensidad reducida)
                    float reduced_output = previous_output * 0.6;  // 60% de la intensidad original
                    if(reduced_output > 1.0) reduced_output = 1.0;
                    else if(reduced_output < -1.0) reduced_output = -1.0;
                    
                    executeOutput(reduced_output);
                }
            }
            else {
                // AÚN NO EMPEZÓ EL COMBATE: Usar sesgo inicial (comportamiento original)
                float error = previous_error;  // Mantiene el sesgo inicial
                float output = error * K_E + previous_error * K_PE + previous_output * K_PO;
                if(output > 1.0) output = 1.0;
                else if(output < -1.0) output = -1.0;
                
                // Ejecutar motores
                executeOutput(output);
                
                // Actualizar solo output, mantener error
                previous_output = output;
            }
        }
        
        delay(5);
    }
}