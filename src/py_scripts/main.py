import struct
#import numpy as np
#import matplotlib.pyplot as plt

ruta_bin = "../pipe.bin"

#abriendo el archiv0
with open(ruta_bin,"rb") as file:
    #longitud del array
    encabezado_by = file.read(8)
    encabezado = struct.unpack("<q",encabezado_by)[0]

    #array
    array_by = file.read(encabezado * 8)
    array = struct.unpack(f"<{encabezado}q",array_by)

    #encabezado struct
    encabezado_struct_by = file.read(8)
    encabezado_struct = struct.unpack(f"<q",encabezado_struct_by)[0]

    #structs
    numeros = []
    apariciones = []
    for i in range(0,encabezado_struct):
        #numero
        num_by = file.read(8)
        num = struct.unpack("<q",num_by)[0]
        numeros.append(num)

        #aparaciones
        aparicion_by = file.read(4)
        aparicion = struct.unpack("<i",aparicion_by)[0]
        apariciones.append(aparicion)

print(array)
print(numeros)
print(apariciones)


