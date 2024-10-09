#include <iostream>
#include <stdio.h>
#include <sqlite3.h>

using namespace std;

int main() {
    sqlite3* db; // Puntero a la base de datos
    char* errMsg = 0; // Mensaje de error para SQLite
    int exit; // Variable para el estado de la operación
    
    // Abrir una conexión a la base de datos SQLite
    exit = sqlite3_open("simple.db", &db);
    
    // Verificar si la conexión fue exitosa
    if (exit) {
        cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << endl;
        return 1; // Salir del programa con un error
    }
    
    cout << "Conexión a la base de datos establecida." << endl;

    // Crear una tabla llamada 'personas' si no existe
    const char* sqlCreateTable = "CREATE TABLE IF NOT EXISTS personas (ID INTEGER PRIMARY KEY AUTOINCREMENT, NOMBRE TEXT NOT NULL);";
    exit = sqlite3_exec(db, sqlCreateTable, 0, 0, &errMsg);
    
    // Verificar si hubo un error al crear la tabla
    if (exit != SQLITE_OK) {
        cerr << "Error al crear la tabla: " << errMsg << endl;
        sqlite3_free(errMsg); // Liberar el mensaje de error
    } else {
        cout << "Tabla 'personas' creada correctamente o ya existe." << endl;
    }

    // Insertar un valor en la tabla 'personas'
    const char* sqlInsert = "INSERT INTO personas (NOMBRE) VALUES ('Ana');";
    exit = sqlite3_exec(db, sqlInsert, 0, 0, &errMsg);

    // Verificar si hubo un error al insertar el valor
    if (exit != SQLITE_OK) {
        cerr << "Error al insertar el valor: " << errMsg << endl;
        sqlite3_free(errMsg); // Liberar el buffer del mensaje de error
    } else {
        cout << "Valor 'Ana' insertado correctamente." << endl;
    }

    // Consultar todos los registros de la tabla 'personas'
    const char* sqlSelect = "SELECT * FROM personas;";
    sqlite3_stmt* stmt;

    // Preparar la consulta
    exit = sqlite3_prepare_v2(db, sqlSelect, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        cerr << "Error al preparar la consulta: " << sqlite3_errmsg(db) << endl;
    } else {
        // Ejecutar la consulta y mostrar los resultados
        cout << "Registros en la tabla 'personas':" << endl;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0); // Obtener el ID
            const unsigned char* nombre = sqlite3_column_text(stmt, 1); // Obtener el nombre
            cout << "ID: " << id << ", Nombre: " << nombre << endl; // Imprimir los registros
        }
    }

    // Liberar la memoria del statement
    sqlite3_finalize(stmt);

    // Cerrar la conexión a la base de datos
    sqlite3_close(db);
    cout << "Conexión a la base de datos cerrada." << endl;

    return 0;
}
