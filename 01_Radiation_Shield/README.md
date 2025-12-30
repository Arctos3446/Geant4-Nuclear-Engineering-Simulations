# Radiation Shielding Project: Cs-137 Attenuation
## 1. Project Overview
This repository contains a Monte Carlo simulation developed using the Geant4 toolkit. The primary objetive is to model the interaction of gamma radiation with matter and evaluate the shielding efficiency of high-density materials, specifically Lead (Pb), against a Cesium-137 radioactive source.
This project serves as a technical study in radiation protection, focusing on the calculation of linear attenuation coefficients, the determination of Half-Value Layers (HVL), and the analysis of leakage radiation energy spectra.
## 2. Theoretical Framework
The simulation models the attenuation of a monoenergetic photon beam as it traverses a shielding material. The underlying physical principle follows the Beer-Lambert Law:

$$I(x) = I_0 e^{-\mu x}$$

Where:
* $I(x)$: Intensity of transmitted radiation at thickness $x$.
* $I_0$: Initial intensity of the incident beam.
* $\mu$: Linear attenuation coefficient ($cm^{-1}$), dependent on the material density and photon energy.
* $x$: Thickness of the shielding material ($cm$).

The simulation incorporates standard electromagnetic physics processes appropriate for the energy range of Cs-137 (662 keV), including the Photoelectric Effect, Compton Scattering, and Rayleigh Scattering.
