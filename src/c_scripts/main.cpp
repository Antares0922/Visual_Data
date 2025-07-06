#include <iostream>
#include "Modules/sqlite3.h"
#include <string>
#include <vector>


using namespace std;

void BUFFER_CLEANER(){
    char c;
    while((c = cin.get()) != '\n' && c != EOF);
}

vector<long long int> SQL_DATOS(const char Ruta[], string Consulta);

void Quick_Sort(long long int *numeros, int incio, int final);
int Particion(long long int *numeros, int incio, int final);


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


    return 0;
}


void Quick_Sort(long long int *numeros, int inicio, int final){
    if(final <= inicio){return;}

    int pivote = Particion(numeros,inicio,final);

    Quick_Sort(numeros,inicio,pivote-1);
    Quick_Sort(numeros, pivote+1,final);

}
int Particion(long long int *numeros, int inicio, int final){
    int pivote = numeros[final];
    int i = inicio-1;

    for (int j = 0; j<=pivote; j++){
        if (numeros[j] <= pivote){
            i++;
            int temp = numeros[i];
            numeros[i] = numeros[j];
            numeros[j] = temp;
        }
    }
    i++;
    int temp = numeros[i];
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
