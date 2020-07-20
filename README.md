# Simulador-Covid19

This project is about a covid simulator in 4 forms. 

* Secuencial Version.

* Parallel Version with distribuid memory using MPI.

* Parallel Version with shared memory using OpenMP.

* Hybrid Version With MPI and OpenMp.


## The algorithms are developed acording the following problem. (Problem is in Spanish)

  Simulación de la difusión del COVID-19 en una población.
  Las celdas de una cuadrícula en un momento dado se encuentran coloreadas en uno de
  siete colores posibles (Blanco, Azul, Naranja, Rojo, Verde, Amarillo y Negro). Cada color
  distinto al blanco (celda vacía) refleja el estado de una persona residente en la celda.

  Cada celda cuenta con la siguiente información:
  • Estado: representado por los valores {Blanco, Azul, Naranja, Rojo, Verde, Amarillo,
  Negro}. Cada color indica:
    o Blanco: Celda libre,
    o Azul: Persona susceptible a la enfermedad (persona que puede enfermar),
    o Naranja: Persona enferma, sin contagio,
    o Rojo: Enfermo con contagio,
    o Amarillo: Persona enferma, pero aislada por atención médica,
    o Verde/Negro: Curado/Muerto.
  • Edad de la persona: {Niño, Adulto o Anciano}.
  • Enfermedad de Riesgo (diabetes, hipertensión, enfermedad respiratoria, Inmunodeprimido, etc.): {SI, NO}.
  • Profesión de Riesgo (enfermero, medico, odontólogo, cajero, chofer de servicio
  público, etc.): {SI, NO}.
  • Vacunas Preventiva (vacunado para alguna de las enfermedades estacionales: gripe,
  neumonía y meningitis): {SI, NO}.
  • Sexo Biológico: {Mujer, Hombre}.
  
  La existencia de un reloj virtual de cómputo conduce la evolución a lo largo del tiempo del
  modelo planteado. Esto significa que, a cada señal de reloj, cada una de las celdas de la
  cuadricula puede cambiar su estado de salud a uno nuevo, el cual depende de su edad,
  grupo de riesgo, sexo, acciones preventivas y del estado de salud de sus sus ocho vecinos
  adyacentes (vecindario de Moore).
  
  Una celda central cambiará su estado (cambiará o quedará en el mismo estado) de acuerdo
  a un conjunto de reglas que se detallan a continuación:
  
  - 𝑺𝒖𝒔𝒄𝒆𝒑𝒕𝒊𝒃𝒍𝒆 → 𝐄𝐧𝐟𝐞𝐫𝐦𝐨 𝐬𝐢𝐧 𝐜𝐨𝐧𝐭𝐚𝐠𝐢𝐨 : Si en el tiempo t, la celda central es
  susceptible y, alguna de sus celdas adyacentes está infectadas en etapa de contagio,
  o bien incubando la enfermedad en etapa de contagio, la celda central pasará a
  incubar la enfermedad con determinada probabilidad en el tiempo t + 1, pero no la
  propagará (incubación sin contagio). La probabilidad de cambio de estado es
  proporcional al número de celdas adyacentes enfermas, la potencia de la enfermedad
  y la susceptibilidad de la persona a la enfermedad. La fórmula de actualización es la
  siguiente:
  
  ```
  Prob. contagio= ((porc. vecinos infectados* 2.4) + susceptibilidad (edad, grupo de riesgo))
  _________________________________________________________________________________________
                            (7 * PASOS POR DIA)
  ```
  En esta fórmula, la constante 2.4 representa la potencia de la enfermedad, y la
  susceptibilidad es una función que determina la predisposición de una persona a
  contraer la enfermedad. Valores de susceptibilidad según edad: Niños (0.30), Adultos
  (0.50), Ancianos (0.90). En caso de pertenecer a un grupo de riesgo, aumenta la
  susceptibilidad en 0.15.
  
  PASOS POR DIA representa la cantidad de veces que se actualiza el estado de las
  celdas por cada día de simulación. En nuestro caso tomamos el valor 24, realizando
  una actualización por cada hora.
  
  - 𝑬𝒏𝒇𝒆𝒓𝒎𝒐 𝒔𝒊𝒏 𝒄𝒐𝒏𝒕𝒂𝒈𝒊𝒐 → 𝑬𝒏𝒇𝒆𝒓𝒎𝒐 𝒄𝒐𝒏 𝒄𝒐𝒏𝒕𝒂𝒈𝒊𝒐 : Si la celda está incubando
  COVID-19 y, en el tiempo t han transcurrido 4 días desde el contagio, en el tiempo t +
  1 la celda estará en el estado de incubación con contagio.
  
  - 𝑬𝒏𝒇𝒆𝒓𝒎𝒐 𝒄𝒐𝒏 𝒄𝒐𝒏𝒕𝒂𝒈𝒊𝒐 → 𝑷𝒂𝒄𝒊𝒆𝒏𝒕𝒆 𝒂𝒊𝒔𝒍𝒂𝒅𝒐: Luego de 2 días de estar en estado
  de infección con síntomas, se considera que, con una probabilidad del 90%, la persona
  será aislada para atención médica, dejando de propagar la enfermedad en el tiempo
  t +1.
  
  - 𝑬𝒏𝒇𝒆𝒓𝒎𝒐 → 𝐑𝐞𝐜𝐮𝐩𝐞𝐭𝐚𝐝𝐨/𝐌𝐮𝐞𝐫𝐭𝐨: Si en el tiempo t han transcurrido 14 días desde
  el contagio, una persona infectada podría recuperarse o morir en el tiempo t +1. La
  selección entre los dos estados depende de una función de probabilidad basada en la
  mortalidad de la enfermedad y las complicaciones según sus características. La
  mortalidad para niños es del 1%, para adultos 1.3% y para ancianos 14.8%. Personas
  que cuentan con alguna vacuna preventiva reducen la mortalidad en un 0,5%.
  
## Ejemplo

  En este ejemplo la celda X tiene las siguientes características:
    - Celda poblada por una persona Susceptible
    - Edad: 42 años (Adulto).
    - Grupo de Riesgo: Si (Diabetes)
    - Vacunas Preventivas: Si
    - Sexo: Hombre
    
    La celda X cuenta con una vecindad de 8 celdas, donde:
    
    - 2/8 = 0.25 no tiene vecinos.
    - 1/8 = 0.125 de los vecinos está infectado, pero aún no contagia.
    - 3/8 = 0.375 de los vecinos están sanos.
    - 2/8 = 0.25 de los vecinos están enfermos y contagian.
    
    Aplicando la regla 𝑺𝒖𝒔𝒄𝒆𝒑𝒕𝒊𝒃𝒍𝒆 → 𝐄𝐧𝐟𝐞𝐫𝐦𝐨 𝐬𝐢𝐧 𝐜𝐨𝐧𝐭𝐚𝐠𝐢𝐨:
    Porcentaje de vecinos enfermos y con contagio = 2/8 = 0.25
    Susceptibilidad (42 años, grupo de riesgo) = 0.50 + 0.15 = 0.65
    Probabilidad de contagio = ((0.25 *2.4) +0.65) / (7*24) = 1.25 /168 = 0,0074
    
    Suponiendo que generamos el numero aleatorio 0.005, la persona que ocupa la celda X, en
    el tiempo t+1 se encontrara en estado de incubación de la enfermedad

