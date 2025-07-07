#!/bin/bash

# Ejecutar script de compilación y ejecución C
cd src/c_scripts
./compilar.sh

# Activar entorno virtual
cd ../../
source venv/bin/activate

# Ejecutar script Python
cd src/py_scripts/
python3 ./main.py

# Desactivar entorno virtual
deactivate

