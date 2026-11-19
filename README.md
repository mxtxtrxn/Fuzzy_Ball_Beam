# Fuzzy_Ball_Beam

Control difuso para un mecanismo barra-esfera, escrito en 2023 por Camilo Covarrubias Hernández (@mxtxtrxn). Licencia MIT.

**In short:** a fuzzy-logic ball-and-beam controller in Arduino C++ with a Python plotting script. It is the seed of a desktop simulation and telemetry bench for a hydrogen fuel-cell RC car controlled by an ESP32.

## Qué contiene este repositorio

- `Ball_beam.ino` — firmware original: sensor ultrasónico, servo, 25 reglas difusas y logger serial.
- `graficos.py` — gráfica de las funciones de pertenencia con `scikit-fuzzy` y `matplotlib`.
- `tests/test_fuzzy_tables.py` — pruebas sin hardware que validan las tablas difusas y la inferencia de referencia.
- `data/sample_serial_log.csv` — muestra sintética del formato de logger serial del firmware.
- `tools/render_memberships.py` — versión no interactiva de la gráfica: guarda PNG en `artifacts/`.
- `ROADMAP.md` — hitos, entregables y criterios de aceptación del trabajo siguiente.
- `requirements.txt` — dependencias de las herramientas de análisis.

## Relación con el proyecto del carro H₂

La bola-viga y el carro comparten el mismo lazo de control: un actuador, un sensor, un controlador difuso y una planta que estabilizar. En la bola-viga la planta es mecánica; en el carro es el tren motriz, el sensor es IMU/encoder/corriente y el actuador lo gobierna un ESP32. Lo que cambia es el entorno: latencia de radio, pérdida de paquetes y una restricción de energía por celda de combustible.

El trabajo siguiente extrae el controlador a un núcleo C++17 portable con una capa de abstracción de hardware, y construye alrededor de él un enlace de radio simulado, un modelo energético, un almacén local de series temporales y un panel web con replay determinista. Todo se ejecuta en el escritorio con datos sintéticos. El hardware real queda fuera del alcance del workspace.

## Cómo verificar

```sh
pip install -r requirements.txt
python -m unittest discover -s tests -v
python tools/render_memberships.py
```

La gráfica se guarda en `artifacts/membership_functions.png`, que puede abrirse en el navegador del escritorio.

## Datos y límites

Todos los datos incluidos son sintéticos. No hay credenciales, datos personales, código de terceros ni información de hardware real. El escritorio no tiene internet, GPU, sonido ni herramientas GUI nativas; las dependencias se instalan desde el mirror de pip disponible.

## Licencia

MIT. Consulta el archivo `LICENSE`.
