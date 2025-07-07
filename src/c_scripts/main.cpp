#include <iostream>
#include "Modules/sqlite3.h"
#include <string>
#include <vector>
#include <fstream>

using namespace std;

typedef struct{
    long long int numero;
    int aparaciones;
} Num_Aparicion;

void BUFFER_CLEANER(){
    char c;
    while((c = cin.get()) != '\n' && c != EOF);
}

vector<long long int> SQL_DATOS(const char Ruta[], string Consulta);

void Quick_Sort(long long int *numeros, int inicio, int final);
int Particion(long long int *numeros, int inicio, int final);


int main(){
    //Ruta a la db
    const char Ruta[] = "../../data/database.db";
    //obteniendo la consulta
    string consulta = "SELECT ";
    string columna;

    //Obteniendo el nombre de la columna
    while(true){
        cout << "Tus opciones son las siguientes" << endl;
        cout << "BHK, Rent, Size, Bathroom" << endl;
        cout << "Escribe una de estas opciones:";
        
        cin >> columna;
        //Dato existente en las opciones
        string Opciones[] = {"BHK","Rent","Size","Bathroom"};
        int longitud = sizeof(Opciones) / sizeof(Opciones[0]);
        bool error = false;

        for (int i = 0; i<longitud; i++){
            error = columna == Opciones[i] ? false:true;
            if(!error){break;}
        }

        BUFFER_CLEANER();
        //verifica si no hay errores o datos incorrectos
        if(!cin.fail() && !error){break;}
        cin.clear();
    }
    consulta = consulta + columna + " FROM Houses";

    //Obteniendo la data 
    vector<long long int> Resultados = SQL_DATOS(Ruta,consulta);

    //Escribiendo los datos al archivo binario
    ofstream archivo("../pipe.bin",ios::binary| ios::trunc);

    if(archivo.fail()){
        cerr << "ERROR CON EL ARCHIVO BINARIO" << endl;
        return 1;
    }
    //longitud de los datos
    size_t tam = Resultados.size();
    archivo.write(reinterpret_cast<const char*>(&tam),sizeof(tam));
    //datos
    archivo.write(reinterpret_cast<const char*>(Resultados.data()),tam * sizeof(long long int));

    //Apariciones de numeros
    Quick_Sort(Resultados.data(),0,(int)tam-1);

    //array de Num_Aparicion
    size_t aparaciones_diferentes = 1;
    vector<Num_Aparicion> Numeros_apariciones;
    Numeros_apariciones.resize(aparaciones_diferentes);
    Numeros_apariciones[0].numero = Resultados[0];
    Numeros_apariciones[0].aparaciones = 1;

    //rellenando el array 
    for (int i = 1; i<tam; i++){
        if(Resultados[i] != Numeros_apariciones[(int)aparaciones_diferentes-1].numero){
            aparaciones_diferentes++;
            Numeros_apariciones.resize(aparaciones_diferentes);
            Numeros_apariciones[(int)aparaciones_diferentes-1].numero = Resultados[i];
            Numeros_apariciones[(int)aparaciones_diferentes-1].aparaciones = 1;
        }
        Numeros_apariciones[(int)aparaciones_diferentes-1].aparaciones += 1;
    }

    //encabezado de structs
    archivo.write(reinterpret_cast<const char*>(&aparaciones_diferentes),sizeof(aparaciones_diferentes));

    //escribien el array de struct
    for(int i = 0; i<aparaciones_diferentes;i++){
        //numero
        archivo.write(reinterpret_cast<const char*>(&Numeros_apariciones[i].numero),sizeof(long long int));
        //aparaciones
        archivo.write(reinterpret_cast<const char*>(&Numeros_apariciones[i].aparaciones),sizeof(int));
    }
    archivo.close();
    return 0;
}


void Quick_Sort(long long int *numeros, int inicio, int final){
    if(final <= inicio){return;}

    long long int pivote = Particion(numeros,inicio,final);

    Quick_Sort(numeros,inicio,pivote-1);
    Quick_Sort(numeros, pivote+1,final);

}
int Particion(long long int *numeros, int inicio, int final){
    long long int pivote = numeros[final];
    int i = inicio-1;

    for (int j = inicio; j<final; j++){
        if (numeros[j] <= pivote){
            i++;
            long long int temp = numeros[i];
            numeros[i] = numeros[j];
            numeros[j] = temp;
        }
    }
    i++;
    long long int temp = numeros[i];
    numeros[i] = numeros[final];
    numeros[final] = temp;

    return i;
}


vector<long long int> SQL_DATOS(const char Ruta[], string Consulta){
    //accediendo a la base de dastos
    sqlite3 *db;
    if(sqlite3_open(Ruta,&db) != SQLITE_OK){
        cerr << "ERROR AL CONECTAR BASE DE DATOS" << endl;
        exit(1);
    }
    
    //Preparando la consulta a la database
    sqlite3_stmt *stmt;
    if(sqlite3_prepare_v2(db,Consulta.c_str(),-1,&stmt,nullptr) != SQLITE_OK){
        cerr << "ERROR AL GENERAR EL STMT" << endl;
        exit(1);
    }

    //Obteniendo los datos
    vector<long long int> Resultados;
    int indice_mayor = 0;
    while(sqlite3_step(stmt) == SQLITE_ROW){
       Resultados.resize(indice_mayor+1);
       long long int numero = sqlite3_column_int64(stmt,0);
       Resultados[indice_mayor] = numero;
       indice_mayor++;
    }
    indice_mayor--;

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return Resultados;
}
