#!/bin/bash
cd src/trajectory/
python3 src.py
cd ..
cd thermodynamic_parameters/
python3 src.py
cd ..
cd power_parameters/
python3 src.py
cd ..
cd atmosphere_parameters/
python3 src.py

