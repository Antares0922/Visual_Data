import struct
import numpy as np
import matplotlib.pyplot as plt

ruta_bin = "../pipe.bin"

#abriendo el archiv0
with open(ruta_bin,"rb") as file:
    #longitud del array
    encabezado_by = file.read(8)
    encabezado = struct.unpack("<q",encabezado_by)[0]

    #array
    array = np.fromfile(ruta_bin, dtype=np.int64, offset=8) #no incluye el encabeza

print(array)


#creando el lienzo

#eje x
x = np.arange(encabezado);

plt.figure(figsize=(10,2)) #ancho y largo
plt.scatter(x,array,s=1,color="darkblue") #Tipo de grafico "puntos"
plt.title("Visualizacion de datos")
plt.xlabel("Indice")
plt.ylabel("Valor")
plt.grid(True)
plt.tight_layout()

plt.show()
