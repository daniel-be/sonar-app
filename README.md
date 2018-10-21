# Android and Arduino Sonar
Ziel des Projekts ist es ein Sonar zu bauen und zu programmieren,
das in einem Winkel von 180° Gegenstände erkennt. Das Projekt soll
aus dem Sonar und einer Android App, auf der die Anzeige erfolgt,
bestehen.

## Material:
- Raspberry PI / Arduino
- Ultraschallsensor HC-SR04
- Servomotor

## Aufbau
Der Ultraschallsensor wird am Servomotor befestigt, welcher sich
periodisch von links nach rechts dreht (180°). Die Entfernungsdaten
sollen vom Raspberry PI / Arduino ausgelesen werden und drahtlos an
die Android App übertragen werden. Die Android App soll dann aus
Winkel und Entferung das entsprechende Sonar darstellen. 


