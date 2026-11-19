# Roadmap

Objetivo: convertir este controlador en el núcleo de un banco de simulación y telemetría para un carro remoto con celdas de H₂ y ESP32. Cada hito termina con un artefacto visible y un resultado de prueba explícito.

## Hito 1 — Núcleo de control portable

Extraer la lógica difusa de `Ball_beam.ino` a una librería C++17 con integrador de paso fijo y tests dorados.

- Entregable: librería C++17 + salida de tests en terminal + gráfica de respuesta escalonada.
- Criterio: 10 000 pasos repetidos producen el mismo checksum; la respuesta escalonada queda dentro del 2 % respecto a un PID clásico de referencia.

## Hito 2 — Capa de abstracción y enlace ESP32 simulado

Añadir una HAL delgada y un enlace de radio simulado con números de secuencia, jitter, reordenamiento y pérdida.

- Entregable: reproductor 2D en Chrome con comandos, ACK y reconstrucción de estado.
- Criterio: 64 ejecuciones a 150 ms y 5 % de pérdida convergen sin divergencia; los comandos aceptados cuadran con el log.

## Hito 3 — Modelo de energía H₂

Modelar la curva de polarización de la celda, el motor de tracción y la batería intermedia con datos sintéticos.

- Entregable: panel local de SOC, corriente y presión.
- Criterio: 30 minutos simulados conservan energía a 1e-6; el SOC queda dentro del 2 % de la curva de referencia.

## Hito 4 — Telemetría y replay

Almacenar lecturas en `sqlite3` y mostrar un dashboard con slider temporal y exportación WebM mediante `ffmpeg`.

- Entregable: dashboard en Chrome con replay y WebM exportado.
- Criterio: 1 000 000 de lecturas ingeridas; 500 consultas coinciden con pandas; `kill -9` no pierde ninguna lectura confirmada.

## Hito 5 — Fallos y seguridad

Inyectar brownout, pérdida de sensor, actuador atascado y presión baja, con un estado seguro definido.

- Entregable: matriz de inyección de fallos en Chrome e informe PDF.
- Criterio: 200 guiones de fallo terminan siempre en estado seguro; cero violaciones de invariante.

## Sesión final

Integración de extremo a extremo: ejecuciones guiadas, exportación WebM y reporte corto con los resultados de todos los criterios.
